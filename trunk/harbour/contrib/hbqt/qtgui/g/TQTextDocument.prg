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


FUNCTION QTextDocument( ... )
   RETURN HB_QTextDocument():new( ... )

FUNCTION QTextDocumentFromPointer( ... )
   RETURN HB_QTextDocument():fromPointer( ... )


CREATE CLASS QTextDocument INHERIT HbQtObjectHandler, HB_QObject FUNCTION HB_QTextDocument

   METHOD  new( ... )

   METHOD  addResource                   // ( nType, oQUrl, oQVariant )                        -> NIL
   METHOD  adjustSize                    // (  )                                               -> NIL
   METHOD  begin                         // (  )                                               -> oQTextBlock
   METHOD  blockCount                    // (  )                                               -> nInt
   METHOD  characterAt                   // ( nPos )                                           -> oQChar
   METHOD  characterCount                // (  )                                               -> nInt
   METHOD  clear                         // (  )                                               -> NIL
   METHOD  clone                         // ( oQObject )                                       -> oQTextDocument
   METHOD  defaultFont                   // (  )                                               -> oQFont
   METHOD  defaultStyleSheet             // (  )                                               -> cQString
   METHOD  defaultTextOption             // (  )                                               -> oQTextOption
   METHOD  documentLayout                // (  )                                               -> oQAbstractTextDocumentLayout
   METHOD  documentMargin                // (  )                                               -> nQreal
   METHOD  drawContents                  // ( oQPainter, oQRectF )                             -> NIL
   METHOD  end                           // (  )                                               -> oQTextBlock
   METHOD  find                          // ( cSubString, oQTextCursor, nOptions )             -> oQTextCursor
                                         // ( oQRegExp, oQTextCursor, nOptions )               -> oQTextCursor
                                         // ( cSubString, nPosition, nOptions )                -> oQTextCursor
                                         // ( oQRegExp, nPosition, nOptions )                  -> oQTextCursor
   METHOD  findBlock                     // ( nPos )                                           -> oQTextBlock
   METHOD  findBlockByLineNumber         // ( nLineNumber )                                    -> oQTextBlock
   METHOD  findBlockByNumber             // ( nBlockNumber )                                   -> oQTextBlock
   METHOD  firstBlock                    // (  )                                               -> oQTextBlock
   METHOD  idealWidth                    // (  )                                               -> nQreal
   METHOD  indentWidth                   // (  )                                               -> nQreal
   METHOD  isEmpty                       // (  )                                               -> lBool
   METHOD  isModified                    // (  )                                               -> lBool
   METHOD  isRedoAvailable               // (  )                                               -> lBool
   METHOD  isUndoAvailable               // (  )                                               -> lBool
   METHOD  isUndoRedoEnabled             // (  )                                               -> lBool
   METHOD  lastBlock                     // (  )                                               -> oQTextBlock
   METHOD  lineCount                     // (  )                                               -> nInt
   METHOD  markContentsDirty             // ( nPosition, nLength )                             -> NIL
   METHOD  maximumBlockCount             // (  )                                               -> nInt
   METHOD  metaInformation               // ( nInfo )                                          -> cQString
   METHOD  object                        // ( nObjectIndex )                                   -> oQTextObject
   METHOD  objectForFormat               // ( oQTextFormat )                                   -> oQTextObject
   METHOD  pageCount                     // (  )                                               -> nInt
   METHOD  pageSize                      // (  )                                               -> oQSizeF
   METHOD  print                         // ( oQPrinter )                                      -> NIL
   METHOD  redo                          // ( oQTextCursor )                                   -> NIL
   METHOD  resource                      // ( nType, oQUrl )                                   -> oQVariant
   METHOD  revision                      // (  )                                               -> nInt
   METHOD  rootFrame                     // (  )                                               -> oQTextFrame
   METHOD  setDefaultFont                // ( oQFont )                                         -> NIL
   METHOD  setDefaultStyleSheet          // ( cSheet )                                         -> NIL
   METHOD  setDefaultTextOption          // ( oQTextOption )                                   -> NIL
   METHOD  setDocumentLayout             // ( oQAbstractTextDocumentLayout )                   -> NIL
   METHOD  setDocumentMargin             // ( nMargin )                                        -> NIL
   METHOD  setHtml                       // ( cHtml )                                          -> NIL
   METHOD  setIndentWidth                // ( nWidth )                                         -> NIL
   METHOD  setMaximumBlockCount          // ( nMaximum )                                       -> NIL
   METHOD  setMetaInformation            // ( nInfo, cString )                                 -> NIL
   METHOD  setPageSize                   // ( oQSizeF )                                        -> NIL
   METHOD  setPlainText                  // ( cText )                                          -> NIL
   METHOD  setTextWidth                  // ( nWidth )                                         -> NIL
   METHOD  setUndoRedoEnabled            // ( lEnable )                                        -> NIL
   METHOD  setUseDesignMetrics           // ( lB )                                             -> NIL
   METHOD  size                          // (  )                                               -> oQSizeF
   METHOD  textWidth                     // (  )                                               -> nQreal
   METHOD  toHtml                        // ( oQByteArray )                                    -> cQString
   METHOD  toPlainText                   // (  )                                               -> cQString
   METHOD  undo                          // ( oQTextCursor )                                   -> NIL
   METHOD  useDesignMetrics              // (  )                                               -> lBool
                                         // (  )                                               -> NIL
   METHOD  setModified                   // ( lM )                                             -> NIL
                                         // (  )                                               -> NIL

   ENDCLASS


