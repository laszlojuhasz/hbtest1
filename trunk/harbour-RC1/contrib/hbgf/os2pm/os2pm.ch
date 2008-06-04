/*
 * $Id$
 */


/*
 * Harbour Project source code:
 * Harbour GUI framework for IBM OS/2 Presentation Manager
 *
 * Copyright 2001 Antonio Linares <alinares@fivetech.com>
 * Copyright 2001 Maurilio Longo <maurilio.longo@libero.it>
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



#define WC_FRAME           "#1"  //0xffff0001
#define WC_COMBOBOX        "#2"  //0xffff0002
#define WC_BUTTON          "#3"  //0xffff0003
#define WC_MENU            "#4"  //0xffff0004
#define WC_STATIC          "#5"  //0xffff0005
#define WC_ENTRYFIELD      "#6"  //0xffff0006
#define WC_LISTBOX         "#7"  //0xffff0007
#define WC_SCROLLBAR       "#8"  //0xffff0008
#define WC_TITLEBAR        "#9"  //0xffff0009
#define WC_MLE             "#10" //0xffff000a
#define WC_APPSTAT         "#16" //0xffff0010
#define WC_KBDSTAT         "#17" //0xffff0011
#define WC_PECIC           "#18" //0xffff0012
#define WC_DBE_KKPOPUP     "#19" //0xffff0013
#define WC_SPINBUTTON      "#32" //0xffff0020
#define WC_CONTAINER       "#37" //0xffff0025
#define WC_SLIDER          "#38" //0xffff0026
#define WC_VALUESET        "#39" //0xffff0027
#define WC_NOTEBOOK        "#40" //0xffff0028
#define WC_PENFIRST        "#41" //0xffff0029
#define WC_PENLAST         "#44" //0xffff002c
#define WC_MMPMFIRST       "#64" //0xffff0040
#define WC_CIRCULARSLIDER  "#65" //0xffff0041
#define WC_MMPMLAST        "#79" //0xffff004f

#define WS_VISIBLE         0x80000000
#define WS_DISABLED        0x40000000
#define WS_CLIPCHILDREN    0x20000000
#define WS_CLIPSIBLINGS    0x10000000
#define WS_PARENTCLIP      0x08000000
#define WS_SAVEBITS        0x04000000
#define WS_SYNCPAINT       0x02000000
#define WS_MINIMIZED       0x01000000
#define WS_MAXIMIZED       0x00800000
#define WS_ANIMATE         0x00400000
#define WS_GROUP           0x00010000
#define WS_TABSTOP         0x00020000
#define WS_MULTISELECT     0x00040000

#define CS_MOVENOTIFY      0x00000001
#define CS_SIZEREDRAW      0x00000004
#define CS_HITTEST         0x00000008
#define CS_PUBLIC          0x00000010
#define CS_FRAME           0x00000020
#define CS_CLIPCHILDREN    0x20000000
#define CS_CLIPSIBLINGS    0x10000000
#define CS_PARENTCLIP      0x08000000
#define CS_SAVEBITS        0x04000000
#define CS_SYNCPAINT       0x02000000

#define WM_NULL               0x0000
#define WM_CREATE             0x0001
#define WM_DESTROY            0x0002
#define WM_ENABLE             0x0004
#define WM_SHOW               0x0005
#define WM_MOVE               0x0006
#define WM_SIZE               0x0007
#define WM_ADJUSTWINDOWPOS    0x0008
#define WM_CALCVALIDRECTS     0x0009
#define WM_SETWINDOWPARAMS    0x000a
#define WM_QUERYWINDOWPARAMS  0x000b
#define WM_HITTEST            0x000c
#define WM_ACTIVATE           0x000d
#define WM_SETFOCUS           0x000f
#define WM_SETSELECTION       0x0010
#define WM_PPAINT             0x0011
#define WM_PSETFOCUS          0x0012
#define WM_PSYSCOLORCHANGE    0x0013
#define WM_PSIZE              0x0014
#define WM_PACTIVATE          0x0015
#define WM_PCONTROL           0x0016
#define WM_COMMAND            0x0020
#define WM_SYSCOMMAND         0x0021
#define WM_HELP               0x0022
#define WM_PAINT              0x0023
#define WM_TIMER              0x0024
#define WM_SEM1               0x0025
#define WM_SEM2               0x0026
#define WM_SEM3               0x0027
#define WM_SEM4               0x0028
#define WM_CLOSE              0x0029
#define WM_QUIT               0x002a
#define WM_SYSCOLORCHANGE     0x002b
#define WM_SYSVALUECHANGED    0x002d
#define WM_APPTERMINATENOTIFY 0x002e
#define WM_PRESPARAMCHANGED   0x002f
#define WM_CONTROL            0x0030
#define WM_VSCROLL            0x0031
#define WM_HSCROLL            0x0032
#define WM_INITMENU           0x0033
#define WM_MENUSELECT         0x0034
#define WM_MENUEND            0x0035
#define WM_DRAWITEM           0x0036
#define WM_MEASUREITEM        0x0037
#define WM_CONTROLPOINTER     0x0038
#define WM_QUERYDLGCODE       0x003a
#define WM_INITDLG            0x003b
#define WM_SUBSTITUTESTRING   0x003c
#define WM_MATCHMNEMONIC      0x003d
#define WM_SAVEAPPLICATION    0x003e
#define WM_HELPBASE           0x0f00
#define WM_HELPTOP            0x0fff
#define WM_USER               0x1000
#define WM_FLASHWINDOW        0x0040
#define WM_FORMATFRAME        0x0041
#define WM_UPDATEFRAME        0x0042
#define WM_FOCUSCHANGE        0x0043
#define WM_SETBORDERSIZE      0x0044
#define WM_TRACKFRAME         0x0045
#define WM_MINMAXFRAME        0x0046
#define WM_SETICON            0x0047
#define WM_QUERYICON          0x0048
#define WM_SETACCELTABLE      0x0049
#define WM_QUERYACCELTABLE    0x004a
#define WM_TRANSLATEACCEL     0x004b
#define WM_QUERYTRACKINFO     0x004c
#define WM_QUERYBORDERSIZE    0x004d
#define WM_NEXTMENU           0x004e
#define WM_ERASEBACKGROUND    0x004f
#define WM_QUERYFRAMEINFO     0x0050
#define WM_QUERYFOCUSCHAIN    0x0051
#define WM_OWNERPOSCHANGE     0x0052
#define WM_CALCFRAMERECT      0x0053
#define WM_WINDOWPOSCHANGED   0x0055
#define WM_ADJUSTFRAMEPOS     0x0056
#define WM_QUERYFRAMECTLCOUNT 0x0059
#define WM_QUERYHELPINFO      0x005b
#define WM_SETHELPINFO        0x005c
#define WM_ERROR              0x005d
#define WM_REALIZEPALETTE     0x005e
#define WM_QUERYCONVERTPOS    0x00b0
#define WM_RENDERFMT          0x0060
#define WM_RENDERALLFMTS      0x0061
#define WM_DESTROYCLIPBOARD   0x0062
#define WM_PAINTCLIPBOARD     0x0063
#define WM_SIZECLIPBOARD      0x0064
#define WM_HSCROLLCLIPBOARD   0x0065
#define WM_VSCROLLCLIPBOARD   0x0066
#define WM_MOUSEFIRST         0x0070
#define WM_MOUSEMOVE          0x0070
#define WM_BUTTONCLICKFIRST   0x0071
#define WM_BUTTON1DOWN        0x0071
#define WM_BUTTON1UP          0x0072
#define WM_BUTTON1DBLCLK      0x0073
#define WM_BUTTON2DOWN        0x0074
#define WM_BUTTON2UP          0x0075
#define WM_BUTTON2DBLCLK      0x0076
#define WM_BUTTON3DOWN        0x0077
#define WM_BUTTON3UP          0x0078
#define WM_BUTTON3DBLCLK      0x0079
#define WM_BUTTONCLICKLAST    0x0079
#define WM_MOUSELAST          0x0079
#define WM_CHAR               0x007a
#define WM_VIOCHAR            0x007b
#define WM_JOURNALNOTIFY      0x007c
#define WM_MOUSEMAP           0x007d
#define WM_VRNDISABLED        0x007e
#define WM_VRNENABLED         0x007f
#define WM_EXTMOUSEFIRST      0x0410
#define WM_CHORD              0x0410
#define WM_BUTTON1MOTIONSTART 0x0411
#define WM_BUTTON1MOTIONEND   0x0412
#define WM_BUTTON1CLICK       0x0413
#define WM_BUTTON2MOTIONSTART 0x0414
#define WM_BUTTON2MOTIONEND   0x0415
#define WM_BUTTON2CLICK       0x0416
#define WM_BUTTON3MOTIONSTART 0x0417
#define WM_BUTTON3MOTIONEND   0x0418
#define WM_BUTTON3CLICK       0x0419
#define WM_EXTMOUSELAST       0x0419
#define WM_MOUSETRANSLATEFIRST   0x0420
#define WM_BEGINDRAG          0x0420
#define WM_ENDDRAG            0x0421
#define WM_SINGLESELECT       0x0422
#define WM_OPEN               0x0423
#define WM_CONTEXTMENU        0x0424
#define WM_CONTEXTHELP        0x0425
#define WM_TEXTEDIT           0x0426
#define WM_BEGINSELECT        0x0427
#define WM_ENDSELECT          0x0428
#define WM_MOUSETRANSLATELAST 0x0428
#define WM_PICKUP             0x0429
#define WM_PENFIRST           0x0481
#define WM_PENLAST            0x049f
#define WM_MMPMFIRST          0x0500
#define WM_MMPMLAST           0x05ff
#define WM_BIDI_FIRST         0x0bd0
#define WM_BIDI_LAST          0x0bff
#define WM_MSGBOXINIT         0x010e
#define WM_MSGBOXDISMISS      0x010f
#define WM_CTLCOLORCHANGE     0x0129
#define WM_QUERYCTLTYPE       0x0130 /*0x012a?*/
#define WM_DRAGFIRST          0x0310
#define WM_DRAGLAST           0x032f

