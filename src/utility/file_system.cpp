// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#include "jump/utility/file_system.hpp"

#include "jump/debug/error_data.hpp"
#include "jump/debug/exception.hpp"

namespace jump {
FileSystem::FileSystem(const std::filesystem::path& path) :
    m_root{path} {

    if (!std::filesystem::exists(m_root)) {
        if (!std::filesystem::create_directories(m_root)) {
            throw RuntimeError{FileIOError{.resource = m_root.native()}};
        }
    }
}

/// Any time we open a file (be it for input or output), we must supply an
/// explicit access specifier so that no files are accidentally overwritten or
/// written to instead of read from, etc. Files can be opened for input or
/// output (or both), and in the case of output they can be opened for appending
/// data or truncation (overwriting). This gives the five different options:
/// input, output (truncation), output(append), input/output (truncation), and
/// input/output (append).
///
/// Check to see if the specified handle is already registered, and throw an
/// error if this is the case. If it is not already registered, attempt to open
/// a file with the requested access mode at the provided file location. Throw
/// an error if the file cannot be successfully opened, otherwise insert the new
/// file stream into the internal container.
void FileSystem::open(const std::string& key, const std::string& filename,
        const std::ios_base::openmode& mode) {
    if (m_files.find(key) != m_files.end()) {
        throw RuntimeError{InvalidArgumentError{.argument = "key", .value = key,
            .expected = "unused key"}};
    }

    auto file{std::make_pair(key, std::make_unique<std::fstream>(
                m_root/filename, mode))};
    if (!(file.second->is_open() && file.second->good())) {
        throw RuntimeError{FileIOError{
            .resource = (m_root/filename).native()}};
    }
    m_files.insert(std::move(file));
}

void FileSystem::close(const std::string& key) {
    if (auto it{m_files.find(key)}; it != m_files.end()) {
        m_files.erase(it);
    } else {
        throw RuntimeError{InvalidArgumentError{.argument = "key", .value = key,
            .expected = "valid key"}};
    }
}

void FileSystem::close_all() {
    m_files.clear();
}

auto FileSystem::file_stream(const std::string& filename,
        const FileMode& mode) const -> std::fstream {
    std::fstream file{m_root/filename, mode};
    if (!(file.is_open() && file.good())) {
        throw RuntimeError{FileIOError{
            .resource = (m_root/filename).native()}};
    }
    return file;
}

auto FileSystem::read_file_as_text(const std::string& filename) const
        -> std::string {
    auto in{file_stream(filename, mode_in)};
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

auto FileSystem::operator[](const std::string& key) -> std::fstream& {
    if (auto it{m_files.find(key)}; it != m_files.end()) {
        return *(it->second);
    } else {
        throw RuntimeError{InvalidArgumentError{.argument = "key", .value = key,
            .expected = "valid key"}};
    }
}
}   // namespace jump

