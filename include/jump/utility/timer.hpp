// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024--2025. SPDX-Licence-Identifier: MPL-2.0
// This Source Code Form is subject to the terms of the Mozilla Public Licence,
// v. 2.0. If a copy of the MPL was not distributed with this file, you can
// obtain one at https://mozilla.org/MPL/2.0/

#ifndef JUMP_TIMER_HPP
#define JUMP_TIMER_HPP

#include <chrono>
#include <ostream>
#include <string>

namespace jump {
/// \brief Enables timing of function calls and the calculation of average and
/// total time taken.
class Timer {
    public:
        // \brief Type alias to reduce verbosity.
        using Clock = std::chrono::steady_clock;

        /// \brief Construct the `Timer` in a stopped state, with a given task
        /// description, and an elapsed time of zero.
        explicit Timer(std::string task = "");

        /// \brief Start a new timing session (preserving the stored elapsed
        /// time), if the `Timer` is stopped, otherwise do nothing.
        void start();
        /// \brief Stop the current timing session if it running, otherwise do
        /// nothing. Add the new duration to the running total.
        void stop();
        /// \brief Stop the `Timer` if it is running and reset the measured
        /// elapsed time.
        void reset();

        /// \brief Return task string.
        auto task() const -> const std::string&;
        /// \brief Return current running state.
        auto stopped() const -> bool;
        /// \brief Return the number of completed start/stop cycles.
        auto lap_count() const -> const long&;
        /// \brief Return the total elapsed time of the timing sessions.
        auto running_time() const -> Clock::duration;
        /// \brief Return the total elapsed time of the timing sessions in the
        /// format provided by formatted_time.
        auto formatted_running_time() const -> std::string;
        /// \brief Return the average time of completed timing sessions.
        auto average_time() const -> Clock::duration;
        /// \brief Return the average time of completed timing sessions in the
        /// format provided by formatted_time.
        auto formatted_average_time() const -> std::string;

    private:
        /// \brief Returns the time in a formatted string appropriate for the
        /// time duration supplied.
        auto formatted_time(Clock::duration time) const -> std::string;

    private:
        /// \brief Flag to show if the `Timer` is currently stopped.
        bool m_stopped{true};
        /// \brief Counter of the number of start/stop laps, for the calculation
        /// of the average time.
        long m_counter{0};
        /// \brief Start time for the current timing session.
        Clock::time_point m_start;
        /// \brief Storage of the total duration the `Timer` has been running
        /// since its creation.
        Clock::duration m_delta_t {Clock::duration::zero()};
        /// \brief String storing a description of the task being timed.
        std::string m_task;
};

/// \relates Timer
/// \brief Print summary of timing results to a stream.
auto operator<<(std::ostream& out, const Timer& rhs) -> std::ostream&;
}   // namespace jump

#endif  // JUMP_TIMER_HPP

