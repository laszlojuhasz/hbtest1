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


REQUEST __HBQTWEBKIT


FUNCTION QWebPage( ... )
   RETURN HB_QWebPage():new( ... )

FUNCTION QWebPageFromPointer( ... )
   RETURN HB_QWebPage():fromPointer( ... )


CREATE CLASS QWebPage INHERIT HbQtObjectHandler, HB_QObject FUNCTION HB_QWebPage

   METHOD  new( ... )

   METHOD  action                        // ( nAction )                                        -> oQAction
   METHOD  bytesReceived                 // (  )                                               -> nQuint64
   METHOD  createStandardContextMenu     // (  )                                               -> oQMenu
   METHOD  currentFrame                  // (  )                                               -> oQWebFrame
   METHOD  findText                      // ( cSubString, nOptions )                           -> lBool
   METHOD  focusNextPrevChild            // ( lNext )                                          -> lBool
   METHOD  forwardUnsupportedContent     // (  )                                               -> lBool
   METHOD  history                       // (  )                                               -> oQWebHistory
   METHOD  inputMethodQuery              // ( nProperty )                                      -> oQVariant
   METHOD  isContentEditable             // (  )                                               -> lBool
   METHOD  isModified                    // (  )                                               -> lBool
   METHOD  linkDelegationPolicy          // (  )                                               -> nLinkDelegationPolicy
   METHOD  mainFrame                     // (  )                                               -> oQWebFrame
   METHOD  palette                       // (  )                                               -> oQPalette
   METHOD  pluginFactory                 // (  )                                               -> oQWebPluginFactory
   METHOD  selectedText                  // (  )                                               -> cQString
   METHOD  setContentEditable            // ( lEditable )                                      -> NIL
   METHOD  setForwardUnsupportedContent  // ( lForward )                                       -> NIL
   METHOD  setLinkDelegationPolicy       // ( nPolicy )                                        -> NIL
   METHOD  setPalette                    // ( oQPalette )                                      -> NIL
   METHOD  setPluginFactory              // ( oQWebPluginFactory )                             -> NIL
   METHOD  setView                       // ( oQWidget )                                       -> NIL
   METHOD  setViewportSize               // ( oQSize )                                         -> NIL
   METHOD  settings                      // (  )                                               -> oQWebSettings
   METHOD  supportsExtension             // ( nExtension )                                     -> lBool
   METHOD  swallowContextMenuEvent       // ( oQContextMenuEvent )                             -> lBool
   METHOD  totalBytes                    // (  )                                               -> nQuint64
   METHOD  triggerAction                 // ( nAction, lChecked )                              -> NIL
   METHOD  updatePositionDependentActions // ( oQPoint )                                        -> NIL
   METHOD  view                          // (  )                                               -> oQWidget
   METHOD  viewportSize                  // (  )                                               -> oQSize

   ENDCLASS


METHOD QWebPage:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QWebPage( ... )
   RETURN Self


METHOD QWebPage:action( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QActionFromPointer( Qt_QWebPage_action( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:bytesReceived( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QWebPage_bytesReceived( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:createStandardContextMenu( ... )
   SWITCH PCount()
   CASE 0
      RETURN QMenuFromPointer( Qt_QWebPage_createStandardContextMenu( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:currentFrame( ... )
   SWITCH PCount()
   CASE 0
      RETURN QWebFrameFromPointer( Qt_QWebPage_currentFrame( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:findText( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QWebPage_findText( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_findText( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:focusNextPrevChild( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_focusNextPrevChild( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:forwardUnsupportedContent( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QWebPage_forwardUnsupportedContent( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:history( ... )
   SWITCH PCount()
   CASE 0
      RETURN QWebHistoryFromPointer( Qt_QWebPage_history( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:inputMethodQuery( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QVariantFromPointer( Qt_QWebPage_inputMethodQuery( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:isContentEditable( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QWebPage_isContentEditable( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:isModified( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QWebPage_isModified( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:linkDelegationPolicy( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QWebPage_linkDelegationPolicy( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:mainFrame( ... )
   SWITCH PCount()
   CASE 0
      RETURN QWebFrameFromPointer( Qt_QWebPage_mainFrame( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:palette( ... )
   SWITCH PCount()
   CASE 0
      RETURN QPaletteFromPointer( Qt_QWebPage_palette( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:pluginFactory( ... )
   SWITCH PCount()
   CASE 0
      RETURN QWebPluginFactoryFromPointer( Qt_QWebPage_pluginFactory( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:selectedText( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QWebPage_selectedText( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:setContentEditable( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_setContentEditable( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:setForwardUnsupportedContent( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_setForwardUnsupportedContent( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:setLinkDelegationPolicy( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_setLinkDelegationPolicy( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:setPalette( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_setPalette( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:setPluginFactory( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_setPluginFactory( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:setView( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_setView( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:setViewportSize( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_setViewportSize( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:settings( ... )
   SWITCH PCount()
   CASE 0
      RETURN QWebSettingsFromPointer( Qt_QWebPage_settings( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:supportsExtension( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_supportsExtension( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:swallowContextMenuEvent( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_swallowContextMenuEvent( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:totalBytes( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QWebPage_totalBytes( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:triggerAction( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isLogical( hb_pvalue( 2 ) )
         RETURN Qt_QWebPage_triggerAction( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_triggerAction( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:updatePositionDependentActions( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QWebPage_updatePositionDependentActions( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:view( ... )
   SWITCH PCount()
   CASE 0
      RETURN QWidgetFromPointer( Qt_QWebPage_view( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QWebPage:viewportSize( ... )
   SWITCH PCount()
   CASE 0
      RETURN QSizeFromPointer( Qt_QWebPage_viewportSize( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()

