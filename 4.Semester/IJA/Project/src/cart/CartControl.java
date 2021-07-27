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
import java.util.ArrayList;

/**
 * CartControl je třída pracující nad operacemi s pohybem vozíku
 */
public class CartControl {

    private int position_x;
    private int position_y;   
    private int start_x;
    private int start_y;
    private int orig_start_x;
    private int orig_start_y;
    private int last_position_orig;
    private MapControl map = new MapControl();
    private CoordsConverter cnv = new CoordsConverter();
    private PathFinder dk;
    public static ArrayList<CartControl> free_carts = new ArrayList<CartControl>();
    public String[] current_path;
    public boolean cart_loaded;
    public boolean cart_go4ware;
    public boolean cart_start;
    public boolean cart_busy;
    public CartContent content;
    private String[] last_path;

    /**
    * CartControl Inicializace
    * Nastaví atributy, umístí na mapu vozík a zazálohuje
    * originální hodnotu v mapě na místě vozíku.
    * @param beginning Počáteční poloha v "zakódované" String formě
    */
    public CartControl(String beginning) {
        int[] pair = cnv.coordsInt(beginning);
        content = new CartContent();
        this.position_x = pair[0];
        this.position_y = pair[1];
        this.start_x = this.position_x;
        this.start_y = this.position_y;
        this.orig_start_x = this.position_x;
        this.orig_start_y = this.position_y;
        this.last_position_orig = map.getItem(this.position_x, this.position_y);
        this.cart_loaded = false;
        this.cart_go4ware = false;
        this.cart_start = false;
        this.cart_busy = false;
        if (this.last_position_orig == 0 || this.last_position_orig == 1) {
            map.setItem(this.position_x, this.position_y, 7);
        }
        else {
            throw new ArithmeticException("Na tuto pozici nelze vozik umistit!");
        }
        free_carts.add(this);
    }

    /**
    * gotoPosition
    * Metoda přesune vozík na určenou pozici a zazálohuje původní hodnotu místa.
    * @param x Poloha X
    * @param y Poloha Y
    */
    public void gotoPosition(int x, int y) {
        this.position_x = x;
        this.position_y = y;
        this.start_x = this.position_x;
        this.start_y = this.position_y;
        this.cartMoveCheck();
    }

    /**
    * getOriginalStart
    * Metoda ziská originální startovní pozice vozíku.
    * @return Originální pozice vozíku.
    */
    public int[] getOriginalStart() {
        int[] start = {this.orig_start_x, this.orig_start_y};
        return start;
    }

    private void cartMoveCheck() {
        map.setItem(this.position_x, this.position_y, 7);
    }

    /**
    * getPosition
    * Metoda vrátí aktuální pozici vozíku.
    * @return Aktuální pozice vozíku
    */
    public int[] getPosition() {
        int[] pos = {this.position_x, this.position_y};
        return pos;
    }

    /**
    * findPath
    * Metoda najde nejkratší cestu mezi začátkem a destinací.
    * @param dst_x Souřadnice X cíle, k němuž nejkratší cestu chceme.
    * @param dst_y Souřadnice Y cíle, k němuž nejkratší cestu chceme.
    * @return Nejkratší cesta k cíli ve formě arraye prvků typu String představující souřadnice
    */
    public String[] findPath(int dst_x, int dst_y) {
        try {
            this.dk = new PathFinder(this.map.getMap(), this.start_x, this.start_y, dst_x, dst_y);
            this.last_path = this.dk.Dijkstra();
            return this.last_path;
        }
        catch (Exception e) {
            return this.last_path;
        }
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