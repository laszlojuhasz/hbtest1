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


FUNCTION QGraphicsItem( ... )
   RETURN HB_QGraphicsItem():new( ... )

FUNCTION QGraphicsItemFromPointer( ... )
   RETURN HB_QGraphicsItem():fromPointer( ... )


CREATE CLASS QGraphicsItem INHERIT HbQtObjectHandler FUNCTION HB_QGraphicsItem

   METHOD  new( ... )

   METHOD  acceptDrops                   // (  )                                               -> lBool
   METHOD  acceptHoverEvents             // (  )                                               -> lBool
   METHOD  acceptedMouseButtons          // (  )                                               -> nQt_MouseButtons
   METHOD  advance                       // ( nPhase )                                         -> NIL
   METHOD  boundingRect                  // (  )                                               -> oQRectF
   METHOD  boundingRegion                // ( oQTransform )                                    -> oQRegion
   METHOD  boundingRegionGranularity     // (  )                                               -> nQreal
   METHOD  cacheMode                     // (  )                                               -> nCacheMode
   METHOD  childItems                    // (  )                                               -> oQList_QGraphicsItem
   METHOD  childrenBoundingRect          // (  )                                               -> oQRectF
   METHOD  clearFocus                    // (  )                                               -> NIL
   METHOD  clipPath                      // (  )                                               -> oQPainterPath
   METHOD  collidesWithItem              // ( oQGraphicsItem, nMode )                          -> lBool
   METHOD  collidesWithPath              // ( oQPainterPath, nMode )                           -> lBool
   METHOD  collidingItems                // ( nMode )                                          -> oQList_QGraphicsItem
   METHOD  commonAncestorItem            // ( oQGraphicsItem )                                 -> oQGraphicsItem
   METHOD  contains                      // ( oQPointF )                                       -> lBool
   METHOD  cursor                        // (  )                                               -> oQCursor
   METHOD  data                          // ( nKey )                                           -> oQVariant
   METHOD  deviceTransform               // ( oQTransform )                                    -> oQTransform
   METHOD  effectiveOpacity              // (  )                                               -> nQreal
   METHOD  ensureVisible                 // ( oQRectF, nXmargin, nYmargin )                    -> NIL
                                         // ( nX, nY, nW, nH, nXmargin, nYmargin )             -> NIL
   METHOD  flags                         // (  )                                               -> nGraphicsItemFlags
   METHOD  grabKeyboard                  // (  )                                               -> NIL
   METHOD  grabMouse                     // (  )                                               -> NIL
   METHOD  group                         // (  )                                               -> oQGraphicsItemGroup
   METHOD  handlesChildEvents            // (  )                                               -> lBool
   METHOD  hasCursor                     // (  )                                               -> lBool
   METHOD  hasFocus                      // (  )                                               -> lBool
   METHOD  hide                          // (  )                                               -> NIL
   METHOD  installSceneEventFilter       // ( oQGraphicsItem )                                 -> NIL
   METHOD  isAncestorOf                  // ( oQGraphicsItem )                                 -> lBool
   METHOD  isClipped                     // (  )                                               -> lBool
   METHOD  isEnabled                     // (  )                                               -> lBool
   METHOD  isObscured                    // (  )                                               -> lBool
                                         // ( nX, nY, nW, nH )                                 -> lBool
                                         // ( oQRectF )                                        -> lBool
   METHOD  isObscuredBy                  // ( oQGraphicsItem )                                 -> lBool
   METHOD  isSelected                    // (  )                                               -> lBool
   METHOD  isUnderMouse                  // (  )                                               -> lBool
   METHOD  isVisible                     // (  )                                               -> lBool
   METHOD  isVisibleTo                   // ( oQGraphicsItem )                                 -> lBool
   METHOD  isWidget                      // (  )                                               -> lBool
   METHOD  isWindow                      // (  )                                               -> lBool
   METHOD  itemTransform                 // ( oQGraphicsItem, @lOk )                           -> oQTransform
   METHOD  mapFromItem                   // ( oQGraphicsItem, oQPointF )                       -> oQPointF
                                         // ( oQGraphicsItem, oQRectF )                        -> oQPolygonF
                                         // ( oQGraphicsItem, oQPolygonF )                     -> oQPolygonF
                                         // ( oQGraphicsItem, oQPainterPath )                  -> oQPainterPath
                                         // ( oQGraphicsItem, nX, nY, nW, nH )                 -> oQPolygonF
                                         // ( oQGraphicsItem, nX, nY )                         -> oQPointF
   METHOD  mapFromParent                 // ( oQPointF )                                       -> oQPointF
                                         // ( oQRectF )                                        -> oQPolygonF
                                         // ( oQPolygonF )                                     -> oQPolygonF
                                         // ( oQPainterPath )                                  -> oQPainterPath
                                         // ( nX, nY, nW, nH )                                 -> oQPolygonF
                                         // ( nX, nY )                                         -> oQPointF
   METHOD  mapFromScene                  // ( oQPointF )                                       -> oQPointF
                                         // ( oQRectF )                                        -> oQPolygonF
                                         // ( oQPolygonF )                                     -> oQPolygonF
                                         // ( oQPainterPath )                                  -> oQPainterPath
                                         // ( nX, nY, nW, nH )                                 -> oQPolygonF
                                         // ( nX, nY )                                         -> oQPointF
   METHOD  mapRectFromItem               // ( oQGraphicsItem, oQRectF )                        -> oQRectF
                                         // ( oQGraphicsItem, nX, nY, nW, nH )                 -> oQRectF
   METHOD  mapRectFromParent             // ( oQRectF )                                        -> oQRectF
                                         // ( nX, nY, nW, nH )                                 -> oQRectF
   METHOD  mapRectFromScene              // ( oQRectF )                                        -> oQRectF
                                         // ( nX, nY, nW, nH )                                 -> oQRectF
   METHOD  mapRectToItem                 // ( oQGraphicsItem, oQRectF )                        -> oQRectF
                                         // ( oQGraphicsItem, nX, nY, nW, nH )                 -> oQRectF
   METHOD  mapRectToParent               // ( oQRectF )                                        -> oQRectF
                                         // ( nX, nY, nW, nH )                                 -> oQRectF
   METHOD  mapRectToScene                // ( oQRectF )                                        -> oQRectF
                                         // ( nX, nY, nW, nH )                                 -> oQRectF
   METHOD  mapToItem                     // ( oQGraphicsItem, oQPointF )                       -> oQPointF
                                         // ( oQGraphicsItem, oQRectF )                        -> oQPolygonF
                                         // ( oQGraphicsItem, oQPolygonF )                     -> oQPolygonF
                                         // ( oQGraphicsItem, oQPainterPath )                  -> oQPainterPath
                                         // ( oQGraphicsItem, nX, nY, nW, nH )                 -> oQPolygonF
                                         // ( oQGraphicsItem, nX, nY )                         -> oQPointF
   METHOD  mapToParent                   // ( oQPointF )                                       -> oQPointF
                                         // ( oQRectF )                                        -> oQPolygonF
                                         // ( oQPolygonF )                                     -> oQPolygonF
                                         // ( oQPainterPath )                                  -> oQPainterPath
                                         // ( nX, nY, nW, nH )                                 -> oQPolygonF
                                         // ( nX, nY )                                         -> oQPointF
   METHOD  mapToScene                    // ( oQPointF )                                       -> oQPointF
                                         // ( oQRectF )                                        -> oQPolygonF
                                         // ( oQPolygonF )                                     -> oQPolygonF
                                         // ( oQPainterPath )                                  -> oQPainterPath
                                         // ( nX, nY, nW, nH )                                 -> oQPolygonF
                                         // ( nX, nY )                                         -> oQPointF
   METHOD  moveBy                        // ( nDx, nDy )                                       -> NIL
   METHOD  opacity                       // (  )                                               -> nQreal
   METHOD  opaqueArea                    // (  )                                               -> oQPainterPath
   METHOD  paint                         // ( oQPainter, oQStyleOptionGraphicsItem, oQWidget ) -> NIL
   METHOD  parentItem                    // (  )                                               -> oQGraphicsItem
   METHOD  parentWidget                  // (  )                                               -> oQGraphicsWidget
   METHOD  pos                           // (  )                                               -> oQPointF
   METHOD  removeSceneEventFilter        // ( oQGraphicsItem )                                 -> NIL
   METHOD  resetTransform                // (  )                                               -> NIL
   METHOD  rotate                        // ( nAngle )                                         -> NIL
   METHOD  scale                         // ( nSx, nSy )                                       -> NIL
   METHOD  scene                         // (  )                                               -> oQGraphicsScene
   METHOD  sceneBoundingRect             // (  )                                               -> oQRectF
   METHOD  scenePos                      // (  )                                               -> oQPointF
   METHOD  sceneTransform                // (  )                                               -> oQTransform
   METHOD  scroll                        // ( nDx, nDy, oQRectF )                              -> NIL
   METHOD  setAcceptDrops                // ( lOn )                                            -> NIL
   METHOD  setAcceptHoverEvents          // ( lEnabled )                                       -> NIL
   METHOD  setAcceptedMouseButtons       // ( nButtons )                                       -> NIL
   METHOD  setBoundingRegionGranularity  // ( nGranularity )                                   -> NIL
   METHOD  setCacheMode                  // ( nMode, oQSize )                                  -> NIL
   METHOD  setCursor                     // ( oQCursor )                                       -> NIL
   METHOD  setData                       // ( nKey, oQVariant )                                -> NIL
   METHOD  setEnabled                    // ( lEnabled )                                       -> NIL
   METHOD  setFlag                       // ( nFlag, lEnabled )                                -> NIL
   METHOD  setFlags                      // ( nFlags )                                         -> NIL
   METHOD  setFocus                      // ( nFocusReason )                                   -> NIL
   METHOD  setGroup                      // ( oQGraphicsItemGroup )                            -> NIL
   METHOD  setHandlesChildEvents         // ( lEnabled )                                       -> NIL
   METHOD  setOpacity                    // ( nOpacity )                                       -> NIL
   METHOD  setParentItem                 // ( oQGraphicsItem )                                 -> NIL
   METHOD  setPos                        // ( oQPointF )                                       -> NIL
                                         // ( nX, nY )                                         -> NIL
   METHOD  setSelected                   // ( lSelected )                                      -> NIL
   METHOD  setToolTip                    // ( cToolTip )                                       -> NIL
   METHOD  setTransform                  // ( oQTransform, lCombine )                          -> NIL
   METHOD  setVisible                    // ( lVisible )                                       -> NIL
   METHOD  setZValue                     // ( nZ )                                             -> NIL
   METHOD  shape                         // (  )                                               -> oQPainterPath
   METHOD  shear                         // ( nSh, nSv )                                       -> NIL
   METHOD  show                          // (  )                                               -> NIL
   METHOD  toolTip                       // (  )                                               -> cQString
   METHOD  topLevelItem                  // (  )                                               -> oQGraphicsItem
   METHOD  topLevelWidget                // (  )                                               -> oQGraphicsWidget
   METHOD  transform                     // (  )                                               -> oQTransform
   METHOD  translate                     // ( nDx, nDy )                                       -> NIL
   METHOD  type                          // (  )                                               -> nInt
   METHOD  ungrabKeyboard                // (  )                                               -> NIL
   METHOD  ungrabMouse                   // (  )                                               -> NIL
   METHOD  unsetCursor                   // (  )                                               -> NIL
   METHOD  update                        // ( oQRectF )                                        -> NIL
                                         // ( nX, nY, nWidth, nHeight )                        -> NIL
   METHOD  window                        // (  )                                               -> oQGraphicsWidget
   METHOD  x                             // (  )                                               -> nQreal
   METHOD  y                             // (  )                                               -> nQreal
   METHOD  zValue                        // (  )                                               -> nQreal

   ENDCLASS