#define HWND_DESKTOP          1
#define HWND_OBJECT           2
#define HWND_TOP              3
#define HWND_BOTTOM           4
#define HWND_THREADCAPTURE    5
#define HWND_PARENT           NIL

#define FCF_TITLEBAR          0x00000001
#define FCF_SYSMENU           0x00000002
#define FCF_MENU              0x00000004
#define FCF_SIZEBORDER        0x00000008
#define FCF_MINBUTTON         0x00000010
#define FCF_MAXBUTTON         0x00000020
#define FCF_MINMAX            (FCF_MINBUTTON + FCF_MAXBUTTON)
#define FCF_VERTSCROLL        0x00000040
#define FCF_HORZSCROLL        0x00000080
#define FCF_DLGBORDER         0x00000100
#define FCF_BORDER            0x00000200
#define FCF_SHELLPOSITION     0x00000400
#define FCF_TASKLIST          0x00000800
#define FCF_NOBYTEALIGN       0x00001000
#define FCF_NOMOVEWITHOWNER   0x00002000
#define FCF_ICON              0x00004000
#define FCF_ACCELTABLE        0x00008000
#define FCF_SYSMODAL          0x00010000
#define FCF_SCREENALIGN       0x00020000
#define FCF_MOUSEALIGN        0x00040000
#define FCF_PALETTE_NORMAL    0x00080000
#define FCF_PALETTE_HELP      0x00100000
#define FCF_PALETTE_POPUPODD  0x00200000
#define FCF_PALETTE_POPUPEVEN 0x00400000
#define FCF_HIDEBUTTON        0x01000000
#define FCF_HIDEMAX           0x01000020
#define FCF_AUTOICON          0x40000000
#define FCF_DBE_APPSTAT       0x80000000
#define FCF_STANDARD          0x0000cc3f

