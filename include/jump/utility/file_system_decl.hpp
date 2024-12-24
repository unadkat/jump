// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_FILE_SYSTEM_DECL_HPP
#define JUMP_FILE_SYSTEM_DECL_HPP

#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <string>

#include "jump/debug/exception.hpp"

namespace jump {
/// \brief Deals with the storage of input and output file streams, and ensures
/// that files are closed appropriately when finished with.
class FileSystem {
    public:
        using FileMode = std::ios_base::openmode;

        /// \brief Use current working directory as root folder for all input
        /// and output files opened by this object, optionally take a
        /// subdirectory location to be used.
        FileSystem(const std::string& path = "");
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
                const std::ios_base::openmode& mode);
        /// \brief Close file with the given handle.
        void close(const std::string& key);
        /// \brief Close all open files.
        void close_all();

        /// \brief Open file with a specified filename (relative to the root
        /// directory) for quick data output (in truncation mode), without
        /// explicitly storing the file handle for later access.
        std::fstream quick_write(const std::string& filename) const;
        /// \brief Open file with a specified filename (relative to the root
        /// directory) for quick data reading, without explicitly storing the
        /// file handle for later access.
        std::fstream quick_read(const std::string& filename) const;

        /// \brief Return file stream corresponding to the supplied handle, if
        /// it exists, otherwise throw an exception.
        std::fstream& operator()(const std::string& key);

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

    private:
        /// \brief Stores file streams as shared pointers indexed by a
        /// user-supplied handle.
        std::map<std::string, std::unique_ptr<std::fstream>> m_files;
        /// \brief The root directory to use for all input and output files.
        std::filesystem::path m_root;
};
}   // namespace jump

#endif  // JUMP_FILE_SYSTEM_DECL_HPP