METHOD QTextDocument:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QTextDocument( ... )
   RETURN Self


METHOD QTextDocument:addResource( ... )
   SWITCH PCount()
   CASE 3
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) ) .AND. hb_isObject( hb_pvalue( 3 ) )
         RETURN Qt_QTextDocument_addResource( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:adjustSize( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_adjustSize( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:begin( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextBlockFromPointer( Qt_QTextDocument_begin( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:blockCount( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_blockCount( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:characterAt( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QCharFromPointer( Qt_QTextDocument_characterAt( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:characterCount( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_characterCount( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:clear( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_clear( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:clone( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QTextDocumentFromPointer( Qt_QTextDocument_clone( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 0
      RETURN QTextDocumentFromPointer( Qt_QTextDocument_clone( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:defaultFont( ... )
   SWITCH PCount()
   CASE 0
      RETURN QFontFromPointer( Qt_QTextDocument_defaultFont( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:defaultStyleSheet( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_defaultStyleSheet( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:defaultTextOption( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextOptionFromPointer( Qt_QTextDocument_defaultTextOption( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:documentLayout( ... )
   SWITCH PCount()
   CASE 0
      RETURN QAbstractTextDocumentLayoutFromPointer( Qt_QTextDocument_documentLayout( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:documentMargin( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_documentMargin( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:drawContents( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN Qt_QTextDocument_drawContents( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_drawContents( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:end( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextBlockFromPointer( Qt_QTextDocument_end( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:find( ... )
   SWITCH PCount()
   CASE 3
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find_2( ::pPtr, ... ) )
      CASE hb_isChar( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find( ::pPtr, ... ) )
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find_3( ::pPtr, ... ) )
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find_2( ::pPtr, ... ) )
      CASE hb_isChar( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find( ::pPtr, ... ) )
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find_3( ::pPtr, ... ) )
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find_2( ::pPtr, ... ) )
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QTextCursorFromPointer( Qt_QTextDocument_find_3( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:findBlock( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QTextBlockFromPointer( Qt_QTextDocument_findBlock( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:findBlockByLineNumber( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QTextBlockFromPointer( Qt_QTextDocument_findBlockByLineNumber( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:findBlockByNumber( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QTextBlockFromPointer( Qt_QTextDocument_findBlockByNumber( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:firstBlock( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextBlockFromPointer( Qt_QTextDocument_firstBlock( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:idealWidth( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_idealWidth( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:indentWidth( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_indentWidth( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:isEmpty( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_isEmpty( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:isModified( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_isModified( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:isRedoAvailable( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_isRedoAvailable( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:isUndoAvailable( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_isUndoAvailable( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:isUndoRedoEnabled( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_isUndoRedoEnabled( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:lastBlock( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextBlockFromPointer( Qt_QTextDocument_lastBlock( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:lineCount( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_lineCount( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:markContentsDirty( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QTextDocument_markContentsDirty( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:maximumBlockCount( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_maximumBlockCount( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:metaInformation( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_metaInformation( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:object( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QTextObjectFromPointer( Qt_QTextDocument_object( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:objectForFormat( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QTextObjectFromPointer( Qt_QTextDocument_objectForFormat( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:pageCount( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_pageCount( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:pageSize( ... )
   SWITCH PCount()
   CASE 0
      RETURN QSizeFFromPointer( Qt_QTextDocument_pageSize( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:print( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_print( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:redo( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_redo( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QTextDocument_redo_1( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:resource( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN QVariantFromPointer( Qt_QTextDocument_resource( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:revision( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_revision( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:rootFrame( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTextFrameFromPointer( Qt_QTextDocument_rootFrame( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setDefaultFont( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setDefaultFont( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setDefaultStyleSheet( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setDefaultStyleSheet( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setDefaultTextOption( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setDefaultTextOption( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setDocumentLayout( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setDocumentLayout( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setDocumentMargin( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setDocumentMargin( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setHtml( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setHtml( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setIndentWidth( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setIndentWidth( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setMaximumBlockCount( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setMaximumBlockCount( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setMetaInformation( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isChar( hb_pvalue( 2 ) )
         RETURN Qt_QTextDocument_setMetaInformation( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setPageSize( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setPageSize( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setPlainText( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setPlainText( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setTextWidth( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setTextWidth( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setUndoRedoEnabled( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setUndoRedoEnabled( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setUseDesignMetrics( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setUseDesignMetrics( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:size( ... )
   SWITCH PCount()
   CASE 0
      RETURN QSizeFFromPointer( Qt_QTextDocument_size( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:textWidth( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_textWidth( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:toHtml( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_toHtml( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QTextDocument_toHtml( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:toPlainText( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_toPlainText( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:undo( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_undo( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QTextDocument_undo_1( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:useDesignMetrics( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QTextDocument_useDesignMetrics( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QTextDocument:setModified( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QTextDocument_setModified( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QTextDocument_setModified( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()

