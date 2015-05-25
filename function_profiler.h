#ifndef function_profiler_h
#define function_profiler_h

// http://preshing.com/20111203/a-c-profiling-module-for-multithreaded-apis/

#include <boost/chrono.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/sum.hpp>

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
  using double_duration = boost::chrono::duration<double, duration::period>;
  using ms              = boost::chrono::duration<double, boost::milli>;

  // Accumulator
  using min_tag  = boost::accumulators::tag::min;
  using max_tag  = boost::accumulators::tag::max;
  using mean_tag = boost::accumulators::tag::mean;
  using sum_tag  = boost::accumulators::tag::sum;
  using features = boost::accumulators::features<min_tag, max_tag, mean_tag, sum_tag>;

  function_profiler_stats() = delete;
  function_profiler_stats(const std::string &name)
  {
    std::ostringstream os;
    os << "[function_profiler][" << std::hex << std::this_thread::get_id() << "][" << name << "]";
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
    const auto now = clock::now();
    m_accumulator(duration{now - m_last}.count());
    m_last = std::move(now);

    // Check if we need to report current stats
    if (m_last - m_last_report > report_interval) {
      m_last_report = m_last;
      report();
    }
  }

private:
  inline void report() const {
    std::printf("%s #%02lu, min %.5f, max %.5f, mean %.5f, total %.5f\n",
                m_prefix.c_str(),
                boost::accumulators::count(m_accumulator),
                ms{duration{boost::accumulators::min(m_accumulator)}}.count(),
                ms{duration{boost::accumulators::max(m_accumulator)}}.count(),
                ms{double_duration{boost::accumulators::mean(m_accumulator)}}.count(),
                ms{duration{boost::accumulators::sum(m_accumulator)}}.count());
  }

private:
  static constexpr auto report_interval = boost::chrono::seconds{1};

  ///< Statistics accumulator
  boost::accumulators::accumulator_set<duration::rep, features> m_accumulator;
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

// Note: __func__ is not a preprocessor macro, it's a 'function-local
// predefined variable' (char *), hence we don't expand it.
#define PROFILE_FUNCTION()                              \
  thread_local function_profiler_stats info{__func__};  \
  function_profiler scoped_profiler{info};

#endif // function_profiler_h
