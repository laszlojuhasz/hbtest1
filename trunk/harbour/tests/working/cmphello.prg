//
// Compile Hello
//
// This program compiles hello.prg
//
function Main()

   local cOs := Upper( OS() )

   QOut( "About to compile Hello.prg" )
   QOut()
   if at( "WINDOWS", cOs ) != 0 .or. at( "DOS", cOs ) != 0 .or. ;
      at( "OS/2", cOs )                         // OS/2, DOS, Windows version
      __Run( "..\..\bin\harbour.exe hello.prg /gHRB" )
   else                                         // Unix / Linux version
      __Run( "../../bin/harbour.exe hello.prg /gHRB" )
   endif
   QOut( "Finished compiling" )
return nil
