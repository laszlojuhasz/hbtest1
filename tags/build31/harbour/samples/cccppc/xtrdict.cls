//
// $Id$
//

//*******************************************************************
// xtrdict.cls: #xtranslate sz�t�r oszt�ly.
// 1999, Csisz�r Levente

class XTRDICT OBJECT

   // Egyenl�re ezek EXTRDICT-ekb�l �ll� t�mb�k.
   attrib cmdict  // Az xcommand-ok sz�t�ra.
   attrib trdict  // Az xtranslate-ek sz�t�ra.

   attrib cmdictTree  // Az xcommand-ok f�ja.
   attrib trdictTree  // Az xtranslate-ek f�ja.
   
   nmethod add(extrdict)
   // Felvesz egy extrdict objektumot a sz�t�rba.
   
   nmethod printStr(printBlock,xcmd)
   // A sz�t�r elemeket string-k�nt ki�rhat� form�ban �tadja a 
   // printBlock-nak.
   // Ha a printBlock nincs megadva, akkor string-k�nt visszaadja.
   // Ha az xcmd nem �res, akkor az xcommand-okat adja.
   
   rmethod oinit()

   nmethod getExtrList(item,xcmd)
   // Ad egy list�t azokr�l az extrdict-ekr�l, amiknek az els� 
   // tokene illeszkedik az item-re.
   // Ha az xcmd nem �res, akkor az xcommand-okat adja.

