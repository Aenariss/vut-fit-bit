/**
* GoodsItem
* GoodsItem představují konkrétní položku zboží daného typu.
* Inspirováno domácím úkolem č.1.
*
* @author Vojtěch Fiala <xfiala61>
*/

package src.shelf_manipulation.goods;

/**
 * Goods GoodsItem představují konkrétní položku zboží daného typu.
 * Inspirováno domácím úkolem č.1.
 */
public interface GoodsItem {

    /**
    * goods
    * Metoda pro získání typu konkrétní položky.
    * @return Typu zboží.
    */
    Goods goods();

    /**
    * goods
    * Metoda pro "prodání" konkrétní položky - odstraní tuto položku ze skladu.
    * @return True/False Výsledek operace.
    */
    boolean sell();
}
