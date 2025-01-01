// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025, released under GPL-3.0-or-later
// See COPYING for full licence details

#ifndef JUMP_IO_HPP
#define JUMP_IO_HPP

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"

#include <ios>
#include <string>

namespace jump {
/// \brief Read all the content of the given fstream into a std::string. Note:
/// parameter is a universal reference to be able to bind to both lvalue- and
/// rvalue-references.
template <typename Fs>
auto read_file_contents(Fs&& in) -> std::string;

// ========================================================================
// Implementation
// ========================================================================

template <typename Fs>
inline auto read_file_contents(Fs&& in) -> std::string {
    // Reset in::gcount in case the file has been read from before. If this
    // fails for any reason we should have not goodbit
    in.peek();
    if (!(in.is_open() && in.good())) {
        throw RuntimeError{FileIOError{.resource = "std::fstream"}};
    }

    std::string contents;
    // Find the size of the file by seeking to the end and then getting the
    // position of the stream
    in.seekg(0, std::ios::end);
    const auto num_chars{in.tellg()};
    contents.resize(num_chars);
    // Return to the start of the file
    in.seekg(0, std::ios::beg);
    // Read the full contents of the file
    in.read(&contents[0], num_chars);
    // Check how many characters were read successfully
    if (in.gcount() != num_chars) {
        throw RuntimeError{FileIOError{.resource = "std::fstream"}};
    }

    return contents;
}
}   // namespace jump

#endif  // JUMP_IO_HPP

