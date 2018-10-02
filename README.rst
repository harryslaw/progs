=====
progs
=====
Simple command line utilities
-----------------------------

Some simple programs with simple use cases. I hope that someone out there
finds them to come in handy.

charge
------

   Analyses the given file for battery info, defaults to
   `/sys/class/power_supply/BAT%i/uevent` and its output is formatable. It has a
   continuous output mode, `stay` where it outputs after each `interval`

hash
----

   Finds the SHA256 value of the given files and can also rapidly print the files
   contents.

pseudo
------

   Outputs an array of any given size with pseudo random numbers between 1 and a
   given number.

when
----

   A small clock utility that can format its output, see strftime(3). It also
   has the `stay` option which makes it output continuously based on the given
   `interval`.
