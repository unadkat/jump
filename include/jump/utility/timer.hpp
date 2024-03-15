// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TIMER_HPP
#define JUMP_TIMER_HPP

#include "jump/utility/timer_decl.hpp"

namespace jump {
/// The time is returned formatted based on the largest time unit
/// (hours/minutes/seconds) that have been measured. If the number of hours is
/// non-zero, the time is given in the format hh:mm:ss.sss. Otherwise if the
/// number of minutes is non-zero, the time is given in the format mm:ss.sss.
/// Failing both of these, the time is returned in the format ss.sss with a
/// trailing 's' to denote seconds.
inline std::string Timer::formatted_time(Clock::duration time) const {
    auto ms{std::chrono::duration_cast<std::chrono::milliseconds>(
            time).count()};

    decltype(ms) minutes{0}, hours{0};
    Real seconds{0.};

    hours = ms/(1000*60*60);
    ms %= 1000*60*60;
    minutes = ms/(1000*60);
    ms %= 1000*60;
    seconds = 0.001*ms;

    if (hours > 0) {
        return std::format("{:d}:{:02d}:{:06.3f}", hours, minutes, seconds);
    } else if (minutes > 0) {
        return std::format("{:d}{:06.3f}", minutes, seconds);
    } else {
        return std::format("{:.3f}s", seconds);
    }
}

inline Timer::Timer(std::string task) :
    m_task{std::move(task)} {
}

inline void Timer::start() {
    if (m_stopped) {
        m_start = Clock::now();
        ++m_counter;
        m_stopped = false;
    }
}

inline void Timer::stop() {
    if (!m_stopped) {
        m_delta_t += Clock::now() - m_start;
        m_stopped = true;
    }
}

inline void Timer::reset() {
    stop();
    m_counter = 0;
    m_delta_t = Clock::duration::zero();
}

inline const std::string& Timer::task() const {
    return m_task;
}

inline bool Timer::stopped() const {
    return m_stopped;
}

inline const long& Timer::lap_count() const {
    return m_counter;
}

inline Timer::Clock::duration Timer::running_time() const {
    auto time{m_delta_t};
    if (!m_stopped) {
        time += Clock::now() - m_start;
    }
    return time;
}

inline std::string Timer::formatted_running_time() const {
    return formatted_time(running_time());
}

inline Timer::Clock::duration Timer::average_time() const {
    if (m_counter == 0) {
        return Clock::duration::zero();
    } else {
        return m_delta_t/m_counter;
    }
}

inline std::string Timer::formatted_average_time() const {
    return formatted_time(average_time());
}

/// \relates Timer
/// \brief Print summary of timing results to a stream.
template <typename Os>
inline Os& operator<<(Os& out, const Timer& rhs) {
    if (rhs.task() != "") {
        out << rhs.task() << '\n';
    }
    out << "Total elapsed time: " << rhs.formatted_running_time();
    if (rhs.stopped() && rhs.lap_count() > 1) {
        out << ", average time: " << rhs.formatted_average_time();
    }
    return out;
}
}   // namespace jump

#endif  // JUMP_TIMER_HPP