METHOD QGraphicsItem:new( ... )
   LOCAL p
   FOR EACH p IN { ... }
      hb_pvalue( p:__enumIndex(), __hbqt_ptr( p ) )
   NEXT
   ::pPtr := Qt_QGraphicsItem( ... )
   RETURN Self


METHOD QGraphicsItem:acceptDrops( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_acceptDrops( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:acceptHoverEvents( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_acceptHoverEvents( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:acceptedMouseButtons( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_acceptedMouseButtons( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:advance( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_advance( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:boundingRect( ... )
   SWITCH PCount()
   CASE 0
      RETURN QRectFFromPointer( Qt_QGraphicsItem_boundingRect( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:boundingRegion( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QRegionFromPointer( Qt_QGraphicsItem_boundingRegion( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:boundingRegionGranularity( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_boundingRegionGranularity( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:cacheMode( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_cacheMode( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:childItems( ... )
   SWITCH PCount()
   CASE 0
      RETURN QListFromPointer( Qt_QGraphicsItem_childItems( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:childrenBoundingRect( ... )
   SWITCH PCount()
   CASE 0
      RETURN QRectFFromPointer( Qt_QGraphicsItem_childrenBoundingRect( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:clearFocus( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_clearFocus( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:clipPath( ... )
   SWITCH PCount()
   CASE 0
      RETURN QPainterPathFromPointer( Qt_QGraphicsItem_clipPath( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:collidesWithItem( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_collidesWithItem( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_collidesWithItem( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:collidesWithPath( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_collidesWithPath( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_collidesWithPath( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:collidingItems( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QListFromPointer( Qt_QGraphicsItem_collidingItems( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 0
      RETURN QListFromPointer( Qt_QGraphicsItem_collidingItems( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:commonAncestorItem( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QGraphicsItemFromPointer( Qt_QGraphicsItem_commonAncestorItem( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:contains( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_contains( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:cursor( ... )
   SWITCH PCount()
   CASE 0
      RETURN QCursorFromPointer( Qt_QGraphicsItem_cursor( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:data( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN QVariantFromPointer( Qt_QGraphicsItem_data( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:deviceTransform( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QTransformFromPointer( Qt_QGraphicsItem_deviceTransform( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:effectiveOpacity( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_effectiveOpacity( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:ensureVisible( ... )
   SWITCH PCount()
   CASE 6
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) ) .AND. hb_isNumeric( hb_pvalue( 5 ) ) .AND. hb_isNumeric( hb_pvalue( 6 ) )
         RETURN Qt_QGraphicsItem_ensureVisible_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 5
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) ) .AND. hb_isNumeric( hb_pvalue( 5 ) )
         RETURN Qt_QGraphicsItem_ensureVisible_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN Qt_QGraphicsItem_ensureVisible_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 3
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN Qt_QGraphicsItem_ensureVisible( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_ensureVisible( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_ensureVisible( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QGraphicsItem_ensureVisible( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:flags( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_flags( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:grabKeyboard( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_grabKeyboard( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:grabMouse( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_grabMouse( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:group( ... )
   SWITCH PCount()
   CASE 0
      RETURN QGraphicsItemGroupFromPointer( Qt_QGraphicsItem_group( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:handlesChildEvents( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_handlesChildEvents( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:hasCursor( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_hasCursor( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:hasFocus( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_hasFocus( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:hide( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_hide( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:installSceneEventFilter( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_installSceneEventFilter( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isAncestorOf( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_isAncestorOf( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isClipped( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_isClipped( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isEnabled( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_isEnabled( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isObscured( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN Qt_QGraphicsItem_isObscured_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_isObscured_2( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QGraphicsItem_isObscured( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isObscuredBy( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_isObscuredBy( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isSelected( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_isSelected( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isUnderMouse( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_isUnderMouse( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isVisible( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_isVisible( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isVisibleTo( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_isVisibleTo( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isWidget( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_isWidget( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:isWindow( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_isWindow( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:itemTransform( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isLogical( hb_pvalue( 2 ) )
         RETURN QTransformFromPointer( Qt_QGraphicsItem_itemTransform( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QTransformFromPointer( Qt_QGraphicsItem_itemTransform( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapFromItem( ... )
   SWITCH PCount()
   CASE 5
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) ) .AND. hb_isNumeric( hb_pvalue( 5 ) )
         RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromItem_4( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 3
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN QPointFFromPointer( Qt_QGraphicsItem_mapFromItem_5( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) ) + __objGetClsName( hb_pvalue( 2 ) )
         CASE "QGRAPHICSITEMQPAINTERPATH"
            RETURN QPainterPathFromPointer( Qt_QGraphicsItem_mapFromItem_3( ::pPtr, ... ) )
         CASE "QGRAPHICSITEMQPOINTF"
            RETURN QPointFFromPointer( Qt_QGraphicsItem_mapFromItem( ::pPtr, ... ) )
         CASE "QGRAPHICSITEMQRECTF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromItem_1( ::pPtr, ... ) )
         CASE "QGRAPHICSITEMQPOLYGONF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromItem_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapFromParent( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromParent_4( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN QPointFFromPointer( Qt_QGraphicsItem_mapFromParent_5( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QPAINTERPATH"
            RETURN QPainterPathFromPointer( Qt_QGraphicsItem_mapFromParent_3( ::pPtr, ... ) )
         CASE "QPOINTF"
            RETURN QPointFFromPointer( Qt_QGraphicsItem_mapFromParent( ::pPtr, ... ) )
         CASE "QRECTF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromParent_1( ::pPtr, ... ) )
         CASE "QPOLYGONF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromParent_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapFromScene( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromScene_4( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN QPointFFromPointer( Qt_QGraphicsItem_mapFromScene_5( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QPAINTERPATH"
            RETURN QPainterPathFromPointer( Qt_QGraphicsItem_mapFromScene_3( ::pPtr, ... ) )
         CASE "QPOINTF"
            RETURN QPointFFromPointer( Qt_QGraphicsItem_mapFromScene( ::pPtr, ... ) )
         CASE "QRECTF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromScene_1( ::pPtr, ... ) )
         CASE "QPOLYGONF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapFromScene_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapRectFromItem( ... )
   SWITCH PCount()
   CASE 5
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) ) .AND. hb_isNumeric( hb_pvalue( 5 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectFromItem_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectFromItem( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapRectFromParent( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectFromParent_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectFromParent( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapRectFromScene( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectFromScene_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectFromScene( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapRectToItem( ... )
   SWITCH PCount()
   CASE 5
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) ) .AND. hb_isNumeric( hb_pvalue( 5 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectToItem_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectToItem( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapRectToParent( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectToParent_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectToParent( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapRectToScene( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectToScene_1( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN QRectFFromPointer( Qt_QGraphicsItem_mapRectToScene( ::pPtr, ... ) )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapToItem( ... )
   SWITCH PCount()
   CASE 5
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) ) .AND. hb_isNumeric( hb_pvalue( 5 ) )
         RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToItem_4( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 3
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) )
         RETURN QPointFFromPointer( Qt_QGraphicsItem_mapToItem_5( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) ) + __objGetClsName( hb_pvalue( 2 ) )
         CASE "QGRAPHICSITEMQPAINTERPATH"
            RETURN QPainterPathFromPointer( Qt_QGraphicsItem_mapToItem_3( ::pPtr, ... ) )
         CASE "QGRAPHICSITEMQPOINTF"
            RETURN QPointFFromPointer( Qt_QGraphicsItem_mapToItem( ::pPtr, ... ) )
         CASE "QGRAPHICSITEMQRECTF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToItem_1( ::pPtr, ... ) )
         CASE "QGRAPHICSITEMQPOLYGONF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToItem_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapToParent( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToParent_4( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN QPointFFromPointer( Qt_QGraphicsItem_mapToParent_5( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QPAINTERPATH"
            RETURN QPainterPathFromPointer( Qt_QGraphicsItem_mapToParent_3( ::pPtr, ... ) )
         CASE "QPOINTF"
            RETURN QPointFFromPointer( Qt_QGraphicsItem_mapToParent( ::pPtr, ... ) )
         CASE "QRECTF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToParent_1( ::pPtr, ... ) )
         CASE "QPOLYGONF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToParent_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:mapToScene( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToScene_4( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN QPointFFromPointer( Qt_QGraphicsItem_mapToScene_5( ::pPtr, ... ) )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         SWITCH __objGetClsName( hb_pvalue( 1 ) )
         CASE "QPAINTERPATH"
            RETURN QPainterPathFromPointer( Qt_QGraphicsItem_mapToScene_3( ::pPtr, ... ) )
         CASE "QPOINTF"
            RETURN QPointFFromPointer( Qt_QGraphicsItem_mapToScene( ::pPtr, ... ) )
         CASE "QRECTF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToScene_1( ::pPtr, ... ) )
         CASE "QPOLYGONF"
            RETURN QPolygonFFromPointer( Qt_QGraphicsItem_mapToScene_2( ::pPtr, ... ) )
         ENDSWITCH
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:moveBy( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_moveBy( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:opacity( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_opacity( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:opaqueArea( ... )
   SWITCH PCount()
   CASE 0
      RETURN QPainterPathFromPointer( Qt_QGraphicsItem_opaqueArea( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:paint( ... )
   SWITCH PCount()
   CASE 3
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) ) .AND. hb_isObject( hb_pvalue( 3 ) )
         RETURN Qt_QGraphicsItem_paint( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_paint( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:parentItem( ... )
   SWITCH PCount()
   CASE 0
      RETURN QGraphicsItemFromPointer( Qt_QGraphicsItem_parentItem( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:parentWidget( ... )
   SWITCH PCount()
   CASE 0
      RETURN QGraphicsWidgetFromPointer( Qt_QGraphicsItem_parentWidget( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:pos( ... )
   SWITCH PCount()
   CASE 0
      RETURN QPointFFromPointer( Qt_QGraphicsItem_pos( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:removeSceneEventFilter( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_removeSceneEventFilter( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:resetTransform( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_resetTransform( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:rotate( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_rotate( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:scale( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_scale( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:scene( ... )
   SWITCH PCount()
   CASE 0
      RETURN QGraphicsSceneFromPointer( Qt_QGraphicsItem_scene( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:sceneBoundingRect( ... )
   SWITCH PCount()
   CASE 0
      RETURN QRectFFromPointer( Qt_QGraphicsItem_sceneBoundingRect( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:scenePos( ... )
   SWITCH PCount()
   CASE 0
      RETURN QPointFFromPointer( Qt_QGraphicsItem_scenePos( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:sceneTransform( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTransformFromPointer( Qt_QGraphicsItem_sceneTransform( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:scroll( ... )
   SWITCH PCount()
   CASE 3
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isObject( hb_pvalue( 3 ) )
         RETURN Qt_QGraphicsItem_scroll( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_scroll( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setAcceptDrops( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setAcceptDrops( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setAcceptHoverEvents( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setAcceptHoverEvents( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setAcceptedMouseButtons( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setAcceptedMouseButtons( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setBoundingRegionGranularity( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setBoundingRegionGranularity( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setCacheMode( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_setCacheMode( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setCacheMode( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setCursor( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setCursor( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setData( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isObject( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_setData( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setEnabled( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setEnabled( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setFlag( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isLogical( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_setFlag( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setFlag( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setFlags( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setFlags( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setFocus( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setFocus( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QGraphicsItem_setFocus( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setGroup( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setGroup( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setHandlesChildEvents( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setHandlesChildEvents( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setOpacity( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setOpacity( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setParentItem( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setParentItem( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setPos( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_setPos_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setPos( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setSelected( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setSelected( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setToolTip( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isChar( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setToolTip( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setTransform( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) ) .AND. hb_isLogical( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_setTransform( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setTransform( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setVisible( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isLogical( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setVisible( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:setZValue( ... )
   SWITCH PCount()
   CASE 1
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_setZValue( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:shape( ... )
   SWITCH PCount()
   CASE 0
      RETURN QPainterPathFromPointer( Qt_QGraphicsItem_shape( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:shear( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_shear( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:show( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_show( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:toolTip( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_toolTip( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:topLevelItem( ... )
   SWITCH PCount()
   CASE 0
      RETURN QGraphicsItemFromPointer( Qt_QGraphicsItem_topLevelItem( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:topLevelWidget( ... )
   SWITCH PCount()
   CASE 0
      RETURN QGraphicsWidgetFromPointer( Qt_QGraphicsItem_topLevelWidget( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:transform( ... )
   SWITCH PCount()
   CASE 0
      RETURN QTransformFromPointer( Qt_QGraphicsItem_transform( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:translate( ... )
   SWITCH PCount()
   CASE 2
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) )
         RETURN Qt_QGraphicsItem_translate( ::pPtr, ... )
      ENDCASE
      EXIT
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:type( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_type( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:ungrabKeyboard( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_ungrabKeyboard( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:ungrabMouse( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_ungrabMouse( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:unsetCursor( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_unsetCursor( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:update( ... )
   SWITCH PCount()
   CASE 4
      DO CASE
      CASE hb_isNumeric( hb_pvalue( 1 ) ) .AND. hb_isNumeric( hb_pvalue( 2 ) ) .AND. hb_isNumeric( hb_pvalue( 3 ) ) .AND. hb_isNumeric( hb_pvalue( 4 ) )
         RETURN Qt_QGraphicsItem_update_1( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 1
      DO CASE
      CASE hb_isObject( hb_pvalue( 1 ) )
         RETURN Qt_QGraphicsItem_update( ::pPtr, ... )
      ENDCASE
      EXIT
   CASE 0
      RETURN Qt_QGraphicsItem_update( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:window( ... )
   SWITCH PCount()
   CASE 0
      RETURN QGraphicsWidgetFromPointer( Qt_QGraphicsItem_window( ::pPtr, ... ) )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:x( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_x( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:y( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_y( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()


METHOD QGraphicsItem:zValue( ... )
   SWITCH PCount()
   CASE 0
      RETURN Qt_QGraphicsItem_zValue( ::pPtr, ... )
   ENDSWITCH
   RETURN __hbqt_error()

