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

FUNCTION uiToolsutilities( qParent )
   LOCAL oUI
   LOCAL oWidget
   LOCAL qObj := {=>}

   hb_hCaseMatch( qObj, .f. )

   oWidget := QDialog():new( qParent )
  
   oWidget:setObjectName( "DialogTools" )
  
   qObj[ "DialogTools"        ] := oWidget
  
   qObj[ "labelCmdLine"       ] := QLabel():new(qObj[ "DialogTools" ])
   qObj[ "editCmdLine"        ] := QLineEdit():new(qObj[ "DialogTools" ])
   qObj[ "labelName"          ] := QLabel():new(qObj[ "DialogTools" ])
   qObj[ "buttonDown"         ] := QPushButton():new(qObj[ "DialogTools" ])
   qObj[ "buttonUpdate"       ] := QPushButton():new(qObj[ "DialogTools" ])
   qObj[ "labelParams"        ] := QLabel():new(qObj[ "DialogTools" ])
   qObj[ "buttonBrowse"       ] := QPushButton():new(qObj[ "DialogTools" ])
   qObj[ "listNames"          ] := QListWidget():new(qObj[ "DialogTools" ])
   qObj[ "buttonAdd"          ] := QPushButton():new(qObj[ "DialogTools" ])
   qObj[ "buttonDelete"       ] := QPushButton():new(qObj[ "DialogTools" ])
   qObj[ "line"               ] := QFrame():new(qObj[ "DialogTools" ])
   qObj[ "label"              ] := QLabel():new(qObj[ "DialogTools" ])
   qObj[ "editParams"         ] := QLineEdit():new(qObj[ "DialogTools" ])
   qObj[ "buttonUp"           ] := QPushButton():new(qObj[ "DialogTools" ])
   qObj[ "buttonClose"        ] := QPushButton():new(qObj[ "DialogTools" ])
   qObj[ "editName"           ] := QLineEdit():new(qObj[ "DialogTools" ])
   qObj[ "buttonExec"         ] := QPushButton():new(qObj[ "DialogTools" ])
   qObj[ "labelStayIn"        ] := QLabel():new(qObj[ "DialogTools" ])
   qObj[ "editStayIn"         ] := QLineEdit():new(qObj[ "DialogTools" ])
   qObj[ "checkCapture"       ] := QCheckBox():new(qObj[ "DialogTools" ])
   qObj[ "checkOpenCons"      ] := QCheckBox():new(qObj[ "DialogTools" ])
   qObj[ "groupBox"           ] := QGroupBox():new(qObj[ "DialogTools" ])
   qObj[ "comboInitPos"       ] := QComboBox():new(qObj[ "groupBox" ])
   qObj[ "checkDockTop"       ] := QCheckBox():new(qObj[ "groupBox" ])
   qObj[ "listToolbars"       ] := QListWidget():new(qObj[ "groupBox" ])
   qObj[ "checkDockRight"     ] := QCheckBox():new(qObj[ "groupBox" ])
   qObj[ "label_4"            ] := QLabel():new(qObj[ "groupBox" ])
   qObj[ "checkDockBottom"    ] := QCheckBox():new(qObj[ "groupBox" ])
   qObj[ "checkDockLeft"      ] := QCheckBox():new(qObj[ "groupBox" ])
   qObj[ "label_3"            ] := QLabel():new(qObj[ "groupBox" ])
   qObj[ "checkFloatable"     ] := QCheckBox():new(qObj[ "groupBox" ])
   qObj[ "checkInactive"      ] := QCheckBox():new(qObj[ "groupBox" ])
   qObj[ "buttonUserToolbarUpd" ] := QPushButton():new(qObj[ "groupBox" ])
   qObj[ "groupBox_2"         ] := QGroupBox():new(qObj[ "DialogTools" ])
   qObj[ "tableButtons"       ] := QTableWidget():new(qObj[ "groupBox_2" ])
   qObj[ "buttonBtnDown"      ] := QToolButton():new(qObj[ "groupBox_2" ])
   qObj[ "buttonBtnUp"        ] := QToolButton():new(qObj[ "groupBox_2" ])
   qObj[ "groupBox_3"         ] := QGroupBox():new(qObj[ "DialogTools" ])
   qObj[ "comboToolbarAsgnd"  ] := QComboBox():new(qObj[ "groupBox_3" ])
   qObj[ "editImage"          ] := QLineEdit():new(qObj[ "groupBox_3" ])
   qObj[ "label_2"            ] := QLabel():new(qObj[ "groupBox_3" ])
   qObj[ "buttonSetImage"     ] := QToolButton():new(qObj[ "groupBox_3" ])
   qObj[ "label_5"            ] := QLabel():new(qObj[ "groupBox_3" ])
   qObj[ "editTooltip"        ] := QLineEdit():new(qObj[ "groupBox_3" ])
   qObj[ "checkToolActive"    ] := QCheckBox():new(qObj[ "groupBox_3" ])
   
   qObj[ "DialogTools"        ]:resize(602, 425)
   qObj[ "labelCmdLine"       ]:setGeometry(QRect():new(12, 236, 251, 16))
   qObj[ "editCmdLine"        ]:setGeometry(QRect():new(12, 254, 255, 20))
   qObj[ "labelName"          ]:setGeometry(QRect():new(12, 194, 53, 16))
   qObj[ "buttonDown"         ]:setGeometry(QRect():new(176, 96, 95, 24))
   qObj[ "buttonUpdate"       ]:setGeometry(QRect():new(12, 392, 149, 24))
   qObj[ "labelParams"        ]:setGeometry(QRect():new(12, 278, 79, 16))
   qObj[ "buttonBrowse"       ]:setGeometry(QRect():new(176, 210, 95, 24))
   qObj[ "listNames"          ]:setGeometry(QRect():new(12, 19, 149, 173))
   qObj[ "buttonAdd"          ]:setGeometry(QRect():new(176, 18, 95, 24))
   qObj[ "buttonDelete"       ]:setGeometry(QRect():new(176, 44, 95, 24))
   qObj[ "line"               ]:setGeometry(QRect():new(12, 376, 253, 16))
   qObj[ "line"               ]:setFrameShape(4)
   qObj[ "line"               ]:setFrameShadow(48)
   qObj[ "label"              ]:setGeometry(QRect():new(12, 4, 105, 16))
   qObj[ "editParams"         ]:setGeometry(QRect():new(12, 296, 253, 20))
   qObj[ "buttonUp"           ]:setGeometry(QRect():new(176, 70, 95, 24))
   qObj[ "buttonClose"        ]:setGeometry(QRect():new(172, 392, 95, 24))
   qObj[ "editName"           ]:setGeometry(QRect():new(12, 212, 149, 20))
   qObj[ "buttonExec"         ]:setGeometry(QRect():new(176, 168, 95, 24))
   qObj[ "labelStayIn"        ]:setGeometry(QRect():new(14, 320, 95, 16))
   qObj[ "editStayIn"         ]:setGeometry(QRect():new(14, 338, 251, 20))
   qObj[ "checkCapture"       ]:setGeometry(QRect():new(14, 362, 109, 19))
   qObj[ "checkOpenCons"      ]:setGeometry(QRect():new(134, 362, 131, 19))
   qObj[ "groupBox"           ]:setGeometry(QRect():new(284, 164, 307, 111))
   qObj[ "comboInitPos"       ]:setGeometry(QRect():new(114, 32, 93, 22))
   qObj[ "checkDockTop"       ]:setGeometry(QRect():new(246, 28, 53, 19))
   qObj[ "listToolbars"       ]:setGeometry(QRect():new(10, 18, 93, 83))
   qObj[ "checkDockRight"     ]:setGeometry(QRect():new(246, 82, 53, 19))
   qObj[ "label_4"            ]:setGeometry(QRect():new(246, 12, 53, 16))
   qObj[ "checkDockBottom"    ]:setGeometry(QRect():new(246, 64, 53, 19))
   qObj[ "checkDockLeft"      ]:setGeometry(QRect():new(246, 46, 53, 19))
   qObj[ "label_3"            ]:setGeometry(QRect():new(116, 14, 89, 16))
   qObj[ "checkFloatable"     ]:setGeometry(QRect():new(114, 60, 71, 19))
   qObj[ "checkInactive"      ]:setGeometry(QRect():new(114, 82, 71, 19))
   qObj[ "buttonUserToolbarUpd" ]:setGeometry(QRect():new(182, 63, 55, 37))
   qObj[ "groupBox_2"         ]:setGeometry(QRect():new(284, 284, 307, 131))
   qObj[ "tableButtons"       ]:setGeometry(QRect():new(10, 16, 253, 107))
   qObj[ "buttonBtnDown"      ]:setGeometry(QRect():new(272, 16, 25, 20))
   qObj[ "buttonBtnUp"        ]:setGeometry(QRect():new(272, 46, 25, 20))
   qObj[ "groupBox_3"         ]:setGeometry(QRect():new(284, 8, 307, 147))
   qObj[ "comboToolbarAsgnd"  ]:setGeometry(QRect():new(10, 20, 199, 22))
   qObj[ "editImage"          ]:setGeometry(QRect():new(10, 66, 249, 20))
   qObj[ "label_2"            ]:setGeometry(QRect():new(12, 48, 81, 16))
   qObj[ "buttonSetImage"     ]:setGeometry(QRect():new(270, 66, 25, 20))
   qObj[ "label_5"            ]:setGeometry(QRect():new(12, 98, 57, 16))
   qObj[ "editTooltip"        ]:setGeometry(QRect():new(10, 116, 249, 20))
   qObj[ "checkToolActive"    ]:setGeometry(QRect():new(246, 22, 57, 19))
   qObj[ "DialogTools"        ]:setWindowTitle(q__tr("DialogTools", "Tools & Utilities", 0, "UTF8"))
   qObj[ "labelCmdLine"       ]:setText( [Command Line ( Keep blank if excuted via terminal )] )
   qObj[ "editCmdLine"        ]:setToolTip( [<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"> <html><head><meta name="qrichtext" content="1" /><style type="text/css"> p, li { white-space: pre-wrap; } </style></head><body style=" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;"> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-size:8pt;">Keep this field blank if a command prompt has to be invoked to execute the parameters. </span></p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-size:8pt;">This is useful for building any type of project hbIDE do not support yet.</span></p></body></html>] )
   qObj[ "labelName"          ]:setText( [Name:] )
   qObj[ "buttonDown"         ]:setText( [Down] )
   qObj[ "buttonUpdate"       ]:setText( [Update] )
   qObj[ "labelParams"        ]:setText( [Parameters:] )
   qObj[ "buttonBrowse"       ]:setText( [Browse] )
   qObj[ "buttonAdd"          ]:setText( [Add] )
   qObj[ "buttonDelete"       ]:setText( [Delete] )
   qObj[ "label"              ]:setText( [Current Tools:] )
   qObj[ "editParams"         ]:setToolTip( [<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"> <html><head><meta name="qrichtext" content="1" /><style type="text/css"> p, li { white-space: pre-wrap; } </style></head><body style=" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;"> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-size:8pt;">Parameters list may contain batch files, compilers directives, linker commands, if this tool is used to build a project. </span></p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-size:8pt;">Otherwise it may contain parameters passed to the executable supplied in "Command line" field.</span></p> <p align="center" style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;"></p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-size:8pt; font-weight:600;">NOTE</span></p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-size:8pt;">"/" or "\\" characters are recognized as path separators and cannot be used as parameter delimiters for contained applications.</span></p></body></html>] )
   qObj[ "buttonUp"           ]:setText( [Up] )
   qObj[ "buttonClose"        ]:setText( [Close] )
   qObj[ "buttonExec"         ]:setText( [Execute] )
   qObj[ "labelStayIn"        ]:setText( [Start in:] )
   qObj[ "editStayIn"         ]:setToolTip( [<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"> <html><head><meta name="qrichtext" content="1" /><style type="text/css"> p, li { white-space: pre-wrap; } </style></head><body style=" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;"> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">Before executing this utility hbIDE will make this path current and then will run the command lin. </p> <p align="center" style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"></p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">This will specifically help the applications which are expecting a fixed environment for their proper execution.</p></body></html>] )
   qObj[ "checkCapture"       ]:setToolTip( [<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"> <html><head><meta name="qrichtext" content="1" /><style type="text/css"> p, li { white-space: pre-wrap; } </style></head><body style=" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;"> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">This flag initiates the process in the background and all output from the designated application is displayed in the "Output Console" at the bottom of editing area.</p> <p align="center" style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"></p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">This feature is generally suitable for building any project.</p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">The output recieved as such empowers you the same feature as if Harbour project has been compiled, i.e., double click on an error line will open the source in the editor.</p></body></html>] )
   qObj[ "checkCapture"       ]:setText( [Capture Output ?] )
   qObj[ "checkOpenCons"      ]:setToolTip( [<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"> <html><head><meta name="qrichtext" content="1" /><style type="text/css"> p, li { white-space: pre-wrap; } </style></head><body style=" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;"> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">If you check it, "Ouput Console" will be made visible the moment you will execute this utility. </p> <p align="center" style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"></p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">Alternatively you can open the Output Console anytime by clicking on relevant icon on right-toolbar.</p> <p align="center" style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"></p> <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">This has the bearing on visiblity of the widget. The output will ever be routed therein no matter this box is checked or not.</p></body></html>] )
   qObj[ "checkOpenCons"      ]:setText( [Open Output Console ?] )
   qObj[ "groupBox"           ]:setTitle(q__tr("DialogTools", " Toolbars: ", 0, "UTF8"))
   qObj[ "checkDockTop"       ]:setText( [Top] )
   qObj[ "checkDockRight"     ]:setText( [Right] )
   qObj[ "label_4"            ]:setText( [Dockable:] )
   qObj[ "checkDockBottom"    ]:setText( [Bottom] )
   qObj[ "checkDockLeft"      ]:setText( [Left] )
   qObj[ "label_3"            ]:setText( [Iniitial Position:] )
   qObj[ "checkFloatable"     ]:setText( [Floatable] )
   qObj[ "checkInactive"      ]:setText( [Inactive] )
   qObj[ "buttonUserToolbarUpd" ]:setText( [Update] )
   qObj[ "groupBox_2"         ]:setTitle(q__tr("DialogTools", " Toolbar Buttons: ", 0, "UTF8"))
   qObj[ "buttonBtnDown"      ]:setText( [...] )
   qObj[ "buttonBtnUp"        ]:setText( [...] )
   qObj[ "groupBox_3"         ]:setTitle(q__tr("DialogTools", " Current Tools Assignment to Toolbar: ", 0, "UTF8"))
   qObj[ "label_2"            ]:setText( [Image:] )
   qObj[ "buttonSetImage"     ]:setText( [...] )
   qObj[ "label_5"            ]:setText( [Tooltip:] )
   qObj[ "checkToolActive"    ]:setText( [Active] )

   oUI         := HbQtUI():new()
   oUI:qObj    := qObj
   oUI:oWidget := oWidget

   RETURN oUI

/*----------------------------------------------------------------------*/

