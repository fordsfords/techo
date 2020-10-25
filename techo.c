/* techo.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>


/* See https://blog.geeky-boy.com/2014/04/strtoul-preferred-over-atoi.html */
#define SAFE_ATOL(a_,l_) do { \
  char *in_a_ = a_;  char *temp_ = NULL;  long result_; \
  errno = 0; \
  if (strlen(in_a_) > 2 && *in_a_ == '0' && *(in_a_ + 1) == 'x') \
    result_ = strtol(in_a_ + 2, &temp_, 16); \
  else \
    result_ = strtol(in_a_, &temp_, 10); \
  if (errno != 0 || temp_ == in_a_ || temp_ == NULL || *temp_ != '\0') { \
    fprintf(stderr, "%s:%d, Error, invalid numeric value for %s: '%s'\n", \
       __FILE__, __LINE__, #l_, in_a_); \
    usage(NULL); \
  } \
  l_ = result_; /* "return" value of macro */ \
} while (0)


/* Options */
int o_date         = 0;  /* -d */
int o_newline_omit = 0;  /* -n */
int o_precision    = 3;  /* -p precision */


int pow_10[7] = { 1, 10, 100, 1000, 10000, 100000, 1000000 };


char usage_str[] = "Usage: techo [-h] [-d] [-n] [-p precision] [message ...]";
void usage(char *msg) {
  if (msg) fprintf(stderr, "\n%s\n\n", msg);
  fprintf(stderr, "%s\n", usage_str);
  exit(1);
}  /* usage */

void help() {
  fprintf(stderr, "%s\n"
    "where:\n"
    "-h - print help\n"
    "-d - include date\n"
    "-n - newline omit from output\n"
    "-p precision - number of decimal digits for seconds (0-6, default to 3).\n"
    "message - zero or more text strings to be printed.\n",
    usage_str);
  exit(0);
}  /* help */

void parse_cmdline(int argc, char **argv)
{
  int opt;

  while ((opt = getopt(argc, argv, "hdnp:")) != EOF) {
    switch (opt) {
      case 'h':
        help();
        break;
      case 'd':
        o_date = 1;
        break;
      case 'n':
        o_newline_omit = 1;
        break;
      case 'p':
        SAFE_ATOL(optarg, o_precision);
        if (o_precision < 0 || o_precision > 6) usage("precision must be 0-6");
        break;
      default:
        usage(NULL);
    }  /* switch */
  }  /* while */
}  /* parse_cmdline */


void t()
{
  struct timeval cur_time_tv;
  struct tm tm_buf;

  gettimeofday(&cur_time_tv, NULL);
  localtime_r(&cur_time_tv.tv_sec, &tm_buf);  /* Break down current time. */

  if (o_date) {
    printf("%04d-%02d-%02d ",
      (int)tm_buf.tm_year + 1900, (int)tm_buf.tm_mon, (int)tm_buf.tm_mday);
  }

  if (o_precision > 0) {
    printf("%02d:%02d:%02d.%0*d",
      (int)tm_buf.tm_hour, (int)tm_buf.tm_min, (int)tm_buf.tm_sec,
      o_precision, (int)(cur_time_tv.tv_usec / pow_10[6 - o_precision]));
  } else {
    printf("%02d:%02d:%02d",
      (int)tm_buf.tm_hour, (int)tm_buf.tm_min, (int)tm_buf.tm_sec);
  }
}


int main(int argc, char **argv)
{
  parse_cmdline(argc, argv);

  t();

  while (optind < argc) {
    printf(" %s", argv[optind]);
    optind ++;
  }

  if (! o_newline_omit) {
    printf("\n");
  }

  return 0;
}  /* main */
