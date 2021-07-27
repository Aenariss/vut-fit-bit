/**
* GoodsShelf
* GoodsShelf reprezentuje regál a zajišťuje operace s položkami v něm.
* Inspirováno domácím úkolem č.1.
*
* @author Vojtěch Fiala <xfiala61>
*/

package src.shelf_manipulation.goods;


/**
 * Goods GoodsShelf reprezentuje regál a zajišťuje operace s položkami v něm.
 * Inspirováno domácím úkolem č.1.
 */
public interface GoodsShelf {

    /**
    * put
    * Vloží položku do regálu.
    * @param var1 Konkrétní položka správného typu.
    */
    void put(GoodsItem var1);

    /**
    * containsGoods
    * Metoda pro zjištění, zda-li položka obsahuje daný typ zboží.
    * @param var1 Typ zboží.
    * @return True/False v závislosti na výsledku.
    */
    boolean containsGoods(Goods var1);

    /**
    * removeAny
    * Metoda pro odstranění položky z regálu.
    * @return Odstraněná položka.
    */
    GoodsItem removeAny();

    /**
    * size
    * Metoda pro zjištění počtu položek v regálu.
    * @return Počet položek.
    */
    int size();

    /**
    * getShelfType
    * Metoda pro zjištění názvu typu zboží, které se do regálu ukládá.
    * @return Typ zboží.
    */
    String getShelfType();

}
