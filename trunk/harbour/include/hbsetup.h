/*
 * $Id$
 *
 * Harbour compiler and runtime configuration file
*/

#ifndef hbsetuph
#define hbsetuph

/* The name of starting procedure
 * Note: You have to define it in case when Harbour cannot find the proper
 * starting procedure (due to incorrect order of static data initialization)
 *
 * The list of compilers that require it:
 * - Watcom C/C++ 10.0
 *
 * By default we are using automatic lookup (symbol not defined)
*/
#define HARBOUR_START_PROCEDURE "MAIN"

/* This symbol defines if we want an ability to create and link OBJ files
 * generated by Harbour compiler
 *
 * By default it is disabled (symbol is not defined)
*/
/*#define HARBOUR_OBJ_GENERATION*/


/* Operating system specific definitions
 */
#define OS_PATH_DELIMITER        '\\'
#define OS_PATH_LIST_SEPARATOR   ';'

#endif
