// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_EXTERNAL_HPP
#define JUMP_EXTERNAL_HPP

#ifdef __has_include
    #if defined(JUMP_USE_CBLAS) && __has_include(<cblas.h>)
        #include <cblas.h>
        #define JUMP_HAS_CBLAS
    #endif  // __has_include(<cblas.h>)
#endif  // __has_include

#ifdef __has_include
    #if defined(JUMP_USE_LAPACKE) && __has_include(<lapacke.h>)
        #include <lapacke.h>
        #define JUMP_HAS_LAPACKE
    #endif  // __has_include(<lapacke.h>)
#endif  // __has_include

#endif  // JUMP_EXTERNAL_HPP