#define MIA_NODISMISS         0x0020
#define MIA_FRAMED            0x1000
#define MIA_CHECKED           0x2000
#define MIA_DISABLED          0x4000
#define MIA_HILITED           0x8000
#define MIS_TEXT              0x0001
#define MIS_BITMAP            0x0002
#define MIS_SEPARATOR         0x0004
#define MIS_OWNERDRAW         0x0008
#define MIS_SUBMENU           0x0010
#define MIS_MULTMENU          0x0020
#define MIS_SYSCOMMAND        0x0040
#define MIS_HELP              0x0080
#define MIS_STATIC            0x0100
#define MIS_BUTTONSEPARATOR   0x0200
#define MIS_BREAK             0x0400
#define MIS_BREAKSEPARATOR    0x0800
#define MIS_GROUP             0x1000
#define MIS_SINGLE            0x2000
#define MIT_END               (-1)
#define MIT_NONE              (-1)
#define MIT_MEMERROR          (-1)
#define MIT_ERROR             (-1)
#define MIT_FIRST             (-2)
#define MIT_LAST              (-3)

#define BS_PUSHBUTTON         0
#define BS_CHECKBOX           1
#define BS_AUTOCHECKBOX       2
#define BS_RADIOBUTTON        3
#define BS_AUTORADIOBUTTON    4
#define BS_3STATE             5
#define BS_AUTO3STATE         6
#define BS_USERBUTTON         7
#define BS_PRIMARYSTYLES      0x000f
#define BS_TEXT               0x0010
#define BS_MINIICON           0x0020
#define BS_BITMAP             0x0040
#define BS_ICON               0x0080
#define BS_HELP               0x0100
#define BS_SYSCOMMAND         0x0200
#define BS_DEFAULT            0x0400
#define BS_NOPOINTERFOCUS     0x0800
#define BS_NOBORDER           0x1000
#define BS_NOCURSORSELECT     0x2000
#define BS_AUTOSIZE           0x4000

#define MLS_WORDWRAP          0x0001
#define MLS_BORDER            0x0002
#define MLS_VSCROLL           0x0004
#define MLS_HSCROLL           0x0008
#define MLS_READONLY          0x0010
#define MLS_IGNORETAB         0x0020
#define MLS_DISABLEUNDO       0x0040

#define ES_LEFT               0x0000
#define ES_CENTER             0x0001
#define ES_RIGHT              0x0002
#define ES_AUTOSCROLL         0x0004
#define ES_MARGIN             0x0008
#define ES_AUTOTAB            0x0010
#define ES_READONLY           0x0020
#define ES_COMMAND            0x0040
#define ES_UNREADABLE         0x0080
#define ES_AUTOSIZE           0x0200
#define ES_ANY                0x0000
#define ES_SBCS               0x1000
#define ES_DBCS               0x2000
#define ES_MIXED              0x3000

#define CMDSRC_OTHER          0
#define CMDSRC_PUSHBUTTON     1
#define CMDSRC_MENU           2
#define CMDSRC_ACCELERATOR    3
#define CMDSRC_FONTDLG        4
#define CMDSRC_FILEDLG        5
#define CMDSRC_PRINTDLG       6
#define CMDSRC_COLORDLG       7


