#include "viper.h"

int main(int argc, char *argv[]) {
   char *file = 0;			// filename passwordfile
   char pass[MAXENCPWDLENGTH+1] = "";	// encrypted password
   char *user = 0;			// username in passwordfile
   char *lsf  = 0;			// filename loadsourcefile
   char *pf   = 0;			// filename progressfile
   int  rf    = 0;			// runtime limit
   int  chr   = 0;			// characterset
   int  pws   = 1;			// min passwordlength
   int  pwl   = DEFAULTPWLENGTH;	// max passwordlength
   int  ui    = 10;			// console update interval
   int  vo    = 0;			// verbose output
   int  i     = 0;			// loop variable
   FILE *fp_lsf;			// loadsourcefile
   FILE *fp_file;			// passwordfile
   FILE *fp_cset;			// character set file
   char line[255];			// tmp buffer
   char vp_stat[255];			// last saved status
   char * linebuf = NULL;

   printf("\nViper v1.6 (Hale 05/12/2000) - C version by Frank4DD (05/05/2014)\n");
   printf("Wiltered Fire - www.wilter.com/wf, incl. bugfixes by David C. Rankin\n\n");
	
   /* need help? */
   if ( argc == 1 || (!(strcmp (argv[1], "-h"))) || (!(strcmp (argv[1], "-?"))) ) {
      help();
   }
	
   /* verbose output on? */
   for (i = 1; i < argc; i++) {
      if (! (strcmp (argv[i], "-v"  ))) { vo = 1; i++;}
   }
	
   if (vo) {
      if ( (argc != 2) && (argc != 4) && (argc != 6) &&
           (argc != 8) && (argc != 10) && (argc != 12) 
            && (argc != 14) && (argc != 16) ) {
         printf("missing value for argument: try viper -h\n");
         exit(-1);
      }
   }
   else {
      if ( (argc != 1) && (argc != 3) && (argc != 5) &&
           (argc != 7) && (argc != 9) && (argc != 11) 
	    && (argc != 13) && (argc != 15) ) {
         printf("missing value for argument: try viper -h\n");
         exit(-1);
      }
   }
	
   /* process command line arguments */
   for (i = 1; i < argc; i++) { 	
      if (! (strcmp (argv[i], "-f"  ))) { file =      argv[i+1] ; i++;}
      else if (! (strcmp (argv[i], "-u"  ))) { user =      argv[i+1] ; i++;}
      else if (! (strcmp (argv[i], "-c"  ))) { chr  = atoi(argv[i+1]); i++;}
      else if (! (strcmp (argv[i], "-pwl"))) { pwl  = atoi(argv[i+1]); i++;}
      else if (! (strcmp (argv[i], "-ui" ))) { ui   = atoi(argv[i+1]); i++;}
      else if (! (strcmp (argv[i], "-pws"))) { pws  = atoi(argv[i+1]); i++;}
      else if (! (strcmp (argv[i], "-lsf"))) { lsf  =      argv[i+1] ; i++;}
      else if (! (strcmp (argv[i], "-pf" ))) { pf   =      argv[i+1] ; i++;}
      else if (! (strcmp (argv[i], "-rf" ))) { rf   = atoi(argv[i+1]); i++;}
      else if (! (strcmp (argv[i], "-v"  ))) { continue; }
      else { printf("Unknown argument \"%s\": try viper -h\n", argv[i]); exit(-1); }
   }
	
   /* break early if calling from file */
   if (lsf) {
      if ( (fp_lsf = fopen(lsf, "r+")) == NULL ) {
         printf("Error: Can't open %s!\n", lsf);
         exit(-1);
      }
      fscanf (fp_lsf, "%s", vp_stat);
		
      /* check to see if run has been completed */
	
      if (! (strcmp (vp_stat, FIN_IDENT))) {
         printf("The saved run has been completed.\n");
         printf("Check %s for details.\n", lsf);
         fclose(fp_lsf);
         exit(-1);
      }
			
      /* continue otherwise */
      fscanf (fp_lsf, "%d", &lsf_out.ci_pws);
      fscanf (fp_lsf, "%d", &lsf_out.ci_pwl);
      fscanf (fp_lsf, "%s", lsf_out.ci_pass);
      fscanf (fp_lsf, "%s", lsf_out.ci_user);
      fscanf (fp_lsf, "%s", lsf_out.ci_dnum);
      fscanf (fp_lsf, "%s", lsf_out.ci_cset);
      fscanf (fp_lsf, "%s", lsf_out.ci_pf);
      fscanf (fp_lsf, "%d", &lsf_out.ci_ui);
      fclose(fp_lsf);
      lsf_out.ci_rf = 0;
      lsf_out.ci_vo = vo;
      printf("...loaded parameters from file %s.\n", lsf);
      crack(lsf_out);
   }
	
   /* check for required arguments */
   if (!file) {
      printf("Error: Password filename required!\n");
      exit(-1);
   }
   else if (!user) {
      printf("Error: Username required!\n");
      exit(-1);
   }
	
   /* attempt to load account from file */
   else if ( (fp_file = fopen(file, "r")) == NULL ) {
      printf("Error: Can't open %s!\n", file);
      exit(-1);
   }
		
   while ( (fscanf (fp_file, "%s", line) != EOF) ) {
      if (! (strcmp (user, strtok(line, ":"))) ) {
         linebuf = strtok(NULL, ":");
         if ( !linebuf ) {
            printf("No password for user %s!\n", user);
            exit(-1);
         }
         strcpy(pass, linebuf);
         if ( !pass || (strlen(pass)) <  4 ) {
            printf("Error: Bad password for user %s!\n", user);
            exit(-1);
         }
         printf("Found: user %s pw: %s\n", user, pass);
      }
   }
   fclose(fp_file);
   if ( !pass || (strlen(pass)) <  4 ) {
      printf("Error: No user %s in file %s!\n", user, file);
      exit(-1);
   }

   /* load character set */
   if ( (fp_cset = fopen(CHARSET_FILE, "r")) == NULL ) {
      printf("Error: Can't open %s!\n", CHARSET_FILE);
      exit(-1);
   }

   while ( (fscanf (fp_cset, "%s", line) != EOF) ) {
      if ( chr == (atoi(line)) ) {
         fscanf (fp_cset, "%s", lsf_out.ci_cset);
          break;
      }
   }
	
   if ( !lsf_out.ci_cset || (strlen(lsf_out.ci_cset)) < 2 ) {
      printf("Error: Bad charset %d in %s!\n", chr, CHARSET_FILE);
      exit(-1);
   }
   else {
      printf("Found: Charset %d in %s\n", chr, CHARSET_FILE);
   }
	
   fclose(fp_cset);
	
   /* write data in struct */
   lsf_out.ci_rf = rf;
   if (pf) { strcpy (lsf_out.ci_pf, pf); }
   lsf_out.ci_pws = pws;
   lsf_out.ci_pwl = pwl;
   strcpy (lsf_out.ci_pass, pass);
   strcpy (lsf_out.ci_user, user);
   lsf_out.ci_ui = ui;
   lsf_out.ci_vo = vo;
   printf("...command line parameters loaded.\n");
   crack(lsf_out);
}

