# techo
Echo-like command with high-precision timestamp.
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
and those logs should hae time stamps.
Typicaly this is done like this:

````
echo `date` "Message to log."
````

This is fine, except sometimes (especially with distributed programs),
finer time precision is desired to see exact sequences of events.

The ```techo``` command gives time precision down to the microsecond
(defaults to millisecond).


## Why not date "+%N"?

The GNU date command supports the "%N" formatting directive which
includes nanoseconds.

However, not all Unixes use a recent GNU "date" command.
In particular, MacOS does not support "%N" out of the box.
(And yes, that can be fixed with homebrew, but I didn't want dependencies.)

Also, the date command doesn't support suppressing the trailing newline,
which can be handy sometimes.

Also, it requires a bunch of extra scripting to specify the precision,
and typically requires using bash-specific constucts for the arithmetic.

Also, it leads to more concise lines. This:
````
techo "Hello"
````
is more readable than:
````
echo `date +"%H:%M:%S.%N"` "Hello"
````

Finally, programming is my hobby and writing this was a fun diversion.


## Usage

````
Usage: techo [-h] [-d] [-n] [-p precision] [message ...]
where:
-h - print help
-d - include date
-n - newline omit from output
-p precision - number of decimal digits for seconds (0-6, default to 3).
message - zero or more text strings to be printed.
````

Note, the times printed are truncated, not rounded.
I.e. if the timestamp sampled ends with ".1995" and is printed with "-p 1",
it will print as ".1"


## Examples

````
bash-3.2$ ./techo
13:39:58.629
bash-3.2$ ./techo -d -p 0  # include date, second precision
2020-09-25 13:40:28
bash-3.2$ ./techo -p 6 Hello  # include message, microsecond precision
13:41:24.574298 Hello
bash-3.2$ ./techo -n goodby   # Newline omit (system prompt is appended).
13:41:52.712 goodbybash-3.2$
````


## Building

This is a very simple program with no dependencies.
It should build fine on any Unix with just:

````
gcc -o techo techo.c
````

Let me know if you encounter a Unix that doesn't like this.
