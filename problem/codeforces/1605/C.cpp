// #define TIME
// #define DEBUG_
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <set>
#include <unordered_map>
#include <vector>

#ifdef DEBUG_
#define debug(format, ...) printf(format, ##__VA_ARGS__)
#else
#define debug(...)
#endif

using namespace std;
using LL = long long;
using uLL = unsigned long long;
using uI = unsigned int;
using Pll = pair<LL, LL>;

random_device rd;
mt19937 rnd(rd());
mt19937_64 rnd_64(rd());

namespace IO {
namespace IN {
#define getc() (p1 == p2 && (p2 = (p1 = buf) + inbuf->sgetn(buf, MAX_INPUT), p1 == p2) ? EOF : *p1++)
const int MAX_INPUT = 1000000;
char buf[MAX_INPUT], *p1, *p2;
template <typename T>
inline int redi(T& x) {
  static std::streambuf* inbuf = cin.rdbuf();
  int f = x = 0, flag = 1;
  char ch = getc();
  if (ch == EOF) return EOF;
  while (!std::isdigit(ch)) ch == '-' && (f = 1), ch = getc();
  if (std::isdigit(ch)) x = x * 10 + ch - '0', ch = getc(), flag = 0;
  while (std::isdigit(ch)) x = x * 10 + ch - 48, ch = getc();
  return x = f ? -x : x, ch == EOF ? EOF : flag;
}
template <typename T, typename... Args>
inline int redi(T& a, Args&... args) { return redi(a) != EOF ? redi(args...), 0 : EOF; }
inline int redc(char& ch) {
  static std::streambuf* inbuf = cin.rdbuf();
  return (ch = getc()) == EOF ? EOF : 0;
}
inline int redst(char* st) {
  if (redc(*st) != EOF && (*st))
    while (redc(*(++st)) != EOF) 1;
  else
    return EOF;
  return 0;
}
inline int redst(string& s) {
  s = "";
  char c;
  for (; redc(c) != EOF && !isspace(c); s.push_back(c)) 1;
  return c == EOF ? EOF : 0;
}
#undef getc
}  // namespace IN
namespace OUT {
template <typename T>
inline void put_non(T x) {
  static std::streambuf* outbuf = cout.rdbuf();
  static char stack[21];
  static int top = 0;
  if (x < 0) outbuf->sputc('-'), x = -x;
  if (!x) {
    outbuf->sputc('0');
    return;
  }
  while (x) stack[++top] = x % 10 + '0', x /= 10;
  while (top) outbuf->sputc(stack[top]), --top;
}
template <typename T>
inline void put(const char ch, T x) { put_non(x), putc(ch); }
template <typename T>
inline void put(T x) { put('\n', x); }
inline void putc(const char ch) {
  static std::streambuf* outbuf = cout.rdbuf();
  outbuf->sputc(ch);
}
inline void putst(const char* st) {
  static std::streambuf* outbuf = cout.rdbuf();
  do outbuf->sputc(*(st++));
  while (*st);
}
inline void putst(string st) { putst(st.c_str()); }
template <typename T>
inline void put(const char* st, T x) { put_non(x), putst(st); }
template <typename T, typename... Args>
inline void put(T a, Args... args) { put(a), put(args...); }
template <typename T, typename... Args>
inline void put(const char ch, T a, Args... args) { put(ch, a), put(ch, args...); }
template <typename T, typename... Args>
inline void put(const char* st, T a, Args... args) { put(st, a), put(st, args...); }
}  // namespace OUT
using IN::redc;
using IN::redi;
using IN::redst;
using OUT::put;
using OUT::put_non;
using OUT::putc;
using OUT::putst;
}  // namespace IO

