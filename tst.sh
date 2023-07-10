#!/bin/bash
# tst.sh - build and test the project.

ASSRT() {
  eval "test $1"

  if [ $? -ne 0 ]; then
    echo "ASSRT ERROR `basename ${BASH_SOURCE[1]}`:${BASH_LINENO[0]}, not true: '$1'" >&2
    exit 1
  fi
}  # ASSRT


# Update doc table of contents (see https://github.com/fordsfords/mdtoc).
if which mdtoc.pl >/dev/null; then mdtoc.pl -b "" README.md;
elif [ -x ../mdtoc/mdtoc.pl ]; then ../mdtoc/mdtoc.pl -b "" README.md;
else echo "FYI: mdtoc.pl not found; see https://github.com/fordsfords/mdtoc"
fi


gcc -Wall -o techo techo.c ; ASSRT "$? -eq 0"

./techo -h >tst.x ; ASSRT "$? -eq 0"
egrep "^[Ww]here:" tst.x >/dev/null ; ASSRT "$? -eq 0"

# This one should return bad status.
./techo -? 2>tst.x ; ASSRT "$? -ne 0"
egrep "^Usage" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -d -n >tst.x ; ASSRT "$? -eq 0"
# Since no newline, wc should report 0 lines but 2 words.
wc tst.x | egrep "^  *0  *2 " >/dev/null ; ASSRT "$? -eq 0"
D=`date +%Y-%m-%d`
egrep "^$D" tst.x >/dev/null; ASSRT "$? -eq 0"

./techo >tst.x ; ASSRT "$? -eq 0"
# Since has newline, wc should report 1 line, 1 word.
wc tst.x | egrep "^  *1  *1 " >/dev/null ; ASSRT "$? -eq 0"
egrep "^[0-9][0-9]:[0-9][0-9]:[0-9][0-9]\.[0-9][0-9][0-9]" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -p 6 testing 1 2 3 >tst.x ; ASSRT "$? -eq 0"
# Since has newline, wc should report 1 line, 5 words.
wc tst.x | egrep "^  *1  *5 " >/dev/null ; ASSRT "$? -eq 0"
egrep "^[0-9][0-9]:[0-9][0-9]:[0-9][0-9]\.[0-9][0-9][0-9][0-9][0-9][0-9] testing 1 2 3" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -n -p 0 testing 1 2 3 >tst.x ; ASSRT "$? -eq 0"
# Since no newline, wc should report 0 lines but 5 words.
wc tst.x | egrep "^  *0  *5 " >/dev/null ; ASSRT "$? -eq 0"
egrep "^[0-9][0-9]:[0-9][0-9]:[0-9][0-9] testing 1 2 3" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -D 0 testing 4 5 6 >tst.x ; ASSRT "$? -eq 0"
# Since has newline, wc should report 1 line, 5 words.
wc tst.x | egrep "^  *1  *5 " >/dev/null ; ASSRT "$? -eq 0"
egrep "^[0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9] testing 4 5 6" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 59200000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^59\.100 sec$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 60100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^1\.000 min$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 3546100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^59\.100 min$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 3600100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^1\.000 hr$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 83160100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^23\.100 hr$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 86400100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^1\.000 day$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 2514240100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^29\.100 day$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 2628000100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^1\.000 mo$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 31273200100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^11\.900 mo$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 31536000100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^1\.000 yr$" tst.x >/dev/null ; ASSRT "$? -eq 0"

./techo -T 315360000100000 -D 100000 -H >tst.x ; ASSRT "$? -eq 0"
egrep "^10\.000 yr$" tst.x >/dev/null ; ASSRT "$? -eq 0"

START=`./techo -D 0` ; ASSRT "$? -eq 0"
DUR=`./techo -D $START` ; ASSRT "$? -eq 0"
ASSRT "$DUR -gt 0 -a $DUR -lt 10000"

sleep 1
DUR=`./techo -D $START` ; ASSRT "$? -eq 0"
ASSRT "$DUR -gt 1000000 -a $DUR -lt 1100000"

./techo -D $START -H >tst.x ; ASSRT "$? -eq 0"
egrep "^1\.0[0-9][0-9] sec$" tst.x >/dev/null ; ASSRT "$? -eq 0"

exit
