/*
 * $Id$
 */

#include <itemapi.h>
#include <extend.h>
#include <errorapi.h>
#include <ctoharb.h>
#include <filesys.h>

#ifdef OS_UNIX_COMPATIBLE
   #include <sys/stat.h>
   #include <unistd.h>
#endif

#define BUFFER_SIZE 8192

static long hb_fsCopy(BYTEP ,BYTEP ) ;

HARBOUR HB___COPYFILE( void )

{
   PHB_ITEM pError;
   if ( ISCHAR(1) && ISCHAR(2) )
      hb_retnl(hb_fsCopy((BYTEP)hb_parc(1),(BYTEP)hb_parc(2)));
   else
   {
      pError = hb_errNew();
      hb_errPutDescription(pError, "Error BASE/2010  Argument error: __COPYFILE" );
      hb_errLaunch(pError);
      hb_errRelease(pError);
   }
   return;
} 

static long hb_fsCopy(BYTEP source,BYTEP dest)

{
   FHANDLE  dHANDLE,sHANDLE;
   char     *buffer;
   USHORT   usCount;
   ULONG    ulCount = 0L;
   PHB_ITEM pError;
#ifdef OS_UNIX_COMPATIBLE   
   struct stat struFileInfo;
   int iSuccess;
#endif   
   sHANDLE = hb_fsOpen(source, FO_READ);
   if ( hb_fsError() )
   {
      pError = hb_errNew();
      hb_errPutDescription(pError, "Error BASE/2012  Open error: __COPYFILE" );
      hb_errLaunch(pError);
      hb_errRelease(pError);
      return( -1L) ;
   }
#ifdef OS_UNIX_COMPATIBLE
   iSuccess =fstat( sHANDLE, &struFileInfo );
#endif   
   dHANDLE = hb_fsCreate(dest,FC_NORMAL);
   if ( hb_fsError() )
   {
      hb_fsClose(sHANDLE);
      pError = hb_errNew();
      hb_errPutDescription(pError, "Error BASE/2012  Create error: __COPYFILE" );
      hb_errLaunch(pError);
      hb_errRelease(pError);
      return( -2L) ;
   }
   buffer  = (char *)hb_xgrab( BUFFER_SIZE );
   usCount = hb_fsRead(sHANDLE,buffer,BUFFER_SIZE);
   while( TRUE )
   {
      ulCount += (ULONG)usCount;
      hb_fsWrite(dHANDLE,buffer, usCount);
      usCount = hb_fsRead(sHANDLE,buffer,BUFFER_SIZE);
      if (usCount != BUFFER_SIZE )
      {
         break;
      }
   }
   ulCount += (ULONG)usCount;
   hb_fsWrite(dHANDLE,buffer,usCount);
   hb_xfree(buffer);
   hb_fsClose(sHANDLE);
#ifdef OS_UNIX_COMPATIBLE
   if( iSuccess == 0 )
      fchmod( dHANDLE, struFileInfo.st_mode );
#endif   
   hb_fsClose(dHANDLE);
   return( ulCount );
}
