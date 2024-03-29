//
// $Id$
//

class META BEHAVIOR
// Ennek az oszt�lynak az elemei metaoszt�lyok.


   attrib ofClass  
   // Ha ennek az metaoszt�lynak csak egy eleme lehet, akkor
   // ez egy blokk, amit ki�rt�kelve megkapjuk azt az egy
   // oszt�lyt. Ha ennek a metaoszt�lynak t�bb eleme is lehet,
   // akkor ez nil.

   rmethod oinit{me_oinit}(parent,name,nAttribs,nMethods,amBlock,methodsBlock)
   // Inicializ�lja a 'this' metaoszt�lyt.
   // Mj.: H�vja a Behavior oinit()-j�t, �s inicializ�lja
   // az ofClass-t 'nil'-re.

   rmethod onew{me_onew}(parent,name,nAttribs,nMethods,amBlock,methodsBlock)
   // L�trehoz egy �j oszt�lyt, �s inicializ�lja.
   // Az �j oszt�ly a 'this' eleme lesz, teh�t az �j oszt�ly
   // oszt�lya a 'this'lesz.
   // Ha az ofClass nem �res, akkor a 'this' meta oszt�lynak m�r
   // van eleme. Egy (k�z�ns�ges) metaoszt�lynak csak egy eleme
   // lehet, �gy ez hiba!
   // Ha az ofClass �res, akkor az �j oszt�lyt oda be�rja.

   // Mj.: Ez a 'Meta' egy elem�nek a m�velete. A 'Meta' elemei
   // pedig meta oszt�lyok. Teh�t ez egy meta osztaly egy elem�t
   // hozza l�tre, a meta oszt�lyok elemei pedig oszt�lyok. Teh�t
   // ez egy �j oszt�lyt (�s nem metaoszt�lyt!!!) hoz l�tre.

   rmethod ocreate{me_ocreate}()                             
   // L�trehoz egy �j oszt�lyt, de nem inicializ�lja.

   
   // nmethod makeClassId{me_makeClassId}(aClass)
   // Hozz�ad egy oszt�lyt az idClass-okhoz. Az oszt�ly id-j�vel
   // t�r vissza. Ha az oszt�lynak m�r van id-je, akkor azzal t�r
   // vissza.

   // nmethod classFromId{me_classFromId}(id)
   // Ha van olyan oszt�ly, aminek az 'id' az id-je, akkor
   // azt adja egy�bk�nt nil-t.

   // nmethod findClassId{me_findClassId}(aClass)
   // Ha az oszt�lynak van id-je, akkor azt adja, egy�bk�nt nil-t.
