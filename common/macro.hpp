#ifndef common_macro_hpp
#define common_macro_hpp

// This macro is used to perfectly forward a function call to another function,
// which can be used to implement, e.g., function aliases.
#define decl_func_alias_(decl, call) \
template <typename... FTyp> inline auto decl(FTyp&&... t) \
  -> decltype(call(std::forward<FTyp>(t)...)) \
{ return call(std::forward<FTyp>(t)...); }

#endif // common_macro_hpp
