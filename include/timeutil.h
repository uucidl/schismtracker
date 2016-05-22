/*
 * Schism Tracker - a cross-platform Impulse Tracker clone
 * copyright (c) 2003-2005 Storlek <storlek@rigelseven.com>
 * copyright (c) 2005-2008 Mrs. Brisby <mrs.brisby@nimh.org>
 * copyright (c) 2009 Storlek & Mrs. Brisby
 * copyright (c) 2010-2012 Storlek
 * URL: http://schismtracker.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* time related types and functions */
#ifndef schismtracker_timeutil_h
#define schismtracker_timeutil_h

#ifdef NEED_TIME
# if TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
# else
#  if HAVE_SYS_TIME_H
#   include <sys/time.h>
#  else
#   include <time.h>
#  endif
# endif
# ifndef timersub
// from FreeBSD
#  define timersub(tvp, uvp, vvp)                                       \
	do {                                                            \
		(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;          \
		(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;       \
		if ((vvp)->tv_usec < 0) {                               \
			(vvp)->tv_sec--;                                \
			(vvp)->tv_usec += 1000000;                      \
		}                                                       \
	} while (0)
# endif
#endif

#ifdef NEED_TIME
# ifndef HAVE_STRPTIME
char *strptime(const char *buf, const char *fmt, struct tm *tm);
# endif
# ifdef WIN32
struct tm *localtime_r(const time_t *timep, struct tm *result);
# endif
#endif

struct civil_time
{
   struct tm value;
};

struct absolute_time_seconds
{
  time_t value;
};

struct absolute_time
{
#ifdef WIN32
  uint64_t seconds;
  uint32_t microseconds;
#else
  struct timeval value;
#endif
};

/* formatting */
/* for get_{time,date}_string, buf should be (at least) 27 chars; anything past that isn't used. */
char *get_date_string(time_t when, char *buf);
char *get_time_string(time_t when, char *buf);

#endif