/* ## function convert() ## */
#include "viper.h"

int convert(double sec_dur, char * strf_duration) {
   double x, y, yr_rest, d_rest, h_rest;
   int year, day, hour, min, sec;

   x = sec_dur / (double) 31536000;
   year = abs((int)x);
   y = year * (double) 31536000;
   yr_rest = sec_dur - y;
	
   x = yr_rest / (double) 86400;
   day = abs((int)x);
   y = day * (double) 86400;
   d_rest = yr_rest - y;
	
   x = d_rest / (double) 3600;
   hour = abs((int)x);
   y = hour * (double) 3600;
   h_rest = d_rest - y;
	
   x = h_rest / (double) 60;
   min = abs((int)x);
   y = (double) min * (double) 60;
   sec = (int) h_rest - y;
	
   if (year) {
      sprintf(strf_duration, "%4.4dyrs:%3.3dd:%2.2dh", year, day, hour);
   }
   else {
      sprintf(strf_duration, "%3.3dd:%2.2dh:%2.2dm:%2.2ds", day, hour, min, sec);
   }
}
