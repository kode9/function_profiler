# Time functions

## clock

    #include <time.h>
	clock_t clock(void);

	The clock() function shall return the implementation's best
    approximation to the processor time used by the process since the
    beginning of an implementation-defined era related only to the
    process invocation.

	C89, C99, POSIX.1-2001.
	clock pre seconds => / CLOCKS_PER_SEC => microseconds

	arbitrary value at start of the process => t = clock_now - clock_at_start
	wrap: 32bit => ~72min
	may return children time, not on linux
	glibc <= 2.17: implemeted using times
	glibc > 2.17: implemented using clock_gettimes

## times

	#include <sys/times.h>
	clock_t times(struct tms *buffer);

	SVr4, 4.3BSD, POSIX.1-2001.

    struct tms {
    clock_t tms_utime;  /* user time */
    clock_t tms_stime;  /* system time */
    clock_t tms_cutime; /* user time of children */
    clock_t tms_cstime; /* system time of children */
    };

	clock ticks => / sysconf(_SC_CLK_TCK)

## getrusage

	#include <sys/resource.h>
	int getrusage(int who, struct rusage *r_usage);

	SVr4, 4.3BSD.  POSIX.1-2001.
	struct timeval ru_utime; /* user CPU time used */
    struct timeval ru_stime; /* system CPU time used */
	RUSAGE_SELF
	RUSAGE_CHILDREN

	Linux: RUSAGE_THREAD (since Linux 2.6.26)

## clock_gettime

	#include <time.h>
	int clock_getres(clockid_t clk_id, struct timespec *res);
	int clock_gettime(clockid_t clk_id, struct timespec *tp);

	SUSv2, POSIX.1-2001.
	struct timespec {
    time_t   tv_sec;        /* seconds */
    long     tv_nsec;       /* nanoseconds */
    };
	_POSIX_C_SOURCE >= 199309L

	can check define _POSIX_TIMERS (<unistd.h>)

	CLOCK_REALTIME (mandatory) wall-clock since Epoch
	if _POSIX_MONOTONIC_CLOCK => CLOCK_MONOTONIC seconds / nanoseconds since arbitrary
	if _POSIX_CPUTIME => CLOCK_PROCESS_CPUTIME_ID => CPU
	if _POSIX_THREAD_CPUTIME => CLOCK_THREAD_CPUTIME_ID => Thread CPU

	Linux
	=> CLOCK_MONOTONIC
	CLOCK_REALTIME_COARSE (since Linux 2.6.32) A faster but less precise version of CLOCK_REALTIME
	CLOCK_MONOTONIC_COARSE (since Linux 2.6.32) A  faster  but less precise version of CLOCK_MONOTONIC
	CLOCK_MONOTONIC_RAW (since Linux 2.6.28) Similar to CLOCK_MONOTONIC but not subject to NTP/adjtime
	CLOCK_BOOTTIME (since Linux 2.6.39) Identical to CLOCK_MONOTONIC, except it also includes any time that the system is suspended
	CLOCK_PROCESS_CPUTIME_ID (since Linux 2.6.12) Per-process CPU-time clock
	CLOCK_THREAD_CPUTIME_ID (since Linux 2.6.12) Thread-specific CPU-time clock
