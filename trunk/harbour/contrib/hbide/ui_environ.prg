/*
 * $Id$
 */

/* -------------------------------------------------------------------- */
/* WARNING: Automatically generated source file. DO NOT EDIT!           */
/*          Instead, edit corresponding .ui file,                       */
/*          with Qt Generator, and run hbqtui.exe.                      */
/* -------------------------------------------------------------------- */
/*                                                                      */
/*               Pritpal Bedi <bedipritpal@hotmail.com>                 */
/*                                                                      */
/* -------------------------------------------------------------------- */

FUNCTION uiEnviron( qParent )
   LOCAL oUI
   LOCAL oWidget
   LOCAL qObj := {=>}

   hb_hCaseMatch( qObj, .f. )

   oWidget := QWidget():new( qParent )
  
   oWidget:setObjectName( "EnvForm" )
  
   qObj[ "EnvForm"            ] := oWidget
  
   qObj[ "label"              ] := QLabel():new(qObj[ "EnvForm" ])
   qObj[ "comboBox"           ] := QComboBox():new(qObj[ "EnvForm" ])
   qObj[ "pushButton_2"       ] := QPushButton():new(qObj[ "EnvForm" ])
   qObj[ "pushButton_3"       ] := QPushButton():new(qObj[ "EnvForm" ])
   qObj[ "label_2"            ] := QLabel():new(qObj[ "EnvForm" ])
   qObj[ "plainTextEdit"      ] := QPlainTextEdit():new(qObj[ "EnvForm" ])
   qObj[ "label_3"            ] := QLabel():new(qObj[ "EnvForm" ])
   qObj[ "comboBox_2"         ] := QComboBox():new(qObj[ "EnvForm" ])
   qObj[ "pushButton"         ] := QPushButton():new(qObj[ "EnvForm" ])
   qObj[ "tabWidget"          ] := QTabWidget():new(qObj[ "EnvForm" ])
   qObj[ "tab"                ] := QWidget():new()
   qObj[ "label_4"            ] := QLabel():new(qObj[ "tab" ])
   qObj[ "lineEdit"           ] := QLineEdit():new(qObj[ "tab" ])
   qObj[ "label_5"            ] := QLabel():new(qObj[ "tab" ])
   qObj[ "lineEdit_2"         ] := QLineEdit():new(qObj[ "tab" ])
   qObj[ "toolButton"         ] := QToolButton():new(qObj[ "tab" ])
   qObj[ "checkBox"           ] := QCheckBox():new(qObj[ "tab" ])
   qObj[ "checkBox_2"         ] := QCheckBox():new(qObj[ "tab" ])
   qObj[ "checkBox_3"         ] := QCheckBox():new(qObj[ "tab" ])
   qObj[ "checkBox_4"         ] := QCheckBox():new(qObj[ "tab" ])
   qObj[ "checkBox_5"         ] := QCheckBox():new(qObj[ "tab" ])
   qObj[ "plainTextEdit_2"    ] := QPlainTextEdit():new(qObj[ "tab" ])
   qObj[ "label_6"            ] := QLabel():new(qObj[ "tab" ])
   qObj[ "label_7"            ] := QLabel():new(qObj[ "tab" ])
   qObj[ "label_8"            ] := QLabel():new(qObj[ "tab" ])
   qObj[ "label_9"            ] := QLabel():new(qObj[ "tab" ])
   qObj[ "label_10"           ] := QLabel():new(qObj[ "tab" ])
   qObj[ "comboBox_3"         ] := QComboBox():new(qObj[ "tab" ])
   qObj[ "comboBox_4"         ] := QComboBox():new(qObj[ "tab" ])
   qObj[ "comboBox_5"         ] := QComboBox():new(qObj[ "tab" ])
   qObj[ "comboBox_6"         ] := QComboBox():new(qObj[ "tab" ])
   qObj[ "label_11"           ] := QLabel():new(qObj[ "tab" ])
   qObj[ "comboBox_7"         ] := QComboBox():new(qObj[ "tab" ])
   qObj[ "tab_2"              ] := QWidget():new()
   qObj[ "lineEdit_3"         ] := QLineEdit():new(qObj[ "tab_2" ])
   qObj[ "toolButton_2"       ] := QToolButton():new(qObj[ "tab_2" ])
   qObj[ "lineEdit_4"         ] := QLineEdit():new(qObj[ "tab_2" ])
   qObj[ "label_12"           ] := QLabel():new(qObj[ "tab_2" ])
   qObj[ "label_13"           ] := QLabel():new(qObj[ "tab_2" ])
   qObj[ "plainTextEdit_3"    ] := QPlainTextEdit():new(qObj[ "tab_2" ])
   qObj[ "label_14"           ] := QLabel():new(qObj[ "tab_2" ])
   qObj[ "tab_3"              ] := QWidget():new()
   qObj[ "plainTextEdit_4"    ] := QPlainTextEdit():new(qObj[ "tab_3" ])
   qObj[ "label_15"           ] := QLabel():new(qObj[ "tab_3" ])
   qObj[ "plainTextEdit_5"    ] := QPlainTextEdit():new(qObj[ "tab_3" ])
   qObj[ "plainTextEdit_6"    ] := QPlainTextEdit():new(qObj[ "tab_3" ])
   qObj[ "plainTextEdit_7"    ] := QPlainTextEdit():new(qObj[ "tab_3" ])
   qObj[ "label_16"           ] := QLabel():new(qObj[ "tab_3" ])
   qObj[ "label_17"           ] := QLabel():new(qObj[ "tab_3" ])
   qObj[ "label_18"           ] := QLabel():new(qObj[ "tab_3" ])
   qObj[ "tab_4"              ] := QWidget():new()
   qObj[ "label_19"           ] := QLabel():new(qObj[ "tab_4" ])
   qObj[ "plainTextEdit_8"    ] := QPlainTextEdit():new(qObj[ "tab_4" ])
   qObj[ "tab_5"              ] := QWidget():new()
   qObj[ "label_20"           ] := QLabel():new(qObj[ "tab_5" ])
   qObj[ "plainTextEdit_9"    ] := QPlainTextEdit():new(qObj[ "tab_5" ])
   qObj[ "tab_6"              ] := QWidget():new()
   qObj[ "label_21"           ] := QLabel():new(qObj[ "tab_6" ])
   qObj[ "plainTextEdit_10"   ] := QPlainTextEdit():new(qObj[ "tab_6" ])
   qObj[ "plainTextEdit_11"   ] := QPlainTextEdit():new(qObj[ "tab_6" ])
   qObj[ "label_22"           ] := QLabel():new(qObj[ "tab_6" ])
   qObj[ "plainTextEdit_12"   ] := QPlainTextEdit():new(qObj[ "tab_6" ])
   qObj[ "label_23"           ] := QLabel():new(qObj[ "tab_6" ])
   qObj[ "tab_7"              ] := QWidget():new()
   qObj[ "plainTextEdit_13"   ] := QPlainTextEdit():new(qObj[ "tab_7" ])
   qObj[ "pushButton_4"       ] := QPushButton():new(qObj[ "EnvForm" ])
   qObj[ "pushButton_5"       ] := QPushButton():new(qObj[ "EnvForm" ])
   
   qObj[ "EnvForm"            ]:resize(554, 488)
   qObj[ "label"              ]:setGeometry(QRect():new(14, 10, 39, 16))
   qObj[ "comboBox"           ]:setGeometry(QRect():new(10, 28, 197, 22))
   qObj[ "pushButton_2"       ]:setGeometry(QRect():new(430, 28, 51, 24))
   qObj[ "pushButton_3"       ]:setGeometry(QRect():new(490, 28, 51, 24))
   qObj[ "label_2"            ]:setGeometry(QRect():new(12, 56, 431, 16))
   qObj[ "plainTextEdit"      ]:setGeometry(QRect():new(10, 74, 531, 53))
   qObj[ "plainTextEdit"      ]:setLineWrapMode(0)
   qObj[ "label_3"            ]:setGeometry(QRect():new(236, 10, 46, 14))
   qObj[ "comboBox_2"         ]:setGeometry(QRect():new(234, 28, 109, 22))
   qObj[ "pushButton"         ]:setGeometry(QRect():new(370, 28, 51, 24))
   qObj[ "tabWidget"          ]:setGeometry(QRect():new(10, 140, 533, 303))
   qObj[ "label_4"            ]:setGeometry(QRect():new(8, 11, 31, 16))
   qObj[ "lineEdit"           ]:setGeometry(QRect():new(38, 10, 119, 20))
   qObj[ "label_5"            ]:setGeometry(QRect():new(170, 12, 67, 16))
   qObj[ "lineEdit_2"         ]:setGeometry(QRect():new(234, 10, 239, 20))
   qObj[ "toolButton"         ]:setGeometry(QRect():new(492, 10, 25, 20))
   qObj[ "checkBox"           ]:setGeometry(QRect():new(10, 38, 191, 19))
   qObj[ "checkBox_2"         ]:setGeometry(QRect():new(10, 62, 173, 19))
   qObj[ "checkBox_3"         ]:setGeometry(QRect():new(10, 86, 173, 19))
   qObj[ "checkBox_4"         ]:setGeometry(QRect():new(10, 110, 175, 19))
   qObj[ "checkBox_5"         ]:setGeometry(QRect():new(10, 134, 193, 19))
   qObj[ "plainTextEdit_2"    ]:setGeometry(QRect():new(10, 174, 465, 93))
   qObj[ "plainTextEdit_2"    ]:setLineWrapMode(0)
   qObj[ "label_6"            ]:setGeometry(QRect():new(10, 156, 463, 16))
   qObj[ "label_6"            ]:setAlignment(132)
   qObj[ "label_7"            ]:setGeometry(QRect():new(288, 38, 109, 13))
   qObj[ "label_8"            ]:setGeometry(QRect():new(288, 62, 119, 16))
   qObj[ "label_9"            ]:setGeometry(QRect():new(288, 86, 119, 16))
   qObj[ "label_10"           ]:setGeometry(QRect():new(288, 110, 113, 16))
   qObj[ "comboBox_3"         ]:setGeometry(QRect():new(416, 38, 57, 22))
   qObj[ "comboBox_4"         ]:setGeometry(QRect():new(416, 62, 57, 22))
   qObj[ "comboBox_5"         ]:setGeometry(QRect():new(416, 86, 57, 22))
   qObj[ "comboBox_6"         ]:setGeometry(QRect():new(416, 110, 57, 22))
   qObj[ "label_11"           ]:setGeometry(QRect():new(288, 134, 115, 16))
   qObj[ "comboBox_7"         ]:setGeometry(QRect():new(416, 134, 57, 22))
   qObj[ "tabWidget"          ]:addTab(qObj[ "tab" ], "")
   qObj[ "lineEdit_3"         ]:setGeometry(QRect():new(38, 10, 119, 20))
   qObj[ "toolButton_2"       ]:setGeometry(QRect():new(492, 12, 25, 20))
   qObj[ "lineEdit_4"         ]:setGeometry(QRect():new(234, 12, 239, 20))
   qObj[ "label_12"           ]:setGeometry(QRect():new(8, 12, 31, 16))
   qObj[ "label_13"           ]:setGeometry(QRect():new(170, 12, 67, 16))
   qObj[ "plainTextEdit_3"    ]:setGeometry(QRect():new(8, 58, 465, 211))
   qObj[ "plainTextEdit_3"    ]:setLineWrapMode(0)
   qObj[ "label_14"           ]:setGeometry(QRect():new(8, 38, 463, 16))
   qObj[ "tabWidget"          ]:addTab(qObj[ "tab_2" ], "")
   qObj[ "plainTextEdit_4"    ]:setGeometry(QRect():new(68, 10, 449, 59))
   qObj[ "plainTextEdit_4"    ]:setLineWrapMode(0)
   qObj[ "label_15"           ]:setGeometry(QRect():new(8, 34, 46, 14))
   qObj[ "plainTextEdit_5"    ]:setGeometry(QRect():new(68, 76, 449, 59))
   qObj[ "plainTextEdit_5"    ]:setLineWrapMode(0)
   qObj[ "plainTextEdit_6"    ]:setGeometry(QRect():new(68, 142, 449, 59))
   qObj[ "plainTextEdit_6"    ]:setLineWrapMode(0)
   qObj[ "plainTextEdit_7"    ]:setGeometry(QRect():new(68, 210, 449, 59))
   qObj[ "plainTextEdit_7"    ]:setLineWrapMode(0)
   qObj[ "label_16"           ]:setGeometry(QRect():new(8, 98, 46, 14))
   qObj[ "label_17"           ]:setGeometry(QRect():new(10, 166, 46, 14))
   qObj[ "label_18"           ]:setGeometry(QRect():new(10, 234, 46, 14))
   qObj[ "tabWidget"          ]:addTab(qObj[ "tab_3" ], "")
   qObj[ "label_19"           ]:setGeometry(QRect():new(8, 10, 511, 16))
   qObj[ "plainTextEdit_8"    ]:setGeometry(QRect():new(8, 32, 511, 237))
   qObj[ "plainTextEdit_8"    ]:setLineWrapMode(0)
   qObj[ "tabWidget"          ]:addTab(qObj[ "tab_4" ], "")
   qObj[ "label_20"           ]:setGeometry(QRect():new(8, 10, 511, 16))
   qObj[ "plainTextEdit_9"    ]:setGeometry(QRect():new(8, 32, 511, 237))
   qObj[ "plainTextEdit_9"    ]:setLineWrapMode(0)
   qObj[ "tabWidget"          ]:addTab(qObj[ "tab_5" ], "")
   qObj[ "label_21"           ]:setGeometry(QRect():new(8, 10, 509, 16))
   qObj[ "plainTextEdit_10"   ]:setGeometry(QRect():new(8, 28, 511, 61))
   qObj[ "plainTextEdit_10"   ]:setLineWrapMode(0)
   qObj[ "plainTextEdit_11"   ]:setGeometry(QRect():new(8, 118, 511, 61))
   qObj[ "plainTextEdit_11"   ]:setLineWrapMode(0)
   qObj[ "label_22"           ]:setGeometry(QRect():new(8, 100, 509, 16))
   qObj[ "plainTextEdit_12"   ]:setGeometry(QRect():new(8, 208, 511, 61))
   qObj[ "plainTextEdit_12"   ]:setLineWrapMode(0)
   qObj[ "label_23"           ]:setGeometry(QRect():new(8, 190, 509, 16))
   qObj[ "tabWidget"          ]:addTab(qObj[ "tab_6" ], "")
   qObj[ "plainTextEdit_13"   ]:setGeometry(QRect():new(8, 10, 511, 259))
   qObj[ "plainTextEdit_13"   ]:setLineWrapMode(0)
   qObj[ "tabWidget"          ]:addTab(qObj[ "tab_7" ], "")
   qObj[ "pushButton_4"       ]:setGeometry(QRect():new(380, 456, 75, 24))
   qObj[ "pushButton_5"       ]:setGeometry(QRect():new(468, 456, 75, 24))
   qObj[ "tabWidget"          ]:setCurrentIndex(0)
   qObj[ "EnvForm"            ]:setWindowTitle(q__tr("EnvForm", "Environment", 0, "UTF8"))
   qObj[ "label"              ]:setText( [Name:] )
   qObj[ "pushButton_2"       ]:setText( [Copy] )
   qObj[ "pushButton_3"       ]:setText( [Paste] )
   qObj[ "label_2"            ]:setText( [Batch Commands: will be executed before any of the compiler/linker command is invoked.] )
   qObj[ "label_3"            ]:setText( [Type:] )
   qObj[ "pushButton"         ]:setText( [New] )
   qObj[ "label_4"            ]:setText( [Exe:] )
   qObj[ "label_5"            ]:setText( [Install Path:] )
   qObj[ "toolButton"         ]:setText( [...] )
   qObj[ "checkBox"           ]:setText( [-a   Automatic memvar declaration] )
   qObj[ "checkBox_2"         ]:setText( [-b   Include debug info] )
   qObj[ "checkBox_3"         ]:setText( [-l   No line numbers   ] )
   qObj[ "checkBox_4"         ]:setText( [-v   Variables are assumed (m->)] )
   qObj[ "checkBox_5"         ]:setText( [-z   Supress short-cut optimizations] )
   qObj[ "label_6"            ]:setText( [More Options] )
   qObj[ "label_7"            ]:setText( [-w   Warning level:] )
   qObj[ "label_8"            ]:setText( [-es   Exit severity level:] )
   qObj[ "label_9"            ]:setText( [-m   No start procedure:] )
   qObj[ "label_10"           ]:setText( [-g   Output type:] )
   qObj[ "label_11"           ]:setText( [-k   Compatibility mode:] )
   qObj[ "tabWidget"          ]:setTabText(qObj[ "tabWidget" ]:indexOf(qObj[ "tab" ]), q__tr("EnvForm", "PRG Compiler", 0, "UTF8"))
   qObj[ "toolButton_2"       ]:setText( [...] )
   qObj[ "label_12"           ]:setText( [Exe:] )
   qObj[ "label_13"           ]:setText( [Install Path:] )
   qObj[ "label_14"           ]:setText( [Compiler commands - write each one on separate line.] )
   qObj[ "tabWidget"          ]:setTabText(qObj[ "tabWidget" ]:indexOf(qObj[ "tab_2" ]), q__tr("EnvForm", "C Compiler", 0, "UTF8"))
   qObj[ "label_15"           ]:setText( [EXE] )
   qObj[ "label_16"           ]:setText( [LIB] )
   qObj[ "label_17"           ]:setText( [DLL] )
   qObj[ "label_18"           ]:setText( [RES] )
   qObj[ "tabWidget"          ]:setTabText(qObj[ "tabWidget" ]:indexOf(qObj[ "tab_3" ]), q__tr("EnvForm", "Linker", 0, "UTF8"))
   qObj[ "label_19"           ]:setText( [Runtime static default files ( libraries ) - each entry on separate line.] )
   qObj[ "tabWidget"          ]:setTabText(qObj[ "tabWidget" ]:indexOf(qObj[ "tab_4" ]), q__tr("EnvForm", "EXE Defaults", 0, "UTF8"))
   qObj[ "label_20"           ]:setText( [DLL import default files ( libraries ) - each entry on separate line.] )
   qObj[ "tabWidget"          ]:setTabText(qObj[ "tabWidget" ]:indexOf(qObj[ "tab_5" ]), q__tr("EnvForm", "DLL Defaults", 0, "UTF8"))
   qObj[ "label_21"           ]:setText( [EXEcutable Projects] )
   qObj[ "label_22"           ]:setText( [LIBrary Projects] )
   qObj[ "label_23"           ]:setText( [DLL Projects ] )
   qObj[ "tabWidget"          ]:setTabText(qObj[ "tabWidget" ]:indexOf(qObj[ "tab_6" ]), q__tr("EnvForm", "User Cmds", 0, "UTF8"))
   qObj[ "tabWidget"          ]:setTabText(qObj[ "tabWidget" ]:indexOf(qObj[ "tab_7" ]), q__tr("EnvForm", "Info", 0, "UTF8"))
   qObj[ "pushButton_4"       ]:setText( [OK] )
   qObj[ "pushButton_5"       ]:setText( [Cancel] )

   oUI         := HbQtUI():new()
   oUI:qObj    := qObj
   oUI:oWidget := oWidget

   RETURN oUI

/*----------------------------------------------------------------------*/

