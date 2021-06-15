# techo
Echo-like command with high-precision timestamp.
Also calculates time differences.
Intended for shell script logging.

## License

I want there to be NO barriers to using this code, so I am releasing it to the public domain.  But "public domain" does not have an internationally agreed upon definition, so I use CC0:

Copyright 2020 Steven Ford http://geeky-boy.com and licensed
"public domain" style under
[CC0](http://creativecommons.org/publicdomain/zero/1.0/):
![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png "CC0")

To the extent possible under law, the contributors to this project have
waived all copyright and related or neighboring rights to this work.
In other words, you can use this code for any purpose without any
restrictions.  This work is published from: United States.  The project home
is https://github.com/fordsfords/pgen

To contact me, Steve Ford, project owner, you can find my email address
at http://geeky-boy.com.  Can't see it?  Keep looking.


## Introduction

Shell scripts are useful tools to write.
For things like test scripts, logging can be important,
and those logs should have time stamps.
Typicaly this is done like this:

````
echo `date` "Message to log."
````

This is fine, except sometimes (especially with distributed programs),
finer time precision is desired to see exact sequences of events.

The ```techo``` command gives time precision down to the microsecond
(defaults to millisecond).


## Usage

````
Usage: techo [-h] [-d] [-D start] [-n] [-p precision] [-T end] [message ...]
where:
  -h - print help
  -H - human-readable output
  -d - include date
  -D start - delta mode; microseconds since 'start'
  -n - newline omit from output
  -p precision - number of decimal digits for seconds (0-6, default=3).
  -T end - for testing purposes
  message - zero or more text strings to be printed.
For details, see https://github.com/fordsfords/techo
````

There are two modes of operation: regular and delta.
The tool defaults to regular mode, in which case it prints a
regular timestamp.
The "-D start" option turns on delta mode,
which is used to measure time durations between two points.

The "-T end" option is only used for testing the program (see "tst.sh").

Note, the times printed are truncated, not rounded.
I.e. if the timestamp sampled ends with ".1009", it will print as ".100".


## Regular Mode

The "-d" and "-p precision" options are for use with regular mode.

````
$ ./techo
13:39:58.629
$ ./techo -d -p 0  # include date, no decimals.
2020-09-25 13:40:28
$ ./techo -p 6 Hello  # include message, microsecond precision
13:41:24.574298 Hello
$ ./techo -n goodby   # Newline omit (system prompt is appended).
13:41:52.712 goodby$
````


## Delta Mode

The "-D start" option turns on delta mode.
This mode is used to measure time durations between two points.

For example:
````
$ START=`./techo -D 0`
$ sleep 2
$ ./techo -D $START
2012773
$ echo $START
1623780027906142
````
So the two "techo" commands happened 2,012,773 microseconds apart.

The "-H" option is for use with delta mode.
Without "-H", it prints microseconds.
With "-H", it prints an appropriate unit with 3 decimals.

For example:
````
$ START=`./techo -D 0`
$ sleep 2
$ ./techo -D $START
2.011 sec
$ echo $START
1623780364223472
````


## Building and Testing

This is a very simple program with no dependencies.
It should build fine on any Unix with just:

````
gcc -o techo techo.c
````

Let me know if you encounter a Unix that doesn't like this.

There is a "tst.sh" script included that builds it and runs a series of
self-tests.


## Why not date +"%N"?

The GNU date command supports the "%N" formatting directive which
provides nanoseconds.

However, not all Unixes use a recent GNU "date" command.
In particular, MacOS does not support "%N" out of the box.
(And yes, that can be fixed with homebrew, but I didn't want dependencies.)

Also, the date command doesn't support suppressing the trailing newline,
which can be handy sometimes.

Also, delta mode is handy, especially with the "-H" option to print
human-friendly times.

Also the techo command leads to more concise scripts. This:
````
techo "Hello"
````
is more readable than:
````
echo `date +"%H:%M:%S.%3N"` "Hello"
````

Finally, programming is my hobby and writing this was a fun diversion.
