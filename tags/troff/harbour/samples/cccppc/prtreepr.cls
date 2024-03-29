//
// $Id$
//

//*******************************************************************
// prtreepr.cls: Parser tree parser. Egy input folyamot egy elemz�
//               f�val elemz� objektum
// 1999, Csisz�r Levente

// Egy elemz� f�t lehet vele fel�p�teni �s kezelni.

class PRTREEPR OBJECT

   attrib eprtree  // A prtree objektum.
   attrib subtree  // Amit �ppen elemz�nk.
   
   attrib result   // Ha az elemz�s sikeres, akkor a megtal�lt 
                   // <terminal>-ban lev� result lesz itt.
                   
   attrib itemArray  // A put()-al betett itemek.
                     // A wordLen ut�ni itemek az el�reolvasott
                     // itemek.
   attrib wordLen    // Ha az elemz�s meg�ll, akkor itt a megtal�lt
                     // sz� hossza van (sikeress�g eset�n), vagy 0,
                     // sikertelens�g eset�n.

   // Ezekhez l�p vissza, ha az elemz�s elakad.
   attrib lastStopWordLen // A legutols� illeszked� sz� hossza.
   // attrib lastStopResult  // A legutols� illeszked� sz�hoz tartoz�
                          // result.
   
   nmethod start()

   nmethod put(item)
   
   rmethod oinit(eprtree)
   
metaclass

   rmethod onew(eprtree)

