// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

inline FileSystem::FileSystem(const std::string& path) :
    m_root{"./" + path} {

    if (!std::filesystem::exists(m_root)) {
        if (!std::filesystem::create_directories(m_root)) {
            throw FileIOError{std::format(
                        "Failed to create root directory \"{}\" for FileSystem",
                        m_root.native())};
        }
    }
}

/// Check to see if the specified handle is already registered, and throw
/// an error if this is the case. If it is not already registered, attempt
/// to open a file with the requested access mode at the provided file
/// location. Throw an error if the file cannot be successfully opened,
/// otherwise insert the new file stream into the internal container.
inline void FileSystem::open(const std::string& key,
        const std::string& filename, const std::ios_base::openmode& mode) {
    if (m_files.find(key) != m_files.end()) {
        throw InvalidArgumentError{std::format("File key \"{}\" already in use",
                    key)};
    }

    auto file{std::make_pair(key, std::make_unique<std::fstream>(
                    m_root/filename, mode))};
    if (!(file.second->is_open() && file.second->good())) {
        throw FileIOError{std::format(
                    "Problem occurred when attempting to open file \"{}\"",
                    (m_root/filename).native())};
    }
    m_files.insert(std::move(file));
}

inline void FileSystem::close(const std::string& key) {
    if (auto it{m_files.find(key)}; it != m_files.end()) {
        m_files.erase(it);
    } else {
        throw InvalidArgumentError{std::format(
                    "File with key \"{}\" was not found", key)};
    }
}

inline void FileSystem::close_all() {
    m_files.clear();
}

inline std::fstream FileSystem::quick_write(const std::string& filename) const {
    std::fstream file{m_root/filename, FileMode::out_trunc};
    if (!(file.is_open() && file.good())) {
        throw FileIOError{std::format(
                    "Problem occurred when attempting to open file \"{}\"",
                    (m_root/filename).native())};
    }
    return file;
}

inline std::fstream FileSystem::quick_read(const std::string& filename) const {
    std::fstream file{m_root/filename, FileMode::in};
    if (!(file.is_open() && file.good())) {
        throw FileIOError{std::format(
                    "Problem occurred when attempting to open file \"{}\"",
                    (m_root/filename).native())};
    }
    return file;
}

inline std::fstream& FileSystem::operator()(const std::string& key) {
    if (auto it{m_files.find(key)}; it != m_files.end()) {
        return *(it->second);
    } else {
        throw InvalidArgumentError{std::format(
                    "File with key \"{}\" was not found", key)};
    }
}

