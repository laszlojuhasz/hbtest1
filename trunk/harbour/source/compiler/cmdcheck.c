/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * Compiler command line and HARBOURCMD/CLIPPERCMD checking
 *
 * Copyright 1999 {list of individual authors and e-mail addresses}
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version, with one exception:
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA (or visit
 * their web site at http://www.gnu.org/).
 *
 */

/*
 * The following parts are Copyright of the individual authors.
 * www - http://www.harbour-project.org
 *
 * Copyright 2000 Ron Pinkas <Ron@Profit-Master.com>
 *    hb_compChkCompilerSwitch()
 *
 * Copyright 1999 Jose Lalin <dezac@corevia.com>
 *    hb_compChkEnvironVar()
 *
 * Copyright 1999-2001 Viktor Szakats <viktor.szakats@syenar.hu>
 *    PackDateTime()
 *    hb_compChkDefineSwitch()
 *    hb_compChkDefines()
 *
 * See doc/license.txt for licensing terms.
 *
 */

#include <time.h>

#include "hbcomp.h"

extern int hb_pp_ParseDefine( char * );

/* TODO: Add support for this compiler switches
   -r -t || hb_getenv( "TMP" )
*/


/* NOTE: Making the date and time info to fit into 32 bits can only be done
         in a "lossy" way, in practice that means it's not possible to unpack
         the exact date/time info from the resulting ULONG. Since the year
         is only stored in 6 bits, 1980 will result in the same bit pattern
         as 2044. The purpose of this value is only used to *differenciate*
         between the dates ( the exact dates are not significant ), so this
         can be used here without problems. [vszakats] */

/* 76543210765432107654321076543210
   |.......|.......|.......|.......
   |____|                               Year    6 bits
         |__|                           Month   4 bits
             |___|                      Day     5 bits
                  |___|                 Hour    5 bits
                       |____|           Minute  6 bits
                             |____|     Second  6 bits */

static ULONG PackDateTime( void )
{
   BYTE szString[4];
   BYTE nValue;

   time_t t;
   struct tm *oTime;

   time( &t );
   oTime = localtime( &t );

   nValue = ( BYTE ) ( ( ( oTime->tm_year + 1900 ) - 1980 ) & ( 2 ^ 6 ) );      /* 6 bits */
   szString[0] = nValue << 2;
   nValue = ( BYTE ) ( oTime->tm_mon + 1 );     /* 4 bits */
   szString[0] |= nValue >> 2;
   szString[1] = nValue << 6;
   nValue = ( BYTE ) ( oTime->tm_mday );        /* 5 bits */
   szString[1] |= nValue << 1;

   nValue = ( BYTE ) oTime->tm_hour;    /* 5 bits */
   szString[1] = nValue >> 4;
   szString[2] = nValue << 4;
   nValue = ( BYTE ) oTime->tm_min;     /* 6 bits */
   szString[2] |= nValue >> 2;
   szString[3] = nValue << 6;
   nValue = ( BYTE ) oTime->tm_sec;     /* 6 bits */
   szString[3] |= nValue;

   return HB_MKLONG( szString[3], szString[2], szString[1], szString[0] );
}

void hb_compChkCompilerSwitch( int iArg, char *Args[] )
{
   /* If iArg is passed check the command line options */
   if( iArg )
   {
      int i;

      /* Check all switches in command line
         They start with an OS_OPT_DELIMITER char
       */
      for( i = 0; i < iArg; i++ )
      {
         if( !HB_ISOPTSEP( Args[i][0] ) )
            continue;

         if( Args[i][0] == '-' )
         {
            int j = 1;
            char Switch[7];

            Switch[0] = '-';

            while( Args[i][j] )
            {
               Switch[1] = Args[i][j];

               if( Args[i][j + 1] == '-' )
               {
                  Switch[2] = '-';
                  Switch[3] = '\0';

                  hb_compChkEnvironVar( ( char * ) Switch );

                  j += 2;
                  continue;
               }
               else
               {
                  switch( Switch[1] )
                  {
                     case 'b':
                     case 'B':
                        if( Args[i][j + 1] && toupper( ( BYTE ) Args[i][j + 1] ) == 'U' &&
                            Args[i][j + 2] && toupper( ( BYTE ) Args[i][j + 2] ) == 'I' &&
                            Args[i][j + 3] && toupper( ( BYTE ) Args[i][j + 3] ) == 'L' &&
                            Args[i][j + 4] && toupper( ( BYTE ) Args[i][j + 4] ) == 'D' )
                        {
                           Switch[2] = 'U';
                           Switch[3] = 'I';
                           Switch[4] = 'L';
                           Switch[5] = 'D';
                           Switch[6] = '\0';

                           hb_compChkEnvironVar( ( char * ) Switch );

                           j += 5;
                           continue;
                        }
                        else if( !Args[i][j + 1] )
                        {
                           Switch[2] = '\0';
                           hb_compChkEnvironVar( ( char * ) Switch );
                           j += 1;
                           continue;
                        }
                        break;

                     case 'c':
                     case 'C':
                        if( Args[i][j + 1] && toupper( ( BYTE ) Args[i][j + 1] ) == 'R' &&
                            Args[i][j + 2] && toupper( ( BYTE ) Args[i][j + 2] ) == 'E' &&
                            Args[i][j + 3] && toupper( ( BYTE ) Args[i][j + 3] ) == 'D' )
                        {
                           Switch[2] = 'R';
                           Switch[3] = 'E';
                           Switch[4] = 'D';
                           Switch[5] = '\0';

                           j += 4;

                           if( Args[i][j] && toupper( ( BYTE ) Args[i][j] ) == 'I' )
                           {
                              j++;
                              if( Args[i][j] && toupper( ( BYTE ) Args[i][j] ) == 'T' )
                              {
                                 j++;
                                 if( Args[i][j] && toupper( ( BYTE ) Args[i][j] ) == 'S' )
                                 {
                                    j++;
                                 }
                              }
                           }

                           hb_compChkEnvironVar( ( char * ) Switch );

                           continue;
                        }
                        else
                        {
                           Switch[2] = '\0';
                           hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, ( char * ) Switch, NULL );
                        }

                     case 'd':
                     case 'D':
                        Args[i] += ( j - 1 );
                        hb_compChkEnvironVar( Args[i] );

                        /* Accept rest as part of #define and continue with next Args[]. */
                        j = strlen( Args[i] );
                        continue;

                     case 'e':
                     case 'E':
                        if( toupper( ( BYTE ) Args[i][j + 1] ) == 'S' && isdigit( ( BYTE ) Args[i][j + 2] ) )
                        {
                           Switch[2] = 'S';
                           Switch[3] = Args[i][j + 2];
                           Switch[4] = '\0';

                           hb_compChkEnvironVar( ( char * ) Switch );

                           j += 3;
                           continue;
                        }
                        else
                        {
                           Switch[2] = '\0';
                           hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, ( char * ) Switch, NULL );
                        }

                        break;

                     case 'g':
                     case 'G':
                        /* Required argument */
                        Switch[2] = Args[i][j + 1];
                        if( Switch[2] )
                        {
                           if( isdigit( ( BYTE ) Args[i][j + 2] ) )
                           {
                              /* Optional argument */
                              Switch[3] = Args[i][j + 2];
                              Switch[4] = '\0';
                              j += 3;
                           }
                           else
                           {
                              /* No optional argument */
                              Switch[3] = '\0';
                              j += 2;
                           }
                           hb_compChkEnvironVar( ( char * ) Switch );
                           continue;
                        }
                        else
                           hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, ( char * ) Switch, NULL );

                     case 'i':
                     case 'I':
                        Args[i] += ( j - 1 );
                        hb_compChkEnvironVar( Args[i] );

                        /* Accept rest as IncludePath and continue with next Args[]. */
                        j = strlen( Args[i] );
                        continue;

                     case 'k':
                     case 'K':
                        Args[i] += ( j - 1 );
                        hb_compChkEnvironVar( Args[i] );

                        /* Accept rest as part of #define and continue with next Args[]. */
                        j = strlen( Args[i] );
                        continue;

                     case 'n':
                     case 'N':
                        /* Required argument */
                        if( Args[i][j + 1] )
                        {
                           /* Optional argument */
                           Switch[2] = Args[i][j + 1];
                           Switch[3] = '\0';
                           j += 2;
                        }
                        else
                        {
                           /* No optional argument */
                           Switch[2] = '\0';
                           j += 1;
                        }
                        hb_compChkEnvironVar( ( char * ) Switch );
                        continue;

                     case 'o':
                     case 'O':
                        Args[i] += ( j - 1 );
                        hb_compChkEnvironVar( Args[i] );

                        /* Accept rest as OutputPath and continue with next Args[]. */
                        j = strlen( Args[i] );
                        continue;

                     case 'p':
                     case 'P':
                        if( Args[i][j + 1] )
                        {
                           Args[i] += ( j - 1 );
                           hb_compChkEnvironVar( Args[i] );

                           /* Accept rest as PPOPath and continue with next Args[]. */
                           j += strlen( Args[i] ) - 1;
                        }
                        else
                        {
                           Switch[2] = '\0';
                           hb_compChkEnvironVar( ( char * ) Switch );
                           j++;
                        }
                        continue;

                     case 'q':
                     case 'Q':
                        if( Args[i][j + 1] && isdigit( ( BYTE ) Args[i][j + 1] ) )
                        {
                           Switch[2] = Args[i][j + 1];
                           Switch[3] = '\0';

                           hb_compChkEnvironVar( ( char * ) Switch );

                           j += 2;
                           continue;
                        }
                        else
                        {
                           Switch[2] = '\0';
                           hb_compChkEnvironVar( ( char * ) Switch );
                        }

                        break;

                     case 'r':
                     case 'R':
                        hb_compChkEnvironVar( Args[i] );
                        j = strlen( Args[i] ) - 1;
                        break;

                     case 'u':
                     case 'U':
                        Args[i] += ( j - 1 );
                        hb_compChkEnvironVar( Args[i] );

                        /* Accept rest as part of .CH Path or "undef:<id>" and continue with next Args[]. */
                        j = strlen( Args[i] );
                        continue;

                     case 'w':
                     case 'W':
                        if( Args[i][j + 1] && isdigit( ( BYTE ) Args[i][j + 1] ) )
                        {
                           Switch[2] = Args[i][j + 1];
                           Switch[3] = '\0';

                           hb_compChkEnvironVar( ( char * ) Switch );

                           j += 2;
                           continue;
                        }
                        else
                        {
                           Switch[2] = '\0';
                           hb_compChkEnvironVar( ( char * ) Switch );
                        }

                        break;

                     case 'x':
                     case 'X':
                        Args[i] += ( j - 1 );
                        hb_compChkEnvironVar( Args[i] );

                        /* Accept rest as INIT Symbol and continue with next Args[]. */
                        j = strlen( Args[i] );
                        continue;

                     default:
                        Switch[2] = '\0';
                        hb_compChkEnvironVar( ( char * ) Switch );
                  }
               }

               j++;
            }

            continue;
         }

       CheckMultiSlashSwitch:
         {
            int j = 1;

            while( Args[i][j] && !HB_ISOPTSEP( Args[i][j] ) )
               j++;

            if( Args[i][j] && Args[i][j] == '/' )
            {
               char cSep = Args[i][j];

               Args[i][j] = 0;

               hb_compChkEnvironVar( Args[i] );

               Args[i] += j;
               Args[i][0] = cSep;

               goto CheckMultiSlashSwitch;
            }
            else
            {
               hb_compChkEnvironVar( Args[i] );
            }
         }
      }
   }
   else
      /* Chech the environment variables */
   {
      /* NOTE: CLIPPERCMD enviroment variable
         is overriden if HARBOURCMD exists
       */
      char *szStrEnv = hb_getenv( "HARBOURCMD" );

      if( !szStrEnv || szStrEnv[0] == '\0' )
      {
         if( szStrEnv )
            hb_xfree( ( void * ) szStrEnv );

         szStrEnv = hb_getenv( "CLIPPERCMD" );
      }

      if( szStrEnv && szStrEnv[0] != '\0' )
      {
         char *szSwitch = strtok( szStrEnv, " " );

         /* Check the environment var
            while it isn't empty.
          */
         while( szSwitch != NULL )
         {
            hb_compChkEnvironVar( szSwitch );
            szSwitch = strtok( NULL, " " );
         }
      }

      if( szStrEnv )
         hb_xfree( ( void * ) szStrEnv );
   }
}

