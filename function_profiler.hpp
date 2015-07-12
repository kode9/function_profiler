#ifndef function_profiler_h
#define function_profiler_h

/// Intrusive thread-safe function profiling
///
/// Author:  Pierre-Luc Perrier <pluc@the-pluc.net>
/// Version: 0.2
/// URL:     https://git.the-pluc.net/function_profiler.git/
///

#ifdef FP_ENABLE

#include <boost/chrono.hpp>

#include <string> // std::string
#include <thread> // std::this_thread::get_id
#include <cstdio> // std::printf
#include <ios>    // std::hex

namespace fp {

// Main structure used to collect data
struct collector {
  // Clocks
  using thread_clock = boost::chrono::thread_clock;
  using thread_time_point = thread_clock::time_point;

  // The duration we use for the report.
  using report_duration = boost::chrono::duration<double, boost::milli>;

  collector() = delete;
  collector(const std::string &name) : m_count{0}, m_thread_accumulator{0}
  {
    std::ostringstream os;
    os << "[FP][" << std::hex << std::this_thread::get_id() << "][" << name << "]";
    m_prefix = os.str();
    m_thread_last = m_last_report = thread_clock::now();
  }

  // We also report upon destruction. Note: this is probably a bad
  // idea.
  ~collector() noexcept
  {
    report();
  }

  inline void start() noexcept
  {
    m_thread_last = thread_clock::now();
  }

  inline void stop()
  {
    ++m_count;
    const auto now = thread_clock::now();
    m_thread_accumulator += now - m_thread_last;
    m_thread_last = std::move(now);

    // Check if we need to report current stats
    if (m_thread_last - m_last_report > report_interval) {
      m_last_report = m_thread_last;
      report();
    }
  }

private:
  inline void report() const
  {
    if (m_count == 0) return;
    auto ms = report_duration{m_thread_accumulator}.count();
    std::printf("%s #%02lu, avg %.5Fms, tot %.5Fms\n", m_prefix.c_str(), m_count, ms / m_count, ms);
  }

private:
  static constexpr auto report_interval = boost::chrono::seconds{1};

  thread_time_point m_thread_last;             // Last thread clock time point
  uint_least64_t m_count;                      // Number of samples
  thread_clock::duration m_thread_accumulator; // Total thread clock duration
  thread_time_point m_last_report;             // Last report
  std::string m_prefix;                        // Prefix used when reporting
};

// RAII structure responsible to update a collector
struct scoped_profiler {
public:
  scoped_profiler(collector &collector_) : m_collector{collector_}
  {
    m_collector.start();
  }

  ~scoped_profiler()
  {
    m_collector.stop();
  }

  scoped_profiler() = delete;
  scoped_profiler(const scoped_profiler &) = delete;
  scoped_profiler(scoped_profiler &&) = delete;

  scoped_profiler &operator=(const scoped_profiler &) = delete;
  scoped_profiler &operator=(scoped_profiler &&) = delete;

private:
  collector &m_collector;
};

constexpr boost::chrono::seconds collector::report_interval;

} // namespace fp

// Note: __func__ is not a preprocessor macro, it's a 'function-local
// predefined variable' (char *), hence we don't expand it.
#define PROFILE_FUNCTION()                                                                                             \
  thread_local fp::collector info{__func__};                                                                           \
  fp::scoped_profiler scoped_profiler{info};

#else // !FB_ENABLE

#define PROFILE_FUNCTION() static_cast<void>(0);

#endif // FB_ENABLE

#endif // function_profiler_h
