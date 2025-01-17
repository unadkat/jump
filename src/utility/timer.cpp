// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#include "jump/utility/timer.hpp"

#include "jump/utility/types.hpp"

#include <format>

namespace jump {
Timer::Timer(std::string task) :
    m_task{std::move(task)} {
}

void Timer::start() {
    if (m_stopped) {
        m_start = Clock::now();
        ++m_counter;
        m_stopped = false;
    }
}

void Timer::stop() {
    if (!m_stopped) {
        m_delta_t += Clock::now() - m_start;
        m_stopped = true;
    }
}

void Timer::reset() {
    stop();
    m_counter = 0;
    m_delta_t = Clock::duration::zero();
}

auto Timer::task() const -> const std::string& {
    return m_task;
}

auto Timer::stopped() const -> bool {
    return m_stopped;
}

auto Timer::lap_count() const -> const long& {
    return m_counter;
}

auto Timer::running_time() const -> Timer::Clock::duration {
    auto time{m_delta_t};
    if (!m_stopped) {
        time += Clock::now() - m_start;
    }
    return time;
}

auto Timer::formatted_running_time() const -> std::string {
    return formatted_time(running_time());
}

auto Timer::average_time() const -> Timer::Clock::duration {
    if (m_counter == 0) {
        return Clock::duration::zero();
    } else {
        return m_delta_t/m_counter;
    }
}

auto Timer::formatted_average_time() const -> std::string {
    return formatted_time(average_time());
}

/// The time is returned formatted based on the largest time unit
/// (hours/minutes/seconds) that have been measured. If the number of hours is
/// non-zero, the time is given in the format hh:mm:ss.sss. Otherwise if the
/// number of minutes is non-zero, the time is given in the format mm:ss.sss.
/// Failing both of these, the time is returned in the format ss.sss with a
/// trailing 's' to denote seconds.
auto Timer::formatted_time(Clock::duration time) const -> std::string {
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

/// \relates Timer
auto operator<<(std::ostream& out, const Timer& rhs) -> std::ostream& {
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

