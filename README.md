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

## Usage

````
techo [-n] [-p precision] [message]
````

Where:

* -n - do not include newline in output
* -p precision - number of decimal digits (default to 3).
