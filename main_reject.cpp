#include "machine.hpp"

/* Trivial test program */

enum Q { q0, q1, q2, q3, };
enum Gamma { a, b, Blank };

template<typename>
struct Delta;

template<>
struct Delta<Display<Q, Gamma, q0, a>> {
    static constexpr Dir shift = R;
    static constexpr Gamma write = b;
    static constexpr Q to = q1;
};

using Input = tape<Gamma, a>;
using Fin = tape<Q, q2>;

run_turing_t<Q, q0, Input, Fin, Delta> main() {}