void hb_compChkEnvironVar( char *szSwitch )
{
   if( szSwitch )
   {
      char *s = szSwitch;

      /* If szSwitch doesn't start with a HB_OSOPTSEP char
         show an error
       */

      /*
         printf( "Switch: %s\n", s );
       */

      if( !HB_ISOPTSEP( *s ) )
         hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, s, NULL );
      else
      {
         s++;
         switch( *s )
         {
            case 'a':
            case 'A':
               if( *( s + 1 ) == '-' )
                  hb_comp_bAutoMemvarAssume = FALSE;
               else
                  hb_comp_bAutoMemvarAssume = TRUE;
               break;

            case 'b':
            case 'B':
            {
               unsigned int i = 0;
               char *szOption = hb_strupr( hb_strdup( s ) );

               while( i < strlen( szOption ) && !HB_ISOPTSEP( szOption[i] ) )
                  i++;
               szOption[i] = '\0';

               if( strcmp( szOption, "BUILD" ) == 0 )
                  hb_comp_bBuildInfo = TRUE;
               else
               {
                  if( *( s + 1 ) == '-' )
                     hb_comp_bDebugInfo = FALSE;
                  else
                  {
                     hb_comp_bDebugInfo = TRUE;
                     hb_comp_bLineNumbers = TRUE;
                  }
               }

               hb_xfree( szOption );
            }
               break;

            case 'c':
            case 'C':
            {
               unsigned int i = 0;
               char *szOption = hb_strupr( hb_strdup( s ) );

               while( i < strlen( szOption ) && !HB_ISOPTSEP( szOption[i] ) )
                  i++;
               szOption[i] = '\0';

               if( strcmp( szOption, "CREDITS" ) == 0 ||
                   strcmp( szOption, "CREDIT" ) == 0 || strcmp( szOption, "CREDI" ) == 0 || strcmp( szOption, "CRED" ) == 0 )
                  hb_comp_bCredits = TRUE;
               else
                  hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, szOption, NULL );

               hb_xfree( szOption );
            }
               break;

            case 'd':
            case 'D':
               /* NOTE: Ignore these -d switches will be processed separately */
               break;

            case 'e':
            case 'E':
               if( *( s + 1 ) == 's' || *( s + 1 ) == 'S' )
               {
                  switch( *( s + 2 ) )
                  {
                     case '\0':
                     case '0':
                        hb_comp_iExitLevel = HB_EXITLEVEL_DEFAULT;
                        break;

                     case '1':
                        hb_comp_iExitLevel = HB_EXITLEVEL_SETEXIT;
                        break;

                     case '2':
                        hb_comp_iExitLevel = HB_EXITLEVEL_DELTARGET;
                        break;

                     default:
                        hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, s, NULL );
                  }
               }
               else
                  hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, s, NULL );

               break;

            case 'g':
            case 'G':
               switch( *( s + 1 ) )
               {
                  case 'c':
                  case 'C':
                     hb_comp_iLanguage = LANG_C;

                     switch( *( s + 2 ) )
                     {
                        case '3':
                           hb_comp_iGenCOutput = HB_COMPGENC_REALCODE;
                           break;

                        case '\0':
                        case '2':
                           hb_comp_iGenCOutput = HB_COMPGENC_VERBOSE;
                           break;

                        case '1':
                           hb_comp_iGenCOutput = HB_COMPGENC_NORMAL;
                           break;

                        case '0':
                           hb_comp_iGenCOutput = HB_COMPGENC_COMPACT;
                           break;

                        default:
                           hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, s, NULL );
                     }
                     break;

                  case 'j':
                  case 'J':
                     hb_comp_iLanguage = LANG_JAVA;
                     break;

                  case 'h':
                  case 'H':
                     hb_comp_iLanguage = LANG_PORT_OBJ;
                     break;

                  case 'i':
                  case 'I':
                     hb_comp_iLanguage = LANG_CLI;
                     break;

                  case 'o':
                  case 'O':
                     hb_comp_iLanguage = LANG_OBJ_MODULE;
                     break;

                  case 'w':
                  case 'W':
                     hb_comp_iLanguage = LANG_OBJ32;
                     break;

                  default:
                     printf( "\nUnsupported output language option\n" );
                     hb_compMainExit(  );
                     exit( EXIT_FAILURE );
               }
               break;

               /* NOTE:
                  h or H from HELP or help
                */
            case 'h':
            case 'H':
            case '?':
               break;

               /* NOTE:
                  It already has support for several include files
                */
            case 'i':
            case 'I':
               hb_fsAddSearchPath( s + 1, &hb_comp_pIncludePath );
               break;

            case 'k':
            case 'K':
            {
               int i = 1;

               while( s[i] )
               {
                  switch( s[i++] )
                  {
                     case '?':
                        hb_compPrintLogo(  );
                        hb_compPrintModes(  );
                        hb_comp_bLogo = FALSE;
                        hb_comp_bQuiet = TRUE;
                        break;

                     case 'h':
                        /* default Harbour mode */
                        hb_comp_Supported |= HB_COMPFLAG_HARBOUR;
                        break;

                     case 'c':
                        /* clear all flags - minimal set of features */
                        hb_comp_Supported = HB_COMPFLAG_OPTJUMP;
                        break;

                     case 'x':
                        hb_comp_Supported |= HB_COMPFLAG_XBASE;
                        break;

                     case 'i':
                        hb_comp_Supported |= HB_COMPFLAG_HB_INLINE;
                        break;

                     case 'J':
                        hb_comp_Supported &= ~HB_COMPFLAG_OPTJUMP;
                        break;

                     case 'r':
                        hb_comp_Supported |= HB_COMPFLAG_RT_MACRO;
                        break;

                     case 's':
                        hb_comp_Supported |= HB_COMPFLAG_ARRSTR;
                        break;

                     default:
                        hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, s, NULL );
                        break;
                  }
               }
            }
               break;

            case 'l':
            case 'L':
               if( *( s + 1 ) == '-' )
                  hb_comp_bLineNumbers = TRUE;
               else
                  hb_comp_bLineNumbers = FALSE;
               break;

            case 'm':
            case 'M':
               if( *( s + 1 ) == '-' )
                  hb_comp_bAutoOpen = TRUE;
               else
                  hb_comp_bAutoOpen = FALSE;
               break;

            case 'n':
            case 'N':
               /*
                  -n1 no start up procedure and no implicit start up procedure
                */
               if( *( s + 1 ) == '1' )
               {
                  hb_comp_bStartProc = FALSE;
                  hb_comp_bNoStartUp = TRUE;
               }
               /*
                  -n or -n0 no implicit start up procedure
                */
               else if( ( *( s + 1 ) == '0' ) || ( *( s + 1 ) == '\0' ) )
                  hb_comp_bStartProc = FALSE;
               /*
                  -n- ceates implicit start up procedure
                */
               else if( *( s + 1 ) == '-' )
                  hb_comp_bStartProc = TRUE;
               /*
                  invalid command
                */
               else
                  hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, s, NULL );
               break;

            case 'o':
            case 'O':
            {
               char *szPath = hb_strdup( s + 1 );

               hb_comp_pOutPath = hb_fsFNameSplit( szPath );
               hb_xfree( szPath );
            }
               break;

               /* Added for preprocessor needs */
            case 'p':
            case 'P':
               if( *( s + 1 ) == '-' )
                  hb_comp_bPPO = 0;
               else if( ! hb_comp_bPPO )
               {
                  /* do not set a path if option specified more then once */
                  char *szPath = hb_strdup( s + 1 );

                  hb_comp_pPpoPath = hb_fsFNameSplit( szPath );
                  hb_xfree( szPath );

                  hb_comp_bPPO = 1;
               }
               break;

            case 'q':
            case 'Q':
               if( *( s + 1 ) == '0' )
                  hb_comp_bLogo = FALSE;

               hb_comp_bQuiet = TRUE;
               break;

            case 'r':
            case 'R':
               if( *( s + 1 ) == '=' )
               {
                  int iOverflow;
                  int iCycles = hb_strValInt( s + 2, &iOverflow );

                  if( !iOverflow && iCycles > 0 )
                     hb_pp_MaxTranslateCycles = ( unsigned int ) iCycles;
               }
               else
               {
                  /* TODO: Implement this switch */
                  printf( "Not yet supported command line option: %s\n", s );
               }
               break;

            case 's':
            case 'S':
               if( *( s + 1 ) == '-' )
                  hb_comp_bSyntaxCheckOnly = FALSE;
               else
                  hb_comp_bSyntaxCheckOnly = TRUE;
               break;

            case 't':
            case 'T':
               /* TODO: Implement this switch */
               printf( "Not yet supported command line option: %s\n", s );
               break;

            case 'u':
            case 'U':
               if( s[1] && toupper( s[1] ) == 'N'
                   && s[2] && toupper( s[2] ) == 'D' && s[3] && toupper( s[3] ) == 'E' && s[4] && toupper( s[4] ) == 'F' && s[5] == ':' )
               {
                  /* NOTE: Ignore these -undef: switches will be processed separately */
                  break;
               }
               hb_pp_STD_CH = hb_strdup( s + 1 );
               break;

            case 'v':
            case 'V':
               if( *( s + 1 ) == '-' )
                  hb_comp_bForceMemvars = FALSE;
               else
                  hb_comp_bForceMemvars = TRUE;
               break;

            case 'w':
            case 'W':
               hb_comp_iWarnings = 1;
               if( s[1] )
               {                /*there is -w<0,1,2,3> probably */
                  hb_comp_iWarnings = s[1] - '0';
                  if( hb_comp_iWarnings < 0 || hb_comp_iWarnings > 4 )
                     hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, s, NULL );
               }
               break;

            case 'x':
            case 'X':
            {
               unsigned int i = 0;
               char *szPrefix = hb_strdup( s + 1 );

               while( i < strlen( szPrefix ) && !HB_ISOPTSEP( szPrefix[i] ) )
                  i++;
               szPrefix[i] = '\0';

               if( strlen( szPrefix ) == 0 )
                  sprintf( szPrefix, "%08lX_", PackDateTime(  ) );

               strncpy( hb_comp_szPrefix, szPrefix, 16 );
               hb_comp_szPrefix[16] = '\0';
               strcat( hb_comp_szPrefix, "_" );

               hb_xfree( szPrefix );
            }
               break;

