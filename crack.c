#include "viper.h"

int crack (crack_input) {
   int  count = 0;
   int  cps = 0;
   int  varlen = strlen(lsf_out.ci_cset);
   int  startpws = lsf_out.ci_pws;
   int  passprg[lsf_out.ci_pwl-1];
   char * testpass;
   int  on = 0;
   int  ui = lsf_out.ci_ui*60;
   int  uicount	= 0;
   int  rf = lsf_out.ci_rf*60;
   char salt[3]			= "";
   int  i, j, y, z, end = 0;
   int  k = 0;
   double tot_num = 0;
   int  time_loop = 0;
   int lines_loop = 0;
   char * pass;
   static struct tm act_time;
   double time_dif;
   double duration;
	
   /* debug only */
   // printf("user>%s pass>%s cset>%s rf>%d pwl>%d pws>%d ui>%d dnum>%s pf>%s\n",
   // lsf_out.ci_user,
   // lsf_out.ci_pass, lsf_out.ci_cset, rf,
   // lsf_out.ci_pwl, lsf_out.ci_pws, ui,
   // lsf_out.ci_dnum, lsf_out.ci_pf);
	
   printf("Character set is %d chars long\n", varlen);
	
   /* get current time */
   time(&read_time);
   start_time = *localtime(&read_time);
   last_time = start_time;
   printf("Starting crack on: %s", asctime(&start_time));
	
   /* go on with the show */
   if (strlen(lsf_out.ci_dnum)) {
      passprg[0]=atoi(strtok(lsf_out.ci_dnum, ",")); i=1;
      printf("Saved progress is: %d", passprg[0]);

      while ( pass = strtok(NULL, ",") ) {
         if (pass && atoi(pass) < 128){passprg[i]=atoi(pass);}
         printf(" %d", passprg[i]); i++;
      }

      printf("\n");
      startpws = i;
      for ( j=i; j<((int) lsf_out.ci_pwl); j++ ) { passprg[j] = 0;}
      on = 1;
   }
	
   strncat(salt, lsf_out.ci_pass, 2);
	
   /* debug only */
   // printf("Salt of the password %13s is: %s\n", lsf_out.ci_pass, salt);
	
   /* make a new password */
   for ( i = startpws; i <= ((int) lsf_out.ci_pwl); i++ ) {
      if (on) { on = 0; }
      else {
         for ( j=0; j<((int) lsf_out.ci_pwl); j++ ) { passprg[j] = 0;}
      }
      tot_num = varlen;
      printf("Cracking for pass length %d", i);
      if(lsf_out.ci_vo) {
         int tc;
         for (tc=1; tc<i; tc++) { tot_num = tot_num * varlen; }
         printf (" (%g possibilities)", tot_num);
      }
      printf("\n");
		
      /* start loop for this length */
      while (passprg[0] != varlen) {
      count++;
		
      /* make actual password out of array */
      for (y=0; y < i; y++) {
         checkpass[y] = lsf_out.ci_cset[passprg[y]];
      }
      checkpass[y+1] = '\0';
			
      /* debug only */
      // for (z=0; z<= (sizeof(passprg)/sizeof(int)); z++)
      // { printf("[%.2d]", passprg[z]);}
      // printf(" - phrase: %s", checkpass);

      /* Here is where the magic happens */
      //testpass = (char *) fcrypt(checkpass, salt);
      testpass = (char *) DES_crypt(checkpass, salt);
		
      /* debug only */
      // printf(" ---> testpwd is: %s\n", testpass);
			
      if (! strcmp(testpass, lsf_out.ci_pass)) {
         // RECALCULATE TIME_DONE HERE!
         time(&read_time);
         act_time=*localtime(&read_time);
         duration = (double) difftime(mktime(&act_time), mktime(&start_time));
         convert(duration, time_done);
         /* debug only */
         //printf("We got it!!!! ----> %s\n", checkpass);
         the_res(lsf_out, checkpass, start_time);
      }
			
      /* debug only */
      // printf("count: %d\n", count);
		
      /* Time check */
      if (count == TIMECHECK) {
         time_loop++;
			
         time(&read_time);
         act_time=*localtime(&read_time);
         time_dif = difftime(mktime(&act_time), mktime(&last_time));
         uicount = uicount + count;
				
         /* debug only */
         // printf("[ act: %s | last: %s | diff: %d | uicount: %d ]\n",
         // asctime(&act_time), asctime(&last_time), time_dif, uicount);

         /* update interval check - calculate duration and cps */	
         /* time_done 'duration' is wrong on exit, fix if to recalculate */
         /* time_done when crack finished */
         if (time_dif >= ui) {
            duration = (double) difftime(mktime(&act_time), mktime(&start_time));
            last_time = act_time;
            cps = uicount / time_dif;
            convert(duration, time_done);
				 	
            /*  v1.4 add-on / time-remaining and percent calculation */
            if(lsf_out.ci_vo) {
               double pre, percent, left, i_sec;
               int width;
               pre = tot_num / (double) TIMECHECK;
               percent = (double) time_loop / pre;
               percent = percent * (double) 10000;
               percent = percent / (double) 100;
               left = tot_num - ((double) time_loop*(double) TIMECHECK);
               i_sec = left / (double) cps;
               convert(i_sec, time_togo);
               if (lines_loop == SCREENHEIGTH-2) {lines_loop = 0;}
               if(lines_loop == 0) {
                  printf("\n[ Length: | Last:    | CPS:    | Time Spent:      | Time Remaining:  | Done:  ]\n");
                  for (width=1; width < SCREENWIDTH; width++) { printf("-"); }
                  printf("\n");
               }
               printf("[    %d    | %8s | %7d | %s | %s | %0#5.2f%c ]\n",
                      i, checkpass, cps, time_done, time_togo, percent, '%' );
               lines_loop++;
            }
            else {
               printf("[ Length: %d | Last: %s | CPS: %d | Time: %s ]\n",
                      i, checkpass, cps, time_done);
            }
		
            /* ## additional conditional routine : auto-save progress (v1.3) */
            if(strlen(lsf_out.ci_pf)) {	
               char dnum[81] = "";
               if ( (fp_pf = fopen(lsf_out.ci_pf, "w")) == NULL ) {
                  printf("Error: Can't open %s!\n", lsf_out.ci_pf);
                  exit(-1);
               }
               for (k=0; k < i; k++) { 
                  /* debug only */
                  // printf("dnum %d: %d\n", k, passprg[k]);
							
                  sprintf(strchr(dnum, '\0'), "%d,", passprg[k]);
               }
               strcpy(strrchr(dnum, ','), "\0");

               fprintf(fp_pf, "--viper_prog--\n");
               fprintf(fp_pf, "%d\n", lsf_out.ci_pws);
               fprintf(fp_pf, "%d\n", lsf_out.ci_pwl);
               fprintf(fp_pf, "%s\n", lsf_out.ci_pass);
               fprintf(fp_pf, "%s\n", lsf_out.ci_user);
               fprintf(fp_pf, "%s\n", dnum);
               fprintf(fp_pf, "%s\n", lsf_out.ci_cset);
               fprintf(fp_pf, "%s\n", lsf_out.ci_pf);
               fprintf(fp_pf, "%d", lsf_out.ci_ui);
     
               fclose(fp_pf);
            }
		
            /* ## check for -rf expiration ## */
            if(lsf_out.ci_rf) {
               if( (lsf_out.ci_rf*3600) >= ((int) duration) ) {
                  printf("\n [ RunFor duration of %d hours expired ]\n",
                  lsf_out.ci_rf);
               }
            }
            time_dif = 0;
            uicount = 0;
            }
            count=0;
         }
			
         passprg[i-1]++;
         for(end = i; end > 0;end --) {
            for (z=0; z <i; z++) {
               if(passprg[i-z] == varlen) { passprg[i-z-1]++; passprg[i-z] = 0; }
            }
         }
      }
   }
   /* if we reach this point, no password matched. Try another charset or length! */
   checkpass[0] = '\0';
   the_res(lsf_out, checkpass, start_time);
}
