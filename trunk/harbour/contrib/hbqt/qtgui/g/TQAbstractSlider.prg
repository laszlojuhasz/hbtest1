/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .qth file,                      */
/*          or the generator tool itself, and run regenarate.           */
/* -------------------------------------------------------------------- */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009-2010 Pritpal Bedi <bedipritpal@hotmail.com>
 * www - http://harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */
/*----------------------------------------------------------------------*/
/*                            C R E D I T S                             */
/*----------------------------------------------------------------------*/
/*
 * Marcos Antonio Gambeta
 *    for providing first ever prototype parsing methods. Though the current
 *    implementation is diametrically different then what he proposed, still
 *    current code shaped on those footsteps.
 *
 * Viktor Szakats
 *    for directing the project with futuristic vision;
 *    for designing and maintaining a complex build system for hbQT, hbIDE;
 *    for introducing many constructs on PRG and C++ levels;
 *    for streamlining signal/slots and events management classes;
 *
 * Istvan Bisz
 *    for introducing QPointer<> concept in the generator;
 *    for testing the library on numerous accounts;
 *    for showing a way how a GC pointer can be detached;
 *
 * Francesco Perillo
 *    for taking keen interest in hbQT development and peeking the code;
 *    for providing tips here and there to improve the code quality;
 *    for hitting bulls eye to describe why few objects need GC detachment;
 *
 * Carlos Bacco
 *    for implementing HBQT_TYPE_Q*Class enums;
 *    for peeking into the code and suggesting optimization points;
 *
 * Przemyslaw Czerpak
 *    for providing tips and trick to manipulate HVM internals to the best
 *    of its use and always showing a path when we get stuck;
 *    A true tradition of a MASTER...
*/
/*----------------------------------------------------------------------*/


#include "hbclass.ch"


FUNCTION QAbstractSlider( ... )
   RETURN HB_QAbstractSlider():new( ... )


CREATE CLASS QAbstractSlider INHERIT HbQtObjectHandler, HB_QWidget FUNCTION HB_QAbstractSlider

   METHOD  new( ... )

   METHOD  hasTracking()
   METHOD  invertedAppearance()
   METHOD  invertedControls()
   METHOD  isSliderDown()
   METHOD  maximum()
   METHOD  minimum()
   METHOD  orientation()
   METHOD  pageStep()
   METHOD  setInvertedAppearance( lBool )
   METHOD  setInvertedControls( lBool )
   METHOD  setMaximum( nInt )
   METHOD  setMinimum( nInt )
   METHOD  setPageStep( nInt )
   METHOD  setRange( nMin, nMax )
   METHOD  setSingleStep( nInt )
   METHOD  setSliderDown( lBool )
   METHOD  setSliderPosition( nInt )
   METHOD  setTracking( lEnable )
   METHOD  singleStep()
   METHOD  sliderPosition()
   METHOD  triggerAction( nAction )
   METHOD  value()
   METHOD  setOrientation( nQt_Orientation )
   METHOD  setValue( nInt )

   ENDCLASS


METHOD QAbstractSlider:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QAbstractSlider( ... )
   RETURN Self


METHOD QAbstractSlider:hasTracking()
   RETURN Qt_QAbstractSlider_hasTracking( ::pPtr )


METHOD QAbstractSlider:invertedAppearance()
   RETURN Qt_QAbstractSlider_invertedAppearance( ::pPtr )


METHOD QAbstractSlider:invertedControls()
   RETURN Qt_QAbstractSlider_invertedControls( ::pPtr )


METHOD QAbstractSlider:isSliderDown()
   RETURN Qt_QAbstractSlider_isSliderDown( ::pPtr )


METHOD QAbstractSlider:maximum()
   RETURN Qt_QAbstractSlider_maximum( ::pPtr )


METHOD QAbstractSlider:minimum()
   RETURN Qt_QAbstractSlider_minimum( ::pPtr )


METHOD QAbstractSlider:orientation()
   RETURN Qt_QAbstractSlider_orientation( ::pPtr )


METHOD QAbstractSlider:pageStep()
   RETURN Qt_QAbstractSlider_pageStep( ::pPtr )


METHOD QAbstractSlider:setInvertedAppearance( lBool )
   RETURN Qt_QAbstractSlider_setInvertedAppearance( ::pPtr, lBool )


METHOD QAbstractSlider:setInvertedControls( lBool )
   RETURN Qt_QAbstractSlider_setInvertedControls( ::pPtr, lBool )


METHOD QAbstractSlider:setMaximum( nInt )
   RETURN Qt_QAbstractSlider_setMaximum( ::pPtr, nInt )


METHOD QAbstractSlider:setMinimum( nInt )
   RETURN Qt_QAbstractSlider_setMinimum( ::pPtr, nInt )


METHOD QAbstractSlider:setPageStep( nInt )
   RETURN Qt_QAbstractSlider_setPageStep( ::pPtr, nInt )


METHOD QAbstractSlider:setRange( nMin, nMax )
   RETURN Qt_QAbstractSlider_setRange( ::pPtr, nMin, nMax )


METHOD QAbstractSlider:setSingleStep( nInt )
   RETURN Qt_QAbstractSlider_setSingleStep( ::pPtr, nInt )


METHOD QAbstractSlider:setSliderDown( lBool )
   RETURN Qt_QAbstractSlider_setSliderDown( ::pPtr, lBool )


METHOD QAbstractSlider:setSliderPosition( nInt )
   RETURN Qt_QAbstractSlider_setSliderPosition( ::pPtr, nInt )


METHOD QAbstractSlider:setTracking( lEnable )
   RETURN Qt_QAbstractSlider_setTracking( ::pPtr, lEnable )


METHOD QAbstractSlider:singleStep()
   RETURN Qt_QAbstractSlider_singleStep( ::pPtr )


METHOD QAbstractSlider:sliderPosition()
   RETURN Qt_QAbstractSlider_sliderPosition( ::pPtr )


METHOD QAbstractSlider:triggerAction( nAction )
   RETURN Qt_QAbstractSlider_triggerAction( ::pPtr, nAction )


METHOD QAbstractSlider:value()
   RETURN Qt_QAbstractSlider_value( ::pPtr )


METHOD QAbstractSlider:setOrientation( nQt_Orientation )
   RETURN Qt_QAbstractSlider_setOrientation( ::pPtr, nQt_Orientation )


METHOD QAbstractSlider:setValue( nInt )
   RETURN Qt_QAbstractSlider_setValue( ::pPtr, nInt )

