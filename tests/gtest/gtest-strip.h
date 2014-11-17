
#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
#define GTEST_INCLUDE_GTEST_GTEST_H_

#include <limits>
#include <ostream>
#include <vector>

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_INTERNAL_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_INTERNAL_H_

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PORT_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PORT_H_

#include <ctype.h>   // for isspace, etc
#include <stddef.h>  // for ptrdiff_t
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32_WCE
# include <sys/types.h>
# include <sys/stat.h>
#endif  // !_WIN32_WCE

#if defined __APPLE__
# include <AvailabilityMacros.h>
# include <TargetConditionals.h>
#endif

#include <iostream>  // NOLINT
#include <sstream>  // NOLINT
#include <string>  // NOLINT

#define GTEST_DEV_EMAIL_ "googletestframework@@googlegroups.com"
#define GTEST_FLAG_PREFIX_ "gtest_"
#define GTEST_FLAG_PREFIX_DASH_ "gtest-"
#define GTEST_FLAG_PREFIX_UPPER_ "GTEST_"
#define GTEST_NAME_ "Google Test"
#define GTEST_PROJECT_URL_ "http://code.google.com/p/googletest/"

#ifdef __GNUC__

# define GTEST_GCC_VER_ \
    (__GNUC__*10000 + __GNUC_MINOR__*100 + __GNUC_PATCHLEVEL__)
#endif  // __GNUC__

#ifdef __CYGWIN__
# define GTEST_OS_CYGWIN 1
#elif defined __SYMBIAN32__
# define GTEST_OS_SYMBIAN 1
#elif defined _WIN32
# define GTEST_OS_WINDOWS 1
# ifdef _WIN32_WCE
#  define GTEST_OS_WINDOWS_MOBILE 1
# elif defined(__MINGW__) || defined(__MINGW32__)
#  define GTEST_OS_WINDOWS_MINGW 1
# else
#  define GTEST_OS_WINDOWS_DESKTOP 1
# endif  // _WIN32_WCE
#elif defined __APPLE__
# define GTEST_OS_MAC 1
# if TARGET_OS_IPHONE
#  define GTEST_OS_IOS 1
#  if TARGET_IPHONE_SIMULATOR
#   define GTEST_OS_IOS_SIMULATOR 1
#  endif
# endif
#elif defined __linux__
# define GTEST_OS_LINUX 1
# if defined __ANDROID__
#  define GTEST_OS_LINUX_ANDROID 1
# endif
#elif defined __MVS__
# define GTEST_OS_ZOS 1
#elif defined(__sun) && defined(__SVR4)
# define GTEST_OS_SOLARIS 1
#elif defined(_AIX)
# define GTEST_OS_AIX 1
#elif defined(__hpux)
# define GTEST_OS_HPUX 1
#elif defined __native_client__
# define GTEST_OS_NACL 1
#elif defined __OpenBSD__
# define GTEST_OS_OPENBSD 1
#elif defined __QNX__
# define GTEST_OS_QNX 1
#endif  // __CYGWIN__

#ifndef GTEST_LANG_CXX11

# if __GXX_EXPERIMENTAL_CXX0X__ || __cplusplus >= 201103L

#  define GTEST_LANG_CXX11 1
# else
#  define GTEST_LANG_CXX11 0
# endif
#endif

#if !GTEST_OS_WINDOWS

# include <unistd.h>
# include <strings.h>
#elif !GTEST_OS_WINDOWS_MOBILE
# include <direct.h>
# include <io.h>
#endif

#if GTEST_OS_LINUX_ANDROID

#  include <android/api-level.h>  // NOLINT
#endif

#ifndef GTEST_HAS_POSIX_RE
# if GTEST_OS_LINUX_ANDROID

#  define GTEST_HAS_POSIX_RE (__ANDROID_API__ >= 9)
# else
#  define GTEST_HAS_POSIX_RE (!GTEST_OS_WINDOWS)
# endif
#endif

#if GTEST_HAS_POSIX_RE

# include <regex.h>  // NOLINT

# define GTEST_USES_POSIX_RE 1

#elif GTEST_OS_WINDOWS

# define GTEST_USES_SIMPLE_RE 1

#else

# define GTEST_USES_SIMPLE_RE 1

#endif  // GTEST_HAS_POSIX_RE

#ifndef GTEST_HAS_EXCEPTIONS

# if defined(_MSC_VER) || defined(__BORLANDC__)

#  ifndef _HAS_EXCEPTIONS
#   define _HAS_EXCEPTIONS 1
#  endif  // _HAS_EXCEPTIONS
#  define GTEST_HAS_EXCEPTIONS _HAS_EXCEPTIONS
# elif defined(__GNUC__) && __EXCEPTIONS

#  define GTEST_HAS_EXCEPTIONS 1
# elif defined(__SUNPRO_CC)

#  define GTEST_HAS_EXCEPTIONS 1
# elif defined(__IBMCPP__) && __EXCEPTIONS

#  define GTEST_HAS_EXCEPTIONS 1
# elif defined(__HP_aCC)

#  define GTEST_HAS_EXCEPTIONS 1
# else

#  define GTEST_HAS_EXCEPTIONS 0
# endif  // defined(_MSC_VER) || defined(__BORLANDC__)
#endif  // GTEST_HAS_EXCEPTIONS

#if !defined(GTEST_HAS_STD_STRING)

# define GTEST_HAS_STD_STRING 1
#elif !GTEST_HAS_STD_STRING

# error "Google Test cannot be used where ::std::string isn't available."
#endif  // !defined(GTEST_HAS_STD_STRING)

#ifndef GTEST_HAS_GLOBAL_STRING

# define GTEST_HAS_GLOBAL_STRING 0

#endif  // GTEST_HAS_GLOBAL_STRING

#ifndef GTEST_HAS_STD_WSTRING

# define GTEST_HAS_STD_WSTRING \
    (!(GTEST_OS_LINUX_ANDROID || GTEST_OS_CYGWIN || GTEST_OS_SOLARIS))

#endif  // GTEST_HAS_STD_WSTRING

#ifndef GTEST_HAS_GLOBAL_WSTRING

# define GTEST_HAS_GLOBAL_WSTRING \
    (GTEST_HAS_STD_WSTRING && GTEST_HAS_GLOBAL_STRING)
#endif  // GTEST_HAS_GLOBAL_WSTRING

#ifndef GTEST_HAS_RTTI

# ifdef _MSC_VER

#  ifdef _CPPRTTI  // MSVC defines this macro iff RTTI is enabled.
#   define GTEST_HAS_RTTI 1
#  else
#   define GTEST_HAS_RTTI 0
#  endif

# elif defined(__GNUC__) && (GTEST_GCC_VER_ >= 40302)

#  ifdef __GXX_RTTI

#   if GTEST_OS_LINUX_ANDROID && defined(_STLPORT_MAJOR) && \
       !defined(__EXCEPTIONS)
#    define GTEST_HAS_RTTI 0
#   else
#    define GTEST_HAS_RTTI 1
#   endif  // GTEST_OS_LINUX_ANDROID && __STLPORT_MAJOR && !__EXCEPTIONS
#  else
#   define GTEST_HAS_RTTI 0
#  endif  // __GXX_RTTI

# elif defined(__clang__)

#  define GTEST_HAS_RTTI __has_feature(cxx_rtti)

# elif defined(__IBMCPP__) && (__IBMCPP__ >= 900)

#  ifdef __RTTI_ALL__
#   define GTEST_HAS_RTTI 1
#  else
#   define GTEST_HAS_RTTI 0
#  endif

# else

#  define GTEST_HAS_RTTI 1

# endif  // _MSC_VER

#endif  // GTEST_HAS_RTTI

#if GTEST_HAS_RTTI
# include <typeinfo>
#endif

#ifndef GTEST_HAS_PTHREAD

# define GTEST_HAS_PTHREAD (GTEST_OS_LINUX || GTEST_OS_MAC || GTEST_OS_HPUX \
    || GTEST_OS_QNX)
#endif  // GTEST_HAS_PTHREAD

#if GTEST_HAS_PTHREAD

# include <pthread.h>  // NOLINT

# include <time.h>  // NOLINT
#endif

#ifndef GTEST_HAS_TR1_TUPLE
# if GTEST_OS_LINUX_ANDROID && defined(_STLPORT_MAJOR)

#  define GTEST_HAS_TR1_TUPLE 0
# else

#  define GTEST_HAS_TR1_TUPLE 1
# endif
#endif  // GTEST_HAS_TR1_TUPLE

#ifndef GTEST_USE_OWN_TR1_TUPLE

# if (defined(__GNUC__) && !defined(__CUDACC__) && (GTEST_GCC_VER_ >= 40000) \
      && !GTEST_OS_QNX && !defined(_LIBCPP_VERSION)) || _MSC_VER >= 1600
#  define GTEST_ENV_HAS_TR1_TUPLE_ 1
# endif

# if GTEST_LANG_CXX11 && (!defined(__GLIBCXX__) || __GLIBCXX__ > 20110325)
#  define GTEST_ENV_HAS_STD_TUPLE_ 1
# endif

# if GTEST_ENV_HAS_TR1_TUPLE_ || GTEST_ENV_HAS_STD_TUPLE_
#  define GTEST_USE_OWN_TR1_TUPLE 0
# else
#  define GTEST_USE_OWN_TR1_TUPLE 1
# endif

#endif  // GTEST_USE_OWN_TR1_TUPLE

#if GTEST_HAS_TR1_TUPLE

# if GTEST_USE_OWN_TR1_TUPLE

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_TUPLE_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_TUPLE_H_

#include <utility>  // For ::std::pair.

#if defined(__SYMBIAN32__) || (defined(__SUNPRO_CC) && __SUNPRO_CC < 0x590)
# define GTEST_DECLARE_TUPLE_AS_FRIEND_ public:
#else
# define GTEST_DECLARE_TUPLE_AS_FRIEND_ \
    template <GTEST_10_TYPENAMES_(U)> friend class tuple; \
   private:
#endif

#define GTEST_0_TUPLE_(T) tuple<>
#define GTEST_1_TUPLE_(T) tuple<T##0, void, void, void, void, void, void, \
    void, void, void>
#define GTEST_2_TUPLE_(T) tuple<T##0, T##1, void, void, void, void, void, \
    void, void, void>
#define GTEST_3_TUPLE_(T) tuple<T##0, T##1, T##2, void, void, void, void, \
    void, void, void>
#define GTEST_4_TUPLE_(T) tuple<T##0, T##1, T##2, T##3, void, void, void, \
    void, void, void>
#define GTEST_5_TUPLE_(T) tuple<T##0, T##1, T##2, T##3, T##4, void, void, \
    void, void, void>
#define GTEST_6_TUPLE_(T) tuple<T##0, T##1, T##2, T##3, T##4, T##5, void, \
    void, void, void>
#define GTEST_7_TUPLE_(T) tuple<T##0, T##1, T##2, T##3, T##4, T##5, T##6, \
    void, void, void>
#define GTEST_8_TUPLE_(T) tuple<T##0, T##1, T##2, T##3, T##4, T##5, T##6, \
    T##7, void, void>
#define GTEST_9_TUPLE_(T) tuple<T##0, T##1, T##2, T##3, T##4, T##5, T##6, \
    T##7, T##8, void>
#define GTEST_10_TUPLE_(T) tuple<T##0, T##1, T##2, T##3, T##4, T##5, T##6, \
    T##7, T##8, T##9>

#define GTEST_0_TYPENAMES_(T)
#define GTEST_1_TYPENAMES_(T) typename T##0
#define GTEST_2_TYPENAMES_(T) typename T##0, typename T##1
#define GTEST_3_TYPENAMES_(T) typename T##0, typename T##1, typename T##2
#define GTEST_4_TYPENAMES_(T) typename T##0, typename T##1, typename T##2, \
    typename T##3
#define GTEST_5_TYPENAMES_(T) typename T##0, typename T##1, typename T##2, \
    typename T##3, typename T##4
#define GTEST_6_TYPENAMES_(T) typename T##0, typename T##1, typename T##2, \
    typename T##3, typename T##4, typename T##5
#define GTEST_7_TYPENAMES_(T) typename T##0, typename T##1, typename T##2, \
    typename T##3, typename T##4, typename T##5, typename T##6
#define GTEST_8_TYPENAMES_(T) typename T##0, typename T##1, typename T##2, \
    typename T##3, typename T##4, typename T##5, typename T##6, typename T##7
#define GTEST_9_TYPENAMES_(T) typename T##0, typename T##1, typename T##2, \
    typename T##3, typename T##4, typename T##5, typename T##6, \
    typename T##7, typename T##8
#define GTEST_10_TYPENAMES_(T) typename T##0, typename T##1, typename T##2, \
    typename T##3, typename T##4, typename T##5, typename T##6, \
    typename T##7, typename T##8, typename T##9

namespace std {
namespace tr1 {

template <typename T0 = void, typename T1 = void, typename T2 = void,
    typename T3 = void, typename T4 = void, typename T5 = void,
    typename T6 = void, typename T7 = void, typename T8 = void,
    typename T9 = void>
class tuple;

namespace gtest_internal {

template <typename T>
struct ByRef { typedef const T& type; };  // NOLINT
template <typename T>
struct ByRef<T&> { typedef T& type; };  // NOLINT

#define GTEST_BY_REF_(T) typename ::std::tr1::gtest_internal::ByRef<T>::type

template <typename T>
struct AddRef { typedef T& type; };  // NOLINT
template <typename T>
struct AddRef<T&> { typedef T& type; };  // NOLINT

#define GTEST_ADD_REF_(T) typename ::std::tr1::gtest_internal::AddRef<T>::type

template <int k> class Get;

template <bool kIndexValid, int kIndex, class Tuple>
struct TupleElement;

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 0, GTEST_10_TUPLE_(T) > {
  typedef T0 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 1, GTEST_10_TUPLE_(T) > {
  typedef T1 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 2, GTEST_10_TUPLE_(T) > {
  typedef T2 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 3, GTEST_10_TUPLE_(T) > {
  typedef T3 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 4, GTEST_10_TUPLE_(T) > {
  typedef T4 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 5, GTEST_10_TUPLE_(T) > {
  typedef T5 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 6, GTEST_10_TUPLE_(T) > {
  typedef T6 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 7, GTEST_10_TUPLE_(T) > {
  typedef T7 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 8, GTEST_10_TUPLE_(T) > {
  typedef T8 type;
};

template <GTEST_10_TYPENAMES_(T)>
struct TupleElement<true, 9, GTEST_10_TUPLE_(T) > {
  typedef T9 type;
};

}  // namespace gtest_internal

template <>
class tuple<> {
 public:
  tuple() {}
  tuple(const tuple& /* t */)  {}
  tuple& operator=(const tuple& /* t */) { return *this; }
};

template <GTEST_1_TYPENAMES_(T)>
class GTEST_1_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0) : f0_(f0) {}

  tuple(const tuple& t) : f0_(t.f0_) {}

  template <GTEST_1_TYPENAMES_(U)>
  tuple(const GTEST_1_TUPLE_(U)& t) : f0_(t.f0_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_1_TYPENAMES_(U)>
  tuple& operator=(const GTEST_1_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_1_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_1_TUPLE_(U)& t) {
    f0_ = t.f0_;
    return *this;
  }

  T0 f0_;
};

template <GTEST_2_TYPENAMES_(T)>
class GTEST_2_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1) : f0_(f0),
      f1_(f1) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_) {}

  template <GTEST_2_TYPENAMES_(U)>
  tuple(const GTEST_2_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_) {}
  template <typename U0, typename U1>
  tuple(const ::std::pair<U0, U1>& p) : f0_(p.first), f1_(p.second) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_2_TYPENAMES_(U)>
  tuple& operator=(const GTEST_2_TUPLE_(U)& t) {
    return CopyFrom(t);
  }
  template <typename U0, typename U1>
  tuple& operator=(const ::std::pair<U0, U1>& p) {
    f0_ = p.first;
    f1_ = p.second;
    return *this;
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_2_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_2_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
};

template <GTEST_3_TYPENAMES_(T)>
class GTEST_3_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_(), f2_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1,
      GTEST_BY_REF_(T2) f2) : f0_(f0), f1_(f1), f2_(f2) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_) {}

  template <GTEST_3_TYPENAMES_(U)>
  tuple(const GTEST_3_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_3_TYPENAMES_(U)>
  tuple& operator=(const GTEST_3_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_3_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_3_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    f2_ = t.f2_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
  T2 f2_;
};

template <GTEST_4_TYPENAMES_(T)>
class GTEST_4_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_(), f2_(), f3_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1,
      GTEST_BY_REF_(T2) f2, GTEST_BY_REF_(T3) f3) : f0_(f0), f1_(f1), f2_(f2),
      f3_(f3) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_), f3_(t.f3_) {}

  template <GTEST_4_TYPENAMES_(U)>
  tuple(const GTEST_4_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_),
      f3_(t.f3_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_4_TYPENAMES_(U)>
  tuple& operator=(const GTEST_4_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_4_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_4_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    f2_ = t.f2_;
    f3_ = t.f3_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
  T2 f2_;
  T3 f3_;
};

template <GTEST_5_TYPENAMES_(T)>
class GTEST_5_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_(), f2_(), f3_(), f4_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1,
      GTEST_BY_REF_(T2) f2, GTEST_BY_REF_(T3) f3,
      GTEST_BY_REF_(T4) f4) : f0_(f0), f1_(f1), f2_(f2), f3_(f3), f4_(f4) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_), f3_(t.f3_),
      f4_(t.f4_) {}

  template <GTEST_5_TYPENAMES_(U)>
  tuple(const GTEST_5_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_),
      f3_(t.f3_), f4_(t.f4_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_5_TYPENAMES_(U)>
  tuple& operator=(const GTEST_5_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_5_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_5_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    f2_ = t.f2_;
    f3_ = t.f3_;
    f4_ = t.f4_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
  T2 f2_;
  T3 f3_;
  T4 f4_;
};

template <GTEST_6_TYPENAMES_(T)>
class GTEST_6_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_(), f2_(), f3_(), f4_(), f5_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1,
      GTEST_BY_REF_(T2) f2, GTEST_BY_REF_(T3) f3, GTEST_BY_REF_(T4) f4,
      GTEST_BY_REF_(T5) f5) : f0_(f0), f1_(f1), f2_(f2), f3_(f3), f4_(f4),
      f5_(f5) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_), f3_(t.f3_),
      f4_(t.f4_), f5_(t.f5_) {}

  template <GTEST_6_TYPENAMES_(U)>
  tuple(const GTEST_6_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_),
      f3_(t.f3_), f4_(t.f4_), f5_(t.f5_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_6_TYPENAMES_(U)>
  tuple& operator=(const GTEST_6_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_6_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_6_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    f2_ = t.f2_;
    f3_ = t.f3_;
    f4_ = t.f4_;
    f5_ = t.f5_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
  T2 f2_;
  T3 f3_;
  T4 f4_;
  T5 f5_;
};

template <GTEST_7_TYPENAMES_(T)>
class GTEST_7_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_(), f2_(), f3_(), f4_(), f5_(), f6_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1,
      GTEST_BY_REF_(T2) f2, GTEST_BY_REF_(T3) f3, GTEST_BY_REF_(T4) f4,
      GTEST_BY_REF_(T5) f5, GTEST_BY_REF_(T6) f6) : f0_(f0), f1_(f1), f2_(f2),
      f3_(f3), f4_(f4), f5_(f5), f6_(f6) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_), f3_(t.f3_),
      f4_(t.f4_), f5_(t.f5_), f6_(t.f6_) {}

  template <GTEST_7_TYPENAMES_(U)>
  tuple(const GTEST_7_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_),
      f3_(t.f3_), f4_(t.f4_), f5_(t.f5_), f6_(t.f6_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_7_TYPENAMES_(U)>
  tuple& operator=(const GTEST_7_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_7_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_7_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    f2_ = t.f2_;
    f3_ = t.f3_;
    f4_ = t.f4_;
    f5_ = t.f5_;
    f6_ = t.f6_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
  T2 f2_;
  T3 f3_;
  T4 f4_;
  T5 f5_;
  T6 f6_;
};

template <GTEST_8_TYPENAMES_(T)>
class GTEST_8_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_(), f2_(), f3_(), f4_(), f5_(), f6_(), f7_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1,
      GTEST_BY_REF_(T2) f2, GTEST_BY_REF_(T3) f3, GTEST_BY_REF_(T4) f4,
      GTEST_BY_REF_(T5) f5, GTEST_BY_REF_(T6) f6,
      GTEST_BY_REF_(T7) f7) : f0_(f0), f1_(f1), f2_(f2), f3_(f3), f4_(f4),
      f5_(f5), f6_(f6), f7_(f7) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_), f3_(t.f3_),
      f4_(t.f4_), f5_(t.f5_), f6_(t.f6_), f7_(t.f7_) {}

  template <GTEST_8_TYPENAMES_(U)>
  tuple(const GTEST_8_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_),
      f3_(t.f3_), f4_(t.f4_), f5_(t.f5_), f6_(t.f6_), f7_(t.f7_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_8_TYPENAMES_(U)>
  tuple& operator=(const GTEST_8_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_8_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_8_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    f2_ = t.f2_;
    f3_ = t.f3_;
    f4_ = t.f4_;
    f5_ = t.f5_;
    f6_ = t.f6_;
    f7_ = t.f7_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
  T2 f2_;
  T3 f3_;
  T4 f4_;
  T5 f5_;
  T6 f6_;
  T7 f7_;
};

template <GTEST_9_TYPENAMES_(T)>
class GTEST_9_TUPLE_(T) {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_(), f2_(), f3_(), f4_(), f5_(), f6_(), f7_(), f8_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1,
      GTEST_BY_REF_(T2) f2, GTEST_BY_REF_(T3) f3, GTEST_BY_REF_(T4) f4,
      GTEST_BY_REF_(T5) f5, GTEST_BY_REF_(T6) f6, GTEST_BY_REF_(T7) f7,
      GTEST_BY_REF_(T8) f8) : f0_(f0), f1_(f1), f2_(f2), f3_(f3), f4_(f4),
      f5_(f5), f6_(f6), f7_(f7), f8_(f8) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_), f3_(t.f3_),
      f4_(t.f4_), f5_(t.f5_), f6_(t.f6_), f7_(t.f7_), f8_(t.f8_) {}

  template <GTEST_9_TYPENAMES_(U)>
  tuple(const GTEST_9_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_),
      f3_(t.f3_), f4_(t.f4_), f5_(t.f5_), f6_(t.f6_), f7_(t.f7_), f8_(t.f8_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_9_TYPENAMES_(U)>
  tuple& operator=(const GTEST_9_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_9_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_9_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    f2_ = t.f2_;
    f3_ = t.f3_;
    f4_ = t.f4_;
    f5_ = t.f5_;
    f6_ = t.f6_;
    f7_ = t.f7_;
    f8_ = t.f8_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
  T2 f2_;
  T3 f3_;
  T4 f4_;
  T5 f5_;
  T6 f6_;
  T7 f7_;
  T8 f8_;
};

template <GTEST_10_TYPENAMES_(T)>
class tuple {
 public:
  template <int k> friend class gtest_internal::Get;

  tuple() : f0_(), f1_(), f2_(), f3_(), f4_(), f5_(), f6_(), f7_(), f8_(),
      f9_() {}

  explicit tuple(GTEST_BY_REF_(T0) f0, GTEST_BY_REF_(T1) f1,
      GTEST_BY_REF_(T2) f2, GTEST_BY_REF_(T3) f3, GTEST_BY_REF_(T4) f4,
      GTEST_BY_REF_(T5) f5, GTEST_BY_REF_(T6) f6, GTEST_BY_REF_(T7) f7,
      GTEST_BY_REF_(T8) f8, GTEST_BY_REF_(T9) f9) : f0_(f0), f1_(f1), f2_(f2),
      f3_(f3), f4_(f4), f5_(f5), f6_(f6), f7_(f7), f8_(f8), f9_(f9) {}

  tuple(const tuple& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_), f3_(t.f3_),
      f4_(t.f4_), f5_(t.f5_), f6_(t.f6_), f7_(t.f7_), f8_(t.f8_), f9_(t.f9_) {}

  template <GTEST_10_TYPENAMES_(U)>
  tuple(const GTEST_10_TUPLE_(U)& t) : f0_(t.f0_), f1_(t.f1_), f2_(t.f2_),
      f3_(t.f3_), f4_(t.f4_), f5_(t.f5_), f6_(t.f6_), f7_(t.f7_), f8_(t.f8_),
      f9_(t.f9_) {}

  tuple& operator=(const tuple& t) { return CopyFrom(t); }

  template <GTEST_10_TYPENAMES_(U)>
  tuple& operator=(const GTEST_10_TUPLE_(U)& t) {
    return CopyFrom(t);
  }

  GTEST_DECLARE_TUPLE_AS_FRIEND_

  template <GTEST_10_TYPENAMES_(U)>
  tuple& CopyFrom(const GTEST_10_TUPLE_(U)& t) {
    f0_ = t.f0_;
    f1_ = t.f1_;
    f2_ = t.f2_;
    f3_ = t.f3_;
    f4_ = t.f4_;
    f5_ = t.f5_;
    f6_ = t.f6_;
    f7_ = t.f7_;
    f8_ = t.f8_;
    f9_ = t.f9_;
    return *this;
  }

  T0 f0_;
  T1 f1_;
  T2 f2_;
  T3 f3_;
  T4 f4_;
  T5 f5_;
  T6 f6_;
  T7 f7_;
  T8 f8_;
  T9 f9_;
};

inline tuple<> make_tuple() { return tuple<>(); }

template <GTEST_1_TYPENAMES_(T)>
inline GTEST_1_TUPLE_(T) make_tuple(const T0& f0) {
  return GTEST_1_TUPLE_(T)(f0);
}

template <GTEST_2_TYPENAMES_(T)>
inline GTEST_2_TUPLE_(T) make_tuple(const T0& f0, const T1& f1) {
  return GTEST_2_TUPLE_(T)(f0, f1);
}

template <GTEST_3_TYPENAMES_(T)>
inline GTEST_3_TUPLE_(T) make_tuple(const T0& f0, const T1& f1, const T2& f2) {
  return GTEST_3_TUPLE_(T)(f0, f1, f2);
}

template <GTEST_4_TYPENAMES_(T)>
inline GTEST_4_TUPLE_(T) make_tuple(const T0& f0, const T1& f1, const T2& f2,
    const T3& f3) {
  return GTEST_4_TUPLE_(T)(f0, f1, f2, f3);
}

template <GTEST_5_TYPENAMES_(T)>
inline GTEST_5_TUPLE_(T) make_tuple(const T0& f0, const T1& f1, const T2& f2,
    const T3& f3, const T4& f4) {
  return GTEST_5_TUPLE_(T)(f0, f1, f2, f3, f4);
}

template <GTEST_6_TYPENAMES_(T)>
inline GTEST_6_TUPLE_(T) make_tuple(const T0& f0, const T1& f1, const T2& f2,
    const T3& f3, const T4& f4, const T5& f5) {
  return GTEST_6_TUPLE_(T)(f0, f1, f2, f3, f4, f5);
}

template <GTEST_7_TYPENAMES_(T)>
inline GTEST_7_TUPLE_(T) make_tuple(const T0& f0, const T1& f1, const T2& f2,
    const T3& f3, const T4& f4, const T5& f5, const T6& f6) {
  return GTEST_7_TUPLE_(T)(f0, f1, f2, f3, f4, f5, f6);
}

template <GTEST_8_TYPENAMES_(T)>
inline GTEST_8_TUPLE_(T) make_tuple(const T0& f0, const T1& f1, const T2& f2,
    const T3& f3, const T4& f4, const T5& f5, const T6& f6, const T7& f7) {
  return GTEST_8_TUPLE_(T)(f0, f1, f2, f3, f4, f5, f6, f7);
}

template <GTEST_9_TYPENAMES_(T)>
inline GTEST_9_TUPLE_(T) make_tuple(const T0& f0, const T1& f1, const T2& f2,
    const T3& f3, const T4& f4, const T5& f5, const T6& f6, const T7& f7,
    const T8& f8) {
  return GTEST_9_TUPLE_(T)(f0, f1, f2, f3, f4, f5, f6, f7, f8);
}

template <GTEST_10_TYPENAMES_(T)>
inline GTEST_10_TUPLE_(T) make_tuple(const T0& f0, const T1& f1, const T2& f2,
    const T3& f3, const T4& f4, const T5& f5, const T6& f6, const T7& f7,
    const T8& f8, const T9& f9) {
  return GTEST_10_TUPLE_(T)(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9);
}

template <typename Tuple> struct tuple_size;

template <GTEST_0_TYPENAMES_(T)>
struct tuple_size<GTEST_0_TUPLE_(T) > {
  static const int value = 0;
};

template <GTEST_1_TYPENAMES_(T)>
struct tuple_size<GTEST_1_TUPLE_(T) > {
  static const int value = 1;
};

template <GTEST_2_TYPENAMES_(T)>
struct tuple_size<GTEST_2_TUPLE_(T) > {
  static const int value = 2;
};

template <GTEST_3_TYPENAMES_(T)>
struct tuple_size<GTEST_3_TUPLE_(T) > {
  static const int value = 3;
};

template <GTEST_4_TYPENAMES_(T)>
struct tuple_size<GTEST_4_TUPLE_(T) > {
  static const int value = 4;
};

template <GTEST_5_TYPENAMES_(T)>
struct tuple_size<GTEST_5_TUPLE_(T) > {
  static const int value = 5;
};

template <GTEST_6_TYPENAMES_(T)>
struct tuple_size<GTEST_6_TUPLE_(T) > {
  static const int value = 6;
};

template <GTEST_7_TYPENAMES_(T)>
struct tuple_size<GTEST_7_TUPLE_(T) > {
  static const int value = 7;
};

template <GTEST_8_TYPENAMES_(T)>
struct tuple_size<GTEST_8_TUPLE_(T) > {
  static const int value = 8;
};

template <GTEST_9_TYPENAMES_(T)>
struct tuple_size<GTEST_9_TUPLE_(T) > {
  static const int value = 9;
};

template <GTEST_10_TYPENAMES_(T)>
struct tuple_size<GTEST_10_TUPLE_(T) > {
  static const int value = 10;
};

template <int k, class Tuple>
struct tuple_element {
  typedef typename gtest_internal::TupleElement<
      k < (tuple_size<Tuple>::value), k, Tuple>::type type;
};

#define GTEST_TUPLE_ELEMENT_(k, Tuple) typename tuple_element<k, Tuple >::type

namespace gtest_internal {

template <>
class Get<0> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(0, Tuple))
  Field(Tuple& t) { return t.f0_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(0, Tuple))
  ConstField(const Tuple& t) { return t.f0_; }
};

template <>
class Get<1> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(1, Tuple))
  Field(Tuple& t) { return t.f1_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(1, Tuple))
  ConstField(const Tuple& t) { return t.f1_; }
};

template <>
class Get<2> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(2, Tuple))
  Field(Tuple& t) { return t.f2_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(2, Tuple))
  ConstField(const Tuple& t) { return t.f2_; }
};

template <>
class Get<3> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(3, Tuple))
  Field(Tuple& t) { return t.f3_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(3, Tuple))
  ConstField(const Tuple& t) { return t.f3_; }
};

template <>
class Get<4> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(4, Tuple))
  Field(Tuple& t) { return t.f4_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(4, Tuple))
  ConstField(const Tuple& t) { return t.f4_; }
};

template <>
class Get<5> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(5, Tuple))
  Field(Tuple& t) { return t.f5_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(5, Tuple))
  ConstField(const Tuple& t) { return t.f5_; }
};

template <>
class Get<6> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(6, Tuple))
  Field(Tuple& t) { return t.f6_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(6, Tuple))
  ConstField(const Tuple& t) { return t.f6_; }
};

template <>
class Get<7> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(7, Tuple))
  Field(Tuple& t) { return t.f7_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(7, Tuple))
  ConstField(const Tuple& t) { return t.f7_; }
};

template <>
class Get<8> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(8, Tuple))
  Field(Tuple& t) { return t.f8_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(8, Tuple))
  ConstField(const Tuple& t) { return t.f8_; }
};

template <>
class Get<9> {
 public:
  template <class Tuple>
  static GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(9, Tuple))
  Field(Tuple& t) { return t.f9_; }  // NOLINT

  template <class Tuple>
  static GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(9, Tuple))
  ConstField(const Tuple& t) { return t.f9_; }
};

}  // namespace gtest_internal

template <int k, GTEST_10_TYPENAMES_(T)>
GTEST_ADD_REF_(GTEST_TUPLE_ELEMENT_(k, GTEST_10_TUPLE_(T)))
get(GTEST_10_TUPLE_(T)& t) {
  return gtest_internal::Get<k>::Field(t);
}

template <int k, GTEST_10_TYPENAMES_(T)>
GTEST_BY_REF_(GTEST_TUPLE_ELEMENT_(k,  GTEST_10_TUPLE_(T)))
get(const GTEST_10_TUPLE_(T)& t) {
  return gtest_internal::Get<k>::ConstField(t);
}

namespace gtest_internal {

template <int kSize1, int kSize2>
struct SameSizeTuplePrefixComparator;

template <>
struct SameSizeTuplePrefixComparator<0, 0> {
  template <class Tuple1, class Tuple2>
  static bool Eq(const Tuple1& /* t1 */, const Tuple2& /* t2 */) {
    return true;
  }
};

template <int k>
struct SameSizeTuplePrefixComparator<k, k> {
  template <class Tuple1, class Tuple2>
  static bool Eq(const Tuple1& t1, const Tuple2& t2) {
    return SameSizeTuplePrefixComparator<k - 1, k - 1>::Eq(t1, t2) &&
        ::std::tr1::get<k - 1>(t1) == ::std::tr1::get<k - 1>(t2);
  }
};

}  // namespace gtest_internal

template <GTEST_10_TYPENAMES_(T), GTEST_10_TYPENAMES_(U)>
inline bool operator==(const GTEST_10_TUPLE_(T)& t,
                       const GTEST_10_TUPLE_(U)& u) {
  return gtest_internal::SameSizeTuplePrefixComparator<
      tuple_size<GTEST_10_TUPLE_(T) >::value,
      tuple_size<GTEST_10_TUPLE_(U) >::value>::Eq(t, u);
}

template <GTEST_10_TYPENAMES_(T), GTEST_10_TYPENAMES_(U)>
inline bool operator!=(const GTEST_10_TUPLE_(T)& t,
                       const GTEST_10_TUPLE_(U)& u) { return !(t == u); }

}  // namespace tr1
}  // namespace std

#undef GTEST_0_TUPLE_
#undef GTEST_1_TUPLE_
#undef GTEST_2_TUPLE_
#undef GTEST_3_TUPLE_
#undef GTEST_4_TUPLE_
#undef GTEST_5_TUPLE_
#undef GTEST_6_TUPLE_
#undef GTEST_7_TUPLE_
#undef GTEST_8_TUPLE_
#undef GTEST_9_TUPLE_
#undef GTEST_10_TUPLE_

#undef GTEST_0_TYPENAMES_
#undef GTEST_1_TYPENAMES_
#undef GTEST_2_TYPENAMES_
#undef GTEST_3_TYPENAMES_
#undef GTEST_4_TYPENAMES_
#undef GTEST_5_TYPENAMES_
#undef GTEST_6_TYPENAMES_
#undef GTEST_7_TYPENAMES_
#undef GTEST_8_TYPENAMES_
#undef GTEST_9_TYPENAMES_
#undef GTEST_10_TYPENAMES_

#undef GTEST_DECLARE_TUPLE_AS_FRIEND_
#undef GTEST_BY_REF_
#undef GTEST_ADD_REF_
#undef GTEST_TUPLE_ELEMENT_

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_TUPLE_H_
# elif GTEST_ENV_HAS_STD_TUPLE_
#  include <tuple>

namespace std {
namespace tr1 {
using ::std::get;
using ::std::make_tuple;
using ::std::tuple;
using ::std::tuple_element;
using ::std::tuple_size;
}
}

# elif GTEST_OS_SYMBIAN

#  ifdef BOOST_HAS_TR1_TUPLE
#   undef BOOST_HAS_TR1_TUPLE
#  endif  // BOOST_HAS_TR1_TUPLE

#  define BOOST_TR1_DETAIL_CONFIG_HPP_INCLUDED
#  include <tuple>

# elif defined(__GNUC__) && (GTEST_GCC_VER_ >= 40000)

#  if !GTEST_HAS_RTTI && GTEST_GCC_VER_ < 40302

#   define _TR1_FUNCTIONAL 1
#   include <tr1/tuple>
#   undef _TR1_FUNCTIONAL  // Allows the user to #include

#  else
#   include <tr1/tuple>  // NOLINT
#  endif  // !GTEST_HAS_RTTI && GTEST_GCC_VER_ < 40302

# else

#  include <tuple>  // NOLINT
# endif  // GTEST_USE_OWN_TR1_TUPLE

#endif  // GTEST_HAS_TR1_TUPLE

#ifndef GTEST_HAS_CLONE

# if GTEST_OS_LINUX && !defined(__ia64__)
#  if GTEST_OS_LINUX_ANDROID

#    if defined(__arm__) && __ANDROID_API__ >= 9
#     define GTEST_HAS_CLONE 1
#    else
#     define GTEST_HAS_CLONE 0
#    endif
#  else
#   define GTEST_HAS_CLONE 1
#  endif
# else
#  define GTEST_HAS_CLONE 0
# endif  // GTEST_OS_LINUX && !defined(__ia64__)

#endif  // GTEST_HAS_CLONE

