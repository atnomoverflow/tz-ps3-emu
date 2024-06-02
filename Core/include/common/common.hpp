/**
 * (c) 2014 Nucleus project. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

// Architectures
#if defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
#define TZ_PS3_EMU_X86_64
#endif

// Platforms
#if defined(__linux__)
#define TZ_PS3_EMU_LINUX
#endif
#if defined(_WIN32) || defined(_WIN64)
#define TZ_PS3_EMU_WIN
#endif
#if defined(__APPLE__) && defined(__MACH__)
#define TZ_PS3_EMU_MACOS
#endif

// Types
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;
struct u128
{
    u64 lo, hi;

	bool operator == (const u128& r) const { return (lo == r.lo) && (hi == r.hi); }
	bool operator != (const u128& r) const { return (lo != r.lo) || (hi != r.hi); }

    u128 operator | (const u128& r) const { return u128{lo | r.lo, hi | r.hi}; }
    u128 operator & (const u128& r) const { return u128{lo & r.lo, hi & r.hi}; }
    u128 operator ^ (const u128& r) const { return u128{lo ^ r.lo, hi ^ r.hi}; }
    u128 operator ~ () const { return u128{~lo, ~hi}; }
};
#include "helpers/BETypes.hpp"
