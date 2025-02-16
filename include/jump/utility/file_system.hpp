// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_FILE_SYSTEM_HPP
#define JUMP_FILE_SYSTEM_HPP

#include <filesystem>
#include <fstream>
#include <ios>
#include <map>
#include <memory>
#include <string>

namespace jump {
/// \brief Deals with the storage of input and output file streams, and ensures
/// that files are closed appropriately when finished with.
class FileSystem {
    public:
        using FileMode = std::ios_base::openmode;

        /// \brief Use current working directory as root folder for all input
        /// and output files opened by this object, optionally take a
        /// subdirectory location to be used.
        explicit FileSystem(const std::filesystem::path& path = ".");
        /// \brief Deleted copy constructor to protect data integrity.
        FileSystem(const FileSystem& source) = delete;
        /// \brief Default move constructor to allow moves (preserves data
        /// integrity).
        FileSystem(FileSystem&& source) = default;

        /// \brief Deleted copy assignment to protect data integrity.
        FileSystem& operator=(const FileSystem& rhs) = delete;
        /// \brief Default move assignment allows moves (preserves data
        /// integrity).
        FileSystem& operator=(FileSystem&& rhs) = default;

        /// \brief Open file with a specified filename (relative to the root
        /// directory), with a given handle, and an explicit access specifier.
        void open(const std::string& key, const std::string& filename,
                const FileMode& mode);
        /// \brief Close file with the given handle.
        void close(const std::string& key);
        /// \brief Close all open files.
        void close_all();

        /// \brief Open file with a specified filename (relative to the root
        /// directory) for quick data input or output, without explicitly
        /// storing the file handle for later access.
        auto file_stream(const std::string& filename,
                const FileMode& mode) const -> std::fstream;
        /// \brief Read all the content of a specified file (relative to the
        /// root directory) as text.
        auto read_file_as_text(const std::string& filename) const
                -> std::string;

        /// \brief Return file stream corresponding to the supplied handle, if
        /// it exists, otherwise throw an exception.
        auto operator[](const std::string& key) -> std::fstream&;

        /// \brief File mode for input.
        static constexpr FileMode mode_in{std::ios_base::in};
        /// \brief File mode for output (truncation).
        static constexpr FileMode mode_out_trunc{std::ios_base::out
            | std::ios_base::trunc};
        /// \brief File mode for output (append).
        static constexpr FileMode mode_out_app{std::ios_base::out
            | std::ios_base::app};
        /// \brief File mode for input and output (truncation).
        static constexpr FileMode mode_random_trunc{std::ios_base::in
            | std::ios_base::out | std::ios_base::trunc};
        /// \brief File mode for input and output (append).
        static constexpr FileMode mode_random_app{std::ios_base::in
            | std::ios_base::out | std::ios_base::app};
        /// \brief Binary file mode.
        static constexpr FileMode mode_binary{std::ios_base::binary};

    private:
        /// \brief Stores file streams as shared pointers indexed by a
        /// user-supplied handle.
        std::map<std::string, std::unique_ptr<std::fstream>> m_files;
        /// \brief The root directory to use for all input and output files.
        std::filesystem::path m_root;
};
}   // namespace jump

#endif  // JUMP_FILE_SYSTEM_HPP

