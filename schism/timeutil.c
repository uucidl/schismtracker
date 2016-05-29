#include "headers.h"

#include "timeutil.h"

// having our own time types has the advantage of smoothing out the operating system differences.
// some examples:
// windows has a struct timeval that comes from winsock and is not 64bit ready
// some OS have unsigned counters for seconds (time_t) and microseconds (at least this is hinted
// by the GNU libc documentation. URL(http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html)

#if defined(WIN32)
static const unsigned __int64 epoch = UINT64CONST(116444736000000000);
// TODO(uucidl): move to osdefs
// Comes from postgresql gettimeofday
// Note: this function is not for Win32 high precision timing purpose. See
// elapsed_time().
//
// TODO(uucidl): this is a legitimate concern. Also sometimes we only care
// about calculating some elapsed time rather than getting an absolute unix epoch time. This is basically
// dependent on whether we are looking to get the civil time afterwards. And even then, going through a
// unix epoch time is unnecessary as long as the translation is known.
struct absolute_time precise_time_now()
{
    struct absolute_time result;
    FILETIME    file_time;
    SYSTEMTIME  system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.LowPart = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;

    result.seconds = (long) ((ularge.QuadPart - epoch) / 10000000L);
    result.microseconds = (long) (system_time.wMilliseconds * 1000);
    return result;
}
#else
struct absolute_time precise_time_now()
{
  struct absolute_time result;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  result.seconds = tv.tv_sec;
  result.microseconds = tv.tv_usec;
  return result;
}
#endif

struct absolute_time_seconds system_time_now()
{
  struct absolute_time_seconds result;
  time(&result.value);
  return result;
}

struct absolute_time absolute_time_subtract(struct absolute_time a,
                                            struct absolute_time b)
{
  // same algo as timersub
  struct absolute_time result;
  result.seconds = a.seconds - b.seconds;
  result.microseconds = a.microseconds - b.microseconds;
  if (result.microseconds < 0) {
    result.seconds--;
    result.microseconds += 1000000;
  }
  return result;
}

struct civil_time civil_time_from_absolute_time(struct absolute_time input)
{
  struct absolute_time_seconds temp;
  temp.value = input.seconds;
  return civil_time_from_absolute_time_seconds(temp);
}

struct civil_time civil_time_from_absolute_time_seconds(struct absolute_time_seconds input)
{
  struct tm y;
  /* DO NOT change this back to localtime(). If some backward platform
     doesn't have localtime_r, it needs to be implemented separately. */
  localtime_r(&input.value, &y);
  struct civil_time result;
  result.tm_year = y.tm_year;
  result.tm_mon = y.tm_mon;
  result.tm_mday = y.tm_mday;
  result.tm_hour = y.tm_hour;
  result.tm_min = y.tm_min;
  result.tm_sec = y.tm_sec;
  return result;
}

/* time formatting functions */

char *get_date_string(struct absolute_time_seconds when, char *buf)
{
        struct civil_time ct = civil_time_from_absolute_time_seconds(when);
	const char *month_str[12] = {
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December",
	};
	snprintf(buf, 27, "%s %d, %d", month_str[ct.tm_mon], ct.tm_mday, 1900 + ct.tm_year);
	return buf;
}

char *get_time_string(struct absolute_time_seconds when, char *buf)
{
        struct civil_time ct = civil_time_from_absolute_time_seconds(when);
	snprintf(buf, 27, "%d:%02d%s", ct.tm_hour % 12 ? : 12, ct.tm_min, ct.tm_hour < 12 ? "am" : "pm");
	return buf;
}
