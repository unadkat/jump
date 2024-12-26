// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_IO_DECL_HPP
#define JUMP_IO_DECL_HPP

#include <string>

namespace jump {
/// \brief Read all the content of the given fstream into a std::string. Note:
/// parameter is a universal reference to be able to bind to both lvalue- and
/// rvalue-references.
template <typename Fs>
auto read_file_contents(Fs&& in) -> std::string;
}   // namespace jump

#endif  // JUMP_IO_DECL_HPP

