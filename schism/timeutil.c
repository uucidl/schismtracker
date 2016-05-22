#include "headers.h"

#include "timeutil.h"

/* time formatting functions */

char *get_date_string(time_t when, char *buf)
{
	struct tm tmr;
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

	/* DO NOT change this back to localtime(). If some backward platform
	doesn't have localtime_r, it needs to be implemented separately. */
	localtime_r(&when, &tmr);
	snprintf(buf, 27, "%s %d, %d", month_str[tmr.tm_mon], tmr.tm_mday, 1900 + tmr.tm_year);
	return buf;
}

char *get_time_string(time_t when, char *buf)
{
	struct tm tmr;

	localtime_r(&when, &tmr);
	snprintf(buf, 27, "%d:%02d%s", tmr.tm_hour % 12 ? : 12, tmr.tm_min, tmr.tm_hour < 12 ? "am" : "pm");
	return buf;
}