#ifndef GTEST_HAS_STREAM_REDIRECTION

# if GTEST_OS_WINDOWS_MOBILE || GTEST_OS_SYMBIAN
#  define GTEST_HAS_STREAM_REDIRECTION 0
# else
#  define GTEST_HAS_STREAM_REDIRECTION 1
# endif  // !GTEST_OS_WINDOWS_MOBILE && !GTEST_OS_SYMBIAN
#endif  // GTEST_HAS_STREAM_REDIRECTION

#if (GTEST_OS_LINUX || GTEST_OS_CYGWIN || GTEST_OS_SOLARIS || \
     (GTEST_OS_MAC && !GTEST_OS_IOS) || GTEST_OS_IOS_SIMULATOR || \
     (GTEST_OS_WINDOWS_DESKTOP && _MSC_VER >= 1400) || \
     GTEST_OS_WINDOWS_MINGW || GTEST_OS_AIX || GTEST_OS_HPUX || \
     GTEST_OS_OPENBSD || GTEST_OS_QNX)
# define GTEST_HAS_DEATH_TEST 1
# include <vector>  // NOLINT
#endif

#define GTEST_HAS_PARAM_TEST 1

#if defined(__GNUC__) || (_MSC_VER >= 1400) || defined(__SUNPRO_CC) || \
    defined(__IBMCPP__) || defined(__HP_aCC)
# define GTEST_HAS_TYPED_TEST 1
# define GTEST_HAS_TYPED_TEST_P 1
#endif

#if GTEST_HAS_PARAM_TEST && GTEST_HAS_TR1_TUPLE && !defined(__SUNPRO_CC)
# define GTEST_HAS_COMBINE 1
#endif

#define GTEST_WIDE_STRING_USES_UTF16_ \
    (GTEST_OS_WINDOWS || GTEST_OS_CYGWIN || GTEST_OS_SYMBIAN || GTEST_OS_AIX)

#if GTEST_OS_LINUX
# define GTEST_CAN_STREAM_RESULTS_ 1
#endif

#ifdef __INTEL_COMPILER
# define GTEST_AMBIGUOUS_ELSE_BLOCKER_
#else
# define GTEST_AMBIGUOUS_ELSE_BLOCKER_ switch (0) case 0: default:  // NOLINT
#endif

#if defined(__GNUC__) && !defined(COMPILER_ICC)
# define GTEST_ATTRIBUTE_UNUSED_ __attribute__ ((unused))
#else
# define GTEST_ATTRIBUTE_UNUSED_
#endif

#define GTEST_DISALLOW_ASSIGN_(type)\
  void operator=(type const &)

#define GTEST_DISALLOW_COPY_AND_ASSIGN_(type)\
  type(type const &);\
  GTEST_DISALLOW_ASSIGN_(type)

#if defined(__GNUC__) && (GTEST_GCC_VER_ >= 30400) && !defined(COMPILER_ICC)
# define GTEST_MUST_USE_RESULT_ __attribute__ ((warn_unused_result))
#else
# define GTEST_MUST_USE_RESULT_
#endif  // __GNUC__ && (GTEST_GCC_VER_ >= 30400) && !COMPILER_ICC

#ifndef GTEST_HAS_SEH

# if defined(_MSC_VER) || defined(__BORLANDC__)

#  define GTEST_HAS_SEH 1
# else

#  define GTEST_HAS_SEH 0
# endif

#endif  // GTEST_HAS_SEH

#ifdef _MSC_VER

# if GTEST_LINKED_AS_SHARED_LIBRARY
#  define GTEST_API_ __declspec(dllimport)
# elif GTEST_CREATE_SHARED_LIBRARY
#  define GTEST_API_ __declspec(dllexport)
# endif

#endif  // _MSC_VER

#ifndef GTEST_API_
# define GTEST_API_
#endif

#ifdef __GNUC__

# define GTEST_NO_INLINE_ __attribute__((noinline))
#else
# define GTEST_NO_INLINE_
#endif

#if defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
# define GTEST_HAS_CXXABI_H_ 1
#else
# define GTEST_HAS_CXXABI_H_ 0
#endif

namespace testing {

class Message;

namespace internal {

class Secret;

template <bool>
struct CompileAssert {
};

#define GTEST_COMPILE_ASSERT_(expr, msg) \
  typedef ::testing::internal::CompileAssert<(static_cast<bool>(expr))> \
      msg[static_cast<bool>(expr) ? 1 : -1] GTEST_ATTRIBUTE_UNUSED_

template <typename T1, typename T2>
struct StaticAssertTypeEqHelper;

template <typename T>
struct StaticAssertTypeEqHelper<T, T> {};

#if GTEST_HAS_GLOBAL_STRING
typedef ::string string;
#else
typedef ::std::string string;
#endif  // GTEST_HAS_GLOBAL_STRING

#if GTEST_HAS_GLOBAL_WSTRING
typedef ::wstring wstring;
#elif GTEST_HAS_STD_WSTRING
typedef ::std::wstring wstring;
#endif  // GTEST_HAS_GLOBAL_WSTRING

GTEST_API_ bool IsTrue(bool condition);

template <typename T>
class scoped_ptr {
 public:
  typedef T element_type;

  explicit scoped_ptr(T* p = NULL) : ptr_(p) {}
  ~scoped_ptr() { reset(); }

  T& operator*() const { return *ptr_; }
  T* operator->() const { return ptr_; }
  T* get() const { return ptr_; }

  T* release() {
    T* const ptr = ptr_;
    ptr_ = NULL;
    return ptr;
  }

  void reset(T* p = NULL) {
    if (p != ptr_) {
      if (IsTrue(sizeof(T) > 0)) {  // Makes sure T is a complete type.
        delete ptr_;
      }
      ptr_ = p;
    }
  }

 private:
  T* ptr_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(scoped_ptr);
};

class GTEST_API_ RE {
 public:

  RE(const RE& other) { Init(other.pattern()); }

  RE(const ::std::string& regex) { Init(regex.c_str()); }  // NOLINT

#if GTEST_HAS_GLOBAL_STRING

  RE(const ::string& regex) { Init(regex.c_str()); }  // NOLINT

#endif  // GTEST_HAS_GLOBAL_STRING

  RE(const char* regex) { Init(regex); }  // NOLINT
  ~RE();

  const char* pattern() const { return pattern_; }

  static bool FullMatch(const ::std::string& str, const RE& re) {
    return FullMatch(str.c_str(), re);
  }
  static bool PartialMatch(const ::std::string& str, const RE& re) {
    return PartialMatch(str.c_str(), re);
  }

#if GTEST_HAS_GLOBAL_STRING

  static bool FullMatch(const ::string& str, const RE& re) {
    return FullMatch(str.c_str(), re);
  }
  static bool PartialMatch(const ::string& str, const RE& re) {
    return PartialMatch(str.c_str(), re);
  }

#endif  // GTEST_HAS_GLOBAL_STRING

  static bool FullMatch(const char* str, const RE& re);
  static bool PartialMatch(const char* str, const RE& re);

 private:
  void Init(const char* regex);

  const char* pattern_;
  bool is_valid_;

#if GTEST_USES_POSIX_RE

  regex_t full_regex_;     // For FullMatch().
  regex_t partial_regex_;  // For PartialMatch().

#else  // GTEST_USES_SIMPLE_RE

  const char* full_pattern_;  // For FullMatch();

#endif

  GTEST_DISALLOW_ASSIGN_(RE);
};

GTEST_API_ ::std::string FormatFileLocation(const char* file, int line);

GTEST_API_ ::std::string FormatCompilerIndependentFileLocation(const char* file,
                                                               int line);

enum GTestLogSeverity {
  GTEST_INFO,
  GTEST_WARNING,
  GTEST_ERROR,
  GTEST_FATAL
};

class GTEST_API_ GTestLog {
 public:
  GTestLog(GTestLogSeverity severity, const char* file, int line);

  ~GTestLog();

  ::std::ostream& GetStream() { return ::std::cerr; }

 private:
  const GTestLogSeverity severity_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(GTestLog);
};

#define GTEST_LOG_(severity) \
    ::testing::internal::GTestLog(::testing::internal::GTEST_##severity, \
                                  __FILE__, __LINE__).GetStream()

inline void LogToStderr() {}
inline void FlushInfoLog() { fflush(NULL); }

#define GTEST_CHECK_(condition) \
    GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
    if (::testing::internal::IsTrue(condition)) \
      ; \
    else \
      GTEST_LOG_(FATAL) << "Condition " #condition " failed. "

#define GTEST_CHECK_POSIX_SUCCESS_(posix_call) \
  if (const int gtest_error = (posix_call)) \
    GTEST_LOG_(FATAL) << #posix_call << "failed with error " \
                      << gtest_error

template<typename To>
inline To ImplicitCast_(To x) { return x; }

template<typename To, typename From>  // use like this: DownCast_<T*>(foo);
inline To DownCast_(From* f) {  // so we only accept pointers

  if (false) {
    const To to = NULL;
    ::testing::internal::ImplicitCast_<From*>(to);
  }

#if GTEST_HAS_RTTI

  GTEST_CHECK_(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
  return static_cast<To>(f);
}

template <class Derived, class Base>
Derived* CheckedDowncastToActualType(Base* base) {
#if GTEST_HAS_RTTI
  GTEST_CHECK_(typeid(*base) == typeid(Derived));
  return dynamic_cast<Derived*>(base);  // NOLINT
#else
  return static_cast<Derived*>(base);  // Poor man's downcast.
#endif
}

#if GTEST_HAS_STREAM_REDIRECTION

GTEST_API_ void CaptureStdout();
GTEST_API_ std::string GetCapturedStdout();
GTEST_API_ void CaptureStderr();
GTEST_API_ std::string GetCapturedStderr();

#endif  // GTEST_HAS_STREAM_REDIRECTION

#if GTEST_HAS_DEATH_TEST

const ::std::vector<testing::internal::string>& GetInjectableArgvs();
void SetInjectableArgvs(const ::std::vector<testing::internal::string>*
                             new_argvs);

extern ::std::vector<testing::internal::string> g_argvs;

#endif  // GTEST_HAS_DEATH_TEST

#if GTEST_HAS_PTHREAD

inline void SleepMilliseconds(int n) {
  const timespec time = {
    0,                  // 0 seconds.
    n * 1000L * 1000L,  // And n ms.
  };
  nanosleep(&time, NULL);
}

class Notification {
 public:
  Notification() : notified_(false) {
    GTEST_CHECK_POSIX_SUCCESS_(pthread_mutex_init(&mutex_, NULL));
  }
  ~Notification() {
    pthread_mutex_destroy(&mutex_);
  }

  void Notify() {
    pthread_mutex_lock(&mutex_);
    notified_ = true;
    pthread_mutex_unlock(&mutex_);
  }

  void WaitForNotification() {
    for (;;) {
      pthread_mutex_lock(&mutex_);
      const bool notified = notified_;
      pthread_mutex_unlock(&mutex_);
      if (notified)
        break;
      SleepMilliseconds(10);
    }
  }

 private:
  pthread_mutex_t mutex_;
  bool notified_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(Notification);
};

class ThreadWithParamBase {
 public:
  virtual ~ThreadWithParamBase() {}
  virtual void Run() = 0;
};

extern "C" inline void* ThreadFuncWithCLinkage(void* thread) {
  static_cast<ThreadWithParamBase*>(thread)->Run();
  return NULL;
}

template <typename T>
class ThreadWithParam : public ThreadWithParamBase {
 public:
  typedef void (*UserThreadFunc)(T);

  ThreadWithParam(
      UserThreadFunc func, T param, Notification* thread_can_start)
      : func_(func),
        param_(param),
        thread_can_start_(thread_can_start),
        finished_(false) {
    ThreadWithParamBase* const base = this;

    GTEST_CHECK_POSIX_SUCCESS_(
        pthread_create(&thread_, 0, &ThreadFuncWithCLinkage, base));
  }
  ~ThreadWithParam() { Join(); }

  void Join() {
    if (!finished_) {
      GTEST_CHECK_POSIX_SUCCESS_(pthread_join(thread_, 0));
      finished_ = true;
    }
  }

  virtual void Run() {
    if (thread_can_start_ != NULL)
      thread_can_start_->WaitForNotification();
    func_(param_);
  }

 private:
  const UserThreadFunc func_;  // User-supplied thread function.
  const T param_;  // User-supplied parameter to the thread function.

  Notification* const thread_can_start_;
  bool finished_;  // true iff we know that the thread function has finished.
  pthread_t thread_;  // The native thread object.

  GTEST_DISALLOW_COPY_AND_ASSIGN_(ThreadWithParam);
};

class MutexBase {
 public:

  void Lock() {
    GTEST_CHECK_POSIX_SUCCESS_(pthread_mutex_lock(&mutex_));
    owner_ = pthread_self();
    has_owner_ = true;
  }

  void Unlock() {

    has_owner_ = false;
    GTEST_CHECK_POSIX_SUCCESS_(pthread_mutex_unlock(&mutex_));
  }

  void AssertHeld() const {
    GTEST_CHECK_(has_owner_ && pthread_equal(owner_, pthread_self()))
        << "The current thread is not holding the mutex @" << this;
  }

 public:
  pthread_mutex_t mutex_;  // The underlying pthread mutex.

  bool has_owner_;
  pthread_t owner_;  // The thread holding the mutex.
};

# define GTEST_DECLARE_STATIC_MUTEX_(mutex) \
    extern ::testing::internal::MutexBase mutex

# define GTEST_DEFINE_STATIC_MUTEX_(mutex) \
    ::testing::internal::MutexBase mutex = { PTHREAD_MUTEX_INITIALIZER, false }

class Mutex : public MutexBase {
 public:
  Mutex() {
    GTEST_CHECK_POSIX_SUCCESS_(pthread_mutex_init(&mutex_, NULL));
    has_owner_ = false;
  }
  ~Mutex() {
    GTEST_CHECK_POSIX_SUCCESS_(pthread_mutex_destroy(&mutex_));
  }

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN_(Mutex);
};

class GTestMutexLock {
 public:
  explicit GTestMutexLock(MutexBase* mutex)
      : mutex_(mutex) { mutex_->Lock(); }

  ~GTestMutexLock() { mutex_->Unlock(); }

 private:
  MutexBase* const mutex_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(GTestMutexLock);
};

typedef GTestMutexLock MutexLock;

class ThreadLocalValueHolderBase {
 public:
  virtual ~ThreadLocalValueHolderBase() {}
};

extern "C" inline void DeleteThreadLocalValue(void* value_holder) {
  delete static_cast<ThreadLocalValueHolderBase*>(value_holder);
}

template <typename T>
class ThreadLocal {
 public:
  ThreadLocal() : key_(CreateKey()),
                  default_() {}
  explicit ThreadLocal(const T& value) : key_(CreateKey()),
                                         default_(value) {}

  ~ThreadLocal() {

    DeleteThreadLocalValue(pthread_getspecific(key_));

    GTEST_CHECK_POSIX_SUCCESS_(pthread_key_delete(key_));
  }

  T* pointer() { return GetOrCreateValue(); }
  const T* pointer() const { return GetOrCreateValue(); }
  const T& get() const { return *pointer(); }
  void set(const T& value) { *pointer() = value; }

 private:

  class ValueHolder : public ThreadLocalValueHolderBase {
   public:
    explicit ValueHolder(const T& value) : value_(value) {}

    T* pointer() { return &value_; }

   private:
    T value_;
    GTEST_DISALLOW_COPY_AND_ASSIGN_(ValueHolder);
  };

  static pthread_key_t CreateKey() {
    pthread_key_t key;

    GTEST_CHECK_POSIX_SUCCESS_(
        pthread_key_create(&key, &DeleteThreadLocalValue));
    return key;
  }

  T* GetOrCreateValue() const {
    ThreadLocalValueHolderBase* const holder =
        static_cast<ThreadLocalValueHolderBase*>(pthread_getspecific(key_));
    if (holder != NULL) {
      return CheckedDowncastToActualType<ValueHolder>(holder)->pointer();
    }

    ValueHolder* const new_holder = new ValueHolder(default_);
    ThreadLocalValueHolderBase* const holder_base = new_holder;
    GTEST_CHECK_POSIX_SUCCESS_(pthread_setspecific(key_, holder_base));
    return new_holder->pointer();
  }

  const pthread_key_t key_;
  const T default_;  // The default value for each thread.

  GTEST_DISALLOW_COPY_AND_ASSIGN_(ThreadLocal);
};

# define GTEST_IS_THREADSAFE 1

#else  // GTEST_HAS_PTHREAD

class Mutex {
 public:
  Mutex() {}
  void Lock() {}
  void Unlock() {}
  void AssertHeld() const {}
};

# define GTEST_DECLARE_STATIC_MUTEX_(mutex) \
  extern ::testing::internal::Mutex mutex

# define GTEST_DEFINE_STATIC_MUTEX_(mutex) ::testing::internal::Mutex mutex

class GTestMutexLock {
 public:
  explicit GTestMutexLock(Mutex*) {}  // NOLINT
};

typedef GTestMutexLock MutexLock;

template <typename T>
class ThreadLocal {
 public:
  ThreadLocal() : value_() {}
  explicit ThreadLocal(const T& value) : value_(value) {}
  T* pointer() { return &value_; }
  const T* pointer() const { return &value_; }
  const T& get() const { return value_; }
  void set(const T& value) { value_ = value; }
 private:
  T value_;
};

# define GTEST_IS_THREADSAFE 0

#endif  // GTEST_HAS_PTHREAD

GTEST_API_ size_t GetThreadCount();

#if defined(__SYMBIAN32__) || defined(__IBMCPP__) || defined(__SUNPRO_CC)

# define GTEST_ELLIPSIS_NEEDS_POD_ 1
#else
# define GTEST_CAN_COMPARE_NULL 1
#endif

#if defined(__SYMBIAN32__) || defined(__IBMCPP__)
# define GTEST_NEEDS_IS_POINTER_ 1
#endif

template <bool bool_value>
struct bool_constant {
  typedef bool_constant<bool_value> type;
  static const bool value = bool_value;
};
template <bool bool_value> const bool bool_constant<bool_value>::value;

typedef bool_constant<false> false_type;
typedef bool_constant<true> true_type;

template <typename T>
struct is_pointer : public false_type {};

template <typename T>
struct is_pointer<T*> : public true_type {};

template <typename Iterator>
struct IteratorTraits {
  typedef typename Iterator::value_type value_type;
};

template <typename T>
struct IteratorTraits<T*> {
  typedef T value_type;
};

template <typename T>
struct IteratorTraits<const T*> {
  typedef T value_type;
};

#if GTEST_OS_WINDOWS
# define GTEST_PATH_SEP_ "\\"
# define GTEST_HAS_ALT_PATH_SEP_ 1

typedef __int64 BiggestInt;
#else
# define GTEST_PATH_SEP_ "/"
# define GTEST_HAS_ALT_PATH_SEP_ 0
typedef long long BiggestInt;  // NOLINT
#endif  // GTEST_OS_WINDOWS

inline bool IsAlpha(char ch) {
  return isalpha(static_cast<unsigned char>(ch)) != 0;
}
inline bool IsAlNum(char ch) {
  return isalnum(static_cast<unsigned char>(ch)) != 0;
}
inline bool IsDigit(char ch) {
  return isdigit(static_cast<unsigned char>(ch)) != 0;
}
inline bool IsLower(char ch) {
  return islower(static_cast<unsigned char>(ch)) != 0;
}
inline bool IsSpace(char ch) {
  return isspace(static_cast<unsigned char>(ch)) != 0;
}
inline bool IsUpper(char ch) {
  return isupper(static_cast<unsigned char>(ch)) != 0;
}
inline bool IsXDigit(char ch) {
  return isxdigit(static_cast<unsigned char>(ch)) != 0;
}
inline bool IsXDigit(wchar_t ch) {
  const unsigned char low_byte = static_cast<unsigned char>(ch);
  return ch == low_byte && isxdigit(low_byte) != 0;
}

inline char ToLower(char ch) {
  return static_cast<char>(tolower(static_cast<unsigned char>(ch)));
}
inline char ToUpper(char ch) {
  return static_cast<char>(toupper(static_cast<unsigned char>(ch)));
}

namespace posix {

#if GTEST_OS_WINDOWS

typedef struct _stat StatStruct;

# ifdef __BORLANDC__
inline int IsATTY(int fd) { return isatty(fd); }
inline int StrCaseCmp(const char* s1, const char* s2) {
  return stricmp(s1, s2);
}
inline char* StrDup(const char* src) { return strdup(src); }
# else  // !__BORLANDC__
#  if GTEST_OS_WINDOWS_MOBILE
inline int IsATTY(int /* fd */) { return 0; }
#  else
inline int IsATTY(int fd) { return _isatty(fd); }
#  endif  // GTEST_OS_WINDOWS_MOBILE
inline int StrCaseCmp(const char* s1, const char* s2) {
  return _stricmp(s1, s2);
}
inline char* StrDup(const char* src) { return _strdup(src); }
# endif  // __BORLANDC__

# if GTEST_OS_WINDOWS_MOBILE
inline int FileNo(FILE* file) { return reinterpret_cast<int>(_fileno(file)); }

# else
inline int FileNo(FILE* file) { return _fileno(file); }
inline int Stat(const char* path, StatStruct* buf) { return _stat(path, buf); }
inline int RmDir(const char* dir) { return _rmdir(dir); }
inline bool IsDir(const StatStruct& st) {
  return (_S_IFDIR & st.st_mode) != 0;
}
# endif  // GTEST_OS_WINDOWS_MOBILE

#else

typedef struct stat StatStruct;

inline int FileNo(FILE* file) { return fileno(file); }
inline int IsATTY(int fd) { return isatty(fd); }
inline int Stat(const char* path, StatStruct* buf) { return stat(path, buf); }
inline int StrCaseCmp(const char* s1, const char* s2) {
  return strcasecmp(s1, s2);
}
inline char* StrDup(const char* src) { return strdup(src); }
inline int RmDir(const char* dir) { return rmdir(dir); }
inline bool IsDir(const StatStruct& st) { return S_ISDIR(st.st_mode); }

#endif  // GTEST_OS_WINDOWS

#ifdef _MSC_VER

# pragma warning(push)
# pragma warning(disable:4996)
#endif

inline const char* StrNCpy(char* dest, const char* src, size_t n) {
  return strncpy(dest, src, n);
}

#if !GTEST_OS_WINDOWS_MOBILE
inline int ChDir(const char* dir) { return chdir(dir); }
#endif
inline FILE* FOpen(const char* path, const char* mode) {
  return fopen(path, mode);
}
#if !GTEST_OS_WINDOWS_MOBILE
inline FILE *FReopen(const char* path, const char* mode, FILE* stream) {
  return freopen(path, mode, stream);
}
inline FILE* FDOpen(int fd, const char* mode) { return fdopen(fd, mode); }
#endif
inline int FClose(FILE* fp) { return fclose(fp); }
#if !GTEST_OS_WINDOWS_MOBILE
inline int Read(int fd, void* buf, unsigned int count) {
  return static_cast<int>(read(fd, buf, count));
}
inline int Write(int fd, const void* buf, unsigned int count) {
  return static_cast<int>(write(fd, buf, count));
}
inline int Close(int fd) { return close(fd); }
inline const char* StrError(int errnum) { return strerror(errnum); }
#endif
inline const char* GetEnv(const char* name) {
#if GTEST_OS_WINDOWS_MOBILE

  return NULL;
#elif defined(__BORLANDC__) || defined(__SunOS_5_8) || defined(__SunOS_5_9)

  const char* const env = getenv(name);
  return (env != NULL && env[0] != '\0') ? env : NULL;
#else
  return getenv(name);
#endif
}

#ifdef _MSC_VER
# pragma warning(pop)  // Restores the warning state.
#endif

#if GTEST_OS_WINDOWS_MOBILE

void Abort();
#else
inline void Abort() { abort(); }
#endif  // GTEST_OS_WINDOWS_MOBILE

}  // namespace posix

#if _MSC_VER >= 1400 && !GTEST_OS_WINDOWS_MOBILE

# define GTEST_SNPRINTF_(buffer, size, format, ...) \
     _snprintf_s(buffer, size, size, format, __VA_ARGS__)
#elif defined(_MSC_VER)

# define GTEST_SNPRINTF_ _snprintf
#else
# define GTEST_SNPRINTF_ snprintf
#endif

const BiggestInt kMaxBiggestInt =
    ~(static_cast<BiggestInt>(1) << (8*sizeof(BiggestInt) - 1));

template <size_t size>
class TypeWithSize {
 public:

  typedef void UInt;
};

template <>
class TypeWithSize<4> {
 public:

  typedef int Int;
  typedef unsigned int UInt;
};

template <>
class TypeWithSize<8> {
 public:
#if GTEST_OS_WINDOWS
  typedef __int64 Int;
  typedef unsigned __int64 UInt;
#else
  typedef long long Int;  // NOLINT
  typedef unsigned long long UInt;  // NOLINT
#endif  // GTEST_OS_WINDOWS
};

typedef TypeWithSize<4>::Int Int32;
typedef TypeWithSize<4>::UInt UInt32;
typedef TypeWithSize<8>::Int Int64;
typedef TypeWithSize<8>::UInt UInt64;
typedef TypeWithSize<8>::Int TimeInMillis;  // Represents time in milliseconds.

#define GTEST_FLAG(name) FLAGS_gtest_##name

#define GTEST_DECLARE_bool_(name) GTEST_API_ extern bool GTEST_FLAG(name)
#define GTEST_DECLARE_int32_(name) \
    GTEST_API_ extern ::testing::internal::Int32 GTEST_FLAG(name)
#define GTEST_DECLARE_string_(name) \
    GTEST_API_ extern ::std::string GTEST_FLAG(name)

#define GTEST_DEFINE_bool_(name, default_val, doc) \
    GTEST_API_ bool GTEST_FLAG(name) = (default_val)
#define GTEST_DEFINE_int32_(name, default_val, doc) \
    GTEST_API_ ::testing::internal::Int32 GTEST_FLAG(name) = (default_val)
#define GTEST_DEFINE_string_(name, default_val, doc) \
    GTEST_API_ ::std::string GTEST_FLAG(name) = (default_val)

#define GTEST_EXCLUSIVE_LOCK_REQUIRED_(locks)
#define GTEST_LOCK_EXCLUDED_(locks)

bool ParseInt32(const Message& src_text, const char* str, Int32* value);

bool BoolFromGTestEnv(const char* flag, bool default_val);
GTEST_API_ Int32 Int32FromGTestEnv(const char* flag, Int32 default_val);
const char* StringFromGTestEnv(const char* flag, const char* default_val);

}  // namespace internal
}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PORT_H_

#if GTEST_OS_LINUX
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
#endif  // GTEST_OS_LINUX

#if GTEST_HAS_EXCEPTIONS
# include <stdexcept>
#endif

#include <ctype.h>
#include <float.h>
#include <string.h>
#include <iomanip>
#include <limits>
#include <set>

#ifndef GTEST_INCLUDE_GTEST_GTEST_MESSAGE_H_
#define GTEST_INCLUDE_GTEST_GTEST_MESSAGE_H_

#include <limits>

void operator<<(const testing::internal::Secret&, int);

namespace testing {

class GTEST_API_ Message {
 private:

  typedef std::ostream& (*BasicNarrowIoManip)(std::ostream&);

 public:

  Message();

  Message(const Message& msg) : ss_(new ::std::stringstream) {  // NOLINT
    *ss_ << msg.GetString();
  }

  explicit Message(const char* str) : ss_(new ::std::stringstream) {
    *ss_ << str;
  }

#if GTEST_OS_SYMBIAN

  template <typename T>
  inline Message& operator <<(const T& value) {
    StreamHelper(typename internal::is_pointer<T>::type(), value);
    return *this;
  }
#else

  template <typename T>
  inline Message& operator <<(const T& val) {

    using ::operator <<;
    *ss_ << val;
    return *this;
  }

  template <typename T>
  inline Message& operator <<(T* const& pointer) {  // NOLINT
    if (pointer == NULL) {
      *ss_ << "(null)";
    } else {
      *ss_ << pointer;
    }
    return *this;
  }
#endif  // GTEST_OS_SYMBIAN

  Message& operator <<(BasicNarrowIoManip val) {
    *ss_ << val;
    return *this;
  }

  Message& operator <<(bool b) {
    return *this << (b ? "true" : "false");
  }

  Message& operator <<(const wchar_t* wide_c_str);
  Message& operator <<(wchar_t* wide_c_str);

#if GTEST_HAS_STD_WSTRING

  Message& operator <<(const ::std::wstring& wstr);
#endif  // GTEST_HAS_STD_WSTRING

#if GTEST_HAS_GLOBAL_WSTRING

  Message& operator <<(const ::wstring& wstr);
#endif  // GTEST_HAS_GLOBAL_WSTRING

  std::string GetString() const;

 private:

#if GTEST_OS_SYMBIAN

  template <typename T>
  inline void StreamHelper(internal::true_type /*is_pointer*/, T* pointer) {
    if (pointer == NULL) {
      *ss_ << "(null)";
    } else {
      *ss_ << pointer;
    }
  }
  template <typename T>
  inline void StreamHelper(internal::false_type /*is_pointer*/,
                           const T& value) {

    using ::operator <<;
    *ss_ << value;
  }
#endif  // GTEST_OS_SYMBIAN

  const internal::scoped_ptr< ::std::stringstream> ss_;

  void operator=(const Message&);
};

inline std::ostream& operator <<(std::ostream& os, const Message& sb) {
  return os << sb.GetString();
}

namespace internal {

template <typename T>
std::string StreamableToString(const T& streamable) {
  return (Message() << streamable).GetString();
}

}  // namespace internal
}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_GTEST_MESSAGE_H_

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_STRING_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_STRING_H_

#ifdef __BORLANDC__

# include <mem.h>
#endif

#include <string.h>
#include <string>

namespace testing {
namespace internal {

class GTEST_API_ String {
 public:

  static const char* CloneCString(const char* c_str);

#if GTEST_OS_WINDOWS_MOBILE

  static LPCWSTR AnsiToUtf16(const char* c_str);

  static const char* Utf16ToAnsi(LPCWSTR utf16_str);
#endif

  static bool CStringEquals(const char* lhs, const char* rhs);

  static std::string ShowWideCString(const wchar_t* wide_c_str);

  static bool WideCStringEquals(const wchar_t* lhs, const wchar_t* rhs);

  static bool CaseInsensitiveCStringEquals(const char* lhs,
                                           const char* rhs);

  static bool CaseInsensitiveWideCStringEquals(const wchar_t* lhs,
                                               const wchar_t* rhs);

  static bool EndsWithCaseInsensitive(
      const std::string& str, const std::string& suffix);

  static std::string FormatIntWidth2(int value);  // "%02d" for width == 2

  static std::string FormatHexInt(int value);

  static std::string FormatByte(unsigned char value);

 private:
  String();  // Not meant to be instantiated.
};  // class String

GTEST_API_ std::string StringStreamToString(::std::stringstream* stream);

}  // namespace internal
}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_STRING_H_

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_FILEPATH_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_FILEPATH_H_

namespace testing {
namespace internal {

class GTEST_API_ FilePath {
 public:
  FilePath() : pathname_("") { }
  FilePath(const FilePath& rhs) : pathname_(rhs.pathname_) { }

  explicit FilePath(const std::string& pathname) : pathname_(pathname) {
    Normalize();
  }

  FilePath& operator=(const FilePath& rhs) {
    Set(rhs);
    return *this;
  }

  void Set(const FilePath& rhs) {
    pathname_ = rhs.pathname_;
  }

  const std::string& string() const { return pathname_; }
  const char* c_str() const { return pathname_.c_str(); }

  static FilePath GetCurrentDir();

  static FilePath MakeFileName(const FilePath& directory,
                               const FilePath& base_name,
                               int number,
                               const char* extension);

  static FilePath ConcatPaths(const FilePath& directory,
                              const FilePath& relative_path);

  static FilePath GenerateUniqueFileName(const FilePath& directory,
                                         const FilePath& base_name,
                                         const char* extension);

  bool IsEmpty() const { return pathname_.empty(); }

  FilePath RemoveTrailingPathSeparator() const;

  FilePath RemoveDirectoryName() const;

  FilePath RemoveFileName() const;

  FilePath RemoveExtension(const char* extension) const;

  bool CreateDirectoriesRecursively() const;

  bool CreateFolder() const;

  bool FileOrDirectoryExists() const;

  bool DirectoryExists() const;

  bool IsDirectory() const;

  bool IsRootDirectory() const;

  bool IsAbsolutePath() const;

 private:

  void Normalize();

  const char* FindLastPathSeparator() const;

