// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_THIRD_PARTY_HPP
#define JUMP_THIRD_PARTY_HPP

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

#endif  // JUMP_THIRD_PARTY_HPP

