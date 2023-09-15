#ifndef api_macro_hpp
#define api_macro_hpp

// This macro is used to perfectly forward a function call to another function,
// which can be used to implement, e.g., function aliases.
#define FUNCTION_FORWARD(forward_from, forward_to) \
template <typename... T> inline auto forward_from(T&&... t) \
-> decltype(forward_to(std::forward<T>(t)...)) \
{ return forward_to(std::forward<T>(t)...); }

#endif // api_macro_hpp
