// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_IO_HPP
#define JUMP_IO_HPP

#include <fstream>
#include <string>

#include "jump/debug/exception.hpp"

namespace Jump {

    /// \brief A collection of free functions to manipulate data between files
    /// and data structures.
    namespace IO {

        /// \brief Read all the content of the given fstream into a std::string.
        template <typename Fstream>
        std::string read_file_contents(Fstream&& in);

        #include "io_impl.hpp"

    }   // namespace IO
}   // namespace Jump

#endif  // JUMP_IO_HPP

