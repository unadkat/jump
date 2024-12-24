// This file forms part of Jump (Jay's Utilities and Mathematical Primitives)
// Copyright (C) Jay Unadkat 2024. Released under GPL-3.0-or-later (see COPYING)

#ifndef JUMP_TIMER_DECL_HPP
#define JUMP_TIMER_DECL_HPP

#include <chrono>
#include <format>
#include <string>

#include "jump/utility/types.hpp"

namespace jump {
/// \brief Enables timing of function calls and the calculation of average and
/// total time taken.
class Timer {
    public:
        // \brief Type alias to reduce verbosity.
        using Clock = std::chrono::steady_clock;

        /// \brief Construct the `Timer` in a stopped state, with a given task
        /// description, and an elapsed time of zero.
        Timer(std::string task = "");

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
        const std::string& task() const;
        /// \brief Return current running state.
        bool stopped() const;
        /// \brief Return the number of completed start/stop cycles.
        const long& lap_count() const;
        /// \brief Return the total elapsed time of the timing sessions.
        Clock::duration running_time() const;
        /// \brief Return the total elapsed time of the timing sessions in the
        /// format provided by formatted_time.
        std::string formatted_running_time() const;
        /// \brief Return the average time of completed timing sessions.
        Clock::duration average_time() const;
        /// \brief Return the average time of completed timing sessions in the
        /// format provided by formatted_time.
        std::string formatted_average_time() const;

    private:
        /// \brief Returns the time in a formatted string appropriate for the
        /// time duration supplied.
        std::string formatted_time(Clock::duration time) const;

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
template <typename Os>
inline Os& operator<<(Os& out, const Timer& rhs);
}   // namespace jump

#endif  // JUMP_TIMER_DECL_HPP

