@rem
@rem $Id$
@rem

@rem ---------------------------------------------------------------
@rem This is a generic template file, if it doesn't fit your own needs
@rem please DON'T MODIFY IT.
@rem
@rem Instead, make a local copy and modify that one, or make a call to
@rem this batch file from your customized one. [vszakats]
@rem ---------------------------------------------------------------

@set HB_ARCHITECTURE=win
@set HB_COMPILER=owatcom

@if     "%OS%" == "Windows_NT" call "%~dp0hbmk.bat" %*
@if not "%OS%" == "Windows_NT" call hbmk.bat %1 %2 %3 %4 %5 %6 %7 %8 %9