  std::string pathname_;
};  // class FilePath

}  // namespace internal
}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_FILEPATH_H_

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_TYPE_UTIL_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_TYPE_UTIL_H_

# if GTEST_HAS_CXXABI_H_
#  include <cxxabi.h>
# elif defined(__HP_aCC)
#  include <acxx_demangle.h>
# endif  // GTEST_HASH_CXXABI_H_

namespace testing {
namespace internal {

template <typename T>
std::string GetTypeName() {
# if GTEST_HAS_RTTI

  const char* const name = typeid(T).name();
#  if GTEST_HAS_CXXABI_H_ || defined(__HP_aCC)
  int status = 0;

#   if GTEST_HAS_CXXABI_H_
  using abi::__cxa_demangle;
#   endif  // GTEST_HAS_CXXABI_H_
  char* const readable_name = __cxa_demangle(name, 0, 0, &status);
  const std::string name_str(status == 0 ? readable_name : name);
  free(readable_name);
  return name_str;
#  else
  return name;
#  endif  // GTEST_HAS_CXXABI_H_ || __HP_aCC

# else

  return "<type>";

# endif  // GTEST_HAS_RTTI
}

#if GTEST_HAS_TYPED_TEST || GTEST_HAS_TYPED_TEST_P

template <typename T1, typename T2>
struct AssertTypeEq;

template <typename T>
struct AssertTypeEq<T, T> {
  typedef bool type;
};

struct None {};

struct Types0 {};

template <typename T1>
struct Types1 {
  typedef T1 Head;
  typedef Types0 Tail;
};
template <typename T1, typename T2>
struct Types2 {
  typedef T1 Head;
  typedef Types1<T2> Tail;
};

template <typename T1, typename T2, typename T3>
struct Types3 {
  typedef T1 Head;
  typedef Types2<T2, T3> Tail;
};

template <typename T1, typename T2, typename T3, typename T4>
struct Types4 {
  typedef T1 Head;
  typedef Types3<T2, T3, T4> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct Types5 {
  typedef T1 Head;
  typedef Types4<T2, T3, T4, T5> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6>
struct Types6 {
  typedef T1 Head;
  typedef Types5<T2, T3, T4, T5, T6> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7>
struct Types7 {
  typedef T1 Head;
  typedef Types6<T2, T3, T4, T5, T6, T7> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8>
struct Types8 {
  typedef T1 Head;
  typedef Types7<T2, T3, T4, T5, T6, T7, T8> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9>
struct Types9 {
  typedef T1 Head;
  typedef Types8<T2, T3, T4, T5, T6, T7, T8, T9> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10>
struct Types10 {
  typedef T1 Head;
  typedef Types9<T2, T3, T4, T5, T6, T7, T8, T9, T10> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11>
struct Types11 {
  typedef T1 Head;
  typedef Types10<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12>
struct Types12 {
  typedef T1 Head;
  typedef Types11<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13>
struct Types13 {
  typedef T1 Head;
  typedef Types12<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14>
struct Types14 {
  typedef T1 Head;
  typedef Types13<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15>
struct Types15 {
  typedef T1 Head;
  typedef Types14<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16>
struct Types16 {
  typedef T1 Head;
  typedef Types15<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17>
struct Types17 {
  typedef T1 Head;
  typedef Types16<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18>
struct Types18 {
  typedef T1 Head;
  typedef Types17<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19>
struct Types19 {
  typedef T1 Head;
  typedef Types18<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20>
struct Types20 {
  typedef T1 Head;
  typedef Types19<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21>
struct Types21 {
  typedef T1 Head;
  typedef Types20<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22>
struct Types22 {
  typedef T1 Head;
  typedef Types21<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23>
struct Types23 {
  typedef T1 Head;
  typedef Types22<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24>
struct Types24 {
  typedef T1 Head;
  typedef Types23<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25>
struct Types25 {
  typedef T1 Head;
  typedef Types24<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26>
struct Types26 {
  typedef T1 Head;
  typedef Types25<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27>
struct Types27 {
  typedef T1 Head;
  typedef Types26<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28>
struct Types28 {
  typedef T1 Head;
  typedef Types27<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29>
struct Types29 {
  typedef T1 Head;
  typedef Types28<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30>
struct Types30 {
  typedef T1 Head;
  typedef Types29<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31>
struct Types31 {
  typedef T1 Head;
  typedef Types30<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32>
struct Types32 {
  typedef T1 Head;
  typedef Types31<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33>
struct Types33 {
  typedef T1 Head;
  typedef Types32<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34>
struct Types34 {
  typedef T1 Head;
  typedef Types33<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35>
struct Types35 {
  typedef T1 Head;
  typedef Types34<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36>
struct Types36 {
  typedef T1 Head;
  typedef Types35<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37>
struct Types37 {
  typedef T1 Head;
  typedef Types36<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38>
struct Types38 {
  typedef T1 Head;
  typedef Types37<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39>
struct Types39 {
  typedef T1 Head;
  typedef Types38<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40>
struct Types40 {
  typedef T1 Head;
  typedef Types39<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41>
struct Types41 {
  typedef T1 Head;
  typedef Types40<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42>
struct Types42 {
  typedef T1 Head;
  typedef Types41<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43>
struct Types43 {
  typedef T1 Head;
  typedef Types42<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44>
struct Types44 {
  typedef T1 Head;
  typedef Types43<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
      T44> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45>
struct Types45 {
  typedef T1 Head;
  typedef Types44<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
      T44, T45> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46>
struct Types46 {
  typedef T1 Head;
  typedef Types45<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
      T44, T45, T46> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47>
struct Types47 {
  typedef T1 Head;
  typedef Types46<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
      T44, T45, T46, T47> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48>
struct Types48 {
  typedef T1 Head;
  typedef Types47<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
      T44, T45, T46, T47, T48> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48, typename T49>
struct Types49 {
  typedef T1 Head;
  typedef Types48<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
      T44, T45, T46, T47, T48, T49> Tail;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48, typename T49, typename T50>
struct Types50 {
  typedef T1 Head;
  typedef Types49<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
      T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
      T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
      T44, T45, T46, T47, T48, T49, T50> Tail;
};

}  // namespace internal

template <typename T1 = internal::None, typename T2 = internal::None,
    typename T3 = internal::None, typename T4 = internal::None,
    typename T5 = internal::None, typename T6 = internal::None,
    typename T7 = internal::None, typename T8 = internal::None,
    typename T9 = internal::None, typename T10 = internal::None,
    typename T11 = internal::None, typename T12 = internal::None,
    typename T13 = internal::None, typename T14 = internal::None,
    typename T15 = internal::None, typename T16 = internal::None,
    typename T17 = internal::None, typename T18 = internal::None,
    typename T19 = internal::None, typename T20 = internal::None,
    typename T21 = internal::None, typename T22 = internal::None,
    typename T23 = internal::None, typename T24 = internal::None,
    typename T25 = internal::None, typename T26 = internal::None,
    typename T27 = internal::None, typename T28 = internal::None,
    typename T29 = internal::None, typename T30 = internal::None,
    typename T31 = internal::None, typename T32 = internal::None,
    typename T33 = internal::None, typename T34 = internal::None,
    typename T35 = internal::None, typename T36 = internal::None,
    typename T37 = internal::None, typename T38 = internal::None,
    typename T39 = internal::None, typename T40 = internal::None,
    typename T41 = internal::None, typename T42 = internal::None,
    typename T43 = internal::None, typename T44 = internal::None,
    typename T45 = internal::None, typename T46 = internal::None,
    typename T47 = internal::None, typename T48 = internal::None,
    typename T49 = internal::None, typename T50 = internal::None>
struct Types {
  typedef internal::Types50<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43, T44, T45, T46, T47, T48, T49, T50> type;
};

template <>
struct Types<internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types0 type;
};
template <typename T1>
struct Types<T1, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types1<T1> type;
};
template <typename T1, typename T2>
struct Types<T1, T2, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types2<T1, T2> type;
};
template <typename T1, typename T2, typename T3>
struct Types<T1, T2, T3, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types3<T1, T2, T3> type;
};
template <typename T1, typename T2, typename T3, typename T4>
struct Types<T1, T2, T3, T4, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types4<T1, T2, T3, T4> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct Types<T1, T2, T3, T4, T5, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types5<T1, T2, T3, T4, T5> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6>
struct Types<T1, T2, T3, T4, T5, T6, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types6<T1, T2, T3, T4, T5, T6> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7>
struct Types<T1, T2, T3, T4, T5, T6, T7, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types7<T1, T2, T3, T4, T5, T6, T7> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types8<T1, T2, T3, T4, T5, T6, T7, T8> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types9<T1, T2, T3, T4, T5, T6, T7, T8, T9> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types11<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types12<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types13<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types14<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types15<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types16<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types17<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types18<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types19<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types20<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types21<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types22<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types23<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types24<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types25<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types26<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types27<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types28<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types29<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types30<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types31<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types32<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types33<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types34<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types35<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types36<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types37<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types38<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types39<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types40<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types41<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, internal::None,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types42<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None, internal::None> {
  typedef internal::Types43<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44,
    internal::None, internal::None, internal::None, internal::None,
    internal::None, internal::None> {
  typedef internal::Types44<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43, T44> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45,
    internal::None, internal::None, internal::None, internal::None,
    internal::None> {
  typedef internal::Types45<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43, T44, T45> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45,
    T46, internal::None, internal::None, internal::None, internal::None> {
  typedef internal::Types46<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43, T44, T45, T46> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45,
    T46, T47, internal::None, internal::None, internal::None> {
  typedef internal::Types47<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43, T44, T45, T46, T47> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45,
    T46, T47, T48, internal::None, internal::None> {
  typedef internal::Types48<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43, T44, T45, T46, T47, T48> type;
};
template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48, typename T49>
struct Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
    T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
    T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45,
    T46, T47, T48, T49, internal::None> {
  typedef internal::Types49<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43, T44, T45, T46, T47, T48, T49> type;
};

namespace internal {

# define GTEST_TEMPLATE_ template <typename T> class

template <GTEST_TEMPLATE_ Tmpl>
struct TemplateSel {
  template <typename T>
  struct Bind {
    typedef Tmpl<T> type;
  };
};

# define GTEST_BIND_(TmplSel, T) \
  TmplSel::template Bind<T>::type

template <typename T>
struct NoneT {};

struct Templates0 {};

template <GTEST_TEMPLATE_ T1>
struct Templates1 {
  typedef TemplateSel<T1> Head;
  typedef Templates0 Tail;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2>
struct Templates2 {
  typedef TemplateSel<T1> Head;
  typedef Templates1<T2> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3>
struct Templates3 {
  typedef TemplateSel<T1> Head;
  typedef Templates2<T2, T3> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4>
struct Templates4 {
  typedef TemplateSel<T1> Head;
  typedef Templates3<T2, T3, T4> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5>
struct Templates5 {
  typedef TemplateSel<T1> Head;
  typedef Templates4<T2, T3, T4, T5> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6>
struct Templates6 {
  typedef TemplateSel<T1> Head;
  typedef Templates5<T2, T3, T4, T5, T6> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7>
struct Templates7 {
  typedef TemplateSel<T1> Head;
  typedef Templates6<T2, T3, T4, T5, T6, T7> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8>
struct Templates8 {
  typedef TemplateSel<T1> Head;
  typedef Templates7<T2, T3, T4, T5, T6, T7, T8> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9>
struct Templates9 {
  typedef TemplateSel<T1> Head;
  typedef Templates8<T2, T3, T4, T5, T6, T7, T8, T9> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10>
struct Templates10 {
  typedef TemplateSel<T1> Head;
  typedef Templates9<T2, T3, T4, T5, T6, T7, T8, T9, T10> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11>
struct Templates11 {
  typedef TemplateSel<T1> Head;
  typedef Templates10<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12>
struct Templates12 {
  typedef TemplateSel<T1> Head;
  typedef Templates11<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13>
struct Templates13 {
  typedef TemplateSel<T1> Head;
  typedef Templates12<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14>
struct Templates14 {
  typedef TemplateSel<T1> Head;
  typedef Templates13<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15>
struct Templates15 {
  typedef TemplateSel<T1> Head;
  typedef Templates14<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16>
struct Templates16 {
  typedef TemplateSel<T1> Head;
  typedef Templates15<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17>
struct Templates17 {
  typedef TemplateSel<T1> Head;
  typedef Templates16<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18>
struct Templates18 {
  typedef TemplateSel<T1> Head;
  typedef Templates17<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19>
struct Templates19 {
  typedef TemplateSel<T1> Head;
  typedef Templates18<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20>
struct Templates20 {
  typedef TemplateSel<T1> Head;
  typedef Templates19<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21>
struct Templates21 {
  typedef TemplateSel<T1> Head;
  typedef Templates20<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22>
struct Templates22 {
  typedef TemplateSel<T1> Head;
  typedef Templates21<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23>
struct Templates23 {
  typedef TemplateSel<T1> Head;
  typedef Templates22<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24>
struct Templates24 {
  typedef TemplateSel<T1> Head;
  typedef Templates23<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25>
struct Templates25 {
  typedef TemplateSel<T1> Head;
  typedef Templates24<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26>
struct Templates26 {
  typedef TemplateSel<T1> Head;
  typedef Templates25<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27>
struct Templates27 {
  typedef TemplateSel<T1> Head;
  typedef Templates26<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28>
struct Templates28 {
  typedef TemplateSel<T1> Head;
  typedef Templates27<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29>
struct Templates29 {
  typedef TemplateSel<T1> Head;
  typedef Templates28<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30>
struct Templates30 {
  typedef TemplateSel<T1> Head;
  typedef Templates29<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31>
struct Templates31 {
  typedef TemplateSel<T1> Head;
  typedef Templates30<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32>
struct Templates32 {
  typedef TemplateSel<T1> Head;
  typedef Templates31<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33>
struct Templates33 {
  typedef TemplateSel<T1> Head;
  typedef Templates32<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34>
struct Templates34 {
  typedef TemplateSel<T1> Head;
  typedef Templates33<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35>
struct Templates35 {
  typedef TemplateSel<T1> Head;
  typedef Templates34<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36>
struct Templates36 {
  typedef TemplateSel<T1> Head;
  typedef Templates35<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37>
struct Templates37 {
  typedef TemplateSel<T1> Head;
  typedef Templates36<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38>
struct Templates38 {
  typedef TemplateSel<T1> Head;
  typedef Templates37<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39>
struct Templates39 {
  typedef TemplateSel<T1> Head;
  typedef Templates38<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40>
struct Templates40 {
  typedef TemplateSel<T1> Head;
  typedef Templates39<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41>
struct Templates41 {
  typedef TemplateSel<T1> Head;
  typedef Templates40<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42>
struct Templates42 {
  typedef TemplateSel<T1> Head;
  typedef Templates41<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43>
struct Templates43 {
  typedef TemplateSel<T1> Head;
  typedef Templates42<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44>
struct Templates44 {
  typedef TemplateSel<T1> Head;
  typedef Templates43<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43, T44> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45>
struct Templates45 {
  typedef TemplateSel<T1> Head;
  typedef Templates44<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43, T44, T45> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46>
struct Templates46 {
  typedef TemplateSel<T1> Head;
  typedef Templates45<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43, T44, T45, T46> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46, GTEST_TEMPLATE_ T47>
struct Templates47 {
  typedef TemplateSel<T1> Head;
  typedef Templates46<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43, T44, T45, T46, T47> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46, GTEST_TEMPLATE_ T47, GTEST_TEMPLATE_ T48>
struct Templates48 {
  typedef TemplateSel<T1> Head;
  typedef Templates47<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43, T44, T45, T46, T47, T48> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46, GTEST_TEMPLATE_ T47, GTEST_TEMPLATE_ T48,
    GTEST_TEMPLATE_ T49>
struct Templates49 {
  typedef TemplateSel<T1> Head;
  typedef Templates48<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43, T44, T45, T46, T47, T48, T49> Tail;
};

template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46, GTEST_TEMPLATE_ T47, GTEST_TEMPLATE_ T48,
    GTEST_TEMPLATE_ T49, GTEST_TEMPLATE_ T50>
struct Templates50 {
  typedef TemplateSel<T1> Head;
  typedef Templates49<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
      T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
      T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
      T43, T44, T45, T46, T47, T48, T49, T50> Tail;
};

template <GTEST_TEMPLATE_ T1 = NoneT, GTEST_TEMPLATE_ T2 = NoneT,
    GTEST_TEMPLATE_ T3 = NoneT, GTEST_TEMPLATE_ T4 = NoneT,
    GTEST_TEMPLATE_ T5 = NoneT, GTEST_TEMPLATE_ T6 = NoneT,
    GTEST_TEMPLATE_ T7 = NoneT, GTEST_TEMPLATE_ T8 = NoneT,
    GTEST_TEMPLATE_ T9 = NoneT, GTEST_TEMPLATE_ T10 = NoneT,
    GTEST_TEMPLATE_ T11 = NoneT, GTEST_TEMPLATE_ T12 = NoneT,
    GTEST_TEMPLATE_ T13 = NoneT, GTEST_TEMPLATE_ T14 = NoneT,
    GTEST_TEMPLATE_ T15 = NoneT, GTEST_TEMPLATE_ T16 = NoneT,
    GTEST_TEMPLATE_ T17 = NoneT, GTEST_TEMPLATE_ T18 = NoneT,
    GTEST_TEMPLATE_ T19 = NoneT, GTEST_TEMPLATE_ T20 = NoneT,
    GTEST_TEMPLATE_ T21 = NoneT, GTEST_TEMPLATE_ T22 = NoneT,
    GTEST_TEMPLATE_ T23 = NoneT, GTEST_TEMPLATE_ T24 = NoneT,
    GTEST_TEMPLATE_ T25 = NoneT, GTEST_TEMPLATE_ T26 = NoneT,
    GTEST_TEMPLATE_ T27 = NoneT, GTEST_TEMPLATE_ T28 = NoneT,
    GTEST_TEMPLATE_ T29 = NoneT, GTEST_TEMPLATE_ T30 = NoneT,
    GTEST_TEMPLATE_ T31 = NoneT, GTEST_TEMPLATE_ T32 = NoneT,
    GTEST_TEMPLATE_ T33 = NoneT, GTEST_TEMPLATE_ T34 = NoneT,
    GTEST_TEMPLATE_ T35 = NoneT, GTEST_TEMPLATE_ T36 = NoneT,
    GTEST_TEMPLATE_ T37 = NoneT, GTEST_TEMPLATE_ T38 = NoneT,
    GTEST_TEMPLATE_ T39 = NoneT, GTEST_TEMPLATE_ T40 = NoneT,
    GTEST_TEMPLATE_ T41 = NoneT, GTEST_TEMPLATE_ T42 = NoneT,
    GTEST_TEMPLATE_ T43 = NoneT, GTEST_TEMPLATE_ T44 = NoneT,
    GTEST_TEMPLATE_ T45 = NoneT, GTEST_TEMPLATE_ T46 = NoneT,
    GTEST_TEMPLATE_ T47 = NoneT, GTEST_TEMPLATE_ T48 = NoneT,
    GTEST_TEMPLATE_ T49 = NoneT, GTEST_TEMPLATE_ T50 = NoneT>
struct Templates {
  typedef Templates50<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42, T43, T44, T45, T46, T47, T48, T49, T50> type;
};

template <>
struct Templates<NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT> {
  typedef Templates0 type;
};
template <GTEST_TEMPLATE_ T1>
struct Templates<T1, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT> {
  typedef Templates1<T1> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2>
struct Templates<T1, T2, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT> {
  typedef Templates2<T1, T2> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3>
struct Templates<T1, T2, T3, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates3<T1, T2, T3> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4>
struct Templates<T1, T2, T3, T4, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates4<T1, T2, T3, T4> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5>
struct Templates<T1, T2, T3, T4, T5, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates5<T1, T2, T3, T4, T5> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6>
struct Templates<T1, T2, T3, T4, T5, T6, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates6<T1, T2, T3, T4, T5, T6> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7>
struct Templates<T1, T2, T3, T4, T5, T6, T7, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates7<T1, T2, T3, T4, T5, T6, T7> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates8<T1, T2, T3, T4, T5, T6, T7, T8> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates9<T1, T2, T3, T4, T5, T6, T7, T8, T9> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates11<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates12<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates13<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates14<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates15<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates16<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates17<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT> {
  typedef Templates18<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT> {
  typedef Templates19<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT> {
  typedef Templates20<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT> {
  typedef Templates21<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT> {
  typedef Templates22<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT> {
  typedef Templates23<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT> {
  typedef Templates24<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT> {
  typedef Templates25<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT> {
  typedef Templates26<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT> {
  typedef Templates27<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT> {
  typedef Templates28<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT> {
  typedef Templates29<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates30<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates31<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates32<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates33<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates34<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates35<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates36<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, NoneT, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates37<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, NoneT, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates38<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates39<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, NoneT, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates40<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, NoneT, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates41<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, NoneT,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates42<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates43<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42, T43> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44,
    NoneT, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates44<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42, T43, T44> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44,
    T45, NoneT, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates45<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42, T43, T44, T45> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44,
    T45, T46, NoneT, NoneT, NoneT, NoneT> {
  typedef Templates46<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42, T43, T44, T45, T46> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46, GTEST_TEMPLATE_ T47>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44,
    T45, T46, T47, NoneT, NoneT, NoneT> {
  typedef Templates47<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42, T43, T44, T45, T46, T47> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46, GTEST_TEMPLATE_ T47, GTEST_TEMPLATE_ T48>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44,
    T45, T46, T47, T48, NoneT, NoneT> {
  typedef Templates48<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42, T43, T44, T45, T46, T47, T48> type;
};
template <GTEST_TEMPLATE_ T1, GTEST_TEMPLATE_ T2, GTEST_TEMPLATE_ T3,
    GTEST_TEMPLATE_ T4, GTEST_TEMPLATE_ T5, GTEST_TEMPLATE_ T6,
    GTEST_TEMPLATE_ T7, GTEST_TEMPLATE_ T8, GTEST_TEMPLATE_ T9,
    GTEST_TEMPLATE_ T10, GTEST_TEMPLATE_ T11, GTEST_TEMPLATE_ T12,
    GTEST_TEMPLATE_ T13, GTEST_TEMPLATE_ T14, GTEST_TEMPLATE_ T15,
    GTEST_TEMPLATE_ T16, GTEST_TEMPLATE_ T17, GTEST_TEMPLATE_ T18,
    GTEST_TEMPLATE_ T19, GTEST_TEMPLATE_ T20, GTEST_TEMPLATE_ T21,
    GTEST_TEMPLATE_ T22, GTEST_TEMPLATE_ T23, GTEST_TEMPLATE_ T24,
    GTEST_TEMPLATE_ T25, GTEST_TEMPLATE_ T26, GTEST_TEMPLATE_ T27,
    GTEST_TEMPLATE_ T28, GTEST_TEMPLATE_ T29, GTEST_TEMPLATE_ T30,
    GTEST_TEMPLATE_ T31, GTEST_TEMPLATE_ T32, GTEST_TEMPLATE_ T33,
    GTEST_TEMPLATE_ T34, GTEST_TEMPLATE_ T35, GTEST_TEMPLATE_ T36,
    GTEST_TEMPLATE_ T37, GTEST_TEMPLATE_ T38, GTEST_TEMPLATE_ T39,
    GTEST_TEMPLATE_ T40, GTEST_TEMPLATE_ T41, GTEST_TEMPLATE_ T42,
    GTEST_TEMPLATE_ T43, GTEST_TEMPLATE_ T44, GTEST_TEMPLATE_ T45,
    GTEST_TEMPLATE_ T46, GTEST_TEMPLATE_ T47, GTEST_TEMPLATE_ T48,
    GTEST_TEMPLATE_ T49>
struct Templates<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
    T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
    T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44,
    T45, T46, T47, T48, T49, NoneT> {
  typedef Templates49<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
      T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
      T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
      T42, T43, T44, T45, T46, T47, T48, T49> type;
};

template <typename T>
struct TypeList {
  typedef Types1<T> type;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48, typename T49, typename T50>
struct TypeList<Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    T44, T45, T46, T47, T48, T49, T50> > {
  typedef typename Types<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
      T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
      T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
      T41, T42, T43, T44, T45, T46, T47, T48, T49, T50>::type type;
};

#endif  // GTEST_HAS_TYPED_TEST || GTEST_HAS_TYPED_TEST_P

}  // namespace internal
}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_TYPE_UTIL_H_

#define GTEST_CONCAT_TOKEN_(foo, bar) GTEST_CONCAT_TOKEN_IMPL_(foo, bar)
#define GTEST_CONCAT_TOKEN_IMPL_(foo, bar) foo ## bar

class ProtocolMessage;
namespace proto2 { class Message; }

namespace testing {

class AssertionResult;                 // Result of an assertion.
class Message;                         // Represents a failure message.
class Test;                            // Represents a test.
class TestInfo;                        // Information about a test.
class TestPartResult;                  // Result of a test part.
class UnitTest;                        // A collection of test cases.

template <typename T>
::std::string PrintToString(const T& value);

namespace internal {

struct TraceInfo;                      // Information about a trace point.
class ScopedTrace;                     // Implements scoped trace.
class TestInfoImpl;                    // Opaque implementation of TestInfo
class UnitTestImpl;                    // Opaque implementation of UnitTest

GTEST_API_ extern int g_init_gtest_count;

GTEST_API_ extern const char kStackTraceMarker[];

char IsNullLiteralHelper(Secret* p);
char (&IsNullLiteralHelper(...))[2];  // NOLINT

#ifdef GTEST_ELLIPSIS_NEEDS_POD_

# define GTEST_IS_NULL_LITERAL_(x) false
#else
# define GTEST_IS_NULL_LITERAL_(x) \
    (sizeof(::testing::internal::IsNullLiteralHelper(x)) == 1)
#endif  // GTEST_ELLIPSIS_NEEDS_POD_

GTEST_API_ std::string AppendUserMessage(
    const std::string& gtest_msg, const Message& user_msg);

#if GTEST_HAS_EXCEPTIONS

class GTEST_API_ GoogleTestFailureException : public ::std::runtime_error {
 public:
  explicit GoogleTestFailureException(const TestPartResult& failure);
};

#endif  // GTEST_HAS_EXCEPTIONS

class GTEST_API_ ScopedTrace {
 public:

  ScopedTrace(const char* file, int line, const Message& message);

  ~ScopedTrace();

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN_(ScopedTrace);
} GTEST_ATTRIBUTE_UNUSED_;  // A ScopedTrace object does its job in its

GTEST_API_ AssertionResult EqFailure(const char* expected_expression,
                                     const char* actual_expression,
                                     const std::string& expected_value,
                                     const std::string& actual_value,
                                     bool ignoring_case);

GTEST_API_ std::string GetBoolAssertionFailureMessage(
    const AssertionResult& assertion_result,
    const char* expression_text,
    const char* actual_predicate_value,
    const char* expected_predicate_value);

template <typename RawType>
class FloatingPoint {
 public:

  typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;

  static const size_t kBitCount = 8*sizeof(RawType);

  static const size_t kFractionBitCount =
    std::numeric_limits<RawType>::digits - 1;

  static const size_t kExponentBitCount = kBitCount - 1 - kFractionBitCount;

  static const Bits kSignBitMask = static_cast<Bits>(1) << (kBitCount - 1);

  static const Bits kFractionBitMask =
    ~static_cast<Bits>(0) >> (kExponentBitCount + 1);

  static const Bits kExponentBitMask = ~(kSignBitMask | kFractionBitMask);

  static const size_t kMaxUlps = 4;

  explicit FloatingPoint(const RawType& x) { u_.value_ = x; }

  static RawType ReinterpretBits(const Bits bits) {
    FloatingPoint fp(0);
    fp.u_.bits_ = bits;
    return fp.u_.value_;
  }

  static RawType Infinity() {
    return ReinterpretBits(kExponentBitMask);
  }

  static RawType Max();

  const Bits &bits() const { return u_.bits_; }

  Bits exponent_bits() const { return kExponentBitMask & u_.bits_; }

  Bits fraction_bits() const { return kFractionBitMask & u_.bits_; }

  Bits sign_bit() const { return kSignBitMask & u_.bits_; }

  bool is_nan() const {

    return (exponent_bits() == kExponentBitMask) && (fraction_bits() != 0);
  }

  bool AlmostEquals(const FloatingPoint& rhs) const {

    if (is_nan() || rhs.is_nan()) return false;

    return DistanceBetweenSignAndMagnitudeNumbers(u_.bits_, rhs.u_.bits_)
        <= kMaxUlps;
  }

 private:

  union FloatingPointUnion {
    RawType value_;  // The raw floating-point number.
    Bits bits_;      // The bits that represent the number.
  };

  static Bits SignAndMagnitudeToBiased(const Bits &sam) {
    if (kSignBitMask & sam) {

      return ~sam + 1;
    } else {

      return kSignBitMask | sam;
    }
  }

  static Bits DistanceBetweenSignAndMagnitudeNumbers(const Bits &sam1,
                                                     const Bits &sam2) {
    const Bits biased1 = SignAndMagnitudeToBiased(sam1);
    const Bits biased2 = SignAndMagnitudeToBiased(sam2);
    return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
  }

  FloatingPointUnion u_;
};

template <>
inline float FloatingPoint<float>::Max() { return FLT_MAX; }
template <>
inline double FloatingPoint<double>::Max() { return DBL_MAX; }

typedef FloatingPoint<float> Float;
typedef FloatingPoint<double> Double;

typedef const void* TypeId;

template <typename T>
class TypeIdHelper {
 public:

  static bool dummy_;
};

template <typename T>
bool TypeIdHelper<T>::dummy_ = false;

template <typename T>
TypeId GetTypeId() {

  return &(TypeIdHelper<T>::dummy_);
}

GTEST_API_ TypeId GetTestTypeId();

class TestFactoryBase {
 public:
  virtual ~TestFactoryBase() {}

  virtual Test* CreateTest() = 0;

 protected:
  TestFactoryBase() {}

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestFactoryBase);
};

template <class TestClass>
class TestFactoryImpl : public TestFactoryBase {
 public:
  virtual Test* CreateTest() { return new TestClass; }
};

#if GTEST_OS_WINDOWS

GTEST_API_ AssertionResult IsHRESULTSuccess(const char* expr,
                                            long hr);  // NOLINT
GTEST_API_ AssertionResult IsHRESULTFailure(const char* expr,
                                            long hr);  // NOLINT

#endif  // GTEST_OS_WINDOWS

typedef void (*SetUpTestCaseFunc)();
typedef void (*TearDownTestCaseFunc)();

GTEST_API_ TestInfo* MakeAndRegisterTestInfo(
    const char* test_case_name,
    const char* name,
    const char* type_param,
    const char* value_param,
    TypeId fixture_class_id,
    SetUpTestCaseFunc set_up_tc,
    TearDownTestCaseFunc tear_down_tc,
    TestFactoryBase* factory);

GTEST_API_ bool SkipPrefix(const char* prefix, const char** pstr);

#if GTEST_HAS_TYPED_TEST || GTEST_HAS_TYPED_TEST_P

class GTEST_API_ TypedTestCasePState {
 public:
  TypedTestCasePState() : registered_(false) {}

  bool AddTestName(const char* file, int line, const char* case_name,
                   const char* test_name) {
    if (registered_) {
      fprintf(stderr, "%s Test %s must be defined before "
              "REGISTER_TYPED_TEST_CASE_P(%s, ...).\n",
              FormatFileLocation(file, line).c_str(), test_name, case_name);
      fflush(stderr);
      posix::Abort();
    }
    defined_test_names_.insert(test_name);
    return true;
  }

  const char* VerifyRegisteredTestNames(
      const char* file, int line, const char* registered_tests);

 private:
  bool registered_;
  ::std::set<const char*> defined_test_names_;
};

inline const char* SkipComma(const char* str) {
  const char* comma = strchr(str, ',');
  if (comma == NULL) {
    return NULL;
  }
  while (IsSpace(*(++comma))) {}
  return comma;
}

inline std::string GetPrefixUntilComma(const char* str) {
  const char* comma = strchr(str, ',');
  return comma == NULL ? str : std::string(str, comma);
}

template <GTEST_TEMPLATE_ Fixture, class TestSel, typename Types>
class TypeParameterizedTest {
 public:

  static bool Register(const char* prefix, const char* case_name,
                       const char* test_names, int index) {
    typedef typename Types::Head Type;
    typedef Fixture<Type> FixtureClass;
    typedef typename GTEST_BIND_(TestSel, Type) TestClass;

    MakeAndRegisterTestInfo(
        (std::string(prefix) + (prefix[0] == '\0' ? "" : "/") + case_name + "/"
         + StreamableToString(index)).c_str(),
        GetPrefixUntilComma(test_names).c_str(),
        GetTypeName<Type>().c_str(),
        NULL,  // No value parameter.
        GetTypeId<FixtureClass>(),
        TestClass::SetUpTestCase,
        TestClass::TearDownTestCase,
        new TestFactoryImpl<TestClass>);

    return TypeParameterizedTest<Fixture, TestSel, typename Types::Tail>
        ::Register(prefix, case_name, test_names, index + 1);
  }
};

template <GTEST_TEMPLATE_ Fixture, class TestSel>
class TypeParameterizedTest<Fixture, TestSel, Types0> {
 public:
  static bool Register(const char* /*prefix*/, const char* /*case_name*/,
                       const char* /*test_names*/, int /*index*/) {
    return true;
  }
};

template <GTEST_TEMPLATE_ Fixture, typename Tests, typename Types>
class TypeParameterizedTestCase {
 public:
  static bool Register(const char* prefix, const char* case_name,
                       const char* test_names) {
    typedef typename Tests::Head Head;

    TypeParameterizedTest<Fixture, Head, Types>::Register(
        prefix, case_name, test_names, 0);

    return TypeParameterizedTestCase<Fixture, typename Tests::Tail, Types>
        ::Register(prefix, case_name, SkipComma(test_names));
  }
};

template <GTEST_TEMPLATE_ Fixture, typename Types>
class TypeParameterizedTestCase<Fixture, Templates0, Types> {
 public:
  static bool Register(const char* /*prefix*/, const char* /*case_name*/,
                       const char* /*test_names*/) {
    return true;
  }
};

#endif  // GTEST_HAS_TYPED_TEST || GTEST_HAS_TYPED_TEST_P

GTEST_API_ std::string GetCurrentOsStackTraceExceptTop(
    UnitTest* unit_test, int skip_count);

GTEST_API_ bool AlwaysTrue();

inline bool AlwaysFalse() { return !AlwaysTrue(); }

struct GTEST_API_ ConstCharPtr {
  ConstCharPtr(const char* str) : value(str) {}
  operator bool() const { return true; }
  const char* value;
};

class GTEST_API_ Random {
 public:
  static const UInt32 kMaxRange = 1u << 31;

  explicit Random(UInt32 seed) : state_(seed) {}

  void Reseed(UInt32 seed) { state_ = seed; }

  UInt32 Generate(UInt32 range);

 private:
  UInt32 state_;
  GTEST_DISALLOW_COPY_AND_ASSIGN_(Random);
};

template <typename T1, typename T2>
struct CompileAssertTypesEqual;

template <typename T>
struct CompileAssertTypesEqual<T, T> {
};

template <typename T>
struct RemoveReference { typedef T type; };  // NOLINT
template <typename T>
struct RemoveReference<T&> { typedef T type; };  // NOLINT

#define GTEST_REMOVE_REFERENCE_(T) \
    typename ::testing::internal::RemoveReference<T>::type

template <typename T>
struct RemoveConst { typedef T type; };  // NOLINT
template <typename T>
struct RemoveConst<const T> { typedef T type; };  // NOLINT

template <typename T, size_t N>
struct RemoveConst<const T[N]> {
  typedef typename RemoveConst<T>::type type[N];
};

#if defined(_MSC_VER) && _MSC_VER < 1400

template <typename T, size_t N>
struct RemoveConst<T[N]> {
  typedef typename RemoveConst<T>::type type[N];
};
#endif

#define GTEST_REMOVE_CONST_(T) \
    typename ::testing::internal::RemoveConst<T>::type

#define GTEST_REMOVE_REFERENCE_AND_CONST_(T) \
    GTEST_REMOVE_CONST_(GTEST_REMOVE_REFERENCE_(T))

template <typename T>
struct AddReference { typedef T& type; };  // NOLINT
template <typename T>
struct AddReference<T&> { typedef T& type; };  // NOLINT

#define GTEST_ADD_REFERENCE_(T) \
    typename ::testing::internal::AddReference<T>::type

#define GTEST_REFERENCE_TO_CONST_(T) \
    GTEST_ADD_REFERENCE_(const GTEST_REMOVE_REFERENCE_(T))

template <typename From, typename To>
class ImplicitlyConvertible {
 private:

  static From MakeFrom();

  static char Helper(To);
  static char (&Helper(...))[2];  // NOLINT

 public:

#ifdef _MSC_VER
# pragma warning(push)          // Saves the current warning state.
# pragma warning(disable:4244)  // Temporarily disables warning 4244.

  static const bool value =
      sizeof(Helper(ImplicitlyConvertible::MakeFrom())) == 1;
# pragma warning(pop)           // Restores the warning state.
#elif defined(__BORLANDC__)

  static const bool value = __is_convertible(From, To);
#else
  static const bool value =
      sizeof(Helper(ImplicitlyConvertible::MakeFrom())) == 1;
#endif  // _MSV_VER
};
template <typename From, typename To>
const bool ImplicitlyConvertible<From, To>::value;

template <typename T>
struct IsAProtocolMessage
    : public bool_constant<
  ImplicitlyConvertible<const T*, const ::ProtocolMessage*>::value ||
  ImplicitlyConvertible<const T*, const ::proto2::Message*>::value> {
};

typedef int IsContainer;
template <class C>
IsContainer IsContainerTest(int /* dummy */,
                            typename C::iterator* /* it */ = NULL,
                            typename C::const_iterator* /* const_it */ = NULL) {
  return 0;
}

typedef char IsNotContainer;
template <class C>
IsNotContainer IsContainerTest(long /* dummy */) { return '\0'; }

template<bool> struct EnableIf;
template<> struct EnableIf<true> { typedef void type; };  // NOLINT

template <typename T, typename U>
bool ArrayEq(const T* lhs, size_t size, const U* rhs);

template <typename T, typename U>
inline bool ArrayEq(const T& lhs, const U& rhs) { return lhs == rhs; }

template <typename T, typename U, size_t N>
inline bool ArrayEq(const T(&lhs)[N], const U(&rhs)[N]) {
  return internal::ArrayEq(lhs, N, rhs);
}

template <typename T, typename U>
bool ArrayEq(const T* lhs, size_t size, const U* rhs) {
  for (size_t i = 0; i != size; i++) {
    if (!internal::ArrayEq(lhs[i], rhs[i]))
      return false;
  }
  return true;
}

template <typename Iter, typename Element>
Iter ArrayAwareFind(Iter begin, Iter end, const Element& elem) {
  for (Iter it = begin; it != end; ++it) {
    if (internal::ArrayEq(*it, elem))
      return it;
  }
  return end;
}

template <typename T, typename U>
void CopyArray(const T* from, size_t size, U* to);

template <typename T, typename U>
inline void CopyArray(const T& from, U* to) { *to = from; }

template <typename T, typename U, size_t N>
inline void CopyArray(const T(&from)[N], U(*to)[N]) {
  internal::CopyArray(from, N, *to);
}

template <typename T, typename U>
void CopyArray(const T* from, size_t size, U* to) {
  for (size_t i = 0; i != size; i++) {
    internal::CopyArray(from[i], to + i);
  }
}

enum RelationToSource {
  kReference,  // The NativeArray references the native array.
  kCopy        // The NativeArray makes a copy of the native array and

};

template <typename Element>
class NativeArray {
 public:

  typedef Element value_type;
  typedef Element* iterator;
  typedef const Element* const_iterator;

  NativeArray(const Element* array, size_t count, RelationToSource relation) {
    Init(array, count, relation);
  }

  NativeArray(const NativeArray& rhs) {
    Init(rhs.array_, rhs.size_, rhs.relation_to_source_);
  }

  ~NativeArray() {

    static_cast<void>(StaticAssertTypeEqHelper<Element,
        GTEST_REMOVE_REFERENCE_AND_CONST_(Element)>());
    if (relation_to_source_ == kCopy)
      delete[] array_;
  }

  size_t size() const { return size_; }
  const_iterator begin() const { return array_; }
  const_iterator end() const { return array_ + size_; }
  bool operator==(const NativeArray& rhs) const {
    return size() == rhs.size() &&
        ArrayEq(begin(), size(), rhs.begin());
  }

 private:

  void Init(const Element* array, size_t a_size, RelationToSource relation) {
    if (relation == kReference) {
      array_ = array;
    } else {
      Element* const copy = new Element[a_size];
      CopyArray(array, a_size, copy);
      array_ = copy;
    }
    size_ = a_size;
    relation_to_source_ = relation;
  }

  const Element* array_;
  size_t size_;
  RelationToSource relation_to_source_;

  GTEST_DISALLOW_ASSIGN_(NativeArray);
};

}  // namespace internal
}  // namespace testing

#define GTEST_MESSAGE_AT_(file, line, message, result_type) \
  ::testing::internal::AssertHelper(result_type, file, line, message) \
    = ::testing::Message()

#define GTEST_MESSAGE_(message, result_type) \
  GTEST_MESSAGE_AT_(__FILE__, __LINE__, message, result_type)

#define GTEST_FATAL_FAILURE_(message) \
  return GTEST_MESSAGE_(message, ::testing::TestPartResult::kFatalFailure)

#define GTEST_NONFATAL_FAILURE_(message) \
  GTEST_MESSAGE_(message, ::testing::TestPartResult::kNonFatalFailure)

#define GTEST_SUCCESS_(message) \
  GTEST_MESSAGE_(message, ::testing::TestPartResult::kSuccess)

#define GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement) \
  if (::testing::internal::AlwaysTrue()) { statement; }

#define GTEST_TEST_THROW_(statement, expected_exception, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::internal::ConstCharPtr gtest_msg = "") { \
    bool gtest_caught_expected = false; \
    try { \
      GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    } \
    catch (expected_exception const&) { \
      gtest_caught_expected = true; \
    } \
    catch (...) { \
      gtest_msg.value = \
          "Expected: " #statement " throws an exception of type " \
          #expected_exception ".\n  Actual: it throws a different type."; \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__); \
    } \
    if (!gtest_caught_expected) { \
      gtest_msg.value = \
          "Expected: " #statement " throws an exception of type " \
          #expected_exception ".\n  Actual: it throws nothing."; \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__); \
    } \
  } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testthrow_, __LINE__): \
      fail(gtest_msg.value)

#define GTEST_TEST_NO_THROW_(statement, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::internal::AlwaysTrue()) { \
    try { \
      GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    } \
    catch (...) { \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testnothrow_, __LINE__); \
    } \
  } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testnothrow_, __LINE__): \
      fail("Expected: " #statement " doesn't throw an exception.\n" \
           "  Actual: it throws.")

#define GTEST_TEST_ANY_THROW_(statement, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::internal::AlwaysTrue()) { \
    bool gtest_caught_any = false; \
    try { \
      GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    } \
    catch (...) { \
      gtest_caught_any = true; \
    } \
    if (!gtest_caught_any) { \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testanythrow_, __LINE__); \
    } \
  } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testanythrow_, __LINE__): \
      fail("Expected: " #statement " throws an exception.\n" \
           "  Actual: it doesn't.")

#define GTEST_TEST_BOOLEAN_(expression, text, actual, expected, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (const ::testing::AssertionResult gtest_ar_ = \
      ::testing::AssertionResult(expression)) \
    ; \
  else \
    fail(::testing::internal::GetBoolAssertionFailureMessage(\
        gtest_ar_, text, #actual, #expected).c_str())

#define GTEST_TEST_NO_FATAL_FAILURE_(statement, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::internal::AlwaysTrue()) { \
    ::testing::internal::HasNewFatalFailureHelper gtest_fatal_failure_checker; \
    GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    if (gtest_fatal_failure_checker.has_new_fatal_failure()) { \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testnofatal_, __LINE__); \
    } \
  } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testnofatal_, __LINE__): \
      fail("Expected: " #statement " doesn't generate new fatal " \
           "failures in the current thread.\n" \
           "  Actual: it does.")

#define GTEST_TEST_CLASS_NAME_(test_case_name, test_name) \
  test_case_name##_##test_name##_Test

#define GTEST_TEST_(test_case_name, test_name, parent_class, parent_id)\
class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) : public parent_class {\
 public:\
  GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() {}\
 private:\
  virtual void TestBody();\
  static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
  GTEST_DISALLOW_COPY_AND_ASSIGN_(\
      GTEST_TEST_CLASS_NAME_(test_case_name, test_name));\
};\
\
::testing::TestInfo* const GTEST_TEST_CLASS_NAME_(test_case_name, test_name)\
  ::test_info_ =\
    ::testing::internal::MakeAndRegisterTestInfo(\
        #test_case_name, #test_name, NULL, NULL, \
        (parent_id), \
        parent_class::SetUpTestCase, \
        parent_class::TearDownTestCase, \
        new ::testing::internal::TestFactoryImpl<\
            GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>);\
void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_INTERNAL_H_

#ifndef GTEST_INCLUDE_GTEST_GTEST_DEATH_TEST_H_
#define GTEST_INCLUDE_GTEST_GTEST_DEATH_TEST_H_

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_DEATH_TEST_INTERNAL_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_DEATH_TEST_INTERNAL_H_

#include <stdio.h>

namespace testing {
namespace internal {

GTEST_DECLARE_string_(internal_run_death_test);

const char kDeathTestStyleFlag[] = "death_test_style";
const char kDeathTestUseFork[] = "death_test_use_fork";
const char kInternalRunDeathTestFlag[] = "internal_run_death_test";

#if GTEST_HAS_DEATH_TEST

class GTEST_API_ DeathTest {
 public:

  static bool Create(const char* statement, const RE* regex,
                     const char* file, int line, DeathTest** test);
  DeathTest();
  virtual ~DeathTest() { }

  class ReturnSentinel {
   public:
    explicit ReturnSentinel(DeathTest* test) : test_(test) { }
    ~ReturnSentinel() { test_->Abort(TEST_ENCOUNTERED_RETURN_STATEMENT); }
   private:
    DeathTest* const test_;
    GTEST_DISALLOW_COPY_AND_ASSIGN_(ReturnSentinel);
  } GTEST_ATTRIBUTE_UNUSED_;

  enum TestRole { OVERSEE_TEST, EXECUTE_TEST };

  enum AbortReason {
    TEST_ENCOUNTERED_RETURN_STATEMENT,
    TEST_THREW_EXCEPTION,
    TEST_DID_NOT_DIE
  };

  virtual TestRole AssumeRole() = 0;

  virtual int Wait() = 0;

  virtual bool Passed(bool exit_status_ok) = 0;

  virtual void Abort(AbortReason reason) = 0;

  static const char* LastMessage();

  static void set_last_death_test_message(const std::string& message);

 private:

  static std::string last_death_test_message_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(DeathTest);
};

class DeathTestFactory {
 public:
  virtual ~DeathTestFactory() { }
  virtual bool Create(const char* statement, const RE* regex,
                      const char* file, int line, DeathTest** test) = 0;
};

class DefaultDeathTestFactory : public DeathTestFactory {
 public:
  virtual bool Create(const char* statement, const RE* regex,
                      const char* file, int line, DeathTest** test);
};

GTEST_API_ bool ExitedUnsuccessfully(int exit_status);

# if GTEST_HAS_EXCEPTIONS
#  define GTEST_EXECUTE_DEATH_TEST_STATEMENT_(statement, death_test) \
  try { \
    GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
  } catch (const ::std::exception& gtest_exception) { \
    fprintf(\
        stderr, \
        "\n%s: Caught std::exception-derived exception escaping the " \
        "death test statement. Exception message: %s\n", \
        ::testing::internal::FormatFileLocation(__FILE__, __LINE__).c_str(), \
        gtest_exception.what()); \
    fflush(stderr); \
    death_test->Abort(::testing::internal::DeathTest::TEST_THREW_EXCEPTION); \
  } catch (...) { \
    death_test->Abort(::testing::internal::DeathTest::TEST_THREW_EXCEPTION); \
  }

# else
#  define GTEST_EXECUTE_DEATH_TEST_STATEMENT_(statement, death_test) \
  GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement)

# endif

# define GTEST_DEATH_TEST_(statement, predicate, regex, fail) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::internal::AlwaysTrue()) { \
    const ::testing::internal::RE& gtest_regex = (regex); \
    ::testing::internal::DeathTest* gtest_dt; \
    if (!::testing::internal::DeathTest::Create(#statement, &gtest_regex, \
        __FILE__, __LINE__, &gtest_dt)) { \
      goto GTEST_CONCAT_TOKEN_(gtest_label_, __LINE__); \
    } \
    if (gtest_dt != NULL) { \
      ::testing::internal::scoped_ptr< ::testing::internal::DeathTest> \
          gtest_dt_ptr(gtest_dt); \
      switch (gtest_dt->AssumeRole()) { \
        case ::testing::internal::DeathTest::OVERSEE_TEST: \
          if (!gtest_dt->Passed(predicate(gtest_dt->Wait()))) { \
            goto GTEST_CONCAT_TOKEN_(gtest_label_, __LINE__); \
          } \
          break; \
        case ::testing::internal::DeathTest::EXECUTE_TEST: { \
          ::testing::internal::DeathTest::ReturnSentinel \
              gtest_sentinel(gtest_dt); \
          GTEST_EXECUTE_DEATH_TEST_STATEMENT_(statement, gtest_dt); \
          gtest_dt->Abort(::testing::internal::DeathTest::TEST_DID_NOT_DIE); \
          break; \
        } \
        default: \
          break; \
      } \
    } \
  } else \
    GTEST_CONCAT_TOKEN_(gtest_label_, __LINE__): \
      fail(::testing::internal::DeathTest::LastMessage())

# define GTEST_EXECUTE_STATEMENT_(statement, regex) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::internal::AlwaysTrue()) { \
     GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
  } else \
    ::testing::Message()

class InternalRunDeathTestFlag {
 public:
  InternalRunDeathTestFlag(const std::string& a_file,
                           int a_line,
                           int an_index,
                           int a_write_fd)
      : file_(a_file), line_(a_line), index_(an_index),
        write_fd_(a_write_fd) {}

  ~InternalRunDeathTestFlag() {
    if (write_fd_ >= 0)
      posix::Close(write_fd_);
  }

  const std::string& file() const { return file_; }
  int line() const { return line_; }
  int index() const { return index_; }
  int write_fd() const { return write_fd_; }

 private:
  std::string file_;
  int line_;
  int index_;
  int write_fd_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(InternalRunDeathTestFlag);
};

InternalRunDeathTestFlag* ParseInternalRunDeathTestFlag();

#else  // GTEST_HAS_DEATH_TEST

# define GTEST_UNSUPPORTED_DEATH_TEST_(statement, regex, terminator) \
    GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
    if (::testing::internal::AlwaysTrue()) { \
      GTEST_LOG_(WARNING) \
          << "Death tests are not supported on this platform.\n" \
          << "Statement '" #statement "' cannot be verified."; \
    } else if (::testing::internal::AlwaysFalse()) { \
      ::testing::internal::RE::PartialMatch(".*", (regex)); \
      GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
      terminator; \
    } else \
      ::testing::Message()

#endif  // GTEST_HAS_DEATH_TEST

}  // namespace internal
}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_DEATH_TEST_INTERNAL_H_

namespace testing {

GTEST_DECLARE_string_(death_test_style);

#if GTEST_HAS_DEATH_TEST

namespace internal {

GTEST_API_ bool InDeathTestChild();

}  // namespace internal

# define ASSERT_EXIT(statement, predicate, regex) \
    GTEST_DEATH_TEST_(statement, predicate, regex, GTEST_FATAL_FAILURE_)

# define EXPECT_EXIT(statement, predicate, regex) \
    GTEST_DEATH_TEST_(statement, predicate, regex, GTEST_NONFATAL_FAILURE_)

# define ASSERT_DEATH(statement, regex) \
    ASSERT_EXIT(statement, ::testing::internal::ExitedUnsuccessfully, regex)

# define EXPECT_DEATH(statement, regex) \
    EXPECT_EXIT(statement, ::testing::internal::ExitedUnsuccessfully, regex)

class GTEST_API_ ExitedWithCode {
 public:
  explicit ExitedWithCode(int exit_code);
  bool operator()(int exit_status) const;
 private:

  void operator=(const ExitedWithCode& other);

  const int exit_code_;
};

# if !GTEST_OS_WINDOWS

class GTEST_API_ KilledBySignal {
 public:
  explicit KilledBySignal(int signum);
  bool operator()(int exit_status) const;
 private:
  const int signum_;
};
# endif  // !GTEST_OS_WINDOWS

# ifdef NDEBUG

#  define EXPECT_DEBUG_DEATH(statement, regex) \
  GTEST_EXECUTE_STATEMENT_(statement, regex)

#  define ASSERT_DEBUG_DEATH(statement, regex) \
  GTEST_EXECUTE_STATEMENT_(statement, regex)

# else

#  define EXPECT_DEBUG_DEATH(statement, regex) \
  EXPECT_DEATH(statement, regex)

#  define ASSERT_DEBUG_DEATH(statement, regex) \
  ASSERT_DEATH(statement, regex)

# endif  // NDEBUG for EXPECT_DEBUG_DEATH
#endif  // GTEST_HAS_DEATH_TEST

#if GTEST_HAS_DEATH_TEST
# define EXPECT_DEATH_IF_SUPPORTED(statement, regex) \
    EXPECT_DEATH(statement, regex)
# define ASSERT_DEATH_IF_SUPPORTED(statement, regex) \
    ASSERT_DEATH(statement, regex)
#else
# define EXPECT_DEATH_IF_SUPPORTED(statement, regex) \
    GTEST_UNSUPPORTED_DEATH_TEST_(statement, regex, )
# define ASSERT_DEATH_IF_SUPPORTED(statement, regex) \
    GTEST_UNSUPPORTED_DEATH_TEST_(statement, regex, return)
#endif

}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_GTEST_DEATH_TEST_H_

#ifndef GTEST_INCLUDE_GTEST_GTEST_PARAM_TEST_H_
#define GTEST_INCLUDE_GTEST_GTEST_PARAM_TEST_H_

#if 0

class FooTest : public ::testing::TestWithParam<const char*> {

};

TEST_P(FooTest, DoesBlah) {

  EXPECT_TRUE(foo.Blah(GetParam()));
  ...
}

TEST_P(FooTest, HasBlahBlah) {
  ...
}

INSTANTIATE_TEST_CASE_P(InstantiationName,
                        FooTest,
                        Values("meeny", "miny", "moe"));

const char* pets[] = {"cat", "dog"};
INSTANTIATE_TEST_CASE_P(AnotherInstantiationName, FooTest, ValuesIn(pets));

class BaseTest : public ::testing::Test {

};

class DerivedTest : public BaseTest, public ::testing::WithParamInterface<int> {

};

TEST_F(BaseTest, HasFoo) {

}

TEST_P(DerivedTest, DoesBlah) {

  EXPECT_TRUE(foo.Blah(GetParam()));
}

#endif  // 0

#if !GTEST_OS_SYMBIAN
# include <utility>
#endif

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PARAM_UTIL_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PARAM_UTIL_H_

#include <iterator>
#include <utility>
#include <vector>

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_LINKED_PTR_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_LINKED_PTR_H_

#include <stdlib.h>
#include <assert.h>

namespace testing {
namespace internal {

GTEST_API_ GTEST_DECLARE_STATIC_MUTEX_(g_linked_ptr_mutex);

class linked_ptr_internal {
 public:

  void join_new() {
    next_ = this;
  }

  void join(linked_ptr_internal const* ptr)
      GTEST_LOCK_EXCLUDED_(g_linked_ptr_mutex) {
    MutexLock lock(&g_linked_ptr_mutex);

    linked_ptr_internal const* p = ptr;
    while (p->next_ != ptr) p = p->next_;
    p->next_ = this;
    next_ = ptr;
  }

  bool depart()
      GTEST_LOCK_EXCLUDED_(g_linked_ptr_mutex) {
    MutexLock lock(&g_linked_ptr_mutex);

    if (next_ == this) return true;
    linked_ptr_internal const* p = next_;
    while (p->next_ != this) p = p->next_;
    p->next_ = next_;
    return false;
  }

 private:
  mutable linked_ptr_internal const* next_;
};

template <typename T>
class linked_ptr {
 public:
  typedef T element_type;

  explicit linked_ptr(T* ptr = NULL) { capture(ptr); }
  ~linked_ptr() { depart(); }

  template <typename U> linked_ptr(linked_ptr<U> const& ptr) { copy(&ptr); }
  linked_ptr(linked_ptr const& ptr) {  // NOLINT
    assert(&ptr != this);
    copy(&ptr);
  }

  template <typename U> linked_ptr& operator=(linked_ptr<U> const& ptr) {
    depart();
    copy(&ptr);
    return *this;
  }

  linked_ptr& operator=(linked_ptr const& ptr) {
    if (&ptr != this) {
      depart();
      copy(&ptr);
    }
    return *this;
  }

  void reset(T* ptr = NULL) {
    depart();
    capture(ptr);
  }
  T* get() const { return value_; }
  T* operator->() const { return value_; }
  T& operator*() const { return *value_; }

  bool operator==(T* p) const { return value_ == p; }
  bool operator!=(T* p) const { return value_ != p; }
  template <typename U>
  bool operator==(linked_ptr<U> const& ptr) const {
    return value_ == ptr.get();
  }
  template <typename U>
  bool operator!=(linked_ptr<U> const& ptr) const {
    return value_ != ptr.get();
  }

 private:
  template <typename U>
  friend class linked_ptr;

  T* value_;
  linked_ptr_internal link_;

  void depart() {
    if (link_.depart()) delete value_;
  }

  void capture(T* ptr) {
    value_ = ptr;
    link_.join_new();
  }

  template <typename U> void copy(linked_ptr<U> const* ptr) {
    value_ = ptr->get();
    if (value_)
      link_.join(&ptr->link_);
    else
      link_.join_new();
  }
};

template<typename T> inline
bool operator==(T* ptr, const linked_ptr<T>& x) {
  return ptr == x.get();
}

template<typename T> inline
bool operator!=(T* ptr, const linked_ptr<T>& x) {
  return ptr != x.get();
}

template <typename T>
linked_ptr<T> make_linked_ptr(T* ptr) {
  return linked_ptr<T>(ptr);
}

}  // namespace internal
}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_LINKED_PTR_H_

#ifndef GTEST_INCLUDE_GTEST_GTEST_PRINTERS_H_
#define GTEST_INCLUDE_GTEST_GTEST_PRINTERS_H_

#include <ostream>  // NOLINT
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace testing {

namespace internal2 {

GTEST_API_ void PrintBytesInObjectTo(const unsigned char* obj_bytes,
                                     size_t count,
                                     ::std::ostream* os);

enum TypeKind {
  kProtobuf,              // a protobuf type
  kConvertibleToInteger,  // a type implicitly convertible to BiggestInt

  kOtherType              // anything else
};

template <typename T, TypeKind kTypeKind>
class TypeWithoutFormatter {
 public:

  static void PrintValue(const T& value, ::std::ostream* os) {
    PrintBytesInObjectTo(reinterpret_cast<const unsigned char*>(&value),
                         sizeof(value), os);
  }
};

const size_t kProtobufOneLinerMaxLength = 50;

template <typename T>
class TypeWithoutFormatter<T, kProtobuf> {
 public:
  static void PrintValue(const T& value, ::std::ostream* os) {
    const ::testing::internal::string short_str = value.ShortDebugString();
    const ::testing::internal::string pretty_str =
        short_str.length() <= kProtobufOneLinerMaxLength ?
        short_str : ("\n" + value.DebugString());
    *os << ("<" + pretty_str + ">");
  }
};

template <typename T>
class TypeWithoutFormatter<T, kConvertibleToInteger> {
 public:

  static void PrintValue(const T& value, ::std::ostream* os) {
    const internal::BiggestInt kBigInt = value;
    *os << kBigInt;
  }
};

template <typename Char, typename CharTraits, typename T>
::std::basic_ostream<Char, CharTraits>& operator<<(
    ::std::basic_ostream<Char, CharTraits>& os, const T& x) {
  TypeWithoutFormatter<T,
      (internal::IsAProtocolMessage<T>::value ? kProtobuf :
       internal::ImplicitlyConvertible<const T&, internal::BiggestInt>::value ?
       kConvertibleToInteger : kOtherType)>::PrintValue(x, &os);
  return os;
}

}  // namespace internal2
}  // namespace testing

namespace testing_internal {

template <typename T>
void DefaultPrintNonContainerTo(const T& value, ::std::ostream* os) {

  using namespace ::testing::internal2;  // NOLINT

  *os << value;
}

}  // namespace testing_internal

namespace testing {
namespace internal {

template <typename T>
class UniversalPrinter;

template <typename T>
void UniversalPrint(const T& value, ::std::ostream* os);

template <typename C>
void DefaultPrintTo(IsContainer /* dummy */,
                    false_type /* is not a pointer */,
                    const C& container, ::std::ostream* os) {
  const size_t kMaxCount = 32;  // The maximum number of elements to print.
  *os << '{';
  size_t count = 0;
  for (typename C::const_iterator it = container.begin();
       it != container.end(); ++it, ++count) {
    if (count > 0) {
      *os << ',';
      if (count == kMaxCount) {  // Enough has been printed.
        *os << " ...";
        break;
      }
    }
    *os << ' ';

    internal::UniversalPrint(*it, os);
  }

  if (count > 0) {
    *os << ' ';
  }
  *os << '}';
}

template <typename T>
void DefaultPrintTo(IsNotContainer /* dummy */,
                    true_type /* is a pointer */,
                    T* p, ::std::ostream* os) {
  if (p == NULL) {
    *os << "NULL";
  } else {

    if (IsTrue(ImplicitlyConvertible<T*, const void*>::value)) {

      *os << p;
    } else {

      *os << reinterpret_cast<const void*>(
          reinterpret_cast<internal::UInt64>(p));
    }
  }
}

template <typename T>
void DefaultPrintTo(IsNotContainer /* dummy */,
                    false_type /* is not a pointer */,
                    const T& value, ::std::ostream* os) {
  ::testing_internal::DefaultPrintNonContainerTo(value, os);
}

template <typename T>
void PrintTo(const T& value, ::std::ostream* os) {

  DefaultPrintTo(IsContainerTest<T>(0), is_pointer<T>(), value, os);
}

GTEST_API_ void PrintTo(unsigned char c, ::std::ostream* os);
GTEST_API_ void PrintTo(signed char c, ::std::ostream* os);
inline void PrintTo(char c, ::std::ostream* os) {

  PrintTo(static_cast<unsigned char>(c), os);
}

inline void PrintTo(bool x, ::std::ostream* os) {
  *os << (x ? "true" : "false");
}

GTEST_API_ void PrintTo(wchar_t wc, ::std::ostream* os);

GTEST_API_ void PrintTo(const char* s, ::std::ostream* os);
inline void PrintTo(char* s, ::std::ostream* os) {
  PrintTo(ImplicitCast_<const char*>(s), os);
}

inline void PrintTo(const signed char* s, ::std::ostream* os) {
  PrintTo(ImplicitCast_<const void*>(s), os);
}
inline void PrintTo(signed char* s, ::std::ostream* os) {
  PrintTo(ImplicitCast_<const void*>(s), os);
}
inline void PrintTo(const unsigned char* s, ::std::ostream* os) {
  PrintTo(ImplicitCast_<const void*>(s), os);
}
inline void PrintTo(unsigned char* s, ::std::ostream* os) {
  PrintTo(ImplicitCast_<const void*>(s), os);
}

#if !defined(_MSC_VER) || defined(_NATIVE_WCHAR_T_DEFINED)

GTEST_API_ void PrintTo(const wchar_t* s, ::std::ostream* os);
inline void PrintTo(wchar_t* s, ::std::ostream* os) {
  PrintTo(ImplicitCast_<const wchar_t*>(s), os);
}
#endif

template <typename T>
void PrintRawArrayTo(const T a[], size_t count, ::std::ostream* os) {
  UniversalPrint(a[0], os);
  for (size_t i = 1; i != count; i++) {
    *os << ", ";
    UniversalPrint(a[i], os);
  }
}

#if GTEST_HAS_GLOBAL_STRING
GTEST_API_ void PrintStringTo(const ::string&s, ::std::ostream* os);
inline void PrintTo(const ::string& s, ::std::ostream* os) {
  PrintStringTo(s, os);
}
#endif  // GTEST_HAS_GLOBAL_STRING

GTEST_API_ void PrintStringTo(const ::std::string&s, ::std::ostream* os);
inline void PrintTo(const ::std::string& s, ::std::ostream* os) {
  PrintStringTo(s, os);
}

#if GTEST_HAS_GLOBAL_WSTRING
GTEST_API_ void PrintWideStringTo(const ::wstring&s, ::std::ostream* os);
inline void PrintTo(const ::wstring& s, ::std::ostream* os) {
  PrintWideStringTo(s, os);
}
#endif  // GTEST_HAS_GLOBAL_WSTRING

#if GTEST_HAS_STD_WSTRING
GTEST_API_ void PrintWideStringTo(const ::std::wstring&s, ::std::ostream* os);
inline void PrintTo(const ::std::wstring& s, ::std::ostream* os) {
  PrintWideStringTo(s, os);
}
#endif  // GTEST_HAS_STD_WSTRING

#if GTEST_HAS_TR1_TUPLE

template <typename T>
void PrintTupleTo(const T& t, ::std::ostream* os);

inline void PrintTo(const ::std::tr1::tuple<>& t, ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1>
void PrintTo(const ::std::tr1::tuple<T1>& t, ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2>
void PrintTo(const ::std::tr1::tuple<T1, T2>& t, ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2, typename T3>
void PrintTo(const ::std::tr1::tuple<T1, T2, T3>& t, ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2, typename T3, typename T4>
void PrintTo(const ::std::tr1::tuple<T1, T2, T3, T4>& t, ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void PrintTo(const ::std::tr1::tuple<T1, T2, T3, T4, T5>& t,
             ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
          typename T6>
void PrintTo(const ::std::tr1::tuple<T1, T2, T3, T4, T5, T6>& t,
             ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7>
void PrintTo(const ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7>& t,
             ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8>
void PrintTo(const ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8>& t,
             ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9>
void PrintTo(const ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9>& t,
             ::std::ostream* os) {
  PrintTupleTo(t, os);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9, typename T10>
void PrintTo(
    const ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>& t,
    ::std::ostream* os) {
  PrintTupleTo(t, os);
}
#endif  // GTEST_HAS_TR1_TUPLE

template <typename T1, typename T2>
void PrintTo(const ::std::pair<T1, T2>& value, ::std::ostream* os) {
  *os << '(';

  UniversalPrinter<T1>::Print(value.first, os);
  *os << ", ";
  UniversalPrinter<T2>::Print(value.second, os);
  *os << ')';
}

template <typename T>
class UniversalPrinter {
 public:

#ifdef _MSC_VER
# pragma warning(push)          // Saves the current warning state.
# pragma warning(disable:4180)  // Temporarily disables warning 4180.
#endif  // _MSC_VER

  static void Print(const T& value, ::std::ostream* os) {

    PrintTo(value, os);
  }

#ifdef _MSC_VER
# pragma warning(pop)           // Restores the warning state.
#endif  // _MSC_VER
};

template <typename T>
void UniversalPrintArray(const T* begin, size_t len, ::std::ostream* os) {
  if (len == 0) {
    *os << "{}";
  } else {
    *os << "{ ";
    const size_t kThreshold = 18;
    const size_t kChunkSize = 8;

    if (len <= kThreshold) {
      PrintRawArrayTo(begin, len, os);
    } else {
      PrintRawArrayTo(begin, kChunkSize, os);
      *os << ", ..., ";
      PrintRawArrayTo(begin + len - kChunkSize, kChunkSize, os);
    }
    *os << " }";
  }
}

GTEST_API_ void UniversalPrintArray(
    const char* begin, size_t len, ::std::ostream* os);

GTEST_API_ void UniversalPrintArray(
    const wchar_t* begin, size_t len, ::std::ostream* os);

template <typename T, size_t N>
class UniversalPrinter<T[N]> {
 public:

  static void Print(const T (&a)[N], ::std::ostream* os) {
    UniversalPrintArray(a, N, os);
  }
};

template <typename T>
class UniversalPrinter<T&> {
 public:

#ifdef _MSC_VER
# pragma warning(push)          // Saves the current warning state.
# pragma warning(disable:4180)  // Temporarily disables warning 4180.
#endif  // _MSC_VER

  static void Print(const T& value, ::std::ostream* os) {

    *os << "@" << reinterpret_cast<const void*>(&value) << " ";

    UniversalPrint(value, os);
  }

#ifdef _MSC_VER
# pragma warning(pop)           // Restores the warning state.
#endif  // _MSC_VER
};

template <typename T>
class UniversalTersePrinter {
 public:
  static void Print(const T& value, ::std::ostream* os) {
    UniversalPrint(value, os);
  }
};
template <typename T>
class UniversalTersePrinter<T&> {
 public:
  static void Print(const T& value, ::std::ostream* os) {
    UniversalPrint(value, os);
  }
};
template <typename T, size_t N>
class UniversalTersePrinter<T[N]> {
 public:
  static void Print(const T (&value)[N], ::std::ostream* os) {
    UniversalPrinter<T[N]>::Print(value, os);
  }
};
template <>
class UniversalTersePrinter<const char*> {
 public:
  static void Print(const char* str, ::std::ostream* os) {
    if (str == NULL) {
      *os << "NULL";
    } else {
      UniversalPrint(string(str), os);
    }
  }
};
template <>
class UniversalTersePrinter<char*> {
 public:
  static void Print(char* str, ::std::ostream* os) {
    UniversalTersePrinter<const char*>::Print(str, os);
  }
};

#if GTEST_HAS_STD_WSTRING
template <>
class UniversalTersePrinter<const wchar_t*> {
 public:
  static void Print(const wchar_t* str, ::std::ostream* os) {
    if (str == NULL) {
      *os << "NULL";
    } else {
      UniversalPrint(::std::wstring(str), os);
    }
  }
};
#endif

template <>
class UniversalTersePrinter<wchar_t*> {
 public:
  static void Print(wchar_t* str, ::std::ostream* os) {
    UniversalTersePrinter<const wchar_t*>::Print(str, os);
  }
};

template <typename T>
void UniversalTersePrint(const T& value, ::std::ostream* os) {
  UniversalTersePrinter<T>::Print(value, os);
}

template <typename T>
void UniversalPrint(const T& value, ::std::ostream* os) {

  typedef T T1;
  UniversalPrinter<T1>::Print(value, os);
}

#if GTEST_HAS_TR1_TUPLE
typedef ::std::vector<string> Strings;

template <size_t N>
struct TuplePrefixPrinter {

  template <typename Tuple>
  static void PrintPrefixTo(const Tuple& t, ::std::ostream* os) {
    TuplePrefixPrinter<N - 1>::PrintPrefixTo(t, os);
    *os << ", ";
    UniversalPrinter<typename ::std::tr1::tuple_element<N - 1, Tuple>::type>
        ::Print(::std::tr1::get<N - 1>(t), os);
  }

  template <typename Tuple>
  static void TersePrintPrefixToStrings(const Tuple& t, Strings* strings) {
    TuplePrefixPrinter<N - 1>::TersePrintPrefixToStrings(t, strings);
    ::std::stringstream ss;
    UniversalTersePrint(::std::tr1::get<N - 1>(t), &ss);
    strings->push_back(ss.str());
  }
};

template <>
struct TuplePrefixPrinter<0> {
  template <typename Tuple>
  static void PrintPrefixTo(const Tuple&, ::std::ostream*) {}

  template <typename Tuple>
  static void TersePrintPrefixToStrings(const Tuple&, Strings*) {}
};

template <>
struct TuplePrefixPrinter<1> {
  template <typename Tuple>
  static void PrintPrefixTo(const Tuple& t, ::std::ostream* os) {
    UniversalPrinter<typename ::std::tr1::tuple_element<0, Tuple>::type>::
        Print(::std::tr1::get<0>(t), os);
  }

  template <typename Tuple>
  static void TersePrintPrefixToStrings(const Tuple& t, Strings* strings) {
    ::std::stringstream ss;
    UniversalTersePrint(::std::tr1::get<0>(t), &ss);
    strings->push_back(ss.str());
  }
};

template <typename T>
void PrintTupleTo(const T& t, ::std::ostream* os) {
  *os << "(";
  TuplePrefixPrinter< ::std::tr1::tuple_size<T>::value>::
      PrintPrefixTo(t, os);
  *os << ")";
}

template <typename Tuple>
Strings UniversalTersePrintTupleFieldsToStrings(const Tuple& value) {
  Strings result;
  TuplePrefixPrinter< ::std::tr1::tuple_size<Tuple>::value>::
      TersePrintPrefixToStrings(value, &result);
  return result;
}
#endif  // GTEST_HAS_TR1_TUPLE

}  // namespace internal

template <typename T>
::std::string PrintToString(const T& value) {
  ::std::stringstream ss;
  internal::UniversalTersePrinter<T>::Print(value, &ss);
  return ss.str();
}

}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_GTEST_PRINTERS_H_

#if GTEST_HAS_PARAM_TEST

namespace testing {
namespace internal {

GTEST_API_ void ReportInvalidTestCaseType(const char* test_case_name,
                                          const char* file, int line);

template <typename> class ParamGeneratorInterface;
template <typename> class ParamGenerator;

template <typename T>
class ParamIteratorInterface {
 public:
  virtual ~ParamIteratorInterface() {}

  virtual const ParamGeneratorInterface<T>* BaseGenerator() const = 0;

  virtual void Advance() = 0;

  virtual ParamIteratorInterface* Clone() const = 0;

  virtual const T* Current() const = 0;

  virtual bool Equals(const ParamIteratorInterface& other) const = 0;
};

template <typename T>
class ParamIterator {
 public:
  typedef T value_type;
  typedef const T& reference;
  typedef ptrdiff_t difference_type;

  ParamIterator(const ParamIterator& other) : impl_(other.impl_->Clone()) {}
  ParamIterator& operator=(const ParamIterator& other) {
    if (this != &other)
      impl_.reset(other.impl_->Clone());
    return *this;
  }

  const T& operator*() const { return *impl_->Current(); }
  const T* operator->() const { return impl_->Current(); }

  ParamIterator& operator++() {
    impl_->Advance();
    return *this;
  }

  ParamIterator operator++(int /*unused*/) {
    ParamIteratorInterface<T>* clone = impl_->Clone();
    impl_->Advance();
    return ParamIterator(clone);
  }
  bool operator==(const ParamIterator& other) const {
    return impl_.get() == other.impl_.get() || impl_->Equals(*other.impl_);
  }
  bool operator!=(const ParamIterator& other) const {
    return !(*this == other);
  }

 private:
  friend class ParamGenerator<T>;
  explicit ParamIterator(ParamIteratorInterface<T>* impl) : impl_(impl) {}
  scoped_ptr<ParamIteratorInterface<T> > impl_;
};

template <typename T>
class ParamGeneratorInterface {
 public:
  typedef T ParamType;

  virtual ~ParamGeneratorInterface() {}

  virtual ParamIteratorInterface<T>* Begin() const = 0;
  virtual ParamIteratorInterface<T>* End() const = 0;
};

template<typename T>
class ParamGenerator {
 public:
  typedef ParamIterator<T> iterator;

  explicit ParamGenerator(ParamGeneratorInterface<T>* impl) : impl_(impl) {}
  ParamGenerator(const ParamGenerator& other) : impl_(other.impl_) {}

  ParamGenerator& operator=(const ParamGenerator& other) {
    impl_ = other.impl_;
    return *this;
  }

  iterator begin() const { return iterator(impl_->Begin()); }
  iterator end() const { return iterator(impl_->End()); }

 private:
  linked_ptr<const ParamGeneratorInterface<T> > impl_;
};

template <typename T, typename IncrementT>
class RangeGenerator : public ParamGeneratorInterface<T> {
 public:
  RangeGenerator(T begin, T end, IncrementT step)
      : begin_(begin), end_(end),
        step_(step), end_index_(CalculateEndIndex(begin, end, step)) {}
  virtual ~RangeGenerator() {}

  virtual ParamIteratorInterface<T>* Begin() const {
    return new Iterator(this, begin_, 0, step_);
  }
  virtual ParamIteratorInterface<T>* End() const {
    return new Iterator(this, end_, end_index_, step_);
  }

 private:
  class Iterator : public ParamIteratorInterface<T> {
   public:
    Iterator(const ParamGeneratorInterface<T>* base, T value, int index,
             IncrementT step)
        : base_(base), value_(value), index_(index), step_(step) {}
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<T>* BaseGenerator() const {
      return base_;
    }
    virtual void Advance() {
      value_ = value_ + step_;
      index_++;
    }
    virtual ParamIteratorInterface<T>* Clone() const {
      return new Iterator(*this);
    }
    virtual const T* Current() const { return &value_; }
    virtual bool Equals(const ParamIteratorInterface<T>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const int other_index =
          CheckedDowncastToActualType<const Iterator>(&other)->index_;
      return index_ == other_index;
    }

   private:
    Iterator(const Iterator& other)
        : ParamIteratorInterface<T>(),
          base_(other.base_), value_(other.value_), index_(other.index_),
          step_(other.step_) {}

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<T>* const base_;
    T value_;
    int index_;
    const IncrementT step_;
  };  // class RangeGenerator::Iterator

  static int CalculateEndIndex(const T& begin,
                               const T& end,
                               const IncrementT& step) {
    int end_index = 0;
    for (T i = begin; i < end; i = i + step)
      end_index++;
    return end_index;
  }

  void operator=(const RangeGenerator& other);

  const T begin_;
  const T end_;
  const IncrementT step_;

  const int end_index_;
};  // class RangeGenerator

template <typename T>
class ValuesInIteratorRangeGenerator : public ParamGeneratorInterface<T> {
 public:
  template <typename ForwardIterator>
  ValuesInIteratorRangeGenerator(ForwardIterator begin, ForwardIterator end)
      : container_(begin, end) {}
  virtual ~ValuesInIteratorRangeGenerator() {}

  virtual ParamIteratorInterface<T>* Begin() const {
    return new Iterator(this, container_.begin());
  }
  virtual ParamIteratorInterface<T>* End() const {
    return new Iterator(this, container_.end());
  }

 private:
  typedef typename ::std::vector<T> ContainerType;

  class Iterator : public ParamIteratorInterface<T> {
   public:
    Iterator(const ParamGeneratorInterface<T>* base,
             typename ContainerType::const_iterator iterator)
        : base_(base), iterator_(iterator) {}
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<T>* BaseGenerator() const {
      return base_;
    }
    virtual void Advance() {
      ++iterator_;
      value_.reset();
    }
    virtual ParamIteratorInterface<T>* Clone() const {
      return new Iterator(*this);
    }

    virtual const T* Current() const {
      if (value_.get() == NULL)
        value_.reset(new T(*iterator_));
      return value_.get();
    }
    virtual bool Equals(const ParamIteratorInterface<T>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      return iterator_ ==
          CheckedDowncastToActualType<const Iterator>(&other)->iterator_;
    }

   private:
    Iterator(const Iterator& other)

        : ParamIteratorInterface<T>(),
          base_(other.base_),
          iterator_(other.iterator_) {}

    const ParamGeneratorInterface<T>* const base_;
    typename ContainerType::const_iterator iterator_;

    mutable scoped_ptr<const T> value_;
  };  // class ValuesInIteratorRangeGenerator::Iterator

  void operator=(const ValuesInIteratorRangeGenerator& other);

  const ContainerType container_;
};  // class ValuesInIteratorRangeGenerator

template <class TestClass>
class ParameterizedTestFactory : public TestFactoryBase {
 public:
  typedef typename TestClass::ParamType ParamType;
  explicit ParameterizedTestFactory(ParamType parameter) :
      parameter_(parameter) {}
  virtual Test* CreateTest() {
    TestClass::SetParam(&parameter_);
    return new TestClass();
  }

 private:
  const ParamType parameter_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(ParameterizedTestFactory);
};

template <class ParamType>
class TestMetaFactoryBase {
 public:
  virtual ~TestMetaFactoryBase() {}

  virtual TestFactoryBase* CreateTestFactory(ParamType parameter) = 0;
};

template <class TestCase>
class TestMetaFactory
    : public TestMetaFactoryBase<typename TestCase::ParamType> {
 public:
  typedef typename TestCase::ParamType ParamType;

  TestMetaFactory() {}

  virtual TestFactoryBase* CreateTestFactory(ParamType parameter) {
    return new ParameterizedTestFactory<TestCase>(parameter);
  }

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestMetaFactory);
};

class ParameterizedTestCaseInfoBase {
 public:
  virtual ~ParameterizedTestCaseInfoBase() {}

  virtual const string& GetTestCaseName() const = 0;

  virtual TypeId GetTestCaseTypeId() const = 0;

  virtual void RegisterTests() = 0;

 protected:
  ParameterizedTestCaseInfoBase() {}

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN_(ParameterizedTestCaseInfoBase);
};

template <class TestCase>
class ParameterizedTestCaseInfo : public ParameterizedTestCaseInfoBase {
 public:

  typedef typename TestCase::ParamType ParamType;

  typedef ParamGenerator<ParamType>(GeneratorCreationFunc)();

  explicit ParameterizedTestCaseInfo(const char* name)
      : test_case_name_(name) {}

  virtual const string& GetTestCaseName() const { return test_case_name_; }

  virtual TypeId GetTestCaseTypeId() const { return GetTypeId<TestCase>(); }

  void AddTestPattern(const char* test_case_name,
                      const char* test_base_name,
                      TestMetaFactoryBase<ParamType>* meta_factory) {
    tests_.push_back(linked_ptr<TestInfo>(new TestInfo(test_case_name,
                                                       test_base_name,
                                                       meta_factory)));
  }

  int AddTestCaseInstantiation(const string& instantiation_name,
                               GeneratorCreationFunc* func,
                               const char* /* file */,
                               int /* line */) {
    instantiations_.push_back(::std::make_pair(instantiation_name, func));
    return 0;  // Return value used only to run this method in namespace scope.
  }

  virtual void RegisterTests() {
    for (typename TestInfoContainer::iterator test_it = tests_.begin();
         test_it != tests_.end(); ++test_it) {
      linked_ptr<TestInfo> test_info = *test_it;
      for (typename InstantiationContainer::iterator gen_it =
               instantiations_.begin(); gen_it != instantiations_.end();
               ++gen_it) {
        const string& instantiation_name = gen_it->first;
        ParamGenerator<ParamType> generator((*gen_it->second)());

        string test_case_name;
        if ( !instantiation_name.empty() )
          test_case_name = instantiation_name + "/";
        test_case_name += test_info->test_case_base_name;

        int i = 0;
        for (typename ParamGenerator<ParamType>::iterator param_it =
                 generator.begin();
             param_it != generator.end(); ++param_it, ++i) {
          Message test_name_stream;
          test_name_stream << test_info->test_base_name << "/" << i;
          MakeAndRegisterTestInfo(
              test_case_name.c_str(),
              test_name_stream.GetString().c_str(),
              NULL,  // No type parameter.
              PrintToString(*param_it).c_str(),
              GetTestCaseTypeId(),
              TestCase::SetUpTestCase,
              TestCase::TearDownTestCase,
              test_info->test_meta_factory->CreateTestFactory(*param_it));
        }  // for param_it
      }  // for gen_it
    }  // for test_it
  }  // RegisterTests

 private:

  struct TestInfo {
    TestInfo(const char* a_test_case_base_name,
             const char* a_test_base_name,
             TestMetaFactoryBase<ParamType>* a_test_meta_factory) :
        test_case_base_name(a_test_case_base_name),
        test_base_name(a_test_base_name),
        test_meta_factory(a_test_meta_factory) {}

    const string test_case_base_name;
    const string test_base_name;
    const scoped_ptr<TestMetaFactoryBase<ParamType> > test_meta_factory;
  };
  typedef ::std::vector<linked_ptr<TestInfo> > TestInfoContainer;

  typedef ::std::vector<std::pair<string, GeneratorCreationFunc*> >
      InstantiationContainer;

  const string test_case_name_;
  TestInfoContainer tests_;
  InstantiationContainer instantiations_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(ParameterizedTestCaseInfo);
};  // class ParameterizedTestCaseInfo

class ParameterizedTestCaseRegistry {
 public:
  ParameterizedTestCaseRegistry() {}
  ~ParameterizedTestCaseRegistry() {
    for (TestCaseInfoContainer::iterator it = test_case_infos_.begin();
         it != test_case_infos_.end(); ++it) {
      delete *it;
    }
  }

  template <class TestCase>
  ParameterizedTestCaseInfo<TestCase>* GetTestCasePatternHolder(
      const char* test_case_name,
      const char* file,
      int line) {
    ParameterizedTestCaseInfo<TestCase>* typed_test_info = NULL;
    for (TestCaseInfoContainer::iterator it = test_case_infos_.begin();
         it != test_case_infos_.end(); ++it) {
      if ((*it)->GetTestCaseName() == test_case_name) {
        if ((*it)->GetTestCaseTypeId() != GetTypeId<TestCase>()) {

          ReportInvalidTestCaseType(test_case_name,  file, line);
          posix::Abort();
        } else {

          typed_test_info = CheckedDowncastToActualType<
              ParameterizedTestCaseInfo<TestCase> >(*it);
        }
        break;
      }
    }
    if (typed_test_info == NULL) {
      typed_test_info = new ParameterizedTestCaseInfo<TestCase>(test_case_name);
      test_case_infos_.push_back(typed_test_info);
    }
    return typed_test_info;
  }
  void RegisterTests() {
    for (TestCaseInfoContainer::iterator it = test_case_infos_.begin();
         it != test_case_infos_.end(); ++it) {
      (*it)->RegisterTests();
    }
  }

 private:
  typedef ::std::vector<ParameterizedTestCaseInfoBase*> TestCaseInfoContainer;

  TestCaseInfoContainer test_case_infos_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(ParameterizedTestCaseRegistry);
};

}  // namespace internal
}  // namespace testing

#endif  //  GTEST_HAS_PARAM_TEST

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PARAM_UTIL_H_

#ifndef GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PARAM_UTIL_GENERATED_H_
#define GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PARAM_UTIL_GENERATED_H_

#if GTEST_HAS_PARAM_TEST

namespace testing {

template <typename ForwardIterator>
internal::ParamGenerator<
  typename ::testing::internal::IteratorTraits<ForwardIterator>::value_type>
ValuesIn(ForwardIterator begin, ForwardIterator end);

template <typename T, size_t N>
internal::ParamGenerator<T> ValuesIn(const T (&array)[N]);

template <class Container>
internal::ParamGenerator<typename Container::value_type> ValuesIn(
    const Container& container);

namespace internal {

template <typename T1>
class ValueArray1 {
 public:
  explicit ValueArray1(T1 v1) : v1_(v1) {}

  template <typename T>
  operator ParamGenerator<T>() const { return ValuesIn(&v1_, &v1_ + 1); }

 private:

  void operator=(const ValueArray1& other);

  const T1 v1_;
};

template <typename T1, typename T2>
class ValueArray2 {
 public:
  ValueArray2(T1 v1, T2 v2) : v1_(v1), v2_(v2) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray2& other);

  const T1 v1_;
  const T2 v2_;
};

template <typename T1, typename T2, typename T3>
class ValueArray3 {
 public:
  ValueArray3(T1 v1, T2 v2, T3 v3) : v1_(v1), v2_(v2), v3_(v3) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray3& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
};

template <typename T1, typename T2, typename T3, typename T4>
class ValueArray4 {
 public:
  ValueArray4(T1 v1, T2 v2, T3 v3, T4 v4) : v1_(v1), v2_(v2), v3_(v3),
      v4_(v4) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray4& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class ValueArray5 {
 public:
  ValueArray5(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5) : v1_(v1), v2_(v2), v3_(v3),
      v4_(v4), v5_(v5) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray5& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6>
class ValueArray6 {
 public:
  ValueArray6(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6) : v1_(v1), v2_(v2),
      v3_(v3), v4_(v4), v5_(v5), v6_(v6) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray6& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7>
class ValueArray7 {
 public:
  ValueArray7(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7) : v1_(v1),
      v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray7& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8>
class ValueArray8 {
 public:
  ValueArray8(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7,
      T8 v8) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray8& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9>
class ValueArray9 {
 public:
  ValueArray9(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8,
      T9 v9) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray9& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10>
class ValueArray10 {
 public:
  ValueArray10(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray10& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11>
class ValueArray11 {
 public:
  ValueArray11(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6),
      v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray11& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12>
class ValueArray12 {
 public:
  ValueArray12(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5),
      v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray12& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13>
class ValueArray13 {
 public:
  ValueArray13(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13) : v1_(v1), v2_(v2), v3_(v3), v4_(v4),
      v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11),
      v12_(v12), v13_(v13) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray13& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14>
class ValueArray14 {
 public:
  ValueArray14(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14) : v1_(v1), v2_(v2), v3_(v3),
      v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray14& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15>
class ValueArray15 {
 public:
  ValueArray15(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15) : v1_(v1), v2_(v2),
      v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray15& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16>
class ValueArray16 {
 public:
  ValueArray16(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16) : v1_(v1),
      v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9),
      v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15),
      v16_(v16) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray16& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17>
class ValueArray17 {
 public:
  ValueArray17(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16,
      T17 v17) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray17& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18>
class ValueArray18 {
 public:
  ValueArray18(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray18& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19>
class ValueArray19 {
 public:
  ValueArray19(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6),
      v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13),
      v14_(v14), v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray19& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20>
class ValueArray20 {
 public:
  ValueArray20(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5),
      v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12),
      v13_(v13), v14_(v14), v15_(v15), v16_(v16), v17_(v17), v18_(v18),
      v19_(v19), v20_(v20) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray20& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21>
class ValueArray21 {
 public:
  ValueArray21(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21) : v1_(v1), v2_(v2), v3_(v3), v4_(v4),
      v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11),
      v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16), v17_(v17),
      v18_(v18), v19_(v19), v20_(v20), v21_(v21) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray21& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22>
class ValueArray22 {
 public:
  ValueArray22(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22) : v1_(v1), v2_(v2), v3_(v3),
      v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16),
      v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray22& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23>
class ValueArray23 {
 public:
  ValueArray23(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23) : v1_(v1), v2_(v2),
      v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16),
      v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22),
      v23_(v23) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray23& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24>
class ValueArray24 {
 public:
  ValueArray24(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24) : v1_(v1),
      v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9),
      v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15),
      v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21),
      v22_(v22), v23_(v23), v24_(v24) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray24& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25>
class ValueArray25 {
 public:
  ValueArray25(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24,
      T25 v25) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20),
      v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray25& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26>
class ValueArray26 {
 public:
  ValueArray26(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20),
      v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray26& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27>
class ValueArray27 {
 public:
  ValueArray27(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6),
      v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13),
      v14_(v14), v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19),
      v20_(v20), v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25),
      v26_(v26), v27_(v27) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray27& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28>
class ValueArray28 {
 public:
  ValueArray28(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5),
      v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12),
      v13_(v13), v14_(v14), v15_(v15), v16_(v16), v17_(v17), v18_(v18),
      v19_(v19), v20_(v20), v21_(v21), v22_(v22), v23_(v23), v24_(v24),
      v25_(v25), v26_(v26), v27_(v27), v28_(v28) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray28& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29>
class ValueArray29 {
 public:
  ValueArray29(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29) : v1_(v1), v2_(v2), v3_(v3), v4_(v4),
      v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11),
      v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16), v17_(v17),
      v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22), v23_(v23),
      v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28), v29_(v29) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray29& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30>
class ValueArray30 {
 public:
  ValueArray30(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30) : v1_(v1), v2_(v2), v3_(v3),
      v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16),
      v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22),
      v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28),
      v29_(v29), v30_(v30) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray30& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31>
class ValueArray31 {
 public:
  ValueArray31(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31) : v1_(v1), v2_(v2),
      v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16),
      v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22),
      v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28),
      v29_(v29), v30_(v30), v31_(v31) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray31& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32>
class ValueArray32 {
 public:
  ValueArray32(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32) : v1_(v1),
      v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9),
      v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15),
      v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21),
      v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27),
      v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray32& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33>
class ValueArray33 {
 public:
  ValueArray33(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32,
      T33 v33) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20),
      v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26),
      v27_(v27), v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32),
      v33_(v33) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray33& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34>
class ValueArray34 {
 public:
  ValueArray34(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20),
      v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26),
      v27_(v27), v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32),
      v33_(v33), v34_(v34) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray34& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35>
class ValueArray35 {
 public:
  ValueArray35(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6),
      v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13),
      v14_(v14), v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19),
      v20_(v20), v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25),
      v26_(v26), v27_(v27), v28_(v28), v29_(v29), v30_(v30), v31_(v31),
      v32_(v32), v33_(v33), v34_(v34), v35_(v35) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray35& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36>
class ValueArray36 {
 public:
  ValueArray36(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5),
      v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12),
      v13_(v13), v14_(v14), v15_(v15), v16_(v16), v17_(v17), v18_(v18),
      v19_(v19), v20_(v20), v21_(v21), v22_(v22), v23_(v23), v24_(v24),
      v25_(v25), v26_(v26), v27_(v27), v28_(v28), v29_(v29), v30_(v30),
      v31_(v31), v32_(v32), v33_(v33), v34_(v34), v35_(v35), v36_(v36) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray36& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37>
class ValueArray37 {
 public:
  ValueArray37(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37) : v1_(v1), v2_(v2), v3_(v3), v4_(v4),
      v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11),
      v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16), v17_(v17),
      v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22), v23_(v23),
      v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28), v29_(v29),
      v30_(v30), v31_(v31), v32_(v32), v33_(v33), v34_(v34), v35_(v35),
      v36_(v36), v37_(v37) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray37& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38>
class ValueArray38 {
 public:
  ValueArray38(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38) : v1_(v1), v2_(v2), v3_(v3),
      v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16),
      v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22),
      v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28),
      v29_(v29), v30_(v30), v31_(v31), v32_(v32), v33_(v33), v34_(v34),
      v35_(v35), v36_(v36), v37_(v37), v38_(v38) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray38& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39>
class ValueArray39 {
 public:
  ValueArray39(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39) : v1_(v1), v2_(v2),
      v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16),
      v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22),
      v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28),
      v29_(v29), v30_(v30), v31_(v31), v32_(v32), v33_(v33), v34_(v34),
      v35_(v35), v36_(v36), v37_(v37), v38_(v38), v39_(v39) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray39& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40>
class ValueArray40 {
 public:
  ValueArray40(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40) : v1_(v1),
      v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9),
      v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15),
      v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21),
      v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27),
      v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32), v33_(v33),
      v34_(v34), v35_(v35), v36_(v36), v37_(v37), v38_(v38), v39_(v39),
      v40_(v40) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray40& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41>
class ValueArray41 {
 public:
  ValueArray41(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40,
      T41 v41) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20),
      v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26),
      v27_(v27), v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32),
      v33_(v33), v34_(v34), v35_(v35), v36_(v36), v37_(v37), v38_(v38),
      v39_(v39), v40_(v40), v41_(v41) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray41& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42>
class ValueArray42 {
 public:
  ValueArray42(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20),
      v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26),
      v27_(v27), v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32),
      v33_(v33), v34_(v34), v35_(v35), v36_(v36), v37_(v37), v38_(v38),
      v39_(v39), v40_(v40), v41_(v41), v42_(v42) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray42& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43>
class ValueArray43 {
 public:
  ValueArray43(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42, T43 v43) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6),
      v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13),
      v14_(v14), v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19),
      v20_(v20), v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25),
      v26_(v26), v27_(v27), v28_(v28), v29_(v29), v30_(v30), v31_(v31),
      v32_(v32), v33_(v33), v34_(v34), v35_(v35), v36_(v36), v37_(v37),
      v38_(v38), v39_(v39), v40_(v40), v41_(v41), v42_(v42), v43_(v43) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_), static_cast<T>(v43_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray43& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
  const T43 v43_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44>
class ValueArray44 {
 public:
  ValueArray44(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42, T43 v43, T44 v44) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5),
      v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12),
      v13_(v13), v14_(v14), v15_(v15), v16_(v16), v17_(v17), v18_(v18),
      v19_(v19), v20_(v20), v21_(v21), v22_(v22), v23_(v23), v24_(v24),
      v25_(v25), v26_(v26), v27_(v27), v28_(v28), v29_(v29), v30_(v30),
      v31_(v31), v32_(v32), v33_(v33), v34_(v34), v35_(v35), v36_(v36),
      v37_(v37), v38_(v38), v39_(v39), v40_(v40), v41_(v41), v42_(v42),
      v43_(v43), v44_(v44) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_), static_cast<T>(v43_), static_cast<T>(v44_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray44& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
  const T43 v43_;
  const T44 v44_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45>
class ValueArray45 {
 public:
  ValueArray45(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42, T43 v43, T44 v44, T45 v45) : v1_(v1), v2_(v2), v3_(v3), v4_(v4),
      v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10), v11_(v11),
      v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16), v17_(v17),
      v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22), v23_(v23),
      v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28), v29_(v29),
      v30_(v30), v31_(v31), v32_(v32), v33_(v33), v34_(v34), v35_(v35),
      v36_(v36), v37_(v37), v38_(v38), v39_(v39), v40_(v40), v41_(v41),
      v42_(v42), v43_(v43), v44_(v44), v45_(v45) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_), static_cast<T>(v43_), static_cast<T>(v44_),
        static_cast<T>(v45_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray45& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
  const T43 v43_;
  const T44 v44_;
  const T45 v45_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46>
class ValueArray46 {
 public:
  ValueArray46(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42, T43 v43, T44 v44, T45 v45, T46 v46) : v1_(v1), v2_(v2), v3_(v3),
      v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16),
      v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22),
      v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28),
      v29_(v29), v30_(v30), v31_(v31), v32_(v32), v33_(v33), v34_(v34),
      v35_(v35), v36_(v36), v37_(v37), v38_(v38), v39_(v39), v40_(v40),
      v41_(v41), v42_(v42), v43_(v43), v44_(v44), v45_(v45), v46_(v46) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_), static_cast<T>(v43_), static_cast<T>(v44_),
        static_cast<T>(v45_), static_cast<T>(v46_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray46& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
  const T43 v43_;
  const T44 v44_;
  const T45 v45_;
  const T46 v46_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47>
class ValueArray47 {
 public:
  ValueArray47(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42, T43 v43, T44 v44, T45 v45, T46 v46, T47 v47) : v1_(v1), v2_(v2),
      v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9), v10_(v10),
      v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15), v16_(v16),
      v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21), v22_(v22),
      v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27), v28_(v28),
      v29_(v29), v30_(v30), v31_(v31), v32_(v32), v33_(v33), v34_(v34),
      v35_(v35), v36_(v36), v37_(v37), v38_(v38), v39_(v39), v40_(v40),
      v41_(v41), v42_(v42), v43_(v43), v44_(v44), v45_(v45), v46_(v46),
      v47_(v47) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_), static_cast<T>(v43_), static_cast<T>(v44_),
        static_cast<T>(v45_), static_cast<T>(v46_), static_cast<T>(v47_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray47& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
  const T43 v43_;
  const T44 v44_;
  const T45 v45_;
  const T46 v46_;
  const T47 v47_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48>
class ValueArray48 {
 public:
  ValueArray48(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42, T43 v43, T44 v44, T45 v45, T46 v46, T47 v47, T48 v48) : v1_(v1),
      v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7), v8_(v8), v9_(v9),
      v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14), v15_(v15),
      v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20), v21_(v21),
      v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26), v27_(v27),
      v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32), v33_(v33),
      v34_(v34), v35_(v35), v36_(v36), v37_(v37), v38_(v38), v39_(v39),
      v40_(v40), v41_(v41), v42_(v42), v43_(v43), v44_(v44), v45_(v45),
      v46_(v46), v47_(v47), v48_(v48) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_), static_cast<T>(v43_), static_cast<T>(v44_),
        static_cast<T>(v45_), static_cast<T>(v46_), static_cast<T>(v47_),
        static_cast<T>(v48_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray48& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
  const T43 v43_;
  const T44 v44_;
  const T45 v45_;
  const T46 v46_;
  const T47 v47_;
  const T48 v48_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48, typename T49>
class ValueArray49 {
 public:
  ValueArray49(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42, T43 v43, T44 v44, T45 v45, T46 v46, T47 v47, T48 v48,
      T49 v49) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20),
      v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26),
      v27_(v27), v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32),
      v33_(v33), v34_(v34), v35_(v35), v36_(v36), v37_(v37), v38_(v38),
      v39_(v39), v40_(v40), v41_(v41), v42_(v42), v43_(v43), v44_(v44),
      v45_(v45), v46_(v46), v47_(v47), v48_(v48), v49_(v49) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_), static_cast<T>(v43_), static_cast<T>(v44_),
        static_cast<T>(v45_), static_cast<T>(v46_), static_cast<T>(v47_),
        static_cast<T>(v48_), static_cast<T>(v49_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray49& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
  const T43 v43_;
  const T44 v44_;
  const T45 v45_;
  const T46 v46_;
  const T47 v47_;
  const T48 v48_;
  const T49 v49_;
};

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48, typename T49, typename T50>
class ValueArray50 {
 public:
  ValueArray50(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
      T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
      T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
      T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
      T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
      T42 v42, T43 v43, T44 v44, T45 v45, T46 v46, T47 v47, T48 v48, T49 v49,
      T50 v50) : v1_(v1), v2_(v2), v3_(v3), v4_(v4), v5_(v5), v6_(v6), v7_(v7),
      v8_(v8), v9_(v9), v10_(v10), v11_(v11), v12_(v12), v13_(v13), v14_(v14),
      v15_(v15), v16_(v16), v17_(v17), v18_(v18), v19_(v19), v20_(v20),
      v21_(v21), v22_(v22), v23_(v23), v24_(v24), v25_(v25), v26_(v26),
      v27_(v27), v28_(v28), v29_(v29), v30_(v30), v31_(v31), v32_(v32),
      v33_(v33), v34_(v34), v35_(v35), v36_(v36), v37_(v37), v38_(v38),
      v39_(v39), v40_(v40), v41_(v41), v42_(v42), v43_(v43), v44_(v44),
      v45_(v45), v46_(v46), v47_(v47), v48_(v48), v49_(v49), v50_(v50) {}

  template <typename T>
  operator ParamGenerator<T>() const {
    const T array[] = {static_cast<T>(v1_), static_cast<T>(v2_),
        static_cast<T>(v3_), static_cast<T>(v4_), static_cast<T>(v5_),
        static_cast<T>(v6_), static_cast<T>(v7_), static_cast<T>(v8_),
        static_cast<T>(v9_), static_cast<T>(v10_), static_cast<T>(v11_),
        static_cast<T>(v12_), static_cast<T>(v13_), static_cast<T>(v14_),
        static_cast<T>(v15_), static_cast<T>(v16_), static_cast<T>(v17_),
        static_cast<T>(v18_), static_cast<T>(v19_), static_cast<T>(v20_),
        static_cast<T>(v21_), static_cast<T>(v22_), static_cast<T>(v23_),
        static_cast<T>(v24_), static_cast<T>(v25_), static_cast<T>(v26_),
        static_cast<T>(v27_), static_cast<T>(v28_), static_cast<T>(v29_),
        static_cast<T>(v30_), static_cast<T>(v31_), static_cast<T>(v32_),
        static_cast<T>(v33_), static_cast<T>(v34_), static_cast<T>(v35_),
        static_cast<T>(v36_), static_cast<T>(v37_), static_cast<T>(v38_),
        static_cast<T>(v39_), static_cast<T>(v40_), static_cast<T>(v41_),
        static_cast<T>(v42_), static_cast<T>(v43_), static_cast<T>(v44_),
        static_cast<T>(v45_), static_cast<T>(v46_), static_cast<T>(v47_),
        static_cast<T>(v48_), static_cast<T>(v49_), static_cast<T>(v50_)};
    return ValuesIn(array);
  }

 private:

  void operator=(const ValueArray50& other);

  const T1 v1_;
  const T2 v2_;
  const T3 v3_;
  const T4 v4_;
  const T5 v5_;
  const T6 v6_;
  const T7 v7_;
  const T8 v8_;
  const T9 v9_;
  const T10 v10_;
  const T11 v11_;
  const T12 v12_;
  const T13 v13_;
  const T14 v14_;
  const T15 v15_;
  const T16 v16_;
  const T17 v17_;
  const T18 v18_;
  const T19 v19_;
  const T20 v20_;
  const T21 v21_;
  const T22 v22_;
  const T23 v23_;
  const T24 v24_;
  const T25 v25_;
  const T26 v26_;
  const T27 v27_;
  const T28 v28_;
  const T29 v29_;
  const T30 v30_;
  const T31 v31_;
  const T32 v32_;
  const T33 v33_;
  const T34 v34_;
  const T35 v35_;
  const T36 v36_;
  const T37 v37_;
  const T38 v38_;
  const T39 v39_;
  const T40 v40_;
  const T41 v41_;
  const T42 v42_;
  const T43 v43_;
  const T44 v44_;
  const T45 v45_;
  const T46 v46_;
  const T47 v47_;
  const T48 v48_;
  const T49 v49_;
  const T50 v50_;
};

# if GTEST_HAS_COMBINE

template <typename T1, typename T2>
class CartesianProductGenerator2
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2> > {
 public:
  typedef ::std::tr1::tuple<T1, T2> ParamType;

  CartesianProductGenerator2(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2)
      : g1_(g1), g2_(g2) {}
  virtual ~CartesianProductGenerator2() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current2_;
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    ParamType current_value_;
  };  // class CartesianProductGenerator2::Iterator

  void operator=(const CartesianProductGenerator2& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
};  // class CartesianProductGenerator2

template <typename T1, typename T2, typename T3>
class CartesianProductGenerator3
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2, T3> > {
 public:
  typedef ::std::tr1::tuple<T1, T2, T3> ParamType;

  CartesianProductGenerator3(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2, const ParamGenerator<T3>& g3)
      : g1_(g1), g2_(g2), g3_(g3) {}
  virtual ~CartesianProductGenerator3() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin(), g3_,
        g3_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end(), g3_, g3_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2,
      const ParamGenerator<T3>& g3,
      const typename ParamGenerator<T3>::iterator& current3)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2),
          begin3_(g3.begin()), end3_(g3.end()), current3_(current3)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current3_;
      if (current3_ == end3_) {
        current3_ = begin3_;
        ++current2_;
      }
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_ &&
          current3_ == typed_other->current3_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_),
        begin3_(other.begin3_),
        end3_(other.end3_),
        current3_(other.current3_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_, *current3_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_ ||
          current3_ == end3_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    const typename ParamGenerator<T3>::iterator begin3_;
    const typename ParamGenerator<T3>::iterator end3_;
    typename ParamGenerator<T3>::iterator current3_;
    ParamType current_value_;
  };  // class CartesianProductGenerator3::Iterator

  void operator=(const CartesianProductGenerator3& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
  const ParamGenerator<T3> g3_;
};  // class CartesianProductGenerator3

template <typename T1, typename T2, typename T3, typename T4>
class CartesianProductGenerator4
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2, T3, T4> > {
 public:
  typedef ::std::tr1::tuple<T1, T2, T3, T4> ParamType;

  CartesianProductGenerator4(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2, const ParamGenerator<T3>& g3,
      const ParamGenerator<T4>& g4)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4) {}
  virtual ~CartesianProductGenerator4() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin(), g3_,
        g3_.begin(), g4_, g4_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end(), g3_, g3_.end(),
        g4_, g4_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2,
      const ParamGenerator<T3>& g3,
      const typename ParamGenerator<T3>::iterator& current3,
      const ParamGenerator<T4>& g4,
      const typename ParamGenerator<T4>::iterator& current4)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2),
          begin3_(g3.begin()), end3_(g3.end()), current3_(current3),
          begin4_(g4.begin()), end4_(g4.end()), current4_(current4)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current4_;
      if (current4_ == end4_) {
        current4_ = begin4_;
        ++current3_;
      }
      if (current3_ == end3_) {
        current3_ = begin3_;
        ++current2_;
      }
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_ &&
          current3_ == typed_other->current3_ &&
          current4_ == typed_other->current4_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_),
        begin3_(other.begin3_),
        end3_(other.end3_),
        current3_(other.current3_),
        begin4_(other.begin4_),
        end4_(other.end4_),
        current4_(other.current4_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_, *current3_,
            *current4_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_ ||
          current3_ == end3_ ||
          current4_ == end4_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    const typename ParamGenerator<T3>::iterator begin3_;
    const typename ParamGenerator<T3>::iterator end3_;
    typename ParamGenerator<T3>::iterator current3_;
    const typename ParamGenerator<T4>::iterator begin4_;
    const typename ParamGenerator<T4>::iterator end4_;
    typename ParamGenerator<T4>::iterator current4_;
    ParamType current_value_;
  };  // class CartesianProductGenerator4::Iterator

  void operator=(const CartesianProductGenerator4& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
  const ParamGenerator<T3> g3_;
  const ParamGenerator<T4> g4_;
};  // class CartesianProductGenerator4

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class CartesianProductGenerator5
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2, T3, T4, T5> > {
 public:
  typedef ::std::tr1::tuple<T1, T2, T3, T4, T5> ParamType;

  CartesianProductGenerator5(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2, const ParamGenerator<T3>& g3,
      const ParamGenerator<T4>& g4, const ParamGenerator<T5>& g5)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5) {}
  virtual ~CartesianProductGenerator5() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin(), g3_,
        g3_.begin(), g4_, g4_.begin(), g5_, g5_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end(), g3_, g3_.end(),
        g4_, g4_.end(), g5_, g5_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2,
      const ParamGenerator<T3>& g3,
      const typename ParamGenerator<T3>::iterator& current3,
      const ParamGenerator<T4>& g4,
      const typename ParamGenerator<T4>::iterator& current4,
      const ParamGenerator<T5>& g5,
      const typename ParamGenerator<T5>::iterator& current5)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2),
          begin3_(g3.begin()), end3_(g3.end()), current3_(current3),
          begin4_(g4.begin()), end4_(g4.end()), current4_(current4),
          begin5_(g5.begin()), end5_(g5.end()), current5_(current5)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current5_;
      if (current5_ == end5_) {
        current5_ = begin5_;
        ++current4_;
      }
      if (current4_ == end4_) {
        current4_ = begin4_;
        ++current3_;
      }
      if (current3_ == end3_) {
        current3_ = begin3_;
        ++current2_;
      }
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_ &&
          current3_ == typed_other->current3_ &&
          current4_ == typed_other->current4_ &&
          current5_ == typed_other->current5_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_),
        begin3_(other.begin3_),
        end3_(other.end3_),
        current3_(other.current3_),
        begin4_(other.begin4_),
        end4_(other.end4_),
        current4_(other.current4_),
        begin5_(other.begin5_),
        end5_(other.end5_),
        current5_(other.current5_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_, *current3_,
            *current4_, *current5_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_ ||
          current3_ == end3_ ||
          current4_ == end4_ ||
          current5_ == end5_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    const typename ParamGenerator<T3>::iterator begin3_;
    const typename ParamGenerator<T3>::iterator end3_;
    typename ParamGenerator<T3>::iterator current3_;
    const typename ParamGenerator<T4>::iterator begin4_;
    const typename ParamGenerator<T4>::iterator end4_;
    typename ParamGenerator<T4>::iterator current4_;
    const typename ParamGenerator<T5>::iterator begin5_;
    const typename ParamGenerator<T5>::iterator end5_;
    typename ParamGenerator<T5>::iterator current5_;
    ParamType current_value_;
  };  // class CartesianProductGenerator5::Iterator

  void operator=(const CartesianProductGenerator5& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
  const ParamGenerator<T3> g3_;
  const ParamGenerator<T4> g4_;
  const ParamGenerator<T5> g5_;
};  // class CartesianProductGenerator5

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6>
class CartesianProductGenerator6
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2, T3, T4, T5,
        T6> > {
 public:
  typedef ::std::tr1::tuple<T1, T2, T3, T4, T5, T6> ParamType;

  CartesianProductGenerator6(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2, const ParamGenerator<T3>& g3,
      const ParamGenerator<T4>& g4, const ParamGenerator<T5>& g5,
      const ParamGenerator<T6>& g6)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6) {}
  virtual ~CartesianProductGenerator6() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin(), g3_,
        g3_.begin(), g4_, g4_.begin(), g5_, g5_.begin(), g6_, g6_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end(), g3_, g3_.end(),
        g4_, g4_.end(), g5_, g5_.end(), g6_, g6_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2,
      const ParamGenerator<T3>& g3,
      const typename ParamGenerator<T3>::iterator& current3,
      const ParamGenerator<T4>& g4,
      const typename ParamGenerator<T4>::iterator& current4,
      const ParamGenerator<T5>& g5,
      const typename ParamGenerator<T5>::iterator& current5,
      const ParamGenerator<T6>& g6,
      const typename ParamGenerator<T6>::iterator& current6)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2),
          begin3_(g3.begin()), end3_(g3.end()), current3_(current3),
          begin4_(g4.begin()), end4_(g4.end()), current4_(current4),
          begin5_(g5.begin()), end5_(g5.end()), current5_(current5),
          begin6_(g6.begin()), end6_(g6.end()), current6_(current6)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current6_;
      if (current6_ == end6_) {
        current6_ = begin6_;
        ++current5_;
      }
      if (current5_ == end5_) {
        current5_ = begin5_;
        ++current4_;
      }
      if (current4_ == end4_) {
        current4_ = begin4_;
        ++current3_;
      }
      if (current3_ == end3_) {
        current3_ = begin3_;
        ++current2_;
      }
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_ &&
          current3_ == typed_other->current3_ &&
          current4_ == typed_other->current4_ &&
          current5_ == typed_other->current5_ &&
          current6_ == typed_other->current6_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_),
        begin3_(other.begin3_),
        end3_(other.end3_),
        current3_(other.current3_),
        begin4_(other.begin4_),
        end4_(other.end4_),
        current4_(other.current4_),
        begin5_(other.begin5_),
        end5_(other.end5_),
        current5_(other.current5_),
        begin6_(other.begin6_),
        end6_(other.end6_),
        current6_(other.current6_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_, *current3_,
            *current4_, *current5_, *current6_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_ ||
          current3_ == end3_ ||
          current4_ == end4_ ||
          current5_ == end5_ ||
          current6_ == end6_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    const typename ParamGenerator<T3>::iterator begin3_;
    const typename ParamGenerator<T3>::iterator end3_;
    typename ParamGenerator<T3>::iterator current3_;
    const typename ParamGenerator<T4>::iterator begin4_;
    const typename ParamGenerator<T4>::iterator end4_;
    typename ParamGenerator<T4>::iterator current4_;
    const typename ParamGenerator<T5>::iterator begin5_;
    const typename ParamGenerator<T5>::iterator end5_;
    typename ParamGenerator<T5>::iterator current5_;
    const typename ParamGenerator<T6>::iterator begin6_;
    const typename ParamGenerator<T6>::iterator end6_;
    typename ParamGenerator<T6>::iterator current6_;
    ParamType current_value_;
  };  // class CartesianProductGenerator6::Iterator

  void operator=(const CartesianProductGenerator6& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
  const ParamGenerator<T3> g3_;
  const ParamGenerator<T4> g4_;
  const ParamGenerator<T5> g5_;
  const ParamGenerator<T6> g6_;
};  // class CartesianProductGenerator6

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7>
class CartesianProductGenerator7
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6,
        T7> > {
 public:
  typedef ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7> ParamType;

  CartesianProductGenerator7(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2, const ParamGenerator<T3>& g3,
      const ParamGenerator<T4>& g4, const ParamGenerator<T5>& g5,
      const ParamGenerator<T6>& g6, const ParamGenerator<T7>& g7)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6), g7_(g7) {}
  virtual ~CartesianProductGenerator7() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin(), g3_,
        g3_.begin(), g4_, g4_.begin(), g5_, g5_.begin(), g6_, g6_.begin(), g7_,
        g7_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end(), g3_, g3_.end(),
        g4_, g4_.end(), g5_, g5_.end(), g6_, g6_.end(), g7_, g7_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2,
      const ParamGenerator<T3>& g3,
      const typename ParamGenerator<T3>::iterator& current3,
      const ParamGenerator<T4>& g4,
      const typename ParamGenerator<T4>::iterator& current4,
      const ParamGenerator<T5>& g5,
      const typename ParamGenerator<T5>::iterator& current5,
      const ParamGenerator<T6>& g6,
      const typename ParamGenerator<T6>::iterator& current6,
      const ParamGenerator<T7>& g7,
      const typename ParamGenerator<T7>::iterator& current7)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2),
          begin3_(g3.begin()), end3_(g3.end()), current3_(current3),
          begin4_(g4.begin()), end4_(g4.end()), current4_(current4),
          begin5_(g5.begin()), end5_(g5.end()), current5_(current5),
          begin6_(g6.begin()), end6_(g6.end()), current6_(current6),
          begin7_(g7.begin()), end7_(g7.end()), current7_(current7)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current7_;
      if (current7_ == end7_) {
        current7_ = begin7_;
        ++current6_;
      }
      if (current6_ == end6_) {
        current6_ = begin6_;
        ++current5_;
      }
      if (current5_ == end5_) {
        current5_ = begin5_;
        ++current4_;
      }
      if (current4_ == end4_) {
        current4_ = begin4_;
        ++current3_;
      }
      if (current3_ == end3_) {
        current3_ = begin3_;
        ++current2_;
      }
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_ &&
          current3_ == typed_other->current3_ &&
          current4_ == typed_other->current4_ &&
          current5_ == typed_other->current5_ &&
          current6_ == typed_other->current6_ &&
          current7_ == typed_other->current7_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_),
        begin3_(other.begin3_),
        end3_(other.end3_),
        current3_(other.current3_),
        begin4_(other.begin4_),
        end4_(other.end4_),
        current4_(other.current4_),
        begin5_(other.begin5_),
        end5_(other.end5_),
        current5_(other.current5_),
        begin6_(other.begin6_),
        end6_(other.end6_),
        current6_(other.current6_),
        begin7_(other.begin7_),
        end7_(other.end7_),
        current7_(other.current7_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_, *current3_,
            *current4_, *current5_, *current6_, *current7_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_ ||
          current3_ == end3_ ||
          current4_ == end4_ ||
          current5_ == end5_ ||
          current6_ == end6_ ||
          current7_ == end7_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    const typename ParamGenerator<T3>::iterator begin3_;
    const typename ParamGenerator<T3>::iterator end3_;
    typename ParamGenerator<T3>::iterator current3_;
    const typename ParamGenerator<T4>::iterator begin4_;
    const typename ParamGenerator<T4>::iterator end4_;
    typename ParamGenerator<T4>::iterator current4_;
    const typename ParamGenerator<T5>::iterator begin5_;
    const typename ParamGenerator<T5>::iterator end5_;
    typename ParamGenerator<T5>::iterator current5_;
    const typename ParamGenerator<T6>::iterator begin6_;
    const typename ParamGenerator<T6>::iterator end6_;
    typename ParamGenerator<T6>::iterator current6_;
    const typename ParamGenerator<T7>::iterator begin7_;
    const typename ParamGenerator<T7>::iterator end7_;
    typename ParamGenerator<T7>::iterator current7_;
    ParamType current_value_;
  };  // class CartesianProductGenerator7::Iterator

  void operator=(const CartesianProductGenerator7& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
  const ParamGenerator<T3> g3_;
  const ParamGenerator<T4> g4_;
  const ParamGenerator<T5> g5_;
  const ParamGenerator<T6> g6_;
  const ParamGenerator<T7> g7_;
};  // class CartesianProductGenerator7

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8>
class CartesianProductGenerator8
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6,
        T7, T8> > {
 public:
  typedef ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8> ParamType;

  CartesianProductGenerator8(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2, const ParamGenerator<T3>& g3,
      const ParamGenerator<T4>& g4, const ParamGenerator<T5>& g5,
      const ParamGenerator<T6>& g6, const ParamGenerator<T7>& g7,
      const ParamGenerator<T8>& g8)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6), g7_(g7),
          g8_(g8) {}
  virtual ~CartesianProductGenerator8() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin(), g3_,
        g3_.begin(), g4_, g4_.begin(), g5_, g5_.begin(), g6_, g6_.begin(), g7_,
        g7_.begin(), g8_, g8_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end(), g3_, g3_.end(),
        g4_, g4_.end(), g5_, g5_.end(), g6_, g6_.end(), g7_, g7_.end(), g8_,
        g8_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2,
      const ParamGenerator<T3>& g3,
      const typename ParamGenerator<T3>::iterator& current3,
      const ParamGenerator<T4>& g4,
      const typename ParamGenerator<T4>::iterator& current4,
      const ParamGenerator<T5>& g5,
      const typename ParamGenerator<T5>::iterator& current5,
      const ParamGenerator<T6>& g6,
      const typename ParamGenerator<T6>::iterator& current6,
      const ParamGenerator<T7>& g7,
      const typename ParamGenerator<T7>::iterator& current7,
      const ParamGenerator<T8>& g8,
      const typename ParamGenerator<T8>::iterator& current8)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2),
          begin3_(g3.begin()), end3_(g3.end()), current3_(current3),
          begin4_(g4.begin()), end4_(g4.end()), current4_(current4),
          begin5_(g5.begin()), end5_(g5.end()), current5_(current5),
          begin6_(g6.begin()), end6_(g6.end()), current6_(current6),
          begin7_(g7.begin()), end7_(g7.end()), current7_(current7),
          begin8_(g8.begin()), end8_(g8.end()), current8_(current8)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current8_;
      if (current8_ == end8_) {
        current8_ = begin8_;
        ++current7_;
      }
      if (current7_ == end7_) {
        current7_ = begin7_;
        ++current6_;
      }
      if (current6_ == end6_) {
        current6_ = begin6_;
        ++current5_;
      }
      if (current5_ == end5_) {
        current5_ = begin5_;
        ++current4_;
      }
      if (current4_ == end4_) {
        current4_ = begin4_;
        ++current3_;
      }
      if (current3_ == end3_) {
        current3_ = begin3_;
        ++current2_;
      }
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_ &&
          current3_ == typed_other->current3_ &&
          current4_ == typed_other->current4_ &&
          current5_ == typed_other->current5_ &&
          current6_ == typed_other->current6_ &&
          current7_ == typed_other->current7_ &&
          current8_ == typed_other->current8_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_),
        begin3_(other.begin3_),
        end3_(other.end3_),
        current3_(other.current3_),
        begin4_(other.begin4_),
        end4_(other.end4_),
        current4_(other.current4_),
        begin5_(other.begin5_),
        end5_(other.end5_),
        current5_(other.current5_),
        begin6_(other.begin6_),
        end6_(other.end6_),
        current6_(other.current6_),
        begin7_(other.begin7_),
        end7_(other.end7_),
        current7_(other.current7_),
        begin8_(other.begin8_),
        end8_(other.end8_),
        current8_(other.current8_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_, *current3_,
            *current4_, *current5_, *current6_, *current7_, *current8_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_ ||
          current3_ == end3_ ||
          current4_ == end4_ ||
          current5_ == end5_ ||
          current6_ == end6_ ||
          current7_ == end7_ ||
          current8_ == end8_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    const typename ParamGenerator<T3>::iterator begin3_;
    const typename ParamGenerator<T3>::iterator end3_;
    typename ParamGenerator<T3>::iterator current3_;
    const typename ParamGenerator<T4>::iterator begin4_;
    const typename ParamGenerator<T4>::iterator end4_;
    typename ParamGenerator<T4>::iterator current4_;
    const typename ParamGenerator<T5>::iterator begin5_;
    const typename ParamGenerator<T5>::iterator end5_;
    typename ParamGenerator<T5>::iterator current5_;
    const typename ParamGenerator<T6>::iterator begin6_;
    const typename ParamGenerator<T6>::iterator end6_;
    typename ParamGenerator<T6>::iterator current6_;
    const typename ParamGenerator<T7>::iterator begin7_;
    const typename ParamGenerator<T7>::iterator end7_;
    typename ParamGenerator<T7>::iterator current7_;
    const typename ParamGenerator<T8>::iterator begin8_;
    const typename ParamGenerator<T8>::iterator end8_;
    typename ParamGenerator<T8>::iterator current8_;
    ParamType current_value_;
  };  // class CartesianProductGenerator8::Iterator

  void operator=(const CartesianProductGenerator8& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
  const ParamGenerator<T3> g3_;
  const ParamGenerator<T4> g4_;
  const ParamGenerator<T5> g5_;
  const ParamGenerator<T6> g6_;
  const ParamGenerator<T7> g7_;
  const ParamGenerator<T8> g8_;
};  // class CartesianProductGenerator8

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9>
class CartesianProductGenerator9
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6,
        T7, T8, T9> > {
 public:
  typedef ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9> ParamType;

  CartesianProductGenerator9(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2, const ParamGenerator<T3>& g3,
      const ParamGenerator<T4>& g4, const ParamGenerator<T5>& g5,
      const ParamGenerator<T6>& g6, const ParamGenerator<T7>& g7,
      const ParamGenerator<T8>& g8, const ParamGenerator<T9>& g9)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6), g7_(g7), g8_(g8),
          g9_(g9) {}
  virtual ~CartesianProductGenerator9() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin(), g3_,
        g3_.begin(), g4_, g4_.begin(), g5_, g5_.begin(), g6_, g6_.begin(), g7_,
        g7_.begin(), g8_, g8_.begin(), g9_, g9_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end(), g3_, g3_.end(),
        g4_, g4_.end(), g5_, g5_.end(), g6_, g6_.end(), g7_, g7_.end(), g8_,
        g8_.end(), g9_, g9_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2,
      const ParamGenerator<T3>& g3,
      const typename ParamGenerator<T3>::iterator& current3,
      const ParamGenerator<T4>& g4,
      const typename ParamGenerator<T4>::iterator& current4,
      const ParamGenerator<T5>& g5,
      const typename ParamGenerator<T5>::iterator& current5,
      const ParamGenerator<T6>& g6,
      const typename ParamGenerator<T6>::iterator& current6,
      const ParamGenerator<T7>& g7,
      const typename ParamGenerator<T7>::iterator& current7,
      const ParamGenerator<T8>& g8,
      const typename ParamGenerator<T8>::iterator& current8,
      const ParamGenerator<T9>& g9,
      const typename ParamGenerator<T9>::iterator& current9)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2),
          begin3_(g3.begin()), end3_(g3.end()), current3_(current3),
          begin4_(g4.begin()), end4_(g4.end()), current4_(current4),
          begin5_(g5.begin()), end5_(g5.end()), current5_(current5),
          begin6_(g6.begin()), end6_(g6.end()), current6_(current6),
          begin7_(g7.begin()), end7_(g7.end()), current7_(current7),
          begin8_(g8.begin()), end8_(g8.end()), current8_(current8),
          begin9_(g9.begin()), end9_(g9.end()), current9_(current9)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current9_;
      if (current9_ == end9_) {
        current9_ = begin9_;
        ++current8_;
      }
      if (current8_ == end8_) {
        current8_ = begin8_;
        ++current7_;
      }
      if (current7_ == end7_) {
        current7_ = begin7_;
        ++current6_;
      }
      if (current6_ == end6_) {
        current6_ = begin6_;
        ++current5_;
      }
      if (current5_ == end5_) {
        current5_ = begin5_;
        ++current4_;
      }
      if (current4_ == end4_) {
        current4_ = begin4_;
        ++current3_;
      }
      if (current3_ == end3_) {
        current3_ = begin3_;
        ++current2_;
      }
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_ &&
          current3_ == typed_other->current3_ &&
          current4_ == typed_other->current4_ &&
          current5_ == typed_other->current5_ &&
          current6_ == typed_other->current6_ &&
          current7_ == typed_other->current7_ &&
          current8_ == typed_other->current8_ &&
          current9_ == typed_other->current9_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_),
        begin3_(other.begin3_),
        end3_(other.end3_),
        current3_(other.current3_),
        begin4_(other.begin4_),
        end4_(other.end4_),
        current4_(other.current4_),
        begin5_(other.begin5_),
        end5_(other.end5_),
        current5_(other.current5_),
        begin6_(other.begin6_),
        end6_(other.end6_),
        current6_(other.current6_),
        begin7_(other.begin7_),
        end7_(other.end7_),
        current7_(other.current7_),
        begin8_(other.begin8_),
        end8_(other.end8_),
        current8_(other.current8_),
        begin9_(other.begin9_),
        end9_(other.end9_),
        current9_(other.current9_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_, *current3_,
            *current4_, *current5_, *current6_, *current7_, *current8_,
            *current9_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_ ||
          current3_ == end3_ ||
          current4_ == end4_ ||
          current5_ == end5_ ||
          current6_ == end6_ ||
          current7_ == end7_ ||
          current8_ == end8_ ||
          current9_ == end9_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    const typename ParamGenerator<T3>::iterator begin3_;
    const typename ParamGenerator<T3>::iterator end3_;
    typename ParamGenerator<T3>::iterator current3_;
    const typename ParamGenerator<T4>::iterator begin4_;
    const typename ParamGenerator<T4>::iterator end4_;
    typename ParamGenerator<T4>::iterator current4_;
    const typename ParamGenerator<T5>::iterator begin5_;
    const typename ParamGenerator<T5>::iterator end5_;
    typename ParamGenerator<T5>::iterator current5_;
    const typename ParamGenerator<T6>::iterator begin6_;
    const typename ParamGenerator<T6>::iterator end6_;
    typename ParamGenerator<T6>::iterator current6_;
    const typename ParamGenerator<T7>::iterator begin7_;
    const typename ParamGenerator<T7>::iterator end7_;
    typename ParamGenerator<T7>::iterator current7_;
    const typename ParamGenerator<T8>::iterator begin8_;
    const typename ParamGenerator<T8>::iterator end8_;
    typename ParamGenerator<T8>::iterator current8_;
    const typename ParamGenerator<T9>::iterator begin9_;
    const typename ParamGenerator<T9>::iterator end9_;
    typename ParamGenerator<T9>::iterator current9_;
    ParamType current_value_;
  };  // class CartesianProductGenerator9::Iterator

  void operator=(const CartesianProductGenerator9& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
  const ParamGenerator<T3> g3_;
  const ParamGenerator<T4> g4_;
  const ParamGenerator<T5> g5_;
  const ParamGenerator<T6> g6_;
  const ParamGenerator<T7> g7_;
  const ParamGenerator<T8> g8_;
  const ParamGenerator<T9> g9_;
};  // class CartesianProductGenerator9

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10>
class CartesianProductGenerator10
    : public ParamGeneratorInterface< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6,
        T7, T8, T9, T10> > {
 public:
  typedef ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> ParamType;

  CartesianProductGenerator10(const ParamGenerator<T1>& g1,
      const ParamGenerator<T2>& g2, const ParamGenerator<T3>& g3,
      const ParamGenerator<T4>& g4, const ParamGenerator<T5>& g5,
      const ParamGenerator<T6>& g6, const ParamGenerator<T7>& g7,
      const ParamGenerator<T8>& g8, const ParamGenerator<T9>& g9,
      const ParamGenerator<T10>& g10)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6), g7_(g7), g8_(g8),
          g9_(g9), g10_(g10) {}
  virtual ~CartesianProductGenerator10() {}

  virtual ParamIteratorInterface<ParamType>* Begin() const {
    return new Iterator(this, g1_, g1_.begin(), g2_, g2_.begin(), g3_,
        g3_.begin(), g4_, g4_.begin(), g5_, g5_.begin(), g6_, g6_.begin(), g7_,
        g7_.begin(), g8_, g8_.begin(), g9_, g9_.begin(), g10_, g10_.begin());
  }
  virtual ParamIteratorInterface<ParamType>* End() const {
    return new Iterator(this, g1_, g1_.end(), g2_, g2_.end(), g3_, g3_.end(),
        g4_, g4_.end(), g5_, g5_.end(), g6_, g6_.end(), g7_, g7_.end(), g8_,
        g8_.end(), g9_, g9_.end(), g10_, g10_.end());
  }

 private:
  class Iterator : public ParamIteratorInterface<ParamType> {
   public:
    Iterator(const ParamGeneratorInterface<ParamType>* base,
      const ParamGenerator<T1>& g1,
      const typename ParamGenerator<T1>::iterator& current1,
      const ParamGenerator<T2>& g2,
      const typename ParamGenerator<T2>::iterator& current2,
      const ParamGenerator<T3>& g3,
      const typename ParamGenerator<T3>::iterator& current3,
      const ParamGenerator<T4>& g4,
      const typename ParamGenerator<T4>::iterator& current4,
      const ParamGenerator<T5>& g5,
      const typename ParamGenerator<T5>::iterator& current5,
      const ParamGenerator<T6>& g6,
      const typename ParamGenerator<T6>::iterator& current6,
      const ParamGenerator<T7>& g7,
      const typename ParamGenerator<T7>::iterator& current7,
      const ParamGenerator<T8>& g8,
      const typename ParamGenerator<T8>::iterator& current8,
      const ParamGenerator<T9>& g9,
      const typename ParamGenerator<T9>::iterator& current9,
      const ParamGenerator<T10>& g10,
      const typename ParamGenerator<T10>::iterator& current10)
        : base_(base),
          begin1_(g1.begin()), end1_(g1.end()), current1_(current1),
          begin2_(g2.begin()), end2_(g2.end()), current2_(current2),
          begin3_(g3.begin()), end3_(g3.end()), current3_(current3),
          begin4_(g4.begin()), end4_(g4.end()), current4_(current4),
          begin5_(g5.begin()), end5_(g5.end()), current5_(current5),
          begin6_(g6.begin()), end6_(g6.end()), current6_(current6),
          begin7_(g7.begin()), end7_(g7.end()), current7_(current7),
          begin8_(g8.begin()), end8_(g8.end()), current8_(current8),
          begin9_(g9.begin()), end9_(g9.end()), current9_(current9),
          begin10_(g10.begin()), end10_(g10.end()), current10_(current10)    {
      ComputeCurrentValue();
    }
    virtual ~Iterator() {}

    virtual const ParamGeneratorInterface<ParamType>* BaseGenerator() const {
      return base_;
    }

    virtual void Advance() {
      assert(!AtEnd());
      ++current10_;
      if (current10_ == end10_) {
        current10_ = begin10_;
        ++current9_;
      }
      if (current9_ == end9_) {
        current9_ = begin9_;
        ++current8_;
      }
      if (current8_ == end8_) {
        current8_ = begin8_;
        ++current7_;
      }
      if (current7_ == end7_) {
        current7_ = begin7_;
        ++current6_;
      }
      if (current6_ == end6_) {
        current6_ = begin6_;
        ++current5_;
      }
      if (current5_ == end5_) {
        current5_ = begin5_;
        ++current4_;
      }
      if (current4_ == end4_) {
        current4_ = begin4_;
        ++current3_;
      }
      if (current3_ == end3_) {
        current3_ = begin3_;
        ++current2_;
      }
      if (current2_ == end2_) {
        current2_ = begin2_;
        ++current1_;
      }
      ComputeCurrentValue();
    }
    virtual ParamIteratorInterface<ParamType>* Clone() const {
      return new Iterator(*this);
    }
    virtual const ParamType* Current() const { return &current_value_; }
    virtual bool Equals(const ParamIteratorInterface<ParamType>& other) const {

      GTEST_CHECK_(BaseGenerator() == other.BaseGenerator())
          << "The program attempted to compare iterators "
          << "from different generators." << std::endl;
      const Iterator* typed_other =
          CheckedDowncastToActualType<const Iterator>(&other);

      return (AtEnd() && typed_other->AtEnd()) ||
         (
          current1_ == typed_other->current1_ &&
          current2_ == typed_other->current2_ &&
          current3_ == typed_other->current3_ &&
          current4_ == typed_other->current4_ &&
          current5_ == typed_other->current5_ &&
          current6_ == typed_other->current6_ &&
          current7_ == typed_other->current7_ &&
          current8_ == typed_other->current8_ &&
          current9_ == typed_other->current9_ &&
          current10_ == typed_other->current10_);
    }

   private:
    Iterator(const Iterator& other)
        : base_(other.base_),
        begin1_(other.begin1_),
        end1_(other.end1_),
        current1_(other.current1_),
        begin2_(other.begin2_),
        end2_(other.end2_),
        current2_(other.current2_),
        begin3_(other.begin3_),
        end3_(other.end3_),
        current3_(other.current3_),
        begin4_(other.begin4_),
        end4_(other.end4_),
        current4_(other.current4_),
        begin5_(other.begin5_),
        end5_(other.end5_),
        current5_(other.current5_),
        begin6_(other.begin6_),
        end6_(other.end6_),
        current6_(other.current6_),
        begin7_(other.begin7_),
        end7_(other.end7_),
        current7_(other.current7_),
        begin8_(other.begin8_),
        end8_(other.end8_),
        current8_(other.current8_),
        begin9_(other.begin9_),
        end9_(other.end9_),
        current9_(other.current9_),
        begin10_(other.begin10_),
        end10_(other.end10_),
        current10_(other.current10_) {
      ComputeCurrentValue();
    }

    void ComputeCurrentValue() {
      if (!AtEnd())
        current_value_ = ParamType(*current1_, *current2_, *current3_,
            *current4_, *current5_, *current6_, *current7_, *current8_,
            *current9_, *current10_);
    }
    bool AtEnd() const {

      return
          current1_ == end1_ ||
          current2_ == end2_ ||
          current3_ == end3_ ||
          current4_ == end4_ ||
          current5_ == end5_ ||
          current6_ == end6_ ||
          current7_ == end7_ ||
          current8_ == end8_ ||
          current9_ == end9_ ||
          current10_ == end10_;
    }

    void operator=(const Iterator& other);

    const ParamGeneratorInterface<ParamType>* const base_;

    const typename ParamGenerator<T1>::iterator begin1_;
    const typename ParamGenerator<T1>::iterator end1_;
    typename ParamGenerator<T1>::iterator current1_;
    const typename ParamGenerator<T2>::iterator begin2_;
    const typename ParamGenerator<T2>::iterator end2_;
    typename ParamGenerator<T2>::iterator current2_;
    const typename ParamGenerator<T3>::iterator begin3_;
    const typename ParamGenerator<T3>::iterator end3_;
    typename ParamGenerator<T3>::iterator current3_;
    const typename ParamGenerator<T4>::iterator begin4_;
    const typename ParamGenerator<T4>::iterator end4_;
    typename ParamGenerator<T4>::iterator current4_;
    const typename ParamGenerator<T5>::iterator begin5_;
    const typename ParamGenerator<T5>::iterator end5_;
    typename ParamGenerator<T5>::iterator current5_;
    const typename ParamGenerator<T6>::iterator begin6_;
    const typename ParamGenerator<T6>::iterator end6_;
    typename ParamGenerator<T6>::iterator current6_;
    const typename ParamGenerator<T7>::iterator begin7_;
    const typename ParamGenerator<T7>::iterator end7_;
    typename ParamGenerator<T7>::iterator current7_;
    const typename ParamGenerator<T8>::iterator begin8_;
    const typename ParamGenerator<T8>::iterator end8_;
    typename ParamGenerator<T8>::iterator current8_;
    const typename ParamGenerator<T9>::iterator begin9_;
    const typename ParamGenerator<T9>::iterator end9_;
    typename ParamGenerator<T9>::iterator current9_;
    const typename ParamGenerator<T10>::iterator begin10_;
    const typename ParamGenerator<T10>::iterator end10_;
    typename ParamGenerator<T10>::iterator current10_;
    ParamType current_value_;
  };  // class CartesianProductGenerator10::Iterator

  void operator=(const CartesianProductGenerator10& other);

  const ParamGenerator<T1> g1_;
  const ParamGenerator<T2> g2_;
  const ParamGenerator<T3> g3_;
  const ParamGenerator<T4> g4_;
  const ParamGenerator<T5> g5_;
  const ParamGenerator<T6> g6_;
  const ParamGenerator<T7> g7_;
  const ParamGenerator<T8> g8_;
  const ParamGenerator<T9> g9_;
  const ParamGenerator<T10> g10_;
};  // class CartesianProductGenerator10

template <class Generator1, class Generator2>
class CartesianProductHolder2 {
 public:
CartesianProductHolder2(const Generator1& g1, const Generator2& g2)
      : g1_(g1), g2_(g2) {}
  template <typename T1, typename T2>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2> >(
        new CartesianProductGenerator2<T1, T2>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_)));
  }

 private:

  void operator=(const CartesianProductHolder2& other);

  const Generator1 g1_;
  const Generator2 g2_;
};  // class CartesianProductHolder2

template <class Generator1, class Generator2, class Generator3>
class CartesianProductHolder3 {
 public:
CartesianProductHolder3(const Generator1& g1, const Generator2& g2,
    const Generator3& g3)
      : g1_(g1), g2_(g2), g3_(g3) {}
  template <typename T1, typename T2, typename T3>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2, T3> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2, T3> >(
        new CartesianProductGenerator3<T1, T2, T3>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_),
        static_cast<ParamGenerator<T3> >(g3_)));
  }

 private:

  void operator=(const CartesianProductHolder3& other);

  const Generator1 g1_;
  const Generator2 g2_;
  const Generator3 g3_;
};  // class CartesianProductHolder3

template <class Generator1, class Generator2, class Generator3,
    class Generator4>
class CartesianProductHolder4 {
 public:
CartesianProductHolder4(const Generator1& g1, const Generator2& g2,
    const Generator3& g3, const Generator4& g4)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4) {}
  template <typename T1, typename T2, typename T3, typename T4>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4> >(
        new CartesianProductGenerator4<T1, T2, T3, T4>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_),
        static_cast<ParamGenerator<T3> >(g3_),
        static_cast<ParamGenerator<T4> >(g4_)));
  }

 private:

  void operator=(const CartesianProductHolder4& other);

  const Generator1 g1_;
  const Generator2 g2_;
  const Generator3 g3_;
  const Generator4 g4_;
};  // class CartesianProductHolder4

template <class Generator1, class Generator2, class Generator3,
    class Generator4, class Generator5>
class CartesianProductHolder5 {
 public:
CartesianProductHolder5(const Generator1& g1, const Generator2& g2,
    const Generator3& g3, const Generator4& g4, const Generator5& g5)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5) {}
  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5> >(
        new CartesianProductGenerator5<T1, T2, T3, T4, T5>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_),
        static_cast<ParamGenerator<T3> >(g3_),
        static_cast<ParamGenerator<T4> >(g4_),
        static_cast<ParamGenerator<T5> >(g5_)));
  }

 private:

  void operator=(const CartesianProductHolder5& other);

  const Generator1 g1_;
  const Generator2 g2_;
  const Generator3 g3_;
  const Generator4 g4_;
  const Generator5 g5_;
};  // class CartesianProductHolder5

template <class Generator1, class Generator2, class Generator3,
    class Generator4, class Generator5, class Generator6>
class CartesianProductHolder6 {
 public:
CartesianProductHolder6(const Generator1& g1, const Generator2& g2,
    const Generator3& g3, const Generator4& g4, const Generator5& g5,
    const Generator6& g6)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6) {}
  template <typename T1, typename T2, typename T3, typename T4, typename T5,
      typename T6>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6> >(
        new CartesianProductGenerator6<T1, T2, T3, T4, T5, T6>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_),
        static_cast<ParamGenerator<T3> >(g3_),
        static_cast<ParamGenerator<T4> >(g4_),
        static_cast<ParamGenerator<T5> >(g5_),
        static_cast<ParamGenerator<T6> >(g6_)));
  }

 private:

  void operator=(const CartesianProductHolder6& other);

  const Generator1 g1_;
  const Generator2 g2_;
  const Generator3 g3_;
  const Generator4 g4_;
  const Generator5 g5_;
  const Generator6 g6_;
};  // class CartesianProductHolder6

template <class Generator1, class Generator2, class Generator3,
    class Generator4, class Generator5, class Generator6, class Generator7>
class CartesianProductHolder7 {
 public:
CartesianProductHolder7(const Generator1& g1, const Generator2& g2,
    const Generator3& g3, const Generator4& g4, const Generator5& g5,
    const Generator6& g6, const Generator7& g7)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6), g7_(g7) {}
  template <typename T1, typename T2, typename T3, typename T4, typename T5,
      typename T6, typename T7>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6,
      T7> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7> >(
        new CartesianProductGenerator7<T1, T2, T3, T4, T5, T6, T7>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_),
        static_cast<ParamGenerator<T3> >(g3_),
        static_cast<ParamGenerator<T4> >(g4_),
        static_cast<ParamGenerator<T5> >(g5_),
        static_cast<ParamGenerator<T6> >(g6_),
        static_cast<ParamGenerator<T7> >(g7_)));
  }

 private:

  void operator=(const CartesianProductHolder7& other);

  const Generator1 g1_;
  const Generator2 g2_;
  const Generator3 g3_;
  const Generator4 g4_;
  const Generator5 g5_;
  const Generator6 g6_;
  const Generator7 g7_;
};  // class CartesianProductHolder7

template <class Generator1, class Generator2, class Generator3,
    class Generator4, class Generator5, class Generator6, class Generator7,
    class Generator8>
class CartesianProductHolder8 {
 public:
CartesianProductHolder8(const Generator1& g1, const Generator2& g2,
    const Generator3& g3, const Generator4& g4, const Generator5& g5,
    const Generator6& g6, const Generator7& g7, const Generator8& g8)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6), g7_(g7),
          g8_(g8) {}
  template <typename T1, typename T2, typename T3, typename T4, typename T5,
      typename T6, typename T7, typename T8>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7,
      T8> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8> >(
        new CartesianProductGenerator8<T1, T2, T3, T4, T5, T6, T7, T8>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_),
        static_cast<ParamGenerator<T3> >(g3_),
        static_cast<ParamGenerator<T4> >(g4_),
        static_cast<ParamGenerator<T5> >(g5_),
        static_cast<ParamGenerator<T6> >(g6_),
        static_cast<ParamGenerator<T7> >(g7_),
        static_cast<ParamGenerator<T8> >(g8_)));
  }

 private:

  void operator=(const CartesianProductHolder8& other);

  const Generator1 g1_;
  const Generator2 g2_;
  const Generator3 g3_;
  const Generator4 g4_;
  const Generator5 g5_;
  const Generator6 g6_;
  const Generator7 g7_;
  const Generator8 g8_;
};  // class CartesianProductHolder8

template <class Generator1, class Generator2, class Generator3,
    class Generator4, class Generator5, class Generator6, class Generator7,
    class Generator8, class Generator9>
class CartesianProductHolder9 {
 public:
CartesianProductHolder9(const Generator1& g1, const Generator2& g2,
    const Generator3& g3, const Generator4& g4, const Generator5& g5,
    const Generator6& g6, const Generator7& g7, const Generator8& g8,
    const Generator9& g9)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6), g7_(g7), g8_(g8),
          g9_(g9) {}
  template <typename T1, typename T2, typename T3, typename T4, typename T5,
      typename T6, typename T7, typename T8, typename T9>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8,
      T9> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8,
        T9> >(
        new CartesianProductGenerator9<T1, T2, T3, T4, T5, T6, T7, T8, T9>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_),
        static_cast<ParamGenerator<T3> >(g3_),
        static_cast<ParamGenerator<T4> >(g4_),
        static_cast<ParamGenerator<T5> >(g5_),
        static_cast<ParamGenerator<T6> >(g6_),
        static_cast<ParamGenerator<T7> >(g7_),
        static_cast<ParamGenerator<T8> >(g8_),
        static_cast<ParamGenerator<T9> >(g9_)));
  }

 private:

  void operator=(const CartesianProductHolder9& other);

  const Generator1 g1_;
  const Generator2 g2_;
  const Generator3 g3_;
  const Generator4 g4_;
  const Generator5 g5_;
  const Generator6 g6_;
  const Generator7 g7_;
  const Generator8 g8_;
  const Generator9 g9_;
};  // class CartesianProductHolder9

template <class Generator1, class Generator2, class Generator3,
    class Generator4, class Generator5, class Generator6, class Generator7,
    class Generator8, class Generator9, class Generator10>
class CartesianProductHolder10 {
 public:
CartesianProductHolder10(const Generator1& g1, const Generator2& g2,
    const Generator3& g3, const Generator4& g4, const Generator5& g5,
    const Generator6& g6, const Generator7& g7, const Generator8& g8,
    const Generator9& g9, const Generator10& g10)
      : g1_(g1), g2_(g2), g3_(g3), g4_(g4), g5_(g5), g6_(g6), g7_(g7), g8_(g8),
          g9_(g9), g10_(g10) {}
  template <typename T1, typename T2, typename T3, typename T4, typename T5,
      typename T6, typename T7, typename T8, typename T9, typename T10>
  operator ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8,
      T9, T10> >() const {
    return ParamGenerator< ::std::tr1::tuple<T1, T2, T3, T4, T5, T6, T7, T8,
        T9, T10> >(
        new CartesianProductGenerator10<T1, T2, T3, T4, T5, T6, T7, T8, T9,
            T10>(
        static_cast<ParamGenerator<T1> >(g1_),
        static_cast<ParamGenerator<T2> >(g2_),
        static_cast<ParamGenerator<T3> >(g3_),
        static_cast<ParamGenerator<T4> >(g4_),
        static_cast<ParamGenerator<T5> >(g5_),
        static_cast<ParamGenerator<T6> >(g6_),
        static_cast<ParamGenerator<T7> >(g7_),
        static_cast<ParamGenerator<T8> >(g8_),
        static_cast<ParamGenerator<T9> >(g9_),
        static_cast<ParamGenerator<T10> >(g10_)));
  }

 private:

  void operator=(const CartesianProductHolder10& other);

  const Generator1 g1_;
  const Generator2 g2_;
  const Generator3 g3_;
  const Generator4 g4_;
  const Generator5 g5_;
  const Generator6 g6_;
  const Generator7 g7_;
  const Generator8 g8_;
  const Generator9 g9_;
  const Generator10 g10_;
};  // class CartesianProductHolder10

# endif  // GTEST_HAS_COMBINE

}  // namespace internal
}  // namespace testing

#endif  //  GTEST_HAS_PARAM_TEST

#endif  // GTEST_INCLUDE_GTEST_INTERNAL_GTEST_PARAM_UTIL_GENERATED_H_

#if GTEST_HAS_PARAM_TEST

namespace testing {

template <typename T, typename IncrementT>
internal::ParamGenerator<T> Range(T start, T end, IncrementT step) {
  return internal::ParamGenerator<T>(
      new internal::RangeGenerator<T, IncrementT>(start, end, step));
}

template <typename T>
internal::ParamGenerator<T> Range(T start, T end) {
  return Range(start, end, 1);
}

template <typename ForwardIterator>
internal::ParamGenerator<
  typename ::testing::internal::IteratorTraits<ForwardIterator>::value_type>
ValuesIn(ForwardIterator begin, ForwardIterator end) {
  typedef typename ::testing::internal::IteratorTraits<ForwardIterator>
      ::value_type ParamType;
  return internal::ParamGenerator<ParamType>(
      new internal::ValuesInIteratorRangeGenerator<ParamType>(begin, end));
}

template <typename T, size_t N>
internal::ParamGenerator<T> ValuesIn(const T (&array)[N]) {
  return ValuesIn(array, array + N);
}

template <class Container>
internal::ParamGenerator<typename Container::value_type> ValuesIn(
    const Container& container) {
  return ValuesIn(container.begin(), container.end());
}

template <typename T1>
internal::ValueArray1<T1> Values(T1 v1) {
  return internal::ValueArray1<T1>(v1);
}

template <typename T1, typename T2>
internal::ValueArray2<T1, T2> Values(T1 v1, T2 v2) {
  return internal::ValueArray2<T1, T2>(v1, v2);
}

template <typename T1, typename T2, typename T3>
internal::ValueArray3<T1, T2, T3> Values(T1 v1, T2 v2, T3 v3) {
  return internal::ValueArray3<T1, T2, T3>(v1, v2, v3);
}

template <typename T1, typename T2, typename T3, typename T4>
internal::ValueArray4<T1, T2, T3, T4> Values(T1 v1, T2 v2, T3 v3, T4 v4) {
  return internal::ValueArray4<T1, T2, T3, T4>(v1, v2, v3, v4);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
internal::ValueArray5<T1, T2, T3, T4, T5> Values(T1 v1, T2 v2, T3 v3, T4 v4,
    T5 v5) {
  return internal::ValueArray5<T1, T2, T3, T4, T5>(v1, v2, v3, v4, v5);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6>
internal::ValueArray6<T1, T2, T3, T4, T5, T6> Values(T1 v1, T2 v2, T3 v3,
    T4 v4, T5 v5, T6 v6) {
  return internal::ValueArray6<T1, T2, T3, T4, T5, T6>(v1, v2, v3, v4, v5, v6);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7>
internal::ValueArray7<T1, T2, T3, T4, T5, T6, T7> Values(T1 v1, T2 v2, T3 v3,
    T4 v4, T5 v5, T6 v6, T7 v7) {
  return internal::ValueArray7<T1, T2, T3, T4, T5, T6, T7>(v1, v2, v3, v4, v5,
      v6, v7);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8>
internal::ValueArray8<T1, T2, T3, T4, T5, T6, T7, T8> Values(T1 v1, T2 v2,
    T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8) {
  return internal::ValueArray8<T1, T2, T3, T4, T5, T6, T7, T8>(v1, v2, v3, v4,
      v5, v6, v7, v8);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9>
internal::ValueArray9<T1, T2, T3, T4, T5, T6, T7, T8, T9> Values(T1 v1, T2 v2,
    T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9) {
  return internal::ValueArray9<T1, T2, T3, T4, T5, T6, T7, T8, T9>(v1, v2, v3,
      v4, v5, v6, v7, v8, v9);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10>
internal::ValueArray10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> Values(T1 v1,
    T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10) {
  return internal::ValueArray10<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>(v1,
      v2, v3, v4, v5, v6, v7, v8, v9, v10);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11>
internal::ValueArray11<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
    T11> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11) {
  return internal::ValueArray11<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
      T11>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12>
internal::ValueArray12<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
    T12> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12) {
  return internal::ValueArray12<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13>
internal::ValueArray13<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,
    T13> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13) {
  return internal::ValueArray13<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14>
internal::ValueArray14<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14) {
  return internal::ValueArray14<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,
      v14);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15>
internal::ValueArray15<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8,
    T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15) {
  return internal::ValueArray15<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12,
      v13, v14, v15);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16>
internal::ValueArray16<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7,
    T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16) {
  return internal::ValueArray16<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,
      v12, v13, v14, v15, v16);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17>
internal::ValueArray17<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7,
    T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16, T17 v17) {
  return internal::ValueArray17<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,
      v11, v12, v13, v14, v15, v16, v17);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18>
internal::ValueArray18<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6,
    T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16, T17 v17, T18 v18) {
  return internal::ValueArray18<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18>(v1, v2, v3, v4, v5, v6, v7, v8, v9,
      v10, v11, v12, v13, v14, v15, v16, v17, v18);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19>
internal::ValueArray19<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5,
    T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14,
    T15 v15, T16 v16, T17 v17, T18 v18, T19 v19) {
  return internal::ValueArray19<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19>(v1, v2, v3, v4, v5, v6, v7, v8,
      v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20>
internal::ValueArray20<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20> Values(T1 v1, T2 v2, T3 v3, T4 v4,
    T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13,
    T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20) {
  return internal::ValueArray20<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20>(v1, v2, v3, v4, v5, v6, v7,
      v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21>
internal::ValueArray21<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21> Values(T1 v1, T2 v2, T3 v3, T4 v4,
    T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13,
    T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21) {
  return internal::ValueArray21<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21>(v1, v2, v3, v4, v5, v6,
      v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22>
internal::ValueArray22<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22> Values(T1 v1, T2 v2, T3 v3,
    T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12,
    T13 v13, T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20,
    T21 v21, T22 v22) {
  return internal::ValueArray22<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22>(v1, v2, v3, v4,
      v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
      v20, v21, v22);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23>
internal::ValueArray23<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23> Values(T1 v1, T2 v2,
    T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12,
    T13 v13, T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20,
    T21 v21, T22 v22, T23 v23) {
  return internal::ValueArray23<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23>(v1, v2, v3,
      v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
      v20, v21, v22, v23);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24>
internal::ValueArray24<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24> Values(T1 v1, T2 v2,
    T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12,
    T13 v13, T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20,
    T21 v21, T22 v22, T23 v23, T24 v24) {
  return internal::ValueArray24<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24>(v1, v2,
      v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18,
      v19, v20, v21, v22, v23, v24);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25>
internal::ValueArray25<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25> Values(T1 v1,
    T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11,
    T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19,
    T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25) {
  return internal::ValueArray25<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25>(v1,
      v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17,
      v18, v19, v20, v21, v22, v23, v24, v25);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26>
internal::ValueArray26<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
    T26> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
    T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
    T26 v26) {
  return internal::ValueArray26<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,
      v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27>
internal::ValueArray27<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,
    T27> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
    T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
    T26 v26, T27 v27) {
  return internal::ValueArray27<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,
      v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28>
internal::ValueArray28<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
    T28> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
    T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
    T26 v26, T27 v27, T28 v28) {
  return internal::ValueArray28<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,
      v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27,
      v28);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29>
internal::ValueArray29<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
    T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
    T26 v26, T27 v27, T28 v28, T29 v29) {
  return internal::ValueArray29<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12,
      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26,
      v27, v28, v29);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30>
internal::ValueArray30<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8,
    T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16,
    T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24,
    T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30) {
  return internal::ValueArray30<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,
      v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,
      v26, v27, v28, v29, v30);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31>
internal::ValueArray31<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7,
    T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23,
    T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31) {
  return internal::ValueArray31<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,
      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24,
      v25, v26, v27, v28, v29, v30, v31);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32>
internal::ValueArray32<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7,
    T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23,
    T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31,
    T32 v32) {
  return internal::ValueArray32<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32>(v1, v2, v3, v4, v5, v6, v7, v8, v9,
      v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,
      v24, v25, v26, v27, v28, v29, v30, v31, v32);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33>
internal::ValueArray33<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6,
    T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23,
    T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31,
    T32 v32, T33 v33) {
  return internal::ValueArray33<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33>(v1, v2, v3, v4, v5, v6, v7, v8,
      v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,
      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34>
internal::ValueArray34<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5,
    T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14,
    T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22,
    T23 v23, T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30,
    T31 v31, T32 v32, T33 v33, T34 v34) {
  return internal::ValueArray34<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34>(v1, v2, v3, v4, v5, v6, v7,
      v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22,
      v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35>
internal::ValueArray35<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35> Values(T1 v1, T2 v2, T3 v3, T4 v4,
    T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13,
    T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21,
    T22 v22, T23 v23, T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29,
    T30 v30, T31 v31, T32 v32, T33 v33, T34 v34, T35 v35) {
  return internal::ValueArray35<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35>(v1, v2, v3, v4, v5, v6,
      v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,
      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36>
internal::ValueArray36<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36> Values(T1 v1, T2 v2, T3 v3, T4 v4,
    T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13,
    T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21,
    T22 v22, T23 v23, T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29,
    T30 v30, T31 v31, T32 v32, T33 v33, T34 v34, T35 v35, T36 v36) {
  return internal::ValueArray36<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36>(v1, v2, v3, v4,
      v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
      v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33,
      v34, v35, v36);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37>
internal::ValueArray37<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37> Values(T1 v1, T2 v2, T3 v3,
    T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12,
    T13 v13, T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20,
    T21 v21, T22 v22, T23 v23, T24 v24, T25 v25, T26 v26, T27 v27, T28 v28,
    T29 v29, T30 v30, T31 v31, T32 v32, T33 v33, T34 v34, T35 v35, T36 v36,
    T37 v37) {
  return internal::ValueArray37<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37>(v1, v2, v3,
      v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
      v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33,
      v34, v35, v36, v37);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38>
internal::ValueArray38<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38> Values(T1 v1, T2 v2,
    T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12,
    T13 v13, T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20,
    T21 v21, T22 v22, T23 v23, T24 v24, T25 v25, T26 v26, T27 v27, T28 v28,
    T29 v29, T30 v30, T31 v31, T32 v32, T33 v33, T34 v34, T35 v35, T36 v36,
    T37 v37, T38 v38) {
  return internal::ValueArray38<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38>(v1, v2,
      v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18,
      v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32,
      v33, v34, v35, v36, v37, v38);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39>
internal::ValueArray39<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39> Values(T1 v1, T2 v2,
    T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12,
    T13 v13, T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20,
    T21 v21, T22 v22, T23 v23, T24 v24, T25 v25, T26 v26, T27 v27, T28 v28,
    T29 v29, T30 v30, T31 v31, T32 v32, T33 v33, T34 v34, T35 v35, T36 v36,
    T37 v37, T38 v38, T39 v39) {
  return internal::ValueArray39<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39>(v1,
      v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17,
      v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31,
      v32, v33, v34, v35, v36, v37, v38, v39);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40>
internal::ValueArray40<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40> Values(T1 v1,
    T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11,
    T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19,
    T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25, T26 v26, T27 v27,
    T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33, T34 v34, T35 v35,
    T36 v36, T37 v37, T38 v38, T39 v39, T40 v40) {
  return internal::ValueArray40<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,
      v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,
      v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41>
internal::ValueArray41<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
    T41> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
    T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
    T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
    T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41) {
  return internal::ValueArray41<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14,
      v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28,
      v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42>
internal::ValueArray42<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41,
    T42> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
    T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
    T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
    T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
    T42 v42) {
  return internal::ValueArray42<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13,
      v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27,
      v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40, v41,
      v42);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43>
internal::ValueArray43<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,
    T43> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
    T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
    T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
    T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
    T42 v42, T43 v43) {
  return internal::ValueArray43<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42, T43>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12,
      v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26,
      v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39, v40,
      v41, v42, v43);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44>
internal::ValueArray44<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    T44> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8, T9 v9,
    T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16, T17 v17,
    T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24, T25 v25,
    T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32, T33 v33,
    T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40, T41 v41,
    T42 v42, T43 v43, T44 v44) {
  return internal::ValueArray44<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42, T43, T44>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11,
      v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25,
      v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,
      v40, v41, v42, v43, v44);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45>
internal::ValueArray45<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    T44, T45> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7, T8 v8,
    T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15, T16 v16,
    T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23, T24 v24,
    T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31, T32 v32,
    T33 v33, T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39, T40 v40,
    T41 v41, T42 v42, T43 v43, T44 v44, T45 v45) {
  return internal::ValueArray45<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42, T43, T44, T45>(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10,
      v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24,
      v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37, v38,
      v39, v40, v41, v42, v43, v44, v45);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46>
internal::ValueArray46<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    T44, T45, T46> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7,
    T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23,
    T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31,
    T32 v32, T33 v33, T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39,
    T40 v40, T41 v41, T42 v42, T43 v43, T44 v44, T45 v45, T46 v46) {
  return internal::ValueArray46<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42, T43, T44, T45, T46>(v1, v2, v3, v4, v5, v6, v7, v8, v9,
      v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,
      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,
      v38, v39, v40, v41, v42, v43, v44, v45, v46);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47>
internal::ValueArray47<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    T44, T45, T46, T47> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6, T7 v7,
    T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23,
    T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31,
    T32 v32, T33 v33, T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39,
    T40 v40, T41 v41, T42 v42, T43 v43, T44 v44, T45 v45, T46 v46, T47 v47) {
  return internal::ValueArray47<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42, T43, T44, T45, T46, T47>(v1, v2, v3, v4, v5, v6, v7, v8,
      v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23,
      v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36, v37,
      v38, v39, v40, v41, v42, v43, v44, v45, v46, v47);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48>
internal::ValueArray48<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    T44, T45, T46, T47, T48> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6,
    T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14, T15 v15,
    T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22, T23 v23,
    T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30, T31 v31,
    T32 v32, T33 v33, T34 v34, T35 v35, T36 v36, T37 v37, T38 v38, T39 v39,
    T40 v40, T41 v41, T42 v42, T43 v43, T44 v44, T45 v45, T46 v46, T47 v47,
    T48 v48) {
  return internal::ValueArray48<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42, T43, T44, T45, T46, T47, T48>(v1, v2, v3, v4, v5, v6, v7,
      v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22,
      v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36,
      v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48, typename T49>
internal::ValueArray49<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    T44, T45, T46, T47, T48, T49> Values(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5,
    T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13, T14 v14,
    T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21, T22 v22,
    T23 v23, T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29, T30 v30,
    T31 v31, T32 v32, T33 v33, T34 v34, T35 v35, T36 v36, T37 v37, T38 v38,
    T39 v39, T40 v40, T41 v41, T42 v42, T43 v43, T44 v44, T45 v45, T46 v46,
    T47 v47, T48 v48, T49 v49) {
  return internal::ValueArray49<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42, T43, T44, T45, T46, T47, T48, T49>(v1, v2, v3, v4, v5, v6,
      v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21,
      v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35,
      v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47, v48, v49);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5,
    typename T6, typename T7, typename T8, typename T9, typename T10,
    typename T11, typename T12, typename T13, typename T14, typename T15,
    typename T16, typename T17, typename T18, typename T19, typename T20,
    typename T21, typename T22, typename T23, typename T24, typename T25,
    typename T26, typename T27, typename T28, typename T29, typename T30,
    typename T31, typename T32, typename T33, typename T34, typename T35,
    typename T36, typename T37, typename T38, typename T39, typename T40,
    typename T41, typename T42, typename T43, typename T44, typename T45,
    typename T46, typename T47, typename T48, typename T49, typename T50>
internal::ValueArray50<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
    T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28,
    T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43,
    T44, T45, T46, T47, T48, T49, T50> Values(T1 v1, T2 v2, T3 v3, T4 v4,
    T5 v5, T6 v6, T7 v7, T8 v8, T9 v9, T10 v10, T11 v11, T12 v12, T13 v13,
    T14 v14, T15 v15, T16 v16, T17 v17, T18 v18, T19 v19, T20 v20, T21 v21,
    T22 v22, T23 v23, T24 v24, T25 v25, T26 v26, T27 v27, T28 v28, T29 v29,
    T30 v30, T31 v31, T32 v32, T33 v33, T34 v34, T35 v35, T36 v36, T37 v37,
    T38 v38, T39 v39, T40 v40, T41 v41, T42 v42, T43 v43, T44 v44, T45 v45,
    T46 v46, T47 v47, T48 v48, T49 v49, T50 v50) {
  return internal::ValueArray50<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
      T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
      T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39,
      T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50>(v1, v2, v3, v4,
      v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
      v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33,
      v34, v35, v36, v37, v38, v39, v40, v41, v42, v43, v44, v45, v46, v47,
      v48, v49, v50);
}

inline internal::ParamGenerator<bool> Bool() {
  return Values(false, true);
}

# if GTEST_HAS_COMBINE

template <typename Generator1, typename Generator2>
internal::CartesianProductHolder2<Generator1, Generator2> Combine(
    const Generator1& g1, const Generator2& g2) {
  return internal::CartesianProductHolder2<Generator1, Generator2>(
      g1, g2);
}

template <typename Generator1, typename Generator2, typename Generator3>
internal::CartesianProductHolder3<Generator1, Generator2, Generator3> Combine(
    const Generator1& g1, const Generator2& g2, const Generator3& g3) {
  return internal::CartesianProductHolder3<Generator1, Generator2, Generator3>(
      g1, g2, g3);
}

template <typename Generator1, typename Generator2, typename Generator3,
    typename Generator4>
internal::CartesianProductHolder4<Generator1, Generator2, Generator3,
    Generator4> Combine(
    const Generator1& g1, const Generator2& g2, const Generator3& g3,
        const Generator4& g4) {
  return internal::CartesianProductHolder4<Generator1, Generator2, Generator3,
      Generator4>(
      g1, g2, g3, g4);
}

template <typename Generator1, typename Generator2, typename Generator3,
    typename Generator4, typename Generator5>
internal::CartesianProductHolder5<Generator1, Generator2, Generator3,
    Generator4, Generator5> Combine(
    const Generator1& g1, const Generator2& g2, const Generator3& g3,
        const Generator4& g4, const Generator5& g5) {
  return internal::CartesianProductHolder5<Generator1, Generator2, Generator3,
      Generator4, Generator5>(
      g1, g2, g3, g4, g5);
}

template <typename Generator1, typename Generator2, typename Generator3,
    typename Generator4, typename Generator5, typename Generator6>
internal::CartesianProductHolder6<Generator1, Generator2, Generator3,
    Generator4, Generator5, Generator6> Combine(
    const Generator1& g1, const Generator2& g2, const Generator3& g3,
        const Generator4& g4, const Generator5& g5, const Generator6& g6) {
  return internal::CartesianProductHolder6<Generator1, Generator2, Generator3,
      Generator4, Generator5, Generator6>(
      g1, g2, g3, g4, g5, g6);
}

template <typename Generator1, typename Generator2, typename Generator3,
    typename Generator4, typename Generator5, typename Generator6,
    typename Generator7>
internal::CartesianProductHolder7<Generator1, Generator2, Generator3,
    Generator4, Generator5, Generator6, Generator7> Combine(
    const Generator1& g1, const Generator2& g2, const Generator3& g3,
        const Generator4& g4, const Generator5& g5, const Generator6& g6,
        const Generator7& g7) {
  return internal::CartesianProductHolder7<Generator1, Generator2, Generator3,
      Generator4, Generator5, Generator6, Generator7>(
      g1, g2, g3, g4, g5, g6, g7);
}

template <typename Generator1, typename Generator2, typename Generator3,
    typename Generator4, typename Generator5, typename Generator6,
    typename Generator7, typename Generator8>
internal::CartesianProductHolder8<Generator1, Generator2, Generator3,
    Generator4, Generator5, Generator6, Generator7, Generator8> Combine(
    const Generator1& g1, const Generator2& g2, const Generator3& g3,
        const Generator4& g4, const Generator5& g5, const Generator6& g6,
        const Generator7& g7, const Generator8& g8) {
  return internal::CartesianProductHolder8<Generator1, Generator2, Generator3,
      Generator4, Generator5, Generator6, Generator7, Generator8>(
      g1, g2, g3, g4, g5, g6, g7, g8);
}

template <typename Generator1, typename Generator2, typename Generator3,
    typename Generator4, typename Generator5, typename Generator6,
    typename Generator7, typename Generator8, typename Generator9>
internal::CartesianProductHolder9<Generator1, Generator2, Generator3,
    Generator4, Generator5, Generator6, Generator7, Generator8,
    Generator9> Combine(
    const Generator1& g1, const Generator2& g2, const Generator3& g3,
        const Generator4& g4, const Generator5& g5, const Generator6& g6,
        const Generator7& g7, const Generator8& g8, const Generator9& g9) {
  return internal::CartesianProductHolder9<Generator1, Generator2, Generator3,
      Generator4, Generator5, Generator6, Generator7, Generator8, Generator9>(
      g1, g2, g3, g4, g5, g6, g7, g8, g9);
}

template <typename Generator1, typename Generator2, typename Generator3,
    typename Generator4, typename Generator5, typename Generator6,
    typename Generator7, typename Generator8, typename Generator9,
    typename Generator10>
internal::CartesianProductHolder10<Generator1, Generator2, Generator3,
    Generator4, Generator5, Generator6, Generator7, Generator8, Generator9,
    Generator10> Combine(
    const Generator1& g1, const Generator2& g2, const Generator3& g3,
        const Generator4& g4, const Generator5& g5, const Generator6& g6,
        const Generator7& g7, const Generator8& g8, const Generator9& g9,
        const Generator10& g10) {
  return internal::CartesianProductHolder10<Generator1, Generator2, Generator3,
      Generator4, Generator5, Generator6, Generator7, Generator8, Generator9,
      Generator10>(
      g1, g2, g3, g4, g5, g6, g7, g8, g9, g10);
}
# endif  // GTEST_HAS_COMBINE

# define TEST_P(test_case_name, test_name) \
  class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) \
      : public test_case_name { \
   public: \
    GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() {} \
    virtual void TestBody(); \
   private: \
    static int AddToRegistry() { \
      ::testing::UnitTest::GetInstance()->parameterized_test_registry(). \
          GetTestCasePatternHolder<test_case_name>(\
              #test_case_name, __FILE__, __LINE__)->AddTestPattern(\
                  #test_case_name, \
                  #test_name, \
                  new ::testing::internal::TestMetaFactory< \
                      GTEST_TEST_CLASS_NAME_(test_case_name, test_name)>()); \
      return 0; \
    } \
    static int gtest_registering_dummy_; \
    GTEST_DISALLOW_COPY_AND_ASSIGN_(\
        GTEST_TEST_CLASS_NAME_(test_case_name, test_name)); \
  }; \
  int GTEST_TEST_CLASS_NAME_(test_case_name, \
                             test_name)::gtest_registering_dummy_ = \
      GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::AddToRegistry(); \
  void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody()

# define INSTANTIATE_TEST_CASE_P(prefix, test_case_name, generator) \
  ::testing::internal::ParamGenerator<test_case_name::ParamType> \
      gtest_##prefix##test_case_name##_EvalGenerator_() { return generator; } \
  int gtest_##prefix##test_case_name##_dummy_ = \
      ::testing::UnitTest::GetInstance()->parameterized_test_registry(). \
          GetTestCasePatternHolder<test_case_name>(\
              #test_case_name, __FILE__, __LINE__)->AddTestCaseInstantiation(\
                  #prefix, \
                  &gtest_##prefix##test_case_name##_EvalGenerator_, \
                  __FILE__, __LINE__)

}  // namespace testing

#endif  // GTEST_HAS_PARAM_TEST

#endif  // GTEST_INCLUDE_GTEST_GTEST_PARAM_TEST_H_

#ifndef GTEST_INCLUDE_GTEST_GTEST_PROD_H_
#define GTEST_INCLUDE_GTEST_GTEST_PROD_H_

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

#endif  // GTEST_INCLUDE_GTEST_GTEST_PROD_H_

#ifndef GTEST_INCLUDE_GTEST_GTEST_TEST_PART_H_
#define GTEST_INCLUDE_GTEST_GTEST_TEST_PART_H_

#include <iosfwd>
#include <vector>

namespace testing {

class GTEST_API_ TestPartResult {
 public:

  enum Type {
    kSuccess,          // Succeeded.
    kNonFatalFailure,  // Failed but the test can continue.
    kFatalFailure      // Failed and the test should be terminated.
  };

  TestPartResult(Type a_type,
                 const char* a_file_name,
                 int a_line_number,
                 const char* a_message)
      : type_(a_type),
        file_name_(a_file_name == NULL ? "" : a_file_name),
        line_number_(a_line_number),
        summary_(ExtractSummary(a_message)),
        message_(a_message) {
  }

  Type type() const { return type_; }

  const char* file_name() const {
    return file_name_.empty() ? NULL : file_name_.c_str();
  }

  int line_number() const { return line_number_; }

  const char* summary() const { return summary_.c_str(); }

  const char* message() const { return message_.c_str(); }

  bool passed() const { return type_ == kSuccess; }

  bool failed() const { return type_ != kSuccess; }

  bool nonfatally_failed() const { return type_ == kNonFatalFailure; }

  bool fatally_failed() const { return type_ == kFatalFailure; }

 private:
  Type type_;

  static std::string ExtractSummary(const char* message);

  std::string file_name_;

  int line_number_;
  std::string summary_;  // The test failure summary.
  std::string message_;  // The test failure message.
};

std::ostream& operator<<(std::ostream& os, const TestPartResult& result);

class GTEST_API_ TestPartResultArray {
 public:
  TestPartResultArray() {}

  void Append(const TestPartResult& result);

  const TestPartResult& GetTestPartResult(int index) const;

  int size() const;

 private:
  std::vector<TestPartResult> array_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestPartResultArray);
};

class TestPartResultReporterInterface {
 public:
  virtual ~TestPartResultReporterInterface() {}

  virtual void ReportTestPartResult(const TestPartResult& result) = 0;
};

namespace internal {

class GTEST_API_ HasNewFatalFailureHelper
    : public TestPartResultReporterInterface {
 public:
  HasNewFatalFailureHelper();
  virtual ~HasNewFatalFailureHelper();
  virtual void ReportTestPartResult(const TestPartResult& result);
  bool has_new_fatal_failure() const { return has_new_fatal_failure_; }
 private:
  bool has_new_fatal_failure_;
  TestPartResultReporterInterface* original_reporter_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(HasNewFatalFailureHelper);
};

}  // namespace internal

}  // namespace testing

#endif  // GTEST_INCLUDE_GTEST_GTEST_TEST_PART_H_

#ifndef GTEST_INCLUDE_GTEST_GTEST_TYPED_TEST_H_
#define GTEST_INCLUDE_GTEST_GTEST_TYPED_TEST_H_

#if 0

template <typename T>
class FooTest : public testing::Test {
 public:
  ...
  typedef std::list<T> List;
  static T shared_;
  T value_;
};

typedef testing::Types<char, int, unsigned int> MyTypes;
TYPED_TEST_CASE(FooTest, MyTypes);

TYPED_TEST(FooTest, DoesBlah) {

  TypeParam n = this->value_;

  n += TestFixture::shared_;

  typename TestFixture::List values;
  values.push_back(n);
  ...
}

TYPED_TEST(FooTest, HasPropertyA) { ... }

#endif  // 0

#if 0

template <typename T>
class FooTest : public testing::Test {
  ...
};

TYPED_TEST_CASE_P(FooTest);

TYPED_TEST_P(FooTest, DoesBlah) {

  TypeParam n = 0;
  ...
}

TYPED_TEST_P(FooTest, HasPropertyA) { ... }

REGISTER_TYPED_TEST_CASE_P(FooTest,
                           DoesBlah, HasPropertyA);

typedef testing::Types<char, int, unsigned int> MyTypes;
INSTANTIATE_TYPED_TEST_CASE_P(My, FooTest, MyTypes);

#endif  // 0

#if GTEST_HAS_TYPED_TEST

# define GTEST_TYPE_PARAMS_(TestCaseName) gtest_type_params_##TestCaseName##_

# define TYPED_TEST_CASE(CaseName, Types) \
  typedef ::testing::internal::TypeList< Types >::type \
      GTEST_TYPE_PARAMS_(CaseName)

# define TYPED_TEST(CaseName, TestName) \
  template <typename gtest_TypeParam_> \
  class GTEST_TEST_CLASS_NAME_(CaseName, TestName) \
      : public CaseName<gtest_TypeParam_> { \
   private: \
    typedef CaseName<gtest_TypeParam_> TestFixture; \
    typedef gtest_TypeParam_ TypeParam; \
    virtual void TestBody(); \
  }; \
  bool gtest_##CaseName##_##TestName##_registered_ GTEST_ATTRIBUTE_UNUSED_ = \
      ::testing::internal::TypeParameterizedTest< \
          CaseName, \
          ::testing::internal::TemplateSel< \
              GTEST_TEST_CLASS_NAME_(CaseName, TestName)>, \
          GTEST_TYPE_PARAMS_(CaseName)>::Register(\
              "", #CaseName, #TestName, 0); \
  template <typename gtest_TypeParam_> \
  void GTEST_TEST_CLASS_NAME_(CaseName, TestName)<gtest_TypeParam_>::TestBody()

#endif  // GTEST_HAS_TYPED_TEST

#if GTEST_HAS_TYPED_TEST_P

# define GTEST_CASE_NAMESPACE_(TestCaseName) \
  gtest_case_##TestCaseName##_

# define GTEST_TYPED_TEST_CASE_P_STATE_(TestCaseName) \
  gtest_typed_test_case_p_state_##TestCaseName##_

# define GTEST_REGISTERED_TEST_NAMES_(TestCaseName) \
  gtest_registered_test_names_##TestCaseName##_

# define TYPED_TEST_CASE_P(CaseName) \
  static ::testing::internal::TypedTestCasePState \
      GTEST_TYPED_TEST_CASE_P_STATE_(CaseName)

# define TYPED_TEST_P(CaseName, TestName) \
  namespace GTEST_CASE_NAMESPACE_(CaseName) { \
  template <typename gtest_TypeParam_> \
  class TestName : public CaseName<gtest_TypeParam_> { \
   private: \
    typedef CaseName<gtest_TypeParam_> TestFixture; \
    typedef gtest_TypeParam_ TypeParam; \
    virtual void TestBody(); \
  }; \
  static bool gtest_##TestName##_defined_ GTEST_ATTRIBUTE_UNUSED_ = \
      GTEST_TYPED_TEST_CASE_P_STATE_(CaseName).AddTestName(\
          __FILE__, __LINE__, #CaseName, #TestName); \
  } \
  template <typename gtest_TypeParam_> \
  void GTEST_CASE_NAMESPACE_(CaseName)::TestName<gtest_TypeParam_>::TestBody()

# define REGISTER_TYPED_TEST_CASE_P(CaseName, ...) \
  namespace GTEST_CASE_NAMESPACE_(CaseName) { \
  typedef ::testing::internal::Templates<__VA_ARGS__>::type gtest_AllTests_; \
  } \
  static const char* const GTEST_REGISTERED_TEST_NAMES_(CaseName) = \
      GTEST_TYPED_TEST_CASE_P_STATE_(CaseName).VerifyRegisteredTestNames(\
          __FILE__, __LINE__, #__VA_ARGS__)

# define INSTANTIATE_TYPED_TEST_CASE_P(Prefix, CaseName, Types) \
  bool gtest_##Prefix##_##CaseName GTEST_ATTRIBUTE_UNUSED_ = \
      ::testing::internal::TypeParameterizedTestCase<CaseName, \
          GTEST_CASE_NAMESPACE_(CaseName)::gtest_AllTests_, \
          ::testing::internal::TypeList< Types >::type>::Register(\
              #Prefix, #CaseName, GTEST_REGISTERED_TEST_NAMES_(CaseName))

#endif  // GTEST_HAS_TYPED_TEST_P

#endif  // GTEST_INCLUDE_GTEST_GTEST_TYPED_TEST_H_

namespace testing {

GTEST_DECLARE_bool_(also_run_disabled_tests);

GTEST_DECLARE_bool_(break_on_failure);

GTEST_DECLARE_bool_(catch_exceptions);

GTEST_DECLARE_string_(color);

GTEST_DECLARE_string_(filter);

GTEST_DECLARE_bool_(list_tests);

GTEST_DECLARE_string_(output);

GTEST_DECLARE_bool_(print_time);

GTEST_DECLARE_int32_(random_seed);

GTEST_DECLARE_int32_(repeat);

GTEST_DECLARE_bool_(show_internal_stack_frames);

GTEST_DECLARE_bool_(shuffle);

GTEST_DECLARE_int32_(stack_trace_depth);

GTEST_DECLARE_bool_(throw_on_failure);

GTEST_DECLARE_string_(stream_result_to);

const int kMaxStackTraceDepth = 100;

namespace internal {

class AssertHelper;
class DefaultGlobalTestPartResultReporter;
class ExecDeathTest;
class NoExecDeathTest;
class FinalSuccessChecker;
class GTestFlagSaver;
class StreamingListenerTest;
class TestResultAccessor;
class TestEventListenersAccessor;
class TestEventRepeater;
class UnitTestRecordPropertyTestHelper;
class WindowsDeathTest;
class UnitTestImpl* GetUnitTestImpl();
void ReportFailureInUnknownLocation(TestPartResult::Type result_type,
                                    const std::string& message);

}  // namespace internal

class Test;
class TestCase;
class TestInfo;
class UnitTest;

class GTEST_API_ AssertionResult {
 public:

  AssertionResult(const AssertionResult& other);

  explicit AssertionResult(bool success) : success_(success) {}

  operator bool() const { return success_; }  // NOLINT

  AssertionResult operator!() const;

  const char* message() const {
    return message_.get() != NULL ?  message_->c_str() : "";
  }

  const char* failure_message() const { return message(); }

  template <typename T> AssertionResult& operator<<(const T& value) {
    AppendMessage(Message() << value);
    return *this;
  }

  AssertionResult& operator<<(
      ::std::ostream& (*basic_manipulator)(::std::ostream& stream)) {
    AppendMessage(Message() << basic_manipulator);
    return *this;
  }

 private:

  void AppendMessage(const Message& a_message) {
    if (message_.get() == NULL)
      message_.reset(new ::std::string);
    message_->append(a_message.GetString().c_str());
  }

  bool success_;

  internal::scoped_ptr< ::std::string> message_;

  GTEST_DISALLOW_ASSIGN_(AssertionResult);
};

GTEST_API_ AssertionResult AssertionSuccess();

GTEST_API_ AssertionResult AssertionFailure();

GTEST_API_ AssertionResult AssertionFailure(const Message& msg);

class GTEST_API_ Test {
 public:
  friend class TestInfo;

  typedef internal::SetUpTestCaseFunc SetUpTestCaseFunc;
  typedef internal::TearDownTestCaseFunc TearDownTestCaseFunc;

  virtual ~Test();

  static void SetUpTestCase() {}

  static void TearDownTestCase() {}

  static bool HasFatalFailure();

  static bool HasNonfatalFailure();

  static bool HasFailure() { return HasFatalFailure() || HasNonfatalFailure(); }

  static void RecordProperty(const std::string& key, const std::string& value);
  static void RecordProperty(const std::string& key, int value);

 protected:

  Test();

  virtual void SetUp();

  virtual void TearDown();

 private:

  static bool HasSameFixtureClass();

  virtual void TestBody() = 0;

  void Run();

  void DeleteSelf_() { delete this; }

  const internal::GTestFlagSaver* const gtest_flag_saver_;

  struct Setup_should_be_spelled_SetUp {};
  virtual Setup_should_be_spelled_SetUp* Setup() { return NULL; }

  GTEST_DISALLOW_COPY_AND_ASSIGN_(Test);
};

typedef internal::TimeInMillis TimeInMillis;

class TestProperty {
 public:

  TestProperty(const std::string& a_key, const std::string& a_value) :
    key_(a_key), value_(a_value) {
  }

  const char* key() const {
    return key_.c_str();
  }

  const char* value() const {
    return value_.c_str();
  }

  void SetValue(const std::string& new_value) {
    value_ = new_value;
  }

 private:

  std::string key_;

  std::string value_;
};

class GTEST_API_ TestResult {
 public:

  TestResult();

  ~TestResult();

  int total_part_count() const;

  int test_property_count() const;

  bool Passed() const { return !Failed(); }

  bool Failed() const;

  bool HasFatalFailure() const;

  bool HasNonfatalFailure() const;

  TimeInMillis elapsed_time() const { return elapsed_time_; }

  const TestPartResult& GetTestPartResult(int i) const;

  const TestProperty& GetTestProperty(int i) const;

 private:
  friend class TestInfo;
  friend class TestCase;
  friend class UnitTest;
  friend class internal::DefaultGlobalTestPartResultReporter;
  friend class internal::ExecDeathTest;
  friend class internal::TestResultAccessor;
  friend class internal::UnitTestImpl;
  friend class internal::WindowsDeathTest;

  const std::vector<TestPartResult>& test_part_results() const {
    return test_part_results_;
  }

  const std::vector<TestProperty>& test_properties() const {
    return test_properties_;
  }

  void set_elapsed_time(TimeInMillis elapsed) { elapsed_time_ = elapsed; }

  void RecordProperty(const std::string& xml_element,
                      const TestProperty& test_property);

  static bool ValidateTestProperty(const std::string& xml_element,
                                   const TestProperty& test_property);

  void AddTestPartResult(const TestPartResult& test_part_result);

  int death_test_count() const { return death_test_count_; }

  int increment_death_test_count() { return ++death_test_count_; }

  void ClearTestPartResults();

  void Clear();

  internal::Mutex test_properites_mutex_;

  std::vector<TestPartResult> test_part_results_;

  std::vector<TestProperty> test_properties_;

  int death_test_count_;

  TimeInMillis elapsed_time_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestResult);
};  // class TestResult

class GTEST_API_ TestInfo {
 public:

  ~TestInfo();

  const char* test_case_name() const { return test_case_name_.c_str(); }

  const char* name() const { return name_.c_str(); }

  const char* type_param() const {
    if (type_param_.get() != NULL)
      return type_param_->c_str();
    return NULL;
  }

  const char* value_param() const {
    if (value_param_.get() != NULL)
      return value_param_->c_str();
    return NULL;
  }

  bool should_run() const { return should_run_; }

  bool is_reportable() const {

    return matches_filter_;
  }

  const TestResult* result() const { return &result_; }

 private:
#if GTEST_HAS_DEATH_TEST
  friend class internal::DefaultDeathTestFactory;
#endif  // GTEST_HAS_DEATH_TEST
  friend class Test;
  friend class TestCase;
  friend class internal::UnitTestImpl;
  friend class internal::StreamingListenerTest;
  friend TestInfo* internal::MakeAndRegisterTestInfo(
      const char* test_case_name,
      const char* name,
      const char* type_param,
      const char* value_param,
      internal::TypeId fixture_class_id,
      Test::SetUpTestCaseFunc set_up_tc,
      Test::TearDownTestCaseFunc tear_down_tc,
      internal::TestFactoryBase* factory);

  TestInfo(const std::string& test_case_name,
           const std::string& name,
           const char* a_type_param,   // NULL if not a type-parameterized test
           const char* a_value_param,  // NULL if not a value-parameterized test
           internal::TypeId fixture_class_id,
           internal::TestFactoryBase* factory);

  int increment_death_test_count() {
    return result_.increment_death_test_count();
  }

  void Run();

  static void ClearTestResult(TestInfo* test_info) {
    test_info->result_.Clear();
  }

  const std::string test_case_name_;     // Test case name
  const std::string name_;               // Test name

  const internal::scoped_ptr<const ::std::string> type_param_;

  const internal::scoped_ptr<const ::std::string> value_param_;
  const internal::TypeId fixture_class_id_;   // ID of the test fixture class
  bool should_run_;                 // True iff this test should run
  bool is_disabled_;                // True iff this test is disabled
  bool matches_filter_;             // True if this test matches the

  internal::TestFactoryBase* const factory_;  // The factory that creates

  TestResult result_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestInfo);
};

class GTEST_API_ TestCase {
 public:

  TestCase(const char* name, const char* a_type_param,
           Test::SetUpTestCaseFunc set_up_tc,
           Test::TearDownTestCaseFunc tear_down_tc);

  virtual ~TestCase();

  const char* name() const { return name_.c_str(); }

  const char* type_param() const {
    if (type_param_.get() != NULL)
      return type_param_->c_str();
    return NULL;
  }

  bool should_run() const { return should_run_; }

  int successful_test_count() const;

  int failed_test_count() const;

  int reportable_disabled_test_count() const;

  int disabled_test_count() const;

  int reportable_test_count() const;

  int test_to_run_count() const;

  int total_test_count() const;

  bool Passed() const { return !Failed(); }

  bool Failed() const { return failed_test_count() > 0; }

  TimeInMillis elapsed_time() const { return elapsed_time_; }

  const TestInfo* GetTestInfo(int i) const;

  const TestResult& ad_hoc_test_result() const { return ad_hoc_test_result_; }

 private:
  friend class Test;
  friend class internal::UnitTestImpl;

  std::vector<TestInfo*>& test_info_list() { return test_info_list_; }

  const std::vector<TestInfo*>& test_info_list() const {
    return test_info_list_;
  }

  TestInfo* GetMutableTestInfo(int i);

  void set_should_run(bool should) { should_run_ = should; }

  void AddTestInfo(TestInfo * test_info);

  void ClearResult();

  static void ClearTestCaseResult(TestCase* test_case) {
    test_case->ClearResult();
  }

  void Run();

  void RunSetUpTestCase() { (*set_up_tc_)(); }

  void RunTearDownTestCase() { (*tear_down_tc_)(); }

  static bool TestPassed(const TestInfo* test_info) {
    return test_info->should_run() && test_info->result()->Passed();
  }

  static bool TestFailed(const TestInfo* test_info) {
    return test_info->should_run() && test_info->result()->Failed();
  }

  static bool TestReportableDisabled(const TestInfo* test_info) {
    return test_info->is_reportable() && test_info->is_disabled_;
  }

  static bool TestDisabled(const TestInfo* test_info) {
    return test_info->is_disabled_;
  }

  static bool TestReportable(const TestInfo* test_info) {
    return test_info->is_reportable();
  }

  static bool ShouldRunTest(const TestInfo* test_info) {
    return test_info->should_run();
  }

  void ShuffleTests(internal::Random* random);

  void UnshuffleTests();

  std::string name_;

  const internal::scoped_ptr<const ::std::string> type_param_;

  std::vector<TestInfo*> test_info_list_;

  std::vector<int> test_indices_;

  Test::SetUpTestCaseFunc set_up_tc_;

  Test::TearDownTestCaseFunc tear_down_tc_;

  bool should_run_;

  TimeInMillis elapsed_time_;

  TestResult ad_hoc_test_result_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestCase);
};

class Environment {
 public:

  virtual ~Environment() {}

  virtual void SetUp() {}

  virtual void TearDown() {}
 private:

  struct Setup_should_be_spelled_SetUp {};
  virtual Setup_should_be_spelled_SetUp* Setup() { return NULL; }
};

class TestEventListener {
 public:
  virtual ~TestEventListener() {}

  virtual void OnTestProgramStart(const UnitTest& unit_test) = 0;

  virtual void OnTestIterationStart(const UnitTest& unit_test,
                                    int iteration) = 0;

  virtual void OnEnvironmentsSetUpStart(const UnitTest& unit_test) = 0;

  virtual void OnEnvironmentsSetUpEnd(const UnitTest& unit_test) = 0;

  virtual void OnTestCaseStart(const TestCase& test_case) = 0;

  virtual void OnTestStart(const TestInfo& test_info) = 0;

  virtual void OnTestPartResult(const TestPartResult& test_part_result) = 0;

  virtual void OnTestEnd(const TestInfo& test_info) = 0;

  virtual void OnTestCaseEnd(const TestCase& test_case) = 0;

  virtual void OnEnvironmentsTearDownStart(const UnitTest& unit_test) = 0;

  virtual void OnEnvironmentsTearDownEnd(const UnitTest& unit_test) = 0;

  virtual void OnTestIterationEnd(const UnitTest& unit_test,
                                  int iteration) = 0;

  virtual void OnTestProgramEnd(const UnitTest& unit_test) = 0;
};

class EmptyTestEventListener : public TestEventListener {
 public:
  virtual void OnTestProgramStart(const UnitTest& /*unit_test*/) {}
  virtual void OnTestIterationStart(const UnitTest& /*unit_test*/,
                                    int /*iteration*/) {}
  virtual void OnEnvironmentsSetUpStart(const UnitTest& /*unit_test*/) {}
  virtual void OnEnvironmentsSetUpEnd(const UnitTest& /*unit_test*/) {}
  virtual void OnTestCaseStart(const TestCase& /*test_case*/) {}
  virtual void OnTestStart(const TestInfo& /*test_info*/) {}
  virtual void OnTestPartResult(const TestPartResult& /*test_part_result*/) {}
  virtual void OnTestEnd(const TestInfo& /*test_info*/) {}
  virtual void OnTestCaseEnd(const TestCase& /*test_case*/) {}
  virtual void OnEnvironmentsTearDownStart(const UnitTest& /*unit_test*/) {}
  virtual void OnEnvironmentsTearDownEnd(const UnitTest& /*unit_test*/) {}
  virtual void OnTestIterationEnd(const UnitTest& /*unit_test*/,
                                  int /*iteration*/) {}
  virtual void OnTestProgramEnd(const UnitTest& /*unit_test*/) {}
};

class GTEST_API_ TestEventListeners {
 public:
  TestEventListeners();
  ~TestEventListeners();

  void Append(TestEventListener* listener);

  TestEventListener* Release(TestEventListener* listener);

  TestEventListener* default_result_printer() const {
    return default_result_printer_;
  }

  TestEventListener* default_xml_generator() const {
    return default_xml_generator_;
  }

 private:
  friend class TestCase;
  friend class TestInfo;
  friend class internal::DefaultGlobalTestPartResultReporter;
  friend class internal::NoExecDeathTest;
  friend class internal::TestEventListenersAccessor;
  friend class internal::UnitTestImpl;

  TestEventListener* repeater();

  void SetDefaultResultPrinter(TestEventListener* listener);

  void SetDefaultXmlGenerator(TestEventListener* listener);

  bool EventForwardingEnabled() const;
  void SuppressEventForwarding();

  internal::TestEventRepeater* repeater_;

  TestEventListener* default_result_printer_;

  TestEventListener* default_xml_generator_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(TestEventListeners);
};

class GTEST_API_ UnitTest {
 public:

  static UnitTest* GetInstance();

  int Run() GTEST_MUST_USE_RESULT_;

  const char* original_working_dir() const;

  const TestCase* current_test_case() const
      GTEST_LOCK_EXCLUDED_(mutex_);

  const TestInfo* current_test_info() const
      GTEST_LOCK_EXCLUDED_(mutex_);

  int random_seed() const;

#if GTEST_HAS_PARAM_TEST

  internal::ParameterizedTestCaseRegistry& parameterized_test_registry()
      GTEST_LOCK_EXCLUDED_(mutex_);
#endif  // GTEST_HAS_PARAM_TEST

  int successful_test_case_count() const;

  int failed_test_case_count() const;

  int total_test_case_count() const;

  int test_case_to_run_count() const;

  int successful_test_count() const;

  int failed_test_count() const;

  int reportable_disabled_test_count() const;

  int disabled_test_count() const;

  int reportable_test_count() const;

  int total_test_count() const;

  int test_to_run_count() const;

  TimeInMillis start_timestamp() const;

  TimeInMillis elapsed_time() const;

  bool Passed() const;

  bool Failed() const;

  const TestCase* GetTestCase(int i) const;

  const TestResult& ad_hoc_test_result() const;

  TestEventListeners& listeners();

 private:

  Environment* AddEnvironment(Environment* env);

  void AddTestPartResult(TestPartResult::Type result_type,
                         const char* file_name,
                         int line_number,
                         const std::string& message,
                         const std::string& os_stack_trace)
      GTEST_LOCK_EXCLUDED_(mutex_);

  void RecordProperty(const std::string& key, const std::string& value);

  TestCase* GetMutableTestCase(int i);

  internal::UnitTestImpl* impl() { return impl_; }
  const internal::UnitTestImpl* impl() const { return impl_; }

  friend class Test;
  friend class internal::AssertHelper;
  friend class internal::ScopedTrace;
  friend class internal::StreamingListenerTest;
  friend class internal::UnitTestRecordPropertyTestHelper;
  friend Environment* AddGlobalTestEnvironment(Environment* env);
  friend internal::UnitTestImpl* internal::GetUnitTestImpl();
  friend void internal::ReportFailureInUnknownLocation(
      TestPartResult::Type result_type,
      const std::string& message);

  UnitTest();

  virtual ~UnitTest();

  void PushGTestTrace(const internal::TraceInfo& trace)
      GTEST_LOCK_EXCLUDED_(mutex_);

  void PopGTestTrace()
      GTEST_LOCK_EXCLUDED_(mutex_);

  mutable internal::Mutex mutex_;

  internal::UnitTestImpl* impl_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(UnitTest);
};

inline Environment* AddGlobalTestEnvironment(Environment* env) {
  return UnitTest::GetInstance()->AddEnvironment(env);
}

GTEST_API_ void InitGoogleTest(int* argc, char** argv);

GTEST_API_ void InitGoogleTest(int* argc, wchar_t** argv);

namespace internal {

template <typename ToPrint, typename OtherOperand>
class FormatForComparison {
 public:
  static ::std::string Format(const ToPrint& value) {
    return ::testing::PrintToString(value);
  }
};

template <typename ToPrint, size_t N, typename OtherOperand>
class FormatForComparison<ToPrint[N], OtherOperand> {
 public:
  static ::std::string Format(const ToPrint* value) {
    return FormatForComparison<const ToPrint*, OtherOperand>::Format(value);
  }
};

#define GTEST_IMPL_FORMAT_C_STRING_AS_POINTER_(CharType)                \
  template <typename OtherOperand>                                      \
  class FormatForComparison<CharType*, OtherOperand> {                  \
   public:                                                              \
    static ::std::string Format(CharType* value) {                      \
      return ::testing::PrintToString(static_cast<const void*>(value)); \
    }                                                                   \
  }

GTEST_IMPL_FORMAT_C_STRING_AS_POINTER_(char);
GTEST_IMPL_FORMAT_C_STRING_AS_POINTER_(const char);
GTEST_IMPL_FORMAT_C_STRING_AS_POINTER_(wchar_t);
GTEST_IMPL_FORMAT_C_STRING_AS_POINTER_(const wchar_t);

#undef GTEST_IMPL_FORMAT_C_STRING_AS_POINTER_

#define GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(CharType, OtherStringType) \
  template <>                                                           \
  class FormatForComparison<CharType*, OtherStringType> {               \
   public:                                                              \
    static ::std::string Format(CharType* value) {                      \
      return ::testing::PrintToString(value);                           \
    }                                                                   \
  }

GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(char, ::std::string);
GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(const char, ::std::string);

#if GTEST_HAS_GLOBAL_STRING
GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(char, ::string);
GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(const char, ::string);
#endif

#if GTEST_HAS_GLOBAL_WSTRING
GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(wchar_t, ::wstring);
GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(const wchar_t, ::wstring);
#endif

#if GTEST_HAS_STD_WSTRING
GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(wchar_t, ::std::wstring);
GTEST_IMPL_FORMAT_C_STRING_AS_STRING_(const wchar_t, ::std::wstring);
#endif

#undef GTEST_IMPL_FORMAT_C_STRING_AS_STRING_

template <typename T1, typename T2>
std::string FormatForComparisonFailureMessage(
    const T1& value, const T2& /* other_operand */) {
  return FormatForComparison<T1, T2>::Format(value);
}

template <typename T1, typename T2>
AssertionResult CmpHelperEQ(const char* expected_expression,
                            const char* actual_expression,
                            const T1& expected,
                            const T2& actual) {
#ifdef _MSC_VER
# pragma warning(push)          // Saves the current warning state.
# pragma warning(disable:4389)  // Temporarily disables warning on

#endif

  if (expected == actual) {
    return AssertionSuccess();
  }

#ifdef _MSC_VER
# pragma warning(pop)          // Restores the warning state.
#endif

  return EqFailure(expected_expression,
                   actual_expression,
                   FormatForComparisonFailureMessage(expected, actual),
                   FormatForComparisonFailureMessage(actual, expected),
                   false);
}

GTEST_API_ AssertionResult CmpHelperEQ(const char* expected_expression,
                                       const char* actual_expression,
                                       BiggestInt expected,
                                       BiggestInt actual);

template <bool lhs_is_null_literal>
class EqHelper {
 public:

  template <typename T1, typename T2>
  static AssertionResult Compare(const char* expected_expression,
                                 const char* actual_expression,
                                 const T1& expected,
                                 const T2& actual) {
    return CmpHelperEQ(expected_expression, actual_expression, expected,
                       actual);
  }

  static AssertionResult Compare(const char* expected_expression,
                                 const char* actual_expression,
                                 BiggestInt expected,
                                 BiggestInt actual) {
    return CmpHelperEQ(expected_expression, actual_expression, expected,
                       actual);
  }
};

template <>
class EqHelper<true> {
 public:

  template <typename T1, typename T2>
  static AssertionResult Compare(
      const char* expected_expression,
      const char* actual_expression,
      const T1& expected,
      const T2& actual,

      typename EnableIf<!is_pointer<T2>::value>::type* = 0) {
    return CmpHelperEQ(expected_expression, actual_expression, expected,
                       actual);
  }

  template <typename T>
  static AssertionResult Compare(
      const char* expected_expression,
      const char* actual_expression,

      Secret* /* expected (NULL) */,
      T* actual) {

    return CmpHelperEQ(expected_expression, actual_expression,
                       static_cast<T*>(NULL), actual);
  }
};

#define GTEST_IMPL_CMP_HELPER_(op_name, op)\
template <typename T1, typename T2>\
AssertionResult CmpHelper##op_name(const char* expr1, const char* expr2, \
                                   const T1& val1, const T2& val2) {\
  if (val1 op val2) {\
    return AssertionSuccess();\
  } else {\
    return AssertionFailure() \
        << "Expected: (" << expr1 << ") " #op " (" << expr2\
        << "), actual: " << FormatForComparisonFailureMessage(val1, val2)\
        << " vs " << FormatForComparisonFailureMessage(val2, val1);\
  }\
}\
GTEST_API_ AssertionResult CmpHelper##op_name(\
    const char* expr1, const char* expr2, BiggestInt val1, BiggestInt val2)

GTEST_IMPL_CMP_HELPER_(NE, !=);

GTEST_IMPL_CMP_HELPER_(LE, <=);

GTEST_IMPL_CMP_HELPER_(LT, <);

GTEST_IMPL_CMP_HELPER_(GE, >=);

GTEST_IMPL_CMP_HELPER_(GT, >);

#undef GTEST_IMPL_CMP_HELPER_

GTEST_API_ AssertionResult CmpHelperSTREQ(const char* expected_expression,
                                          const char* actual_expression,
                                          const char* expected,
                                          const char* actual);

GTEST_API_ AssertionResult CmpHelperSTRCASEEQ(const char* expected_expression,
                                              const char* actual_expression,
                                              const char* expected,
                                              const char* actual);

GTEST_API_ AssertionResult CmpHelperSTRNE(const char* s1_expression,
                                          const char* s2_expression,
                                          const char* s1,
                                          const char* s2);

GTEST_API_ AssertionResult CmpHelperSTRCASENE(const char* s1_expression,
                                              const char* s2_expression,
                                              const char* s1,
                                              const char* s2);

GTEST_API_ AssertionResult CmpHelperSTREQ(const char* expected_expression,
                                          const char* actual_expression,
                                          const wchar_t* expected,
                                          const wchar_t* actual);

GTEST_API_ AssertionResult CmpHelperSTRNE(const char* s1_expression,
                                          const char* s2_expression,
                                          const wchar_t* s1,
                                          const wchar_t* s2);

}  // namespace internal

GTEST_API_ AssertionResult IsSubstring(
    const char* needle_expr, const char* haystack_expr,
    const char* needle, const char* haystack);
GTEST_API_ AssertionResult IsSubstring(
    const char* needle_expr, const char* haystack_expr,
    const wchar_t* needle, const wchar_t* haystack);
GTEST_API_ AssertionResult IsNotSubstring(
    const char* needle_expr, const char* haystack_expr,
    const char* needle, const char* haystack);
GTEST_API_ AssertionResult IsNotSubstring(
    const char* needle_expr, const char* haystack_expr,
    const wchar_t* needle, const wchar_t* haystack);
GTEST_API_ AssertionResult IsSubstring(
    const char* needle_expr, const char* haystack_expr,
    const ::std::string& needle, const ::std::string& haystack);
GTEST_API_ AssertionResult IsNotSubstring(
    const char* needle_expr, const char* haystack_expr,
    const ::std::string& needle, const ::std::string& haystack);

#if GTEST_HAS_STD_WSTRING
GTEST_API_ AssertionResult IsSubstring(
    const char* needle_expr, const char* haystack_expr,
    const ::std::wstring& needle, const ::std::wstring& haystack);
GTEST_API_ AssertionResult IsNotSubstring(
    const char* needle_expr, const char* haystack_expr,
    const ::std::wstring& needle, const ::std::wstring& haystack);
#endif  // GTEST_HAS_STD_WSTRING

namespace internal {

template <typename RawType>
AssertionResult CmpHelperFloatingPointEQ(const char* expected_expression,
                                         const char* actual_expression,
                                         RawType expected,
                                         RawType actual) {
  const FloatingPoint<RawType> lhs(expected), rhs(actual);

  if (lhs.AlmostEquals(rhs)) {
    return AssertionSuccess();
  }

  ::std::stringstream expected_ss;
  expected_ss << std::setprecision(std::numeric_limits<RawType>::digits10 + 2)
              << expected;

  ::std::stringstream actual_ss;
  actual_ss << std::setprecision(std::numeric_limits<RawType>::digits10 + 2)
            << actual;

  return EqFailure(expected_expression,
                   actual_expression,
                   StringStreamToString(&expected_ss),
                   StringStreamToString(&actual_ss),
                   false);
}

GTEST_API_ AssertionResult DoubleNearPredFormat(const char* expr1,
                                                const char* expr2,
                                                const char* abs_error_expr,
                                                double val1,
                                                double val2,
                                                double abs_error);

class GTEST_API_ AssertHelper {
 public:

  AssertHelper(TestPartResult::Type type,
               const char* file,
               int line,
               const char* message);
  ~AssertHelper();

  void operator=(const Message& message) const;

 private:

  struct AssertHelperData {
    AssertHelperData(TestPartResult::Type t,
                     const char* srcfile,
                     int line_num,
                     const char* msg)
        : type(t), file(srcfile), line(line_num), message(msg) { }

    TestPartResult::Type const type;
    const char* const file;
    int const line;
    std::string const message;

   private:
    GTEST_DISALLOW_COPY_AND_ASSIGN_(AssertHelperData);
  };

  AssertHelperData* const data_;

  GTEST_DISALLOW_COPY_AND_ASSIGN_(AssertHelper);
};

}  // namespace internal

#if GTEST_HAS_PARAM_TEST

template <typename T>
class WithParamInterface {
 public:
  typedef T ParamType;
  virtual ~WithParamInterface() {}

  const ParamType& GetParam() const {
    GTEST_CHECK_(parameter_ != NULL)
        << "GetParam() can only be called inside a value-parameterized test "
        << "-- did you intend to write TEST_P instead of TEST_F?";
    return *parameter_;
  }

 private:

  static void SetParam(const ParamType* parameter) {
    parameter_ = parameter;
  }

  static const ParamType* parameter_;

  template <class TestClass> friend class internal::ParameterizedTestFactory;
};

template <typename T>
const T* WithParamInterface<T>::parameter_ = NULL;

template <typename T>
class TestWithParam : public Test, public WithParamInterface<T> {
};

#endif  // GTEST_HAS_PARAM_TEST

#define ADD_FAILURE() GTEST_NONFATAL_FAILURE_("Failed")

#define ADD_FAILURE_AT(file, line) \
  GTEST_MESSAGE_AT_(file, line, "Failed", \
                    ::testing::TestPartResult::kNonFatalFailure)

#define GTEST_FAIL() GTEST_FATAL_FAILURE_("Failed")

#if !GTEST_DONT_DEFINE_FAIL
# define FAIL() GTEST_FAIL()
#endif

#define GTEST_SUCCEED() GTEST_SUCCESS_("Succeeded")

#if !GTEST_DONT_DEFINE_SUCCEED
# define SUCCEED() GTEST_SUCCEED()
#endif

#define EXPECT_THROW(statement, expected_exception) \
  GTEST_TEST_THROW_(statement, expected_exception, GTEST_NONFATAL_FAILURE_)
#define EXPECT_NO_THROW(statement) \
  GTEST_TEST_NO_THROW_(statement, GTEST_NONFATAL_FAILURE_)
#define EXPECT_ANY_THROW(statement) \
  GTEST_TEST_ANY_THROW_(statement, GTEST_NONFATAL_FAILURE_)
#define ASSERT_THROW(statement, expected_exception) \
  GTEST_TEST_THROW_(statement, expected_exception, GTEST_FATAL_FAILURE_)
#define ASSERT_NO_THROW(statement) \
  GTEST_TEST_NO_THROW_(statement, GTEST_FATAL_FAILURE_)
#define ASSERT_ANY_THROW(statement) \
  GTEST_TEST_ANY_THROW_(statement, GTEST_FATAL_FAILURE_)

#define EXPECT_TRUE(condition) \
  GTEST_TEST_BOOLEAN_(condition, #condition, false, true, \
                      GTEST_NONFATAL_FAILURE_)
#define EXPECT_FALSE(condition) \
  GTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, \
                      GTEST_NONFATAL_FAILURE_)
#define ASSERT_TRUE(condition) \
  GTEST_TEST_BOOLEAN_(condition, #condition, false, true, \
                      GTEST_FATAL_FAILURE_)
#define ASSERT_FALSE(condition) \
  GTEST_TEST_BOOLEAN_(!(condition), #condition, true, false, \
                      GTEST_FATAL_FAILURE_)

#ifndef GTEST_INCLUDE_GTEST_GTEST_PRED_IMPL_H_
#define GTEST_INCLUDE_GTEST_GTEST_PRED_IMPL_H_

#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
# error Do not include gtest_pred_impl.h directly.  Include gtest.h instead.
#endif  // GTEST_INCLUDE_GTEST_GTEST_H_

#define GTEST_ASSERT_(expression, on_failure) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (const ::testing::AssertionResult gtest_ar = (expression)) \
    ; \
  else \
    on_failure(gtest_ar.failure_message())

template <typename Pred,
          typename T1>
AssertionResult AssertPred1Helper(const char* pred_text,
                                  const char* e1,
                                  Pred pred,
                                  const T1& v1) {
  if (pred(v1)) return AssertionSuccess();

  return AssertionFailure() << pred_text << "("
                            << e1 << ") evaluates to false, where"
                            << "\n" << e1 << " evaluates to " << v1;
}

#define GTEST_PRED_FORMAT1_(pred_format, v1, on_failure)\
  GTEST_ASSERT_(pred_format(#v1, v1), \
                on_failure)

#define GTEST_PRED1_(pred, v1, on_failure)\
  GTEST_ASSERT_(::testing::AssertPred1Helper(#pred, \
                                             #v1, \
                                             pred, \
                                             v1), on_failure)

#define EXPECT_PRED_FORMAT1(pred_format, v1) \
  GTEST_PRED_FORMAT1_(pred_format, v1, GTEST_NONFATAL_FAILURE_)
#define EXPECT_PRED1(pred, v1) \
  GTEST_PRED1_(pred, v1, GTEST_NONFATAL_FAILURE_)
#define ASSERT_PRED_FORMAT1(pred_format, v1) \
  GTEST_PRED_FORMAT1_(pred_format, v1, GTEST_FATAL_FAILURE_)
#define ASSERT_PRED1(pred, v1) \
  GTEST_PRED1_(pred, v1, GTEST_FATAL_FAILURE_)

template <typename Pred,
          typename T1,
          typename T2>
AssertionResult AssertPred2Helper(const char* pred_text,
                                  const char* e1,
                                  const char* e2,
                                  Pred pred,
                                  const T1& v1,
                                  const T2& v2) {
  if (pred(v1, v2)) return AssertionSuccess();

  return AssertionFailure() << pred_text << "("
                            << e1 << ", "
                            << e2 << ") evaluates to false, where"
                            << "\n" << e1 << " evaluates to " << v1
                            << "\n" << e2 << " evaluates to " << v2;
}

#define GTEST_PRED_FORMAT2_(pred_format, v1, v2, on_failure)\
  GTEST_ASSERT_(pred_format(#v1, #v2, v1, v2), \
                on_failure)

#define GTEST_PRED2_(pred, v1, v2, on_failure)\
  GTEST_ASSERT_(::testing::AssertPred2Helper(#pred, \
                                             #v1, \
                                             #v2, \
                                             pred, \
                                             v1, \
                                             v2), on_failure)

#define EXPECT_PRED_FORMAT2(pred_format, v1, v2) \
  GTEST_PRED_FORMAT2_(pred_format, v1, v2, GTEST_NONFATAL_FAILURE_)
#define EXPECT_PRED2(pred, v1, v2) \
  GTEST_PRED2_(pred, v1, v2, GTEST_NONFATAL_FAILURE_)
#define ASSERT_PRED_FORMAT2(pred_format, v1, v2) \
  GTEST_PRED_FORMAT2_(pred_format, v1, v2, GTEST_FATAL_FAILURE_)
#define ASSERT_PRED2(pred, v1, v2) \
  GTEST_PRED2_(pred, v1, v2, GTEST_FATAL_FAILURE_)

template <typename Pred,
          typename T1,
          typename T2,
          typename T3>
AssertionResult AssertPred3Helper(const char* pred_text,
                                  const char* e1,
                                  const char* e2,
                                  const char* e3,
                                  Pred pred,
                                  const T1& v1,
                                  const T2& v2,
                                  const T3& v3) {
  if (pred(v1, v2, v3)) return AssertionSuccess();

  return AssertionFailure() << pred_text << "("
                            << e1 << ", "
                            << e2 << ", "
                            << e3 << ") evaluates to false, where"
                            << "\n" << e1 << " evaluates to " << v1
                            << "\n" << e2 << " evaluates to " << v2
                            << "\n" << e3 << " evaluates to " << v3;
}

#define GTEST_PRED_FORMAT3_(pred_format, v1, v2, v3, on_failure)\
  GTEST_ASSERT_(pred_format(#v1, #v2, #v3, v1, v2, v3), \
                on_failure)

#define GTEST_PRED3_(pred, v1, v2, v3, on_failure)\
  GTEST_ASSERT_(::testing::AssertPred3Helper(#pred, \
                                             #v1, \
                                             #v2, \
                                             #v3, \
                                             pred, \
                                             v1, \
                                             v2, \
                                             v3), on_failure)

#define EXPECT_PRED_FORMAT3(pred_format, v1, v2, v3) \
  GTEST_PRED_FORMAT3_(pred_format, v1, v2, v3, GTEST_NONFATAL_FAILURE_)
#define EXPECT_PRED3(pred, v1, v2, v3) \
  GTEST_PRED3_(pred, v1, v2, v3, GTEST_NONFATAL_FAILURE_)
#define ASSERT_PRED_FORMAT3(pred_format, v1, v2, v3) \
  GTEST_PRED_FORMAT3_(pred_format, v1, v2, v3, GTEST_FATAL_FAILURE_)
#define ASSERT_PRED3(pred, v1, v2, v3) \
  GTEST_PRED3_(pred, v1, v2, v3, GTEST_FATAL_FAILURE_)

template <typename Pred,
          typename T1,
          typename T2,
          typename T3,
          typename T4>
AssertionResult AssertPred4Helper(const char* pred_text,
                                  const char* e1,
                                  const char* e2,
                                  const char* e3,
                                  const char* e4,
                                  Pred pred,
                                  const T1& v1,
                                  const T2& v2,
                                  const T3& v3,
                                  const T4& v4) {
  if (pred(v1, v2, v3, v4)) return AssertionSuccess();

  return AssertionFailure() << pred_text << "("
                            << e1 << ", "
                            << e2 << ", "
                            << e3 << ", "
                            << e4 << ") evaluates to false, where"
                            << "\n" << e1 << " evaluates to " << v1
                            << "\n" << e2 << " evaluates to " << v2
                            << "\n" << e3 << " evaluates to " << v3
                            << "\n" << e4 << " evaluates to " << v4;
}

#define GTEST_PRED_FORMAT4_(pred_format, v1, v2, v3, v4, on_failure)\
  GTEST_ASSERT_(pred_format(#v1, #v2, #v3, #v4, v1, v2, v3, v4), \
                on_failure)

#define GTEST_PRED4_(pred, v1, v2, v3, v4, on_failure)\
  GTEST_ASSERT_(::testing::AssertPred4Helper(#pred, \
                                             #v1, \
                                             #v2, \
                                             #v3, \
                                             #v4, \
                                             pred, \
                                             v1, \
                                             v2, \
                                             v3, \
                                             v4), on_failure)

#define EXPECT_PRED_FORMAT4(pred_format, v1, v2, v3, v4) \
  GTEST_PRED_FORMAT4_(pred_format, v1, v2, v3, v4, GTEST_NONFATAL_FAILURE_)
#define EXPECT_PRED4(pred, v1, v2, v3, v4) \
  GTEST_PRED4_(pred, v1, v2, v3, v4, GTEST_NONFATAL_FAILURE_)
#define ASSERT_PRED_FORMAT4(pred_format, v1, v2, v3, v4) \
  GTEST_PRED_FORMAT4_(pred_format, v1, v2, v3, v4, GTEST_FATAL_FAILURE_)
#define ASSERT_PRED4(pred, v1, v2, v3, v4) \
  GTEST_PRED4_(pred, v1, v2, v3, v4, GTEST_FATAL_FAILURE_)

template <typename Pred,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5>
AssertionResult AssertPred5Helper(const char* pred_text,
                                  const char* e1,
                                  const char* e2,
                                  const char* e3,
                                  const char* e4,
                                  const char* e5,
                                  Pred pred,
                                  const T1& v1,
                                  const T2& v2,
                                  const T3& v3,
                                  const T4& v4,
                                  const T5& v5) {
  if (pred(v1, v2, v3, v4, v5)) return AssertionSuccess();

  return AssertionFailure() << pred_text << "("
                            << e1 << ", "
                            << e2 << ", "
                            << e3 << ", "
                            << e4 << ", "
                            << e5 << ") evaluates to false, where"
                            << "\n" << e1 << " evaluates to " << v1
                            << "\n" << e2 << " evaluates to " << v2
                            << "\n" << e3 << " evaluates to " << v3
                            << "\n" << e4 << " evaluates to " << v4
                            << "\n" << e5 << " evaluates to " << v5;
}

#define GTEST_PRED_FORMAT5_(pred_format, v1, v2, v3, v4, v5, on_failure)\
  GTEST_ASSERT_(pred_format(#v1, #v2, #v3, #v4, #v5, v1, v2, v3, v4, v5), \
                on_failure)

#define GTEST_PRED5_(pred, v1, v2, v3, v4, v5, on_failure)\
  GTEST_ASSERT_(::testing::AssertPred5Helper(#pred, \
                                             #v1, \
                                             #v2, \
                                             #v3, \
                                             #v4, \
                                             #v5, \
                                             pred, \
                                             v1, \
                                             v2, \
                                             v3, \
                                             v4, \
                                             v5), on_failure)

#define EXPECT_PRED_FORMAT5(pred_format, v1, v2, v3, v4, v5) \
  GTEST_PRED_FORMAT5_(pred_format, v1, v2, v3, v4, v5, GTEST_NONFATAL_FAILURE_)
#define EXPECT_PRED5(pred, v1, v2, v3, v4, v5) \
  GTEST_PRED5_(pred, v1, v2, v3, v4, v5, GTEST_NONFATAL_FAILURE_)
#define ASSERT_PRED_FORMAT5(pred_format, v1, v2, v3, v4, v5) \
  GTEST_PRED_FORMAT5_(pred_format, v1, v2, v3, v4, v5, GTEST_FATAL_FAILURE_)
#define ASSERT_PRED5(pred, v1, v2, v3, v4, v5) \
  GTEST_PRED5_(pred, v1, v2, v3, v4, v5, GTEST_FATAL_FAILURE_)

#endif  // GTEST_INCLUDE_GTEST_GTEST_PRED_IMPL_H_

#define EXPECT_EQ(expected, actual) \
  EXPECT_PRED_FORMAT2(::testing::internal:: \
                      EqHelper<GTEST_IS_NULL_LITERAL_(expected)>::Compare, \
                      expected, actual)
#define EXPECT_NE(expected, actual) \
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperNE, expected, actual)
#define EXPECT_LE(val1, val2) \
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperLE, val1, val2)
#define EXPECT_LT(val1, val2) \
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperLT, val1, val2)
#define EXPECT_GE(val1, val2) \
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperGE, val1, val2)
#define EXPECT_GT(val1, val2) \
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperGT, val1, val2)

#define GTEST_ASSERT_EQ(expected, actual) \
  ASSERT_PRED_FORMAT2(::testing::internal:: \
                      EqHelper<GTEST_IS_NULL_LITERAL_(expected)>::Compare, \
                      expected, actual)
#define GTEST_ASSERT_NE(val1, val2) \
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperNE, val1, val2)
#define GTEST_ASSERT_LE(val1, val2) \
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperLE, val1, val2)
#define GTEST_ASSERT_LT(val1, val2) \
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperLT, val1, val2)
#define GTEST_ASSERT_GE(val1, val2) \
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperGE, val1, val2)
#define GTEST_ASSERT_GT(val1, val2) \
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperGT, val1, val2)

#if !GTEST_DONT_DEFINE_ASSERT_EQ
# define ASSERT_EQ(val1, val2) GTEST_ASSERT_EQ(val1, val2)
#endif

#if !GTEST_DONT_DEFINE_ASSERT_NE
# define ASSERT_NE(val1, val2) GTEST_ASSERT_NE(val1, val2)
#endif

#if !GTEST_DONT_DEFINE_ASSERT_LE
# define ASSERT_LE(val1, val2) GTEST_ASSERT_LE(val1, val2)
#endif

#if !GTEST_DONT_DEFINE_ASSERT_LT
# define ASSERT_LT(val1, val2) GTEST_ASSERT_LT(val1, val2)
#endif

#if !GTEST_DONT_DEFINE_ASSERT_GE
# define ASSERT_GE(val1, val2) GTEST_ASSERT_GE(val1, val2)
#endif

#if !GTEST_DONT_DEFINE_ASSERT_GT
# define ASSERT_GT(val1, val2) GTEST_ASSERT_GT(val1, val2)
#endif

#define EXPECT_STREQ(expected, actual) \
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperSTREQ, expected, actual)
#define EXPECT_STRNE(s1, s2) \
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperSTRNE, s1, s2)
#define EXPECT_STRCASEEQ(expected, actual) \
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperSTRCASEEQ, expected, actual)
#define EXPECT_STRCASENE(s1, s2)\
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperSTRCASENE, s1, s2)

#define ASSERT_STREQ(expected, actual) \
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperSTREQ, expected, actual)
#define ASSERT_STRNE(s1, s2) \
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperSTRNE, s1, s2)
#define ASSERT_STRCASEEQ(expected, actual) \
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperSTRCASEEQ, expected, actual)
#define ASSERT_STRCASENE(s1, s2)\
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperSTRCASENE, s1, s2)

#define EXPECT_FLOAT_EQ(expected, actual)\
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<float>, \
                      expected, actual)

#define EXPECT_DOUBLE_EQ(expected, actual)\
  EXPECT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<double>, \
                      expected, actual)

#define ASSERT_FLOAT_EQ(expected, actual)\
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<float>, \
                      expected, actual)

#define ASSERT_DOUBLE_EQ(expected, actual)\
  ASSERT_PRED_FORMAT2(::testing::internal::CmpHelperFloatingPointEQ<double>, \
                      expected, actual)

#define EXPECT_NEAR(val1, val2, abs_error)\
  EXPECT_PRED_FORMAT3(::testing::internal::DoubleNearPredFormat, \
                      val1, val2, abs_error)

#define ASSERT_NEAR(val1, val2, abs_error)\
  ASSERT_PRED_FORMAT3(::testing::internal::DoubleNearPredFormat, \
                      val1, val2, abs_error)

GTEST_API_ AssertionResult FloatLE(const char* expr1, const char* expr2,
                                   float val1, float val2);
GTEST_API_ AssertionResult DoubleLE(const char* expr1, const char* expr2,
                                    double val1, double val2);

#if GTEST_OS_WINDOWS

# define EXPECT_HRESULT_SUCCEEDED(expr) \
    EXPECT_PRED_FORMAT1(::testing::internal::IsHRESULTSuccess, (expr))

# define ASSERT_HRESULT_SUCCEEDED(expr) \
    ASSERT_PRED_FORMAT1(::testing::internal::IsHRESULTSuccess, (expr))

# define EXPECT_HRESULT_FAILED(expr) \
    EXPECT_PRED_FORMAT1(::testing::internal::IsHRESULTFailure, (expr))

# define ASSERT_HRESULT_FAILED(expr) \
    ASSERT_PRED_FORMAT1(::testing::internal::IsHRESULTFailure, (expr))

#endif  // GTEST_OS_WINDOWS

#define ASSERT_NO_FATAL_FAILURE(statement) \
    GTEST_TEST_NO_FATAL_FAILURE_(statement, GTEST_FATAL_FAILURE_)
#define EXPECT_NO_FATAL_FAILURE(statement) \
    GTEST_TEST_NO_FATAL_FAILURE_(statement, GTEST_NONFATAL_FAILURE_)

#define SCOPED_TRACE(message) \
  ::testing::internal::ScopedTrace GTEST_CONCAT_TOKEN_(gtest_trace_, __LINE__)(\
    __FILE__, __LINE__, ::testing::Message() << (message))

template <typename T1, typename T2>
bool StaticAssertTypeEq() {
  (void)internal::StaticAssertTypeEqHelper<T1, T2>();
  return true;
}

#define GTEST_TEST(test_case_name, test_name)\
  GTEST_TEST_(test_case_name, test_name, \
              ::testing::Test, ::testing::internal::GetTestTypeId())

#if !GTEST_DONT_DEFINE_TEST
# define TEST(test_case_name, test_name) GTEST_TEST(test_case_name, test_name)
#endif

#define TEST_F(test_fixture, test_name)\
  GTEST_TEST_(test_fixture, test_name, test_fixture, \
              ::testing::internal::GetTypeId<test_fixture>())

}  // namespace testing

int RUN_ALL_TESTS() GTEST_MUST_USE_RESULT_;

inline int RUN_ALL_TESTS() {
  return ::testing::UnitTest::GetInstance()->Run();
}

#endif  // GTEST_INCLUDE_GTEST_GTEST_H_
