#include "standard.hpp"

/*
Description of a correct user-defined setup (input): (see main.cpp for an example)
- 'Q' is the enum of states
- 'Gamma' is the alphabet enum, it has to contain a value named 'Blank'
- 'Delta' is a struct template which takes 'Display<Q, Gamma, Q State, Gamma Symbol>' instances:
    - each instance represents a single mapping of the delta function
    - the instances implement following static (constexpr) values:
        - write: the symbol (from Gamma) written on the place under the machine's head
        - shift: the direction (L, N, R) where the head moves after writing
        - to: the resulting state of the turing machine
- 'Input' is an instance of 'tape<Gamma, Gamma... Places>' and it is the initial state of the work tape
- 'Fin' is an instance of 'tape<Q, Q... Places>' and it is the set of accepting states

How to run the turing machine:

run_turing_t<Q, Q0, Input, Fin, Delta>

- it returns a type: int if the machine accepts the input; if the machine rejects the input, the type is void and a compile-error is created
    - we can use this type in the definition of main() to create a valid c++ program
    
        run_turing_t<Q, Q0, Input, Fin, Delta> main() {}

see https://en.wikipedia.org/wiki/Turing_machine for a pretty nice formal definition of turing machine
*/

template<typename Gamma, Gamma... Places>
struct tape;

/* Shift directions */
enum Dir { L, N, R };

template<typename Q, typename Gamma, Q State, Gamma Symbol>
struct Display;

template<typename Q, Q State, typename LTape, typename RTape, typename Fin, template<typename> class Delta, typename = void>
struct turing;

template<typename Result, typename Q, Q State, typename LTape, typename RTape, typename Fin, template<typename> class Delta, typename = void>
struct turing_action;

template<typename Q, Q State, typename Fin, typename = void>
struct turing_complete;

template<typename Q, Q State, typename LTape, typename RTape, typename Fin, template<typename> class Delta, typename>
struct turing {
    using type = typename turing_complete<Q, State, Fin>::type;
};

/* Decide action */
template<typename Q, Q State, typename Gamma, Gamma... LTape, Gamma RTHead, Gamma... RTape, Q... Fin, template<typename> class Delta>
struct turing<Q, State, tape<Gamma, LTape...>, tape<Gamma, RTHead, RTape...>, tape<Q, Fin...>, Delta, void_t<decltype(Delta<Display<Q, Gamma, State, RTHead>>{})>> {
    using type = typename turing_action<Delta<Display<Q, Gamma, State, RTHead>>, Q, State, tape<Gamma, LTape...>, tape<Gamma, RTHead, RTape...>, tape<Q, Fin...>, Delta>::type;
};

/* Stretch the tape */
template<typename Q, Q State, typename Gamma, Gamma... LTape, Q... Fin, template<typename> class Delta>
struct turing<Q, State, tape<Gamma, LTape...>, tape<Gamma>, tape<Q, Fin...>, Delta> {
    using type = typename turing<Q, State, tape<Gamma, LTape...>, tape<Gamma, Gamma::Blank>, tape<Q, Fin...>, Delta>::type;
};

/* Left */
template<typename Result, typename Q, Q State, typename Gamma, Gamma... LTape, Gamma LTHead, Gamma RTHead, Gamma... RTape, Q... Fin, template<typename> class Delta>
struct turing_action<Result, Q, State, tape<Gamma, LTHead, LTape...>, tape<Gamma, RTHead, RTape...>, tape<Q, Fin...>, Delta, enable_if_t<Result::shift == L>> {
    using type = typename turing<Q, Result::to, tape<Gamma, LTape...>, tape<Gamma, LTHead, Result::write, RTape...>, tape<Q, Fin...>, Delta>::type;
};

template<typename Result, typename Q, Q State, typename Gamma, Gamma RTHead, Gamma... RTape, Q... Fin, template<typename> class Delta>
struct turing_action<Result, Q, State, tape<Gamma>, tape<Gamma, RTHead, RTape...>, tape<Q, Fin...>, Delta, enable_if_t<Result::shift == L>> {
    using type = typename turing<Q, Result::to, tape<Gamma>, tape<Gamma, Gamma::Blank, Result::write, RTape...>, tape<Q, Fin...>, Delta>::type;
};

/* Right */
template<typename Result, typename Q, Q State, typename Gamma, Gamma... LTape, Gamma RTHead, Gamma... RTape, Q... Fin, template<typename> class Delta>
struct turing_action<Result, Q, State, tape<Gamma, LTape...>, tape<Gamma, RTHead, RTape...>, tape<Q, Fin...>, Delta, enable_if_t<Result::shift == R>> {
    using type = typename turing<Q, Result::to, tape<Gamma, Result::write, LTape...>, tape<Gamma, RTape...>, tape<Q, Fin...>, Delta>::type;
};

/* No moving */
template<typename Result, typename Q, Q State, typename Gamma, Gamma... LTape, Gamma RTHead, Gamma... RTape, Q... Fin, template<typename> class Delta>
struct turing_action<Result, Q, State, tape<Gamma, LTape...>, tape<Gamma, RTHead, RTape...>, tape<Q, Fin...>, Delta, enable_if_t<Result::shift == N>> {
    using type = typename turing<Q, Result::to, tape<Gamma, LTape...>, tape<Gamma, Result::write, RTape...>, tape<Q, Fin...>, Delta>::type;
};

/* Machine halted -> evaluate the result */

template<typename Q, Q State, typename Gamma, Q... Fin>
struct turing_complete<Q, State, tape<Gamma, State, Fin...>> {
    using type = int;
};

template<typename Q, Q State, typename Gamma, Q FHead, Q... Fin>
struct turing_complete<Q, State, tape<Gamma, FHead, Fin...>, enable_if_t<State != FHead>> {
    using type = typename turing_complete<Q, State, tape<Gamma, Fin...>>::type;
};

template<typename Q, Q State, typename Gamma>
struct turing_complete<Q, State, tape<Gamma>> {
    static_assert(lazy_false<Q>::value, "The machine rejected the input");
    using type = void;
};

/* For more convenient running */

template<typename Q, Q State, typename Input, typename Fin, template<typename> class Delta>
struct run_turing;

template<typename Q, Q State, typename Gamma, Gamma... Input, typename Fin, template<typename> class Delta>
struct run_turing<Q, State, tape<Gamma, Input...>, Fin, Delta> {
    using type = typename turing<Q, State, tape<Gamma>, tape<Gamma, Input...>, Fin, Delta>::type;
};

template<typename Q, Q State, typename Input, typename Fin, template<typename> class Delta>
using run_turing_t = typename run_turing<Q, State, Input, Fin, Delta>::type;
