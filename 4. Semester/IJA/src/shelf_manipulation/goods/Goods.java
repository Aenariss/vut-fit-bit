/**
* Goods
* Goods představují rozhraní typu zboží.
* Inspirováno domácím úkolem č.1.
* 
* @author Vojtěch Fiala <xfiala61>
*/

package src.shelf_manipulation.goods;
import java.time.LocalDate;

/**
 * Goods představují rozhraní typu zboží.
 * Inspirováno domácím úkolem č.1.
 */
public interface Goods {

    /**
    * getName
    * Metoda pro získání názvu typu zboží.
    * @return Název typu zboží.
    */
    String getName();

    /**
    * addItem
    * Přidá konkrétní položku pod tento typ zboží.
    * @param var1 Konkrétní položka.
    * @return Výsledek operace.
    */
    boolean addItem(GoodsItem var1);

    /**
    * newItem
    * Vytvoří novou konkrétní položku tohoto typu zboží.
    * @param var1 Současný čas typu LocalDate.
    * @return Odkaz na vytvořený prvek.
    */
    GoodsItem newItem(LocalDate var1);

    /**
    * remove
    * Odstraní konkrétní položku z tohoto typu zboží.
    * @param var1 Konkrétní položka.
    * @return Výsledek operace.
    */
    boolean remove(GoodsItem var1);

    /**
    * empty
    * Zjistí, zda-li nějaká položka tohoto typu existuje.
    * @return Výsledek operace.
    */
    boolean empty();

    /**
    * size
    * Zjistí počet konkrétních položek typu.
    * @return Počet položek.
    */
    int size();
}
