#ifndef const_hpp
#define const_hpp

#include <string>
#include <stdexcept>

// Simple string class that can be used in constexpr contexts.
//
// N is the length of the string, including the null terminator.
// The length of the string represented by this object, or len(), is N-1.
//
// Null bytes are permitted anywhere, but the string must be null-terminated.
// Thus, you can interpret each instance as an array of K null-terminated
// strings with K = NumberOfNullBytes > 0.
//
//   ¦<················ N=14, len()=13, K=5 ················>¦
//   ↓_______________________________________________________↓
//   ['a'|'b'|'c'| 0 |'d'|'e'|'f'| 0 |'g'| 0 | 0 |'h'|'i'| 0 ]
//   ↑¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯↑¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯↑¯¯¯¯¯¯¯↑¯¯¯↑¯¯¯¯¯¯¯¯¯¯¯↑
//   ¦<······A······>¦<······B······>¦<··C··>¦<D>¦<····E····>¦
//
//      [A]:  strlen("abc") == 3
//      [B]:  strlen("def") == 3
//      [C]:  strlen("g")   == 1
//      [D]:  strlen("")    == 0
//      [E]:  strlen("hi")  == 2
//
// This is technically more like a string_view than a string, because it
// does not own the string char data. The only members of a conststring
// object are a pointer to the char data and the length of that data.

template <size_t N>
struct conststring {
public:
  constexpr
    conststring(const char (&str)[N]):
      _str(str), _len(N<=0 ? 0 : N-1) {
    }

  constexpr
    const size_t len() const
      { return _len;
    }

  constexpr
    std::string substr(const size_t pos, const size_t len) const {
      using std::out_of_range;
      return pos < _len
        ? std::string(_str+pos, _str+pos+len)
        : throw out_of_range("");
    }

  constexpr
    operator const char * const() const {
      return _str;
    }

  constexpr
    operator std::string() const {
      return { _str, _len };
    }

  constexpr
    const char operator[](int n) const {
      using std::out_of_range;
      return n < _len
        ? _str[n]
        : throw out_of_range("");
    }

private:
  const char * const _str;
  const size_t _len;
};

#endif
