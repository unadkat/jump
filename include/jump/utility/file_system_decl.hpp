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

namespace Jump {
/// \brief Repackage standard library flags from `std::ios_base` for
/// input/output file access methods.
///
/// Any time we open a file (be it for input or output), we must supply an
/// explicit access specifier so that no files are accidentally overwritten or
/// written to instead of read from, etc. Files can be opened for input or
/// output (or both), and in the case of output they can be opened for appending
/// data or truncation (overwriting). This gives the five different options:
/// input, output (truncation), output(append), input/output (truncation), and
/// input/output (append).
namespace FileMode {
    const std::ios_base::openmode in{std::ios_base::in};
    const std::ios_base::openmode out_trunc{std::ios_base::out
        | std::ios_base::trunc};
    const std::ios_base::openmode out_app{std::ios_base::out
        | std::ios_base::app};
    const std::ios_base::openmode random_trunc{in | out_trunc};
    const std::ios_base::openmode random_app{in | out_app};
}   // namespace FileMode

/// \brief Deals with the storage of input and output file streams, and ensures
/// that files are closed appropriately when finished with.
class FileSystem {
    private:
        /// \brief Stores file streams as shared pointers indexed by a
        /// user-supplied handle.
        std::map<std::string, std::unique_ptr<std::fstream>> m_files;
        /// \brief The root directory to use for all input and output files.
        std::filesystem::path m_root;

    public:
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
        /// directory), with a given handle, and an access specifier taken from
        /// the FileMode namespace. Output precision may also optionally be set.
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
};
}   // namespace Jump

#endif  // JUMP_FILE_SYSTEM_DECL_HPP

