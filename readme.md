## Viper

Frank4DD, @ May 2000

Viper is a prute force UNIX-style password cracker for passwords encrypt with
crypt. It has been developed from Hale's viper 1.4 Perl  program.
While there are other more powerful crack programs out, this one is about
studying the safety of passwords while hardware speed is increasing drastically.
If I remember right, I read that on a PDP-11 the password generation took 30
seconds. Now we can do more then 230.000 generations per second on a single
CPU core of a single system, increasing speed by a factor of several million.
Still, there is some time to go for a 8-character password on full keyspace,
see keyspace.txt.

Originally, the ufc-crypt implementation seemed to be the fastest
crypt function around, using it made porting the program to different platforms
easy.

Viper runs under Linux, Solaris, HPUX and DOS/Windows.


### Benchmarks

The original, Hale's viper.pl (Perl):

|Year| System | CPU | OS | Version | CPS rate |
|----|--------|-----|----|---------|---------:|
|2000| SparcStation | SUN U-Sparc 400Mhz | Solaris | 2.6 | 700 |
|2000| Desktop PC | Pentium III 650Mhz | Linux | 2.2.13 | 16,000 |

Modified viper (C):

|Year| System | CPU | OS | Version | Compiler | CPS rate |
|----|--------|-----|----|---------|----------|---------:|
|2000| HP 9000-B180L |  PA-7300LC 180Mhz | HPUX | 10.20 | gcc | 7000 |
|2000| SparcStation | SUN U-Sparc 400Mhz | Solaris | 2.6 | gcc | 25,000 |
|2000| Desktop PC | Pentium III 650Mhz | Linux | 2.2.13 | gcc | 40,000 |
|2000| Desktop PC | Pentium III 650Mhz | Windows | Win98 | gcc | 50,000 |
|2000| Desktop PC | Pentium III 650Mhz | DOS | 7.0  | djgpp | 50,000 |
|2004| HP Laptop | 1x 1.7 GHz AMD64 | Windows | WinXP | gcc | 120,000 |
|2011| Cloud Server | vCPU (unknown) | Linux | 2.6.31 | gcc | 150,000 |
|2016| Cloud Server | Xeon E5-2680 2.5GHz | Linux | 4.4.0 | gcc | 250,000 |


### Usage

<pre>Viper v1.6 (Hale 05/12/2000) - C version by Frank4DD (05/05/2014)
Wiltered Fire - www.wilter.com/wf, incl. bugfixes by David C. Rankin

        -f <file>    File to load password from (required unless using lsf)
        -u <user>    Username to load from file (required unless using lsf)
        -lsf <file>  Load saved file from previous session
        -pf <file>   Save progress to file at update interval
        -rf #        Amount of time in hours to run for (default infinite)
        -c #         Character set from charset.ini to use (default 1)
        -pws #       Minimum password length (starting value, default 1)
        -pwl #       Maximum password length (default 8 - maximum 16)
        -ui #        Console update interval (in minutes - default 10)
        -v           Verbose output</pre>

### Usage Example

<pre>susie112:/home/me/viper-1.6/src # ./viper -f passwd -u root -ui 1 -v

Viper v1.6 (Hale 05/12/2000) - C version by Frank4DD (05/05/2014)
Wiltered Fire - www.wilter.com/wf, incl. bugfixes by David C. Rankin

Found: user root pw:reUJbHrFWYCQk
Found: Charset 0 in charset.ini
...command line parameters loaded.
Character set is 93 chars long
Starting crack on: Sun Oct  3 23:04:44 2009
Cracking for pass length 1 (93 possibilities)
Cracking for pass length 2 (8649 possibilities)
Cracking for pass length 3 (804357 possibilities)
Cracking for pass length 4 (7.48052e+07 possibilities)

[ Length: | Last:    | CPS:    | Time Spent:      | Time Remaining:  | Done:  ]
-------------------------------------------------------------------------------
[    4    |     kq2r |  150000 | 000d:00h:01m:00s | 000d:00h:07m:18s | 12.03% ]

 The password has been located.
 Username : root
 Password : test
 Started  : Sun Oct  3 23:04:44 2009
 Finished : Sun Oct  3 23:06:30 2009
 Duration : 000d:00h:01m:00s

Viper exiting...</pre>

### Latest Updates

* Viper Version 1.5 has been updated to use the OpenSSL DES routines for 
encrypting. The UFC library has been dropped as outdated and even generating
segfaults on some systems. There  is a performance gain of approx. 25%
coming from the OpenSSL libraries. In addition to the libraries, the OpenSSL
headers (dev package) need to be installed in order to be able to compile Viper.

* Viper Version 1.6 received bugfixes thanks to David C. Rankin.

See also http://fm4dd.com/sw/viper/
