#!/usr/local/bin/perl

print "\nViper v1.4 \(Hale 5/12/2000\)\nWiltered Fire - www.wilter.com/wf\n\n";

$arg_length = @ARGV;
$file = "";
$user = "";
$char = 1;
$pws = 1;
$pwl = 8;      
$ui = 10;
$pf = "";

# need help?

if($ARGV[0] eq "-h" || $ARGV[0] eq "-?" || $ARGV[0] eq "") { &help; }

# process command line arguments

for($i=0;$i<$arg_length;$i++) {
 if($ARGV[$i] eq "-f") { $file = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-u") { $user = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-c") { $char = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-pwl") { $pwl = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-ui") { $ui = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-pws") { $pws = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-lsf") { $lsf = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-pf") { $pf = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-rf") { $runfor = $ARGV[$i+1]; $i++; }
 if($ARGV[$i] eq "-v") { $verbose = 1; }
 }


# handle minimum value (v1.3)
$pws--;

# break early if calling from file

if($lsf) {
 open(LSF,$lsf) || die "Could not open saved file $lsf\!\n";
 $vp_stat = <LSF>; chop($vp_stat);
 
 ## check to see if run has been completed # 
 if($vp_stat eq "--viper_final--") { die "The saved run has been completed - check $lsf for details\n\n"; }

 #continue otherwise

 $pws   = <LSF>; chop($pws);
 $pwl   = <LSF>; chop($pwl);
 $epass = <LSF>; chop($epass);
 $user  = <LSF>; chop($user);
 $nump  = <LSF>; chop($nump);
 $vars  = <LSF>; chop($vars);
 $pf    = <LSF>; chop($pf);
 $ui    = <LSF>; chop($ui);
 @vars = split(/ /,$vars);
 $charset = join("",@vars);
 close(LSF);
 
 print " \[Loaded parameters from file $lsf\]\n";

 &crack($user,$epass,$charset,$runfor,$pwl,$pws,$ui,1,$nump,$pf);
 }

# check for required arguments

if($file eq "" || $user eq "") {
 print "Both the password file and user name are required\n";
 die "\n";
 }

# attempt to load account from file

open(IN,$file) || die "Could not open password file $file\n";
$epass = "";
while(<IN>) {
 chop($_);
 @sort = split(/:/,$_);
 if(@sort[0] eq $user) { $epass = @sort[1]; }
 }
close(IN);
if($epass eq "") { die "User $user could not be found in $file\n"; }
$pl = length($epass);
if($pl < 4) { die "Bad password for user $user\n"; } # eliminate shadowed 
if($verbose eq 1) { print "Found user $user \(pw:$epass\)\n"; }

# load character set

open(IN,"charset.ini") || die "Could not open charset.ini\n";
$charset = "";
while(<IN>) {
 chop($_);
 @sort = split(/ /,$_);
 if(@sort[0] eq $char) { $charset = @sort[1]; }
 }
close(IN);
if($charset eq "") { die "Could not load character set index $char\n"; }

&crack($user,$epass,$charset,$runfor,$pwl,$pws,$ui,0,"",$pf);

######## start subs ###############

## begin help sub ##

sub help {
 print "   -f <file>    File to load password from (required unless using lsf)\n";
 print "   -u <user>    Username to load from file (required unless using lsf)\n\n";
 print "   -lsf <file>  Load saved file from previous session\n";
 print "   -pf <file>   Save progress to file at update interval\n";
 print "   -rf #        Amount of time in minutes to run for (default forever)\n\n";
 print "   -c #         Character set from charset.ini to use (default 1)\n";         
 print "   -pws #       Minimum password length (starting value - default 1)\n";
 print "   -pwl #       Maximum password length (default 8)\n";
 print "   -ui #        Console update interval (in minutes - default 10)\n";
 print "   -v           Verbose output\n";
 die "\n";
 }

## begin crack sub ##

