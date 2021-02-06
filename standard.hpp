/* Just a few helper function we could have imported from std */

template<typename...>
using void_t = void;

template<bool V, typename T = void>
struct enable_if;

template<typename T>
struct enable_if<true, T> { using type = T; };

template<bool V, typename T = void>
using enable_if_t = typename enable_if<V, T>::type;

template<typename T>
struct lazy_false { 
    enum { value = false };  
};