struct BigUint {
  static const int BASE = 100000000, WIDTH = 8;
  vector<int> s;
  int len;
  BigUint& clean() {
    while (!s.back() && s.size() > 1) s.pop_back();
    return *this;
  }
  BigUint(uLL num = 0) { *this = num; }
  BigUint(string s) { *this = s; }
  // BigInt(char *s) { *this = string(s); }
  BigUint& operator=(long long num) {
    s.clear();
    long long t = num;
    while (t) t /= 10, len++;
    do s.push_back(num % BASE), num /= BASE;
    while (num > 0);
    return *this;
  }
  BigUint& operator=(const string& str) {
    s.clear();
    int x, l = ((len = str.length()) - 1) / WIDTH + 1;
    for (int i = 0, end, start; i < l; i++) end = str.length() - i * WIDTH, start = max(0, end - WIDTH), sscanf(str.substr(start, end - start).c_str(), "%d", &x), s.push_back(x);
    return (*this).clean();
  }
  BigUint operator+(const BigUint& b) const {
    BigUint c;
    c.s.clear();
    for (int i = 0, g = 0, x; g || i < s.size() || i < b.s.size(); i++) x = g, i < s.size() && (x += s[i]), i < b.s.size() && (x += b.s[i]), c.s.push_back(x % BASE), g = x / BASE;
    return c;
  }
  BigUint operator-(const BigUint& b) const {
    assert(b <= *this);
    BigUint c;
    c.s.clear();
    for (int i = 0, g = 0, x; g || i < s.size() || i < b.s.size(); i++) x = s[i] + g, i < b.s.size() && (x -= b.s[i]), x < 0 ? (g = -1, x += BASE) : (g = 0), c.s.push_back(x);
    return c.clean();
  }
  BigUint operator*(const BigUint& b) const {
    int i, j;
    uLL g, x;
    vector<uLL> v(s.size() + b.s.size(), 0);
    BigUint c;
    c.s.clear();
    for (i = 0; i < s.size(); i++)
      for (j = 0; j < b.s.size(); j++) v[i + j] += (uLL)(s[i]) * b.s[j];
    for (i = 0, g = 0; g || i < v.size(); i++) x = v[i] + g, c.s.push_back(x % BASE), g = x / BASE;
    return c.clean();
  }
  BigUint operator/(const BigUint& b) const {
    assert(b > 0);
    BigUint c = *this, m;
    for (int i = s.size() - 1; i >= 0; i--) m = m * BASE + s[i], c.s[i] = bsearch(b, m), m -= b * c.s[i];
    return c.clean();
  }
  BigUint operator%(const BigUint& b) const {
    BigUint c = *this, m;
    for (int i = s.size() - 1; i >= 0; i--) m = m * BASE + s[i], c.s[i] = bsearch(b, m), m -= b * c.s[i];
    return m;
  }
  int bsearch(const BigUint& b, const BigUint& m) const {
    int x;
    for (int L = 0, R = BASE - 1; x = (L + R) >> 1, b * x > m || b * (x + 1) <= m; (b * x <= m ? L : R) = x) 1;
    return x;
  }
  BigUint& operator+=(const BigUint& b) { return *this = *this + b; }
  BigUint& operator-=(const BigUint& b) { return *this = *this - b; }
  BigUint& operator*=(const BigUint& b) { return *this = *this * b; }
  BigUint& operator/=(const BigUint& b) { return *this = *this / b; }
  BigUint& operator%=(const BigUint& b) { return *this = *this % b; }
  bool operator<(const BigUint& b) const {
    if (s.size() != b.s.size()) return s.size() < b.s.size();
    for (int i = s.size() - 1; i >= 0; i--)
      if (s[i] != b.s[i]) return s[i] < b.s[i];
    return false;
  }
  bool operator>(const BigUint& b) const { return b < *this; }
  bool operator<=(const BigUint& b) const { return !(b < *this); }
  bool operator>=(const BigUint& b) const { return !(*this < b); }
  bool operator!=(const BigUint& b) const { return b < *this || *this < b; }
  bool operator==(const BigUint& b) const { return !(b < *this) && !(b > *this); }
  void read() {
    string _s;
    IO::redst(_s), *this = _s;
  }
  void write() {
    clean(), IO::put_non(s.back());
    for (int i = s.size() - 2; ~i; --i) {
      char sc[20];
      sprintf(sc, "%08d", s[i]), IO::putst(sc);
    }
  }
};

int main() {
#define ONLINE_JUDGE
#ifndef ONLINE_JUDGE
  freopen(".in", "r", stdin);
  freopen(".out", "w", stdout);
#endif
  ios_base::sync_with_stdio(0);
  cin.tie(0), cout.tie(0);
  
#ifdef TIME
  double t = 1.0 * clock() / CLOCKS_PER_SEC;
  cerr << "\n\nTIME: " << t << "s\n";
#endif
  return 0;
}