/* techo.c */
/*
# This code and its documentation is Copyright 2020 Steven Ford, http://geeky-boy.com
# and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/techo
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

/* See https://github.com/fordsfords/safe_atoi */
#define SAFE_ATOI(a_,l_) do { \
  unsigned long long fs_[9] = { \
    0, 0xff, 0xffff, 0, 0xffffffff, 0, 0, 0, 0xffffffffffffffff }; \
  errno = 0; \
  (l_) = fs_[sizeof(l_)]; \
  if ((l_) < 0) { /* Is result a signed value? */ \
    char *in_a_ = a_;  char *temp_ = NULL;  long long llresult_; \
    if (strlen(in_a_) > 2 && *in_a_ == '0' && *(in_a_ + 1) == 'x') { \
      llresult_ = strtoll(in_a_ + 2, &temp_, 16); \
    } else { \
      llresult_ = strtoll(in_a_, &temp_, 10); \
    } \
    if (errno != 0 || temp_ == in_a_ || temp_ == NULL || *temp_ != '\0') { \
      if (errno == 0) { \
        errno = EINVAL; \
      } \
      fprintf(stderr, "%s:%d, Error, invalid number for %s: '%s'\n", \
         __FILE__, __LINE__, #l_, in_a_); \
      exit(1); \
    } else { /* strtol thinks success; check for overflow. */ \
      (l_) = llresult_; /* "return" value of macro */ \
      if ((l_) != llresult_) { \
        fprintf(stderr, "%s:%d, %s over/under flow: '%s'\n", \
           __FILE__, __LINE__, #l_, in_a_); \
        exit(1); \
      } \
    } \
  } else { \
    char *in_a_ = a_;  char *temp_ = NULL;  unsigned long long llresult_; \
    if (strlen(in_a_) > 2 && *in_a_ == '0' && *(in_a_ + 1) == 'x') { \
      llresult_ = strtoull(in_a_ + 2, &temp_, 16); \
    } else { \
      llresult_ = strtoull(in_a_, &temp_, 10); \
    } \
    if (errno != 0 || temp_ == in_a_ || temp_ == NULL || *temp_ != '\0') { \
      if (errno == 0) { \
        errno = EINVAL; \
      } \
      fprintf(stderr, "%s:%d, Error, invalid number for %s: '%s'\n", \
         __FILE__, __LINE__, #l_, in_a_); \
      exit(1); \
    } else { /* strtol thinks success; check for overflow. */ \
      (l_) = llresult_; /* "return" value of macro */ \
      if ((l_) != llresult_) { \
        fprintf(stderr, "%s:%d, %s over/under flow: '%s'\n", \
           __FILE__, __LINE__, #l_, in_a_); \
        exit(1); \
      } \
    } \
  } \
} while (0)

/* Options */
int o_human = 0;  /* -H */
int o_date = 0;  /* -d */
unsigned long long o_delta_start = -1;  /* -D */
int o_newline_omit = 0;  /* -n */
int o_precision = 3;  /* -p */
unsigned long long o_test_end = -1;  /* -T */


char usage_str[] = "Usage: techo [-h] [-d] [-D start] [-H] [-n] [-p precision] [-T end] [message ...]";
void usage(char *msg) {
  if (msg) fprintf(stderr, "\n%s\n\n", msg);
  fprintf(stderr, "%s\n", usage_str);
  exit(1);
}  /* usage */

void help() {
  printf("%s\n"
    "where:\n"
    "  -h - print help\n"
    "  -H - human-readable output for delta mode\n"
    "  -d - include date\n"
    "  -D start - delta mode; print microseconds since 'start'\n"
    "  -n - newline omit from output\n"
    "  -p precision - number of decimal digits for seconds (0-6, default=3).\n"
    "  -T end - for testing purposes\n"
    "  message - zero or more text strings to be printed.\n"
    "For details, see https://github.com/fordsfords/techo\n",
    usage_str);
  exit(0);
}  /* help */


void parse_cmdline(int argc, char **argv)
{
  int opt;

  while ((opt = getopt(argc, argv, "hHdD:np:T:")) != EOF) {
    switch (opt) {
      case 'h':
        help();
        break;
      case 'H':
        o_human = 1;
        break;
      case 'd':
        o_date = 1;
        break;
      case 'D':
        SAFE_ATOI(optarg, o_delta_start);
        break;
      case 'n':
        o_newline_omit = 1;
        break;
      case 'p':
        SAFE_ATOI(optarg, o_precision);
        if (o_precision < 0 || o_precision > 6) usage("precision must be 0-6");
        break;
      case 'T':
        SAFE_ATOI(optarg, o_test_end);
        break;
      default:
        usage(NULL);
    }  /* switch */
  }  /* while */
}  /* parse_cmdline */


/* Get date/time stamp (date optional) with up to microsecond precision.
 * Returns passed-in string pointer for convenience. */
char *get_timestamp(char *str, int bufsz, int do_date, int precision)
{
  static unsigned long long pow_10[7] = { 1, 10, 100, 1000, 10000, 100000, 1000000 };
  struct timeval cur_time_tv;
  struct tm tm_buf;
  char *rtn_str = str;

  gettimeofday(&cur_time_tv, NULL);
  localtime_r(&cur_time_tv.tv_sec, &tm_buf);  /* Break down current time. */

  if (do_date && precision > 0) {
    snprintf(str, bufsz, "%04d-%02d-%02d %02d:%02d:%02d.%0*d",
        (int)tm_buf.tm_year + 1900, (int)tm_buf.tm_mon, (int)tm_buf.tm_mday,
        (int)tm_buf.tm_hour, (int)tm_buf.tm_min, (int)tm_buf.tm_sec,
        precision, (int)(cur_time_tv.tv_usec / pow_10[6 - precision]));
  }
  else if (do_date && precision == 0) {
    snprintf(str, bufsz, "%04d-%02d-%02d %02d:%02d:%02d",
        (int)tm_buf.tm_year + 1900, (int)tm_buf.tm_mon, (int)tm_buf.tm_mday,
        (int)tm_buf.tm_hour, (int)tm_buf.tm_min, (int)tm_buf.tm_sec);
  }
  else if (!do_date && precision > 0) {
    snprintf(str, bufsz, "%02d:%02d:%02d.%0*d",
        (int)tm_buf.tm_hour, (int)tm_buf.tm_min, (int)tm_buf.tm_sec,
        precision, (int)(cur_time_tv.tv_usec / pow_10[6 - precision]));
  }
  else {  /* !do_date && precision==0 */
    snprintf(str, bufsz, "%02d:%02d:%02d",
        (int)tm_buf.tm_hour, (int)tm_buf.tm_min, (int)tm_buf.tm_sec);
  }

  return rtn_str;
}  /* get_timestamp */


void print_time()
{
  char timestamp[32];

  printf("%s", get_timestamp(timestamp, sizeof(timestamp), o_date, o_precision));
}  /* print_time */


#define USEC_PER_SEC 1000000ull
#define USEC_PER_MIN (USEC_PER_SEC * 60ull)
#define USEC_PER_HR (USEC_PER_MIN * 60ull)
#define USEC_PER_DAY (USEC_PER_HR * 24ull)
#define USEC_PER_MO (USEC_PER_YR / 12ull)
#define USEC_PER_YR (USEC_PER_DAY * 365ull)

void print_delta()
{
  struct timeval cur_time_tv;
  unsigned long long delta_us;

  gettimeofday(&cur_time_tv, NULL);
  delta_us = cur_time_tv.tv_sec;
  delta_us *= USEC_PER_SEC;
  delta_us += cur_time_tv.tv_usec;

  if (o_test_end != -1) {
    /* For testing purposes. */
    delta_us = o_test_end;
  }

  delta_us -= o_delta_start;

  if (o_human) {
    if (delta_us < USEC_PER_MIN) {
      /* Display in sec. */
      float f_sec = (float)delta_us / (float)USEC_PER_SEC;
      printf("%.03f sec", f_sec);
    }
    else if (delta_us < USEC_PER_HR) {
      /* Display in minutes. */
      float f_min = (float)delta_us / (float)USEC_PER_MIN;
      printf("%.03f min", f_min);
    }
    else if (delta_us < USEC_PER_DAY) {
      /* Display in hours. */
      float f_hr = (float)delta_us / (float)USEC_PER_HR;
      printf("%.03f hr", f_hr);
    }
    else if (delta_us < USEC_PER_MO) {
      /* Display in days. */
      float f_day = (float)delta_us / (float)USEC_PER_DAY;
      printf("%.03f day", f_day);
    }
    else if (delta_us < USEC_PER_YR) {
      /* Display in months. */
      float f_mo = (float)delta_us / (float)USEC_PER_MO;
      printf("%.03f mo", f_mo);
    }
    else {
      /* Display in years. */
      float f_yr = (float)delta_us / (float)USEC_PER_YR;
      printf("%.03f yr", f_yr);
    }
  } else {  /* not o_human */
    printf("%llu", delta_us);
  }
}


int main(int argc, char **argv)
{
  parse_cmdline(argc, argv);

  if (o_delta_start != -1) {
    print_delta();
  } else {
    print_time();
  }

  while (optind < argc) {
    printf(" %s", argv[optind]);
    optind ++;
  }

  if (! o_newline_omit) {
    printf("\n");
  }

  return 0;
}  /* main */
