#pragma once

#if 0
#    error Only clang is supported
#endif

#ifdef NDEBUG
constexpr bool kDebug = false;
#else
constexpr bool kDebug = true;
#endif

#define ALWAYS_INLINE [[clang::always_inline]]
#define NEVER_INLINE  [[clang::noinline]]
