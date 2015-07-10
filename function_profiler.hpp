#ifndef function_profiler_h
#define function_profiler_h

/// Intrusive thread-safe function profiling
/// @author Pierre-Luc Perrier <pluc@the-pluc.net>
/// @version 0.2
///
/// Based on an idea by Jeff Preshing:
/// http://preshing.com/20111203/a-c-profiling-module-for-multithreaded-apis/

#ifdef FP_ENABLE

#include <boost/chrono.hpp>

#include <string> // std::string
#include <thread> // std::this_thread::get_id
#include <cstdio> // std::printf
#include <ios>    // std::hex

struct function_profiler_stats
{
  // Clock
  using clock           = boost::chrono::thread_clock;
  using duration        = clock::duration;
  using time_point      = clock::time_point;
  using ms              = boost::chrono::duration<double, boost::milli>;

  function_profiler_stats() = delete;
  function_profiler_stats(const std::string &name)
    : m_accumulator{0}
    , m_count{0}
  {
    std::ostringstream os;
    os << "[FP][" << std::hex << std::this_thread::get_id() << "][" << name << "]";
    m_prefix = os.str();
    m_last = m_last_report = clock::now();
  }

  ~function_profiler_stats()
  {
    report();
  }

  inline void start() {
    m_last = clock::now();
  }

  inline void stop() {
    ++m_count;
    const auto now = clock::now();
    m_accumulator += now - m_last;
    m_last = std::move(now);

    // Check if we need to report current stats
    if (m_last - m_last_report > report_interval) {
      m_last_report = m_last;
      report();
    }
  }

private:
  inline void report() const {
    if (m_count == 0) return;
    auto ms_duration = ms{m_accumulator}.count();
    std::printf("%s #%02lu, avg %.5Fms, tot %.5Fms\n",
		m_prefix.c_str(), m_count, ms_duration / m_count, ms_duration);
  }

private:
  static constexpr auto report_interval = boost::chrono::seconds{1};

  ///< Duration accumulator
  duration m_accumulator;
  ///< Number of samples
  uint_least64_t m_count;
  ///< Last update
  time_point m_last;
  ///< Last report
  time_point m_last_report;
  ///< Prefix used when reporting
  std::string m_prefix;
};

struct function_profiler
{
public:

  function_profiler(function_profiler_stats &stats)
    : m_stats{stats}
  {
    m_stats.start();
  }

  ~function_profiler()
  {
    m_stats.stop();
  }

  function_profiler() = delete;
  function_profiler(const function_profiler &) = delete;
  function_profiler(function_profiler &&) = delete;

  function_profiler & operator =(const function_profiler &) = delete;
  function_profiler & operator =(function_profiler &&) = delete;

private:
  function_profiler_stats &m_stats;
};

constexpr boost::chrono::seconds function_profiler_stats::report_interval;

// Note: __func__ is not a preprocessor macro, it's a 'function-local
// predefined variable' (char *), hence we don't expand it.
#define PROFILE_FUNCTION()                              \
  thread_local function_profiler_stats info{__func__};  \
  function_profiler scoped_profiler{info};

#else // !FB_ENABLE

#define PROFILE_FUNCTION() static_cast<void>(0);

#endif // FB_ENABLE

#endif // function_profiler_h
