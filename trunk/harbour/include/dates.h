/*
 * $Id$
 */

#ifndef HB_DATES_H_
#define HB_DATES_H_

char *   hb_dtoc( const char * szDate, char * szFormattedDate, const char * szDateFormat );
long     hb_dateEncode( long lDay, long lMonth, long lYear );
void     hb_dateDecode( long julian, long * plDay, long * plMonth, long * plYear );

#endif /* HB_DATES_H_ */
