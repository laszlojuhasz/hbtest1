/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .qth file,                      */
/*          or the generator tool itself, and run regenarate.           */
/* -------------------------------------------------------------------- */

/*
 * Harbour Project QT wrapper
 *
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
 * www - http://harbour-project.org
 *
 * For full copyright message and credits, see: CREDITS.txt
 *
 */


#include "hbclass.ch"


REQUEST __HBQTGUI


FUNCTION QStyleHintReturnMask( ... )
   RETURN HB_QStyleHintReturnMask():new( ... )

FUNCTION QStyleHintReturnMaskFromPointer( ... )
   RETURN HB_QStyleHintReturnMask():fromPointer( ... )


CREATE CLASS QStyleHintReturnMask INHERIT HbQtObjectHandler, HB_QStyleHintReturn FUNCTION HB_QStyleHintReturnMask

   METHOD  new( ... )


   ENDCLASS


METHOD QStyleHintReturnMask:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QStyleHintReturnMask( ... )
   RETURN Self

