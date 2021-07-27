/**
* HashIndexFinder
* HashIndexFinder hledá index prvku v HashMap podle jeho hodnoty.
* 
* @author Vojtěch Fiala <xfiala61>
*/

package src.utils;
import java.util.HashMap;
import java.util.Map;
import src.shelf_manipulation.goods.*;
import src.utils.CoordsConverter;
import src.map_manipulation.MapControl;

/**
 * Array HashIndexFinder hledá index prvku v HashMap podle jeho hodnoty.
 */
public class HashIndexFinder {

    private CoordsConverter cc = new CoordsConverter();
    private MapControl mp = new MapControl();

    // inspirovano https://stackoverflow.com/a/9009709

    /**
    * getIndex
    * Najde v mapě index prvku podle jeho hodnoty.
    * @param regal Mapa reprezentující regál.
    * @param searched Hodnota prvku, jehož index se hledá.
    * @return Index prvku v textové podobě.
    */
    public String getDestination(HashMap<String, GoodsShelf> regal, String searched) {
        for (Map.Entry<String, GoodsShelf> entry : regal.entrySet()) {
            String key = entry.getKey();
            String value = entry.getValue().getShelfType();

            if (value == searched) {
                int[] crds =  cc.coordsInt(key);
                if (mp.exists(crds[0],crds[1]+1)) {
                    if (mp.getItem(crds[0], crds[1] + 1) == 1) {
                        key = cc.convertCoords(crds[0], crds[1]+1);
                    }
                }
                else if (mp.exists(crds[0],crds[1]-1)) {
                    if (mp.getItem(crds[0], crds[1] - 1) == 1) {
                        key = cc.convertCoords(crds[0], crds[1]-1);
                    }
                }
                else {
                    key = "error";
                }
                return key;
            }
        }
        return "error";
    }

    public String getIndex(HashMap<String, GoodsShelf> regal, String searched) {
        for (Map.Entry<String, GoodsShelf> entry : regal.entrySet()) {
            String key = entry.getKey();
            String value = entry.getValue().getShelfType();

            if (value == searched) {
                return key;
            }
        }
        return null;
    }
}