#define PMERR_INVALID_HWND                0x1001
#define PMERR_INVALID_HMQ                 0x1002
#define PMERR_PARAMETER_OUT_OF_RANGE      0x1003
#define PMERR_WINDOW_LOCK_UNDERFLOW 0x1004
#define PMERR_WINDOW_LOCK_OVERFLOW  0x1005
#define PMERR_BAD_WINDOW_LOCK_COUNT 0x1006
#define PMERR_WINDOW_NOT_LOCKED     0x1007
#define PMERR_INVALID_SELECTOR      0x1008
#define PMERR_CALL_FROM_WRONG_THREAD   0x1009
#define PMERR_RESOURCE_NOT_FOUND 0x100a
#define PMERR_INVALID_STRING_PARM   0x100b
#define PMERR_INVALID_HHEAP      0x100c
#define PMERR_INVALID_HEAP_POINTER  0x100d
#define PMERR_INVALID_HEAP_SIZE_PARM   0x100e
#define PMERR_INVALID_HEAP_SIZE     0x100f
#define PMERR_INVALID_HEAP_SIZE_WORD   0x1010
#define PMERR_HEAP_OUT_OF_MEMORY 0x1011
#define PMERR_HEAP_MAX_SIZE_REACHED 0x1012
#define PMERR_INVALID_HATOMTBL      0x1013
#define PMERR_INVALID_ATOM    0x1014
#define PMERR_INVALID_ATOM_NAME     0x1015
#define PMERR_INVALID_INTEGER_ATOM  0x1016
#define PMERR_ATOM_NAME_NOT_FOUND   0x1017
#define PMERR_QUEUE_TOO_LARGE    0x1018
#define PMERR_INVALID_FLAG    0x1019
#define PMERR_INVALID_HACCEL     0x101a
#define PMERR_INVALID_HPTR    0x101b
#define PMERR_INVALID_HENUM      0x101c
#define PMERR_INVALID_SRC_CODEPAGE  0x101d
#define PMERR_INVALID_DST_CODEPAGE  0x101e
#define PMERR_UNKNOWN_COMPONENT_ID  0x101f
#define PMERR_UNKNOWN_ERROR_CODE 0x1020
#define PMERR_SEVERITY_LEVELS    0x1021
#define PMERR_INVALID_RESOURCE_FORMAT  0x1034
#define PMERR_NO_MSG_QUEUE    0x1036
#define PMERR_CANNOT_SET_FOCUS      0x1037
#define PMERR_QUEUE_FULL      0x1038
#define PMERR_LIBRARY_LOAD_FAILED   0x1039
#define PMERR_PROCEDURE_LOAD_FAILED 0x103a
#define PMERR_LIBRARY_DELETE_FAILED 0x103b
#define PMERR_PROCEDURE_DELETE_FAILED  0x103c
#define PMERR_ARRAY_TOO_LARGE    0x103d
#define PMERR_ARRAY_TOO_SMALL    0x103e
#define PMERR_DATATYPE_ENTRY_BAD_INDEX 0x103f
#define PMERR_DATATYPE_ENTRY_CTL_BAD   0x1040
#define PMERR_DATATYPE_ENTRY_CTL_MISS  0x1041
#define PMERR_DATATYPE_ENTRY_INVALID   0x1042
#define PMERR_DATATYPE_ENTRY_NOT_NUM   0x1043
#define PMERR_DATATYPE_ENTRY_NOT_OFF   0x1044
#define PMERR_DATATYPE_INVALID      0x1045
#define PMERR_DATATYPE_NOT_UNIQUE   0x1046
#define PMERR_DATATYPE_TOO_LONG     0x1047
#define PMERR_DATATYPE_TOO_SMALL 0x1048
#define PMERR_DIRECTION_INVALID     0x1049
#define PMERR_INVALID_HAB     0x104a
#define PMERR_INVALID_HSTRUCT    0x104d
#define PMERR_LENGTH_TOO_SMALL      0x104e
#define PMERR_MSGID_TOO_SMALL    0x104f
#define PMERR_NO_HANDLE_ALLOC    0x1050
#define PMERR_NOT_IN_A_PM_SESSION   0x1051
#define PMERR_MSG_QUEUE_ALREADY_EXISTS 0x1052
#define PMERR_SEND_MSG_TIMEOUT      0x1053
#define PMERROR_SEND_MSG_FAILED     0x1054
#define PMERR_OLD_RESOURCE    0x1055
#define PMERR_WPDSERVER_IS_ACTIVE   0x1056
#define PMERR_WPDSERVER_NOT_STARTED 0x1057
#define PMERR_SOMDD_IS_ACTIVE    0x1058
#define PMERR_SOMDD_NOT_STARTED     0x1059
#define PMERR_BIDI_FIRST      0x10f0
#define PMERR_BIDI_LAST       0x10ff
#define PMERR_INVALID_PIB     0x1101
#define PMERR_INSUFF_SPACE_TO_ADD   0x1102
#define PMERR_INVALID_GROUP_HANDLE  0x1103
#define PMERR_DUPLICATE_TITLE    0x1104
#define PMERR_INVALID_TITLE      0x1105
#define PMERR_HANDLE_NOT_IN_GROUP   0x1107
#define PMERR_INVALID_TARGET_HANDLE 0x1106
#define PMERR_INVALID_PATH_STATEMENT   0x1108
#define PMERR_NO_PROGRAM_FOUND      0x1109
#define PMERR_INVALID_BUFFER_SIZE   0x110a
#define PMERR_BUFFER_TOO_SMALL      0x110b
#define PMERR_PL_INITIALISATION_FAIL   0x110c
#define PMERR_CANT_DESTROY_SYS_GROUP   0x110d
#define PMERR_INVALID_TYPE_CHANGE   0x110e
#define PMERR_INVALID_PROGRAM_HANDLE   0x110f
#define PMERR_NOT_CURRENT_PL_VERSION   0x1110
#define PMERR_INVALID_CIRCULAR_REF  0x1111
#define PMERR_MEMORY_ALLOCATION_ERR 0x1112
#define PMERR_MEMORY_DEALLOCATION_ERR  0x1113
#define PMERR_TASK_HEADER_TOO_BIG   0x1114
#define PMERR_INVALID_INI_FILE_HANDLE  0x1115
#define PMERR_MEMORY_SHARE    0x1116
#define PMERR_OPEN_QUEUE      0x1117
#define PMERR_CREATE_QUEUE    0x1118
#define PMERR_WRITE_QUEUE     0x1119
#define PMERR_READ_QUEUE      0x111a
#define PMERR_CALL_NOT_EXECUTED     0x111b
#define PMERR_UNKNOWN_APIPKT     0x111c
#define PMERR_INITHREAD_EXISTS      0x111d
#define PMERR_CREATE_THREAD      0x111e
#define PMERR_NO_HK_PROFILE_INSTALLED  0x111f
#define PMERR_INVALID_DIRECTORY     0x1120
#define PMERR_WILDCARD_IN_FILENAME  0x1121
#define PMERR_FILENAME_BUFFER_FULL  0x1122
#define PMERR_FILENAME_TOO_LONG     0x1123
#define PMERR_INI_FILE_IS_SYS_OR_USER  0x1124
#define PMERR_BROADCAST_PLMSG    0x1125
#define PMERR_190_INIT_DONE      0x1126
#define PMERR_HMOD_FOR_PMSHAPI      0x1127
#define PMERR_SET_HK_PROFILE     0x1128
#define PMERR_API_NOT_ALLOWED    0x1129
#define PMERR_INI_STILL_OPEN     0x112a
#define PMERR_PROGDETAILS_NOT_IN_INI   0x112b
#define PMERR_PIBSTRUCT_NOT_IN_INI  0x112c
#define PMERR_INVALID_DISKPROGDETAILS  0x112d
#define PMERR_PROGDETAILS_READ_FAILURE 0x112e
#define PMERR_PROGDETAILS_WRITE_FAILURE 0x112f
#define PMERR_PROGDETAILS_QSIZE_FAILURE 0x1130
#define PMERR_INVALID_PROGDETAILS   0x1131
#define PMERR_SHEPROFILEHOOK_NOT_FOUND 0x1132
#define PMERR_190PLCONVERTED     0x1133
#define PMERR_FAILED_TO_CONVERT_INI_PL 0x1134
#define PMERR_PMSHAPI_NOT_INITIALISED  0x1135
#define PMERR_INVALID_SHELL_API_HOOK_ID 0x1136
#define PMERR_DOS_ERROR       0x1200
#define PMERR_NO_SPACE        0x1201
#define PMERR_INVALID_SWITCH_HANDLE 0x1202
#define PMERR_NO_HANDLE       0x1203
#define PMERR_INVALID_PROCESS_ID 0x1204
#define PMERR_NOT_SHELL       0x1205
#define PMERR_INVALID_WINDOW     0x1206
#define PMERR_INVALID_POST_MSG      0x1207
#define PMERR_INVALID_PARAMETERS 0x1208
#define PMERR_INVALID_PROGRAM_TYPE  0x1209
#define PMERR_NOT_EXTENDED_FOCUS 0x120a
#define PMERR_INVALID_SESSION_ID 0x120b
#define PMERR_SMG_INVALID_ICON_FILE 0x120c
#define PMERR_SMG_ICON_NOT_CREATED  0x120d
#define PMERR_SHL_DEBUG       0x120e
#define PMERR_OPENING_INI_FILE      0x1301
#define PMERR_INI_FILE_CORRUPT      0x1302
#define PMERR_INVALID_PARM    0x1303
#define PMERR_NOT_IN_IDX      0x1304
#define PMERR_NO_ENTRIES_IN_GROUP   0x1305
#define PMERR_INI_WRITE_FAIL     0x1306
#define PMERR_IDX_FULL        0x1307
#define PMERR_INI_PROTECTED      0x1308
#define PMERR_MEMORY_ALLOC    0x1309
#define PMERR_INI_INIT_ALREADY_DONE 0x130a
#define PMERR_INVALID_INTEGER    0x130b
#define PMERR_INVALID_ASCIIZ     0x130c
#define PMERR_CAN_NOT_CALL_SPOOLER  0x130d
#define PMERR_VALIDATION_REJECTED   0x130d /*!*/
#define PMERR_WARNING_WINDOW_NOT_KILLED 0x1401
#define PMERR_ERROR_INVALID_WINDOW  0x1402
#define PMERR_ALREADY_INITIALIZED   0x1403
#define PMERR_MSG_PROG_NO_MOU    0x1405
#define PMERR_MSG_PROG_NON_RECOV 0x1406
#define PMERR_WINCONV_INVALID_PATH  0x1407
#define PMERR_PI_NOT_INITIALISED 0x1408
#define PMERR_PL_NOT_INITIALISED 0x1409
#define PMERR_NO_TASK_MANAGER    0x140a
#define PMERR_SAVE_NOT_IN_PROGRESS  0x140b
#define PMERR_NO_STACK_SPACE     0x140c
#define PMERR_INVALID_COLR_FIELD 0x140d
#define PMERR_INVALID_COLR_VALUE 0x140e
#define PMERR_COLR_WRITE      0x140f
#define PMERR_TARGET_FILE_EXISTS 0x1501
#define PMERR_SOURCE_SAME_AS_TARGET 0x1502
#define PMERR_SOURCE_FILE_NOT_FOUND 0x1503
#define PMERR_INVALID_NEW_PATH      0x1504
#define PMERR_TARGET_FILE_NOT_FOUND 0x1505
#define PMERR_INVALID_DRIVE_NUMBER  0x1506
#define PMERR_NAME_TOO_LONG      0x1507
#define PMERR_NOT_ENOUGH_ROOM_ON_DISK  0x1508
#define PMERR_NOT_ENOUGH_MEM     0x1509
#define PMERR_LOG_DRV_DOES_NOT_EXIST   0x150b
#define PMERR_INVALID_DRIVE      0x150c
#define PMERR_ACCESS_DENIED      0x150d
#define PMERR_NO_FIRST_SLASH     0x150e
#define PMERR_READ_ONLY_FILE     0x150f
#define PMERR_GROUP_PROTECTED    0x151f
#define PMERR_INVALID_PROGRAM_CATEGORY 0x152f
#define PMERR_INVALID_APPL    0x1530
#define PMERR_CANNOT_START    0x1531
#define PMERR_STARTED_IN_BACKGROUND 0x1532
#define PMERR_INVALID_HAPP    0x1533
#define PMERR_CANNOT_STOP     0x1534
#define PMERR_INVALID_FREE_MESSAGE_ID  0x1630
#define PMERR_FUNCTION_NOT_SUPPORTED   0x1641
#define PMERR_INVALID_ARRAY_COUNT   0x1642
#define PMERR_INVALID_LENGTH     0x1643
#define PMERR_INVALID_BUNDLE_TYPE   0x1644
#define PMERR_INVALID_PARAMETER     0x1645
#define PMERR_INVALID_NUMBER_OF_PARMS  0x1646
#define PMERR_GREATER_THAN_64K      0x1647
#define PMERR_INVALID_PARAMETER_TYPE   0x1648
#define PMERR_NEGATIVE_STRCOND_DIM  0x1649
#define PMERR_INVALID_NUMBER_OF_TYPES  0x164a
#define PMERR_INCORRECT_HSTRUCT     0x164b
#define PMERR_INVALID_ARRAY_SIZE 0x164c
#define PMERR_INVALID_CONTROL_DATATYPE 0x164d
#define PMERR_INCOMPLETE_CONTROL_SEQU  0x164e
#define PMERR_INVALID_DATATYPE      0x164f
#define PMERR_INCORRECT_DATATYPE 0x1650
#define PMERR_NOT_SELF_DESCRIBING_DTYP 0x1651
#define PMERR_INVALID_CTRL_SEQ_INDEX   0x1652
#define PMERR_INVALID_TYPE_FOR_LENGTH  0x1653
#define PMERR_INVALID_TYPE_FOR_OFFSET  0x1654
#define PMERR_INVALID_TYPE_FOR_MPARAM  0x1655
#define PMERR_INVALID_MESSAGE_ID 0x1656
#define PMERR_C_LENGTH_TOO_SMALL 0x1657
#define PMERR_APPL_STRUCTURE_TOO_SMALL 0x1658
#define PMERR_INVALID_ERRORINFO_HANDLE 0x1659
#define PMERR_INVALID_CHARACTER_INDEX  0x165a
#define PMERR_OK        0x0000
#define PMERR_ALREADY_IN_AREA    0x2001
#define PMERR_ALREADY_IN_ELEMENT 0x2002
#define PMERR_ALREADY_IN_PATH    0x2003
#define PMERR_ALREADY_IN_SEG     0x2004
#define PMERR_AREA_INCOMPLETE    0x2005
#define PMERR_BASE_ERROR      0x2006
#define PMERR_BITBLT_LENGTH_EXCEEDED   0x2007
#define PMERR_BITMAP_IN_USE      0x2008
#define PMERR_BITMAP_IS_SELECTED 0x2009
#define PMERR_BITMAP_NOT_FOUND      0x200a
#define PMERR_BITMAP_NOT_SELECTED   0x200b
#define PMERR_BOUNDS_OVERFLOW    0x200c
#define PMERR_CALLED_SEG_IS_CHAINED 0x200d
#define PMERR_CALLED_SEG_IS_CURRENT 0x200e
#define PMERR_CALLED_SEG_NOT_FOUND  0x200f
#define PMERR_CANNOT_DELETE_ALL_DATA   0x2010
#define PMERR_CANNOT_REPLACE_ELEMENT_0 0x2011
#define PMERR_COL_TABLE_NOT_REALIZABLE 0x2012
#define PMERR_COL_TABLE_NOT_REALIZED   0x2013
#define PMERR_COORDINATE_OVERFLOW   0x2014
#define PMERR_CORR_FORMAT_MISMATCH  0x2015
#define PMERR_DATA_TOO_LONG      0x2016
#define PMERR_DC_IS_ASSOCIATED      0x2017
#define PMERR_DESC_STRING_TRUNCATED 0x2018
#define PMERR_DEVICE_DRIVER_ERROR_1 0x2019
#define PMERR_DEVICE_DRIVER_ERROR_2 0x201a
#define PMERR_DEVICE_DRIVER_ERROR_3 0x201b
#define PMERR_DEVICE_DRIVER_ERROR_4 0x201c
#define PMERR_DEVICE_DRIVER_ERROR_5 0x201d
#define PMERR_DEVICE_DRIVER_ERROR_6 0x201e
#define PMERR_DEVICE_DRIVER_ERROR_7 0x201f
#define PMERR_DEVICE_DRIVER_ERROR_8 0x2020
#define PMERR_DEVICE_DRIVER_ERROR_9 0x2021
#define PMERR_DEVICE_DRIVER_ERROR_10   0x2022
#define PMERR_DEV_FUNC_NOT_INSTALLED   0x2023
#define PMERR_DOSOPEN_FAILURE    0x2024
#define PMERR_DOSREAD_FAILURE    0x2025
#define PMERR_DRIVER_NOT_FOUND      0x2026
#define PMERR_DUP_SEG         0x2027
#define PMERR_DYNAMIC_SEG_SEQ_ERROR 0x2028
#define PMERR_DYNAMIC_SEG_ZERO_INV  0x2029
#define PMERR_ELEMENT_INCOMPLETE 0x202a
#define PMERR_ESC_CODE_NOT_SUPPORTED   0x202b
#define PMERR_EXCEEDS_MAX_SEG_LENGTH   0x202c
#define PMERR_FONT_AND_MODE_MISMATCH   0x202d
#define PMERR_FONT_FILE_NOT_LOADED  0x202e
#define PMERR_FONT_NOT_LOADED    0x202f
#define PMERR_FONT_TOO_BIG    0x2030
#define PMERR_HARDWARE_INIT_FAILURE 0x2031
#define PMERR_HBITMAP_BUSY    0x2032
#define PMERR_HDC_BUSY        0x2033
#define PMERR_HRGN_BUSY       0x2034
#define PMERR_HUGE_FONTS_NOT_SUPPORTED 0x2035
#define PMERR_ID_HAS_NO_BITMAP      0x2036
#define PMERR_IMAGE_INCOMPLETE      0x2037
#define PMERR_INCOMPAT_COLOR_FORMAT 0x2038
#define PMERR_INCOMPAT_COLOR_OPTIONS   0x2039
#define PMERR_INCOMPATIBLE_BITMAP   0x203a
#define PMERR_INCOMPATIBLE_METAFILE 0x203b
#define PMERR_INCORRECT_DC_TYPE     0x203c
#define PMERR_INSUFFICIENT_DISK_SPACE  0x203d
#define PMERR_INSUFFICIENT_MEMORY   0x203e
#define PMERR_INV_ANGLE_PARM     0x203f
#define PMERR_INV_ARC_CONTROL    0x2040
#define PMERR_INV_AREA_CONTROL      0x2041
#define PMERR_INV_ARC_POINTS     0x2042
#define PMERR_INV_ATTR_MODE      0x2043
#define PMERR_INV_BACKGROUND_COL_ATTR  0x2044
#define PMERR_INV_BACKGROUND_MIX_ATTR  0x2045
#define PMERR_INV_BITBLT_MIX     0x2046
#define PMERR_INV_BITBLT_STYLE      0x2047
#define PMERR_INV_BITMAP_DIMENSION  0x2048
#define PMERR_INV_BOX_CONTROL    0x2049
#define PMERR_INV_BOX_ROUNDING_PARM 0x204a
#define PMERR_INV_CHAR_ANGLE_ATTR   0x204b
#define PMERR_INV_CHAR_DIRECTION_ATTR  0x204c
#define PMERR_INV_CHAR_MODE_ATTR 0x204d
#define PMERR_INV_CHAR_POS_OPTIONS  0x204e
#define PMERR_INV_CHAR_SET_ATTR     0x204f
#define PMERR_INV_CHAR_SHEAR_ATTR   0x2050
#define PMERR_INV_CLIP_PATH_OPTIONS 0x2051
#define PMERR_INV_CODEPAGE    0x2052
#define PMERR_INV_COLOR_ATTR     0x2053
#define PMERR_INV_COLOR_DATA     0x2054
#define PMERR_INV_COLOR_FORMAT      0x2055
#define PMERR_INV_COLOR_INDEX    0x2056
#define PMERR_INV_COLOR_OPTIONS     0x2057
#define PMERR_INV_COLOR_START_INDEX 0x2058
#define PMERR_INV_COORD_OFFSET      0x2059
#define PMERR_INV_COORD_SPACE    0x205a
#define PMERR_INV_COORDINATE     0x205b
#define PMERR_INV_CORRELATE_DEPTH   0x205c
#define PMERR_INV_CORRELATE_TYPE 0x205d
#define PMERR_INV_CURSOR_BITMAP     0x205e
#define PMERR_INV_DC_DATA     0x205f
#define PMERR_INV_DC_TYPE     0x2060
#define PMERR_INV_DEVICE_NAME    0x2061
#define PMERR_INV_DEV_MODES_OPTIONS 0x2062
#define PMERR_INV_DRAW_CONTROL      0x2063
#define PMERR_INV_DRAW_VALUE     0x2064
#define PMERR_INV_DRAWING_MODE      0x2065
#define PMERR_INV_DRIVER_DATA    0x2066
#define PMERR_INV_DRIVER_NAME    0x2067
#define PMERR_INV_DRAW_BORDER_OPTION   0x2068
#define PMERR_INV_EDIT_MODE      0x2069
#define PMERR_INV_ELEMENT_OFFSET 0x206a
#define PMERR_INV_ELEMENT_POINTER   0x206b
#define PMERR_INV_END_PATH_OPTIONS  0x206c
#define PMERR_INV_ESC_CODE    0x206d
#define PMERR_INV_ESCAPE_DATA    0x206e
#define PMERR_INV_EXTENDED_LCID     0x206f
#define PMERR_INV_FILL_PATH_OPTIONS 0x2070
#define PMERR_INV_FIRST_CHAR     0x2071
#define PMERR_INV_FONT_ATTRS     0x2072
#define PMERR_INV_FONT_FILE_DATA 0x2073
#define PMERR_INV_FOR_THIS_DC_TYPE  0x2074
#define PMERR_INV_FORMAT_CONTROL 0x2075
#define PMERR_INV_FORMS_CODE     0x2076
#define PMERR_INV_FONTDEF     0x2077
#define PMERR_INV_GEOM_LINE_WIDTH_ATTR 0x2078
#define PMERR_INV_GETDATA_CONTROL   0x2079
#define PMERR_INV_GRAPHICS_FIELD 0x207a
#define PMERR_INV_HBITMAP     0x207b
#define PMERR_INV_HDC         0x207c
#define PMERR_INV_HJOURNAL    0x207d
#define PMERR_INV_HMF         0x207e
#define PMERR_INV_HPS         0x207f
#define PMERR_INV_HRGN        0x2080
#define PMERR_INV_ID       0x2081
#define PMERR_INV_IMAGE_DATA_LENGTH 0x2082
#define PMERR_INV_IMAGE_DIMENSION   0x2083
#define PMERR_INV_IMAGE_FORMAT      0x2084
#define PMERR_INV_IN_AREA     0x2085
#define PMERR_INV_IN_CALLED_SEG     0x2086
#define PMERR_INV_IN_CURRENT_EDIT_MODE 0x2087
#define PMERR_INV_IN_DRAW_MODE      0x2088
#define PMERR_INV_IN_ELEMENT     0x2089
#define PMERR_INV_IN_IMAGE    0x208a
#define PMERR_INV_IN_PATH     0x208b
#define PMERR_INV_IN_RETAIN_MODE 0x208c
#define PMERR_INV_IN_SEG      0x208d
#define PMERR_INV_IN_VECTOR_SYMBOL  0x208e
#define PMERR_INV_INFO_TABLE     0x208f
#define PMERR_INV_JOURNAL_OPTION 0x2090
#define PMERR_INV_KERNING_FLAGS     0x2091
#define PMERR_INV_LENGTH_OR_COUNT   0x2092
#define PMERR_INV_LINE_END_ATTR     0x2093
#define PMERR_INV_LINE_JOIN_ATTR 0x2094
#define PMERR_INV_LINE_TYPE_ATTR 0x2095
#define PMERR_INV_LINE_WIDTH_ATTR   0x2096
#define PMERR_INV_LOGICAL_ADDRESS   0x2097
#define PMERR_INV_MARKER_BOX_ATTR   0x2098
#define PMERR_INV_MARKER_SET_ATTR   0x2099
#define PMERR_INV_MARKER_SYMBOL_ATTR   0x209a
#define PMERR_INV_MATRIX_ELEMENT 0x209b
#define PMERR_INV_MAX_HITS    0x209c
#define PMERR_INV_METAFILE    0x209d
#define PMERR_INV_METAFILE_LENGTH   0x209e
#define PMERR_INV_METAFILE_OFFSET   0x209f
#define PMERR_INV_MICROPS_DRAW_CONTROL 0x20a0
#define PMERR_INV_MICROPS_FUNCTION  0x20a1
#define PMERR_INV_MICROPS_ORDER     0x20a2
#define PMERR_INV_MIX_ATTR    0x20a3
#define PMERR_INV_MODE_FOR_OPEN_DYN 0x20a4
#define PMERR_INV_MODE_FOR_REOPEN_SEG  0x20a5
#define PMERR_INV_MODIFY_PATH_MODE  0x20a6
#define PMERR_INV_MULTIPLIER     0x20a7
#define PMERR_INV_NESTED_FIGURES 0x20a8
#define PMERR_INV_OR_INCOMPAT_OPTIONS  0x20a9
#define PMERR_INV_ORDER_LENGTH      0x20aa
#define PMERR_INV_ORDERING_PARM     0x20ab
#define PMERR_INV_OUTSIDE_DRAW_MODE 0x20ac
#define PMERR_INV_PAGE_VIEWPORT     0x20ad
#define PMERR_INV_PATH_ID     0x20ae
#define PMERR_INV_PATH_MODE      0x20af
#define PMERR_INV_PATTERN_ATTR      0x20b0
#define PMERR_INV_PATTERN_REF_PT_ATTR  0x20b1
#define PMERR_INV_PATTERN_SET_ATTR  0x20b2
#define PMERR_INV_PATTERN_SET_FONT  0x20b3
#define PMERR_INV_PICK_APERTURE_OPTION 0x20b4
#define PMERR_INV_PICK_APERTURE_POSN   0x20b5
#define PMERR_INV_PICK_APERTURE_SIZE   0x20b6
#define PMERR_INV_PICK_NUMBER    0x20b7
#define PMERR_INV_PLAY_METAFILE_OPTION 0x20b8
#define PMERR_INV_PRIMITIVE_TYPE 0x20b9
#define PMERR_INV_PS_SIZE     0x20ba
#define PMERR_INV_PUTDATA_FORMAT 0x20bb
#define PMERR_INV_QUERY_ELEMENT_NO  0x20bc
#define PMERR_INV_RECT        0x20bd
#define PMERR_INV_REGION_CONTROL 0x20be
#define PMERR_INV_REGION_MIX_MODE   0x20bf
#define PMERR_INV_REPLACE_MODE_FUNC 0x20c0
#define PMERR_INV_RESERVED_FIELD 0x20c1
#define PMERR_INV_RESET_OPTIONS     0x20c2
#define PMERR_INV_RGBCOLOR    0x20c3
#define PMERR_INV_SCAN_START     0x20c4
#define PMERR_INV_SEG_ATTR    0x20c5
#define PMERR_INV_SEG_ATTR_VALUE 0x20c6
#define PMERR_INV_SEG_CH_LENGTH     0x20c7
#define PMERR_INV_SEG_NAME    0x20c8
#define PMERR_INV_SEG_OFFSET     0x20c9
#define PMERR_INV_SETID       0x20ca
#define PMERR_INV_SETID_TYPE     0x20cb
#define PMERR_INV_SET_VIEWPORT_OPTION  0x20cc
#define PMERR_INV_SHARPNESS_PARM 0x20cd
#define PMERR_INV_SOURCE_OFFSET     0x20ce
#define PMERR_INV_STOP_DRAW_VALUE   0x20cf
#define PMERR_INV_TRANSFORM_TYPE 0x20d0
#define PMERR_INV_USAGE_PARM     0x20d1
#define PMERR_INV_VIEWING_LIMITS 0x20d2
#define PMERR_JFILE_BUSY      0x20d3
#define PMERR_JNL_FUNC_DATA_TOO_LONG   0x20d4
#define PMERR_KERNING_NOT_SUPPORTED 0x20d5
#define PMERR_LABEL_NOT_FOUND    0x20d6
#define PMERR_MATRIX_OVERFLOW    0x20d7
#define PMERR_METAFILE_INTERNAL_ERROR  0x20d8
#define PMERR_METAFILE_IN_USE    0x20d9
#define PMERR_METAFILE_LIMIT_EXCEEDED  0x20da
#define PMERR_NAME_STACK_FULL    0x20db
#define PMERR_NOT_CREATED_BY_DEVOPENDC 0x20dc
#define PMERR_NOT_IN_AREA     0x20dd
#define PMERR_NOT_IN_DRAW_MODE      0x20de
#define PMERR_NOT_IN_ELEMENT     0x20df
#define PMERR_NOT_IN_IMAGE    0x20e0
#define PMERR_NOT_IN_PATH     0x20e1
#define PMERR_NOT_IN_RETAIN_MODE 0x20e2
#define PMERR_NOT_IN_SEG      0x20e3
#define PMERR_NO_BITMAP_SELECTED 0x20e4
#define PMERR_NO_CURRENT_ELEMENT 0x20e5
#define PMERR_NO_CURRENT_SEG     0x20e6
#define PMERR_NO_METAFILE_RECORD_HANDLE 0x20e7
#define PMERR_ORDER_TOO_BIG      0x20e8
#define PMERR_OTHER_SET_ID_REFS     0x20e9
#define PMERR_OVERRAN_SEG     0x20ea
#define PMERR_OWN_SET_ID_REFS    0x20eb
#define PMERR_PATH_INCOMPLETE    0x20ec
#define PMERR_PATH_LIMIT_EXCEEDED   0x20ed
#define PMERR_PATH_UNKNOWN    0x20ee
#define PMERR_PEL_IS_CLIPPED     0x20ef
#define PMERR_PEL_NOT_AVAILABLE     0x20f0
#define PMERR_PRIMITIVE_STACK_EMPTY 0x20f1
#define PMERR_PROLOG_ERROR    0x20f2
#define PMERR_PROLOG_SEG_ATTR_NOT_SET  0x20f3
#define PMERR_PS_BUSY         0x20f4
#define PMERR_PS_IS_ASSOCIATED      0x20f5
#define PMERR_RAM_JNL_FILE_TOO_SMALL   0x20f6
#define PMERR_REALIZE_NOT_SUPPORTED 0x20f7
#define PMERR_REGION_IS_CLIP_REGION 0x20f8
#define PMERR_RESOURCE_DEPLETION 0x20f9
#define PMERR_SEG_AND_REFSEG_ARE_SAME  0x20fa
#define PMERR_SEG_CALL_RECURSIVE 0x20fb
#define PMERR_SEG_CALL_STACK_EMPTY  0x20fc
#define PMERR_SEG_CALL_STACK_FULL   0x20fd
#define PMERR_SEG_IS_CURRENT     0x20fe
#define PMERR_SEG_NOT_CHAINED    0x20ff
#define PMERR_SEG_NOT_FOUND      0x2100
#define PMERR_SEG_STORE_LIMIT_EXCEEDED 0x2101
#define PMERR_SETID_IN_USE    0x2102
#define PMERR_SETID_NOT_FOUND    0x2103
#define PMERR_STARTDOC_NOT_ISSUED   0x2104
#define PMERR_STOP_DRAW_OCCURRED 0x2105
#define PMERR_TOO_MANY_METAFILES_IN_USE 0x2106
#define PMERR_TRUNCATED_ORDER    0x2107
#define PMERR_UNCHAINED_SEG_ZERO_INV   0x2108
#define PMERR_UNSUPPORTED_ATTR      0x2109
#define PMERR_UNSUPPORTED_ATTR_VALUE   0x210a
#define PMERR_ENDDOC_NOT_ISSUED     0x210b
#define PMERR_PS_NOT_ASSOCIATED     0x210c
#define PMERR_INV_FLOOD_FILL_OPTIONS   0x210d
#define PMERR_INV_FACENAME    0x210e
#define PMERR_PALETTE_SELECTED      0x210f
#define PMERR_NO_PALETTE_SELECTED   0x2110
#define PMERR_INV_HPAL        0x2111
#define PMERR_PALETTE_BUSY    0x2112
#define PMERR_START_POINT_CLIPPED   0x2113
#define PMERR_NO_FILL         0x2114
#define PMERR_INV_FACENAMEDESC      0x2115
#define PMERR_INV_BITMAP_DATA    0x2116
#define PMERR_INV_CHAR_ALIGN_ATTR   0x2117
#define PMERR_INV_HFONT       0x2118
#define PMERR_HFONT_IS_SELECTED     0x2119
#define PMERR_DRVR_NOT_SUPPORTED 0x2120
#define PMERR_INV_INKPS_FUNCTION 0x2121