sub crack {
 my($user,$epass,$charset,$runfor,$pwl,$pws,$ui,$lsf,$nump,$pf) = @_;

 @vars = split(//,$charset);
 $varlen = @vars;
 $count = 0;
 $refresh = 0;
 $realpass = "";
 $on = 0;
 $ui = $ui*60;
 $runfor = $runfor*60;

 # confirm runfor
 if($runfor) {
 $trunfor = &convert($runfor);
 @tsort = split(/:/,$trunfor); # only want hh:mm
 print " \[Running for @tsort[0]\:@tsort[1]\]\n"; }

 # get current time and get initial time since 1/1/1970
 $sloctime = localtime();
 $stime = time();
 $ltime = $stime;
 $cur_time = $stime;

 # display info and start 
 if($verbose eq 1) { print "\nCharacter set $char loaded with $varlen character length\n"; }
 print "Starting crack on $sloctime\n\n";

 # go on with the show....
 
 if($lsf eq 1) { @pass = split(/\,/,$nump); $on = 1;}

 $salt = substr($epass,0,2); 

 for($i=$pws;$i<$pwl;$i++) {
  ### make new password ###
  $it = $i+1;
  
  # initalize 500k counter
  $fivek = 0;
  
  # initalize the password array #
  if($on eq 1) { $on = 0; } else { for($j=0;$j<$it;$j++) { @pass[$j] = 0; } }
  
  # calculate number of password for this length  
  $tot_num = $mult = $varlen;
  for($tc=1;$tc<$it;$tc++) { $tot_num *= $mult; }

  print "Cracking for pass length $it";
  if($verbose eq 1) { print " \($tot_num possibilities\)"; }
  print "\n";

  ### start loop for this length ###
  while(@pass[0] ne $varlen) {
   $count++;
   $refresh++;   

   ### make actual password out of array ###
   $pass = "";
   for($y=0;$y<@pass;$y++) { $pass = $pass . @vars[@pass[$y]]; }

   ## - done - ##
   
   ### Here is where the magic happens ###
   $cpass = crypt($pass,$salt); # the point of the whole script
   if($epass eq $cpass) { 
    $realpass = $pass;
    the_res($user,$realpass,$stime,$sloctime,$pf); 
    }         

   ## - time check - ##
   if($count eq 500000) { 
    $rtime = time(); 
    $time_dif = $rtime - $cur_time; 
    $uicount = $uicount + $count;
    $count=0;
    $fivek++;
    }
   
   ## - ui check - ##
   if($time_dif >= $ui) {
    # calculate duration and cps
    $ctime = time();
    $cur_time = $ctime;
    $totdur = $ctime - $stime;
    $temp_dur = $ctime - $ltime;
    $ltime = $ctime;
    $cps = int $uicount / $temp_dur;

    # v1.4 add-on / time-remaining and percent calculation #
    $pre = int $tot_num / 500000;
    $percent = $fivek / $pre;
    $percent = int $percent * 10000;
    $percent /= 100;
    
    $left = $tot_num - ($fivek*500000);
    $i_sec = int $left / $cps;
    $etaf = &convert($i_sec);
   
    #change second format into hh:mm:ss

    $dur = &convert($totdur);

    #display data

    print "\n[ Length: $it \| Percent: $percent\% \| CPS: $cps \| Time : $dur ]\n";
    if($verbose eq 1) { print "[ Estimated Time Remaining : $etaf \| Last: $pass]\n"; }
    
    ## additional conditional routine : auto-save progress (v1.3)

    if($pf ne "") {
     $numpass = join("\,",@pass);
     $uia = $ui / 60;
     open(TMP,">$pf") || print STDOUT "Could not open progress file $pf!\n";
     print TMP "--viper_prog--\n$i\n$pwl\n$epass\n$user\n$numpass\n@vars\n$pf\n$uia\n";
     close(TMP);
     }
    
    ## check for -rf expiration
    if($runfor) {
    if($totdur >= $runfor) { die "\n \[ RunFor duration expired \]\n"; } }
    
    ## update time counter
    $uicount = 0;
    $time_dif = 0;
    }
   ## whee... ##   

   ### create next password array ###
   $l = @pass;
   $l--;
   @pass[$l]++;   # wow.. this is an important line... 
   for($r=$l;$r>0;$r--) { if(@pass[$r] eq $varlen) { @pass[$r-1]++; @pass[$r] = 0; } }
   ## done with that ###
   }
  }
 the_res($user,$realpass,$stime,$sloctime,$pf);
 }

## begin results sub ##

sub the_res {
my($user,$realpass,$stime,$sloctime,$pf) = @_;
 
if($realpass ne "") { 
  $loctime = localtime();
  $etime = time();
  $ttime = $etime - $stime;
  $duration = &convert($ttime);
  $message = "
  The password has been located!
  Username : $user 
  Password : $realpass
  Started  : $sloctime
  Finished : $loctime
  Time     : $duration
  ";
  } else {
  $message = "
  The password could not be located
  Username : $user 
  Password : ???
  Started  : $sloctime
  Finished : $loctime
  Time     : $duration
  ";
   }
  print "$message\n";

if($pf ne "") {
 open(FINAL,">$pf") ;
 print FINAL "--viper_final--\n\n$message";
 close(FINAL);
 }

die "Viper exiting...\n";
}

## begin time convert sub (understands days/years v1.4) ##

sub convert {
 my($csec) = @_;
 $cmin = 0; $chour = 0; $cday = 0; $cyear = 0;

 while($csec >= 31536000) { $cyear++; $csec -= 31536000; }
 while($csec >= 86400) { $cday++; $csec -= 86400; }
 while($csec >= 1440) { $chour++; $csec -= 1440; }
 while($csec >= 60) { $cmin++; $csec -= 60; }
 
 $a = length($csec);
 if($a eq 1) { $csec = "0" . $csec; }
 if($a eq 0) { $csec = "00"; }
 $a = length($cmin);
 if($a eq 1) { $cmin = "0" . $cmin; }
 if($a eq 0) { $cmin = "00"; }
 $a = length($chour);
 if($a eq 1) { $chour = "0" . $chour; }
 if($a eq 0) { $chour = "00"; }

 $dur = "";
 if($cyear ne 0) { $dur = $cyear . "y "; }
 if($cday ne 0) { $dur = $dur . $cday . "d "; }
 $dur = $dur . $chour . ":" . $cmin . ":" . $csec;
 
 return $dur;
 }