/* ######## start subs ############### */

/* program help() function */
int help() {
   printf("\t-f <file>    File to load password from (required unless using lsf)\n");
   printf("\t-u <user>    Username to load from file (required unless using lsf)\n");
   printf("\t-lsf <file>  Load saved file from previous session\n");
   printf("\t-pf <file>   Save progress to file at update interval\n");
   printf("\t-rf #        Amount of time in hours to run for (default infinite)\n");
   printf("\t-c #         Character set from charset.ini to use (default 1)\n");
   printf("\t-pws #       Minimum password length (starting value, default 1)\n");
   printf("\t-pwl #       Maximum password length (default %d - maximum %d)\n", 
   	  DEFAULTPWLENGTH, MAXPASSWDLENGTH);
   printf("\t-ui #        Console update interval (in minutes - default 10)\n");
   printf("\t-v           Verbose output\n");
   exit(0);
}

/* The results function the_res() */
void the_res(struct crack_input erg, char * endpass, struct tm start) {
   static struct tm act_time;
   double end_time;
   int r;
   char convert_str[17];
	
   time(&read_time);
   act_time=*localtime(&read_time);
   end_time = difftime(mktime(&act_time), mktime(&start));
   convert(end_time, convert_str);
	
   sprintf(message[0], "%s\n", FIN_IDENT);
   sprintf(message[1], "\n");
	
   if (endpass[0] != '\0') {
      sprintf(message[2], " The password has been located.\n");
      sprintf(message[3], " Username : %s\n", erg.ci_user);
      sprintf(message[4], " Password : %s\n", endpass);
   }
   else {
      sprintf(message[2], " The password could not be located.\n");
      sprintf(message[3], " Username : %s\n", erg.ci_user);
      sprintf(message[4], " Password : %s\n", "** unknown **");
   }
	
   sprintf(message[5], " Started  : %s", asctime(&start));
   sprintf(message[6], " Finished : %s", asctime(&act_time));
   sprintf(message[7], " Duration : %s\n", time_done);
	
   for (r = 1; r <= 7; r++) { printf("%s", message[r]); }

   if(strlen(lsf_out.ci_pf)) {
      if ( (fp_pf = fopen(lsf_out.ci_pf, "w")) == NULL ) {
         printf("Error: Can't open %s!\n", lsf_out.ci_pf);
         exit(-1);
      }
      for (r = 0; r <=7; r++) { fprintf(fp_pf, message[r]); }
      fclose(fp_pf);
   }
	
   printf("\nViper exiting...\n");
   exit(0);
}
