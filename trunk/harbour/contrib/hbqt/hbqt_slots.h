/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009 Marcos Antonio Gambeta <marcosgambeta at gmail dot com>
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 * www - http://www.harbour-project.org
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


#ifndef SLOTS_H

#define SLOTS_H

#include <QObject>
#include <QMainWindow>
#include <QList>
#include <QModelIndex>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>


#include "hbapi.h"
#include "hbapiitm.h"

class Slots: public QObject
{
   Q_OBJECT

public:
   Slots( QObject *parent = 0 );
   ~Slots();
   QList<QWidget*> list1;
   QList<QAction*> list5;
   QList<QString> list2;
   QList<PHB_ITEM> list3;
   QList<bool> list4;

public slots:
   void clicked();
   void triggered();
   void triggered( bool checked );
   void hovered();
   void stateChanged( int state );
   void pressed();
   void released();
   void activated( int index );
   void currentIndexChanged( int index );
   void highlighted( int index );
   void returnPressed();
   void clicked_model( const QModelIndex & index );
   void viewportEntered();
   bool event( QEvent * event );
   void keyPressEvent( QKeyEvent * event );
   void mouseMoveEvent( QMouseEvent * event );
   void hovered( QAction * action );

#if 0
   void accessibleEvent();
   void actionEvent();
   void childEvent();
   void closeEvent();
   void customEvent();
   void dragLeaveEvent();
   void dropEvent();
   void dynamicPropertyChangeEvent();
   void fileOpenEvent();
   void focusEvent();
   void graphicsSceneEvent();
   void helpEvent();
   void hideEvent();
   void hoverEvent();
   void iconDragEvent();
   void inputEvent();
   void inputMethodEvent();
   void moveEvent();
   void paintEvent();
   void resizeEvent();
   void shortcutEvent();
   void showEvent();
   void statusTipEvent();
   void timerEvent();
   void whatsThisClickedEvent();
   void windowStateChangeEvent();
#endif
};


class MyDrawingArea : public QWidget
{
   Q_OBJECT

public:
   MyDrawingArea( QWidget *parent = 0 );
   virtual ~MyDrawingArea( void );

   #if 0
protected slots:
   void keyReleaseEvent( QKeyEvent * event );
   void mousePressEvent( QMouseEvent * event );
   void mouseReleaseEvent( QMouseEvent * event );
   void mouseDoubleClickEvent( QMouseEvent * event );
   void paintEvent( QPaintEvent * event );
   void resizeEvent( QResizeEvent * event );
   void wheelEvent( QWheelEvent * event );
   void timerEvent( QTimerEvent * event );
   void focusInEvent( QFocusEvent * event );
   void focusOutEvent( QFocusEvent * event );
   void moveEvent( QMoveEvent * event );
   bool event( QEvent * event );
   #endif
   void keyPressEvent( QKeyEvent * event );
   void mouseMoveEvent( QMouseEvent * event );

signals:
   void sg_mouseMoveEvent( QMouseEvent * event );
   void sg_keyPressEvent( QKeyEvent * event );

};


class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyMainWindow();
    virtual ~MyMainWindow();

public:
    void closeEvent( QCloseEvent *event );

};

#endif

