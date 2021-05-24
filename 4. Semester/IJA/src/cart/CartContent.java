/**
* Cart Content
* Cart Content pokrývá funkce sloužící pro práci
* s nákladem vozíku.
*
* @author Vojtěch Fiala <xfiala61>
*/

package src.cart;

import src.shelf_manipulation.goods.GoodsItem;
import src.shelf_manipulation.goods.GoodsShelf;


/**
 * CartContent je třída pracující nad operacemi s obsahem vozíku
 */
public class CartContent {

    // Soucasne zbozi a police
    private GoodsItem ware = null;
    private GoodsShelf shelf = null;


    /**
     * getWare
     * Metoda pro získání aktuální položky na vozíku.
     * @return GoodsItem Aktuální položka na vozíku.
     */
    public GoodsItem getWare() {
        return this.ware;
    }

    /**
     * setShelf
     * Metoda pro nastavení zpracovávané police.
     * @param shelf Police, se kterou vozík pracuje.
     */
    public void setShelf(GoodsShelf shelf) {
        this.shelf = shelf;
    }

    /**
     * loadItem
     * Metoda pro odebrání položky z police a naložení na vozík.
     */
    public void loadItem() {
        if (this.shelf.size() > 0)
            this.ware = shelf.removeAny();
        else
            throw new ArithmeticException("V regalu neni dostatek zbozi!");
    }   

    /**
     * unloadItem
     * Metoda pro odebrání položky z vozíku a její "prodej" (celkové odebrání položky ze skladu).
     */
    public void unloadItem() {
        this.ware.sell();
        this.ware = null;
    }
}