#ifdef YYDEBUG
            case 'y':
            case 'Y':
               yydebug = TRUE;
               break;
#endif

            case 'z':
            case 'Z':
               if( *( s + 1 ) == '-' )
                  hb_comp_bShortCuts = TRUE;
               else
                  hb_comp_bShortCuts = FALSE;
               break;

            default:
               hb_compGenError( hb_comp_szErrors, 'F', HB_COMP_ERR_BADOPTION, s, NULL );
               break;
         }
      }
   }
}

void hb_compChkPaths( void )
{
   char *szInclude = hb_getenv( "INCLUDE" );

   if( szInclude )
   {
      if( szInclude[0] != '\0' )
         hb_fsAddSearchPath( szInclude, &hb_comp_pIncludePath );
      hb_xfree( ( void * ) szInclude );
   }
}

static void hb_compChkDefineSwitch( char *pszSwitch )
{
   if( pszSwitch && HB_ISOPTSEP( pszSwitch[0] ) )
   {
      if( pszSwitch[1] == 'd' || pszSwitch[1] == 'D' )
      {
         char *szDefText = hb_strdup( pszSwitch + 2 ), *pAssign, *sDefLine;
         unsigned int i = 0;

         while( i < strlen( szDefText ) && !HB_ISOPTSEP( szDefText[i] ) )
            i++;

         szDefText[i] = '\0';
         if( szDefText )
         {
            if( ( pAssign = strchr( szDefText, '=' ) ) == NULL )
            {
               hb_pp_AddDefine( szDefText, 0 );
            }
            else
            {
               szDefText[pAssign - szDefText] = '\0';

               /* hb_pp_AddDefine( szDefText,  pAssign + 1 ); */
               sDefLine = ( char * ) hb_xgrab( strlen( szDefText ) + 1 + strlen( pAssign + 1 ) + 1 );
               sprintf( sDefLine, "%s %s", szDefText, pAssign + 1 );
               hb_pp_ParseDefine( sDefLine );
               hb_xfree( sDefLine );
            }
         }

         hb_xfree( szDefText );
      }
      else if( pszSwitch[1] && toupper( pszSwitch[1] ) == 'U' &&
               pszSwitch[2] && toupper( pszSwitch[2] ) == 'N' &&
               pszSwitch[3] && toupper( pszSwitch[3] ) == 'D' &&
               pszSwitch[4] && toupper( pszSwitch[4] ) == 'E' &&
               pszSwitch[5] && toupper( pszSwitch[5] ) == 'F' &&
               pszSwitch[6] == ':' )
      {
         char *szDefText = hb_strdup( pszSwitch + 7 );
         char *szDefLine;
         unsigned int i = 0;

         while( szDefText[i] && !HB_ISOPTSEP( szDefText[i] ) )
         {
            i++;
         }

         szDefText[i] = '\0';

         if( szDefText[0] )
         {
            szDefLine = ( char * ) hb_xgrab( 7 + strlen( szDefText ) + 1 );
            sprintf( szDefLine, "#undef %s", szDefText );
            hb_pp_ParseDirective( szDefLine );
            hb_xfree( szDefLine );
         }
         hb_xfree( szDefText );
      }
   }
}

void hb_compChkDefines( int iArg, char *Args[] )
{
   /* Chech the environment variables */
   {
      /* NOTE: CLIPPERCMD enviroment variable is overriden
         if HARBOURCMD exists */
      char *szStrEnv = hb_getenv( "HARBOURCMD" );

      if( !szStrEnv || szStrEnv[0] == '\0' )
      {
         if( szStrEnv )
            hb_xfree( ( void * ) szStrEnv );

         szStrEnv = hb_getenv( "CLIPPERCMD" );
      }

      if( szStrEnv && szStrEnv[0] != '\0' )
      {
         char *szSwitch = strtok( szStrEnv, " " );

         /* Check the environment var while it isn't empty. */
         while( szSwitch != NULL )
         {
            hb_compChkDefineSwitch( szSwitch );
            szSwitch = strtok( NULL, " " );
         }
      }

      if( szStrEnv )
         hb_xfree( ( void * ) szStrEnv );
   }

   /* Check the command line options */
   {
      int i;

      /* Check all switches in command line They start with an OS_OPT_DELIMITER
         char */
      for( i = 0; i < iArg; i++ )
         hb_compChkDefineSwitch( Args[i] );
   }
}
