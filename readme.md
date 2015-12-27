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

System      MHz        OS        Version       compiler       CPS rate
-----------------------------------------------------------------------
PIII        650        Linux     2.2.13        gcc            39062
                       Windows   98            gcc            51282
                       DOS       7.0           djgpp          51282
Hale's viper.pl on Linux:                                     16329

SUN U-Sparc 400        Solaris   2.6           gcc            24691
Hale's viper.pl on Solaris:                                     659

HP778       180        HPUX      10.20         gcc             6993

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

### Latest Updates

Viper Version 1.5 has been updated to use the OpenSSL DES routines for 
encrypting. The UFC library has been dropped as outdated and even generating
segfaults on some systems. There  is a performance gain of approx. 25%
coming from the OpenSSL libraries. In addition to the libraries, the OpenSSL
headers (dev package) need to be installed in order to be able to compile Viper.

Viper Version 1.6 received bugfixes thanks to David C. Rankin.
