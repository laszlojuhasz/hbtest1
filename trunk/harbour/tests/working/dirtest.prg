// directory test
function main(filespec,attribs)

local adir := {}
local x := 0

adir := directory(filespec,attribs)

for x := 1 to len(adir)
   qout(adir[x,1],"|", adir[x,2],"|", adir[x,3],"|", adir[x,4],"|", adir[x,5])
next x

return nil

