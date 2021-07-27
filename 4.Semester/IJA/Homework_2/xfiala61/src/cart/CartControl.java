/**
* Cart Control
* Cart Content pokrývá funkce sloužící pro pohyb
* s vozíkem a hledání ideální cesty.
*
* @author Vojtěch Fiala <xfiala61>
*/

package src.cart;

import src.map_manipulation.MapControl;
import src.utils.CoordsConverter;

/**
 * CartControl je třída pracující nad operacemi s pohybem vozíku
 */
public class CartControl {

    private int position_x;
    private int position_y;   
    private int start_x;
    private int start_y;
    private int last_position_orig;
    private static MapControl map = new MapControl();
    private CoordsConverter cnv = new CoordsConverter();
    private PathFinder dk;

    /**
    * CartControl Inicializace
    * Nastaví atributy, umístí na mapu vozík a zazálohuje
    * originální hodnotu v mapě na místě vozíku.
    * @param beginning Počáteční poloha v "zakódované" String formě
    */
    public CartControl(String beginning) {
        int[] pair = cnv.coordsInt(beginning);
        this.position_x = pair[0];
        this.position_y = pair[1];
        this.start_x = this.position_x;
        this.start_y = this.position_y;
        this.last_position_orig = map.getItem(this.position_x, this.position_y);
        if (this.last_position_orig == 0 || this.last_position_orig == 1) {
            map.setItem(this.position_x, this.position_y, 7);
        }
        else {
            throw new ArithmeticException("Na tuto pozici nelze vozik umistit!");
        }
    }

    /**
    * gotoPosition
    * Metoda přesune vozík na určenou pozici a zazálohuje původní hodnotu místa.
    * @param x Poloha X
    * @param y Poloha Y
    */
    public void gotoPosition(int x, int y) {
        map.setItem(this.position_x, this.position_y, this.last_position_orig);
        this.position_x = x;
        this.position_y = y;
        this.start_x = this.position_x;
        this.start_y = this.position_y;
        this.cartMoveCheck();
    }

    private void cartMoveCheck() {
        if (this.last_position_orig == 0 || this.last_position_orig == 1) {
            this.last_position_orig = map.getItem(this.position_x, this.position_y);
            map.setItem(this.position_x, this.position_y, 7);
        }
        else {
            throw new ArithmeticException("Na tuto pozici nelze vozik umistit!");
        }
    }

    /**
    * findPath
    * Metoda najde nejkratší cestu mezi začátkem a destinací.
    * @param dst_x Souřadnice X cíle, k němuž nejkratší cestu chceme.
    * @param dst_y Souřadnice Y cíle, k němuž nejkratší cestu chceme.
    * @return Nejkratší cesta k cíli ve formě arraye prvků typu String představující souřadnice
    */
    public String[] findPath(int dst_x, int dst_y) {
        this.dk = new PathFinder(this.map.getMap(), this.start_x, this.start_y, dst_x, dst_y);
        return this.dk.Dijkstra();
    }

    /**
    * getDistance
    * Metoda získá vzdálenost mezi počátkem a cílem.
    * @param key Zakódované souřadnice cíle, k němuž vzdálenost chceme.
    * @return Vzdálenost k cíli
    */
    public int getDistance(String key) {
        return this.dk.getDistance(key);
    }

    /**
    * printPath
    * Metoda vypíše nejkrátší cestu mezi 2 body.
    * @param path Cesta, která se má vypsat.
    */
    public void printPath(String[] path) {
        int z = 0;
        for (int i = path.length; i > 0; i--) {
            System.out.printf("Krok %d: %s\n", z++, path[i-1]);
        }
    }

    /**
    * getStart
    * Metoda vrátí počáteční pozici vozíku.
    * @return Zakódované souřadnice vozíku.
    */
    public String getStart() {
        String pos = cnv.convertCoords(this.start_x, this.start_y);
        return pos;
    }
}
