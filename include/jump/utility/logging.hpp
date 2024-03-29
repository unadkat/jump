// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_LOGGING_HPP
#define JUMP_LOGGING_HPP

#include "jump/utility/logging_decl.hpp"

namespace jump {
inline bool Log::colours{true};

inline std::string Log::black(const std::string& message) {
    if (colours) {
        return "\033[30m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::red(const std::string& message) {
    if (colours) {
        return "\033[31m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::green(const std::string& message) {
    if (colours) {
        return "\033[32m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::yellow(const std::string& message) {
    if (colours) {
        return "\033[33m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::blue(const std::string& message) {
    if (colours) {
        return "\033[34m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::magenta(const std::string& message) {
    if (colours) {
        return "\033[35m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::cyan(const std::string& message) {
    if (colours) {
        return "\033[36m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::white(const std::string& message) {
    if (colours) {
        return "\033[37m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::bright_black(const std::string& message) {
    if (colours) {
        return "\033[1;30m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::bright_red(const std::string& message) {
    if (colours) {
        return "\033[1;31m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::bright_green(const std::string& message) {
    if (colours) {
        return "\033[1;32m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::bright_yellow(const std::string& message) {
    if (colours) {
        return "\033[1;33m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::bright_blue(const std::string& message) {
    if (colours) {
        return "\033[1;34m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::bright_magenta(const std::string& message) {
    if (colours) {
        return "\033[1;35m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::bright_cyan(const std::string& message) {
    if (colours) {
        return "\033[1;36m" + message + "\033[0m";
    } else {
        return message;
    }
}

inline std::string Log::bright_white(const std::string& message) {
    if (colours) {
        return "\033[1;37m" + message + "\033[0m";
    } else {
        return message;
    }
}
}   // namespace jump

#endif  // JUMP_LOGGING_HPP

