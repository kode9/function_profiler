#ifndef function_profiler_h
#define function_profiler_h

#include <thread>
#include <glog/logging.h>
#include <glog/raw_logging.h>

#include <boost/chrono.hpp>

struct function_profiler_stats
{
  using clock = boost::chrono::thread_clock;
  using ms = boost::chrono::duration<double, boost::milli>;

  int m_count;
  clock::duration m_total;

  function_profiler_stats() : m_count{0}, m_total{0} {}

  inline void increment() { ++m_count; }
  inline void add_time(const clock::duration &duration) { m_total += duration; }

  inline void print() const {
    if (m_count % 10 == 0) {
      auto tid = std::this_thread::get_id();
      std::ostringstream os; os << tid;
      RAW_LOG(INFO, "Thread %s: %d | %F", os.str().c_str(), m_count, ms{m_total}.count());
    }
  }
};

struct function_profiler
{
public:
  using clock = function_profiler_stats::clock;

  function_profiler(function_profiler_stats &stats)
    : m_stats{stats}, m_start{clock::now()}
  {
    m_stats.increment();
  }

  ~function_profiler()
  {
    m_stats.add_time(clock::now() - m_start);
    m_stats.print();
  }

  function_profiler() = delete;
  function_profiler(const function_profiler &) = delete;
  function_profiler(function_profiler &&) = delete;

  function_profiler & operator =(const function_profiler &) = delete;
  function_profiler & operator =(function_profiler &&) = delete;

private:
  function_profiler_stats &m_stats;
  clock::time_point m_start;
};

#endif // function_profiler_h
