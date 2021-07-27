/**
* PathFinder
* PathFinder slouží k nalezení nejkratší cesty mezi
* 2 zvolenými body. 
*
* @author Vojtěch Fiala <xfiala61>
*/

package src.cart;
import java.util.HashMap;
import src.utils.*;
import java.util.Arrays;

/**
 * PathFinder slouží k nalezení nejkratší cesty mezi 2 body
 */
public class PathFinder {

    private int[][] map;
    private int start_x;
    private int start_y;
    private int dest_x;
    private int dest_y;
    private int rows;
    private int cols;
    private int[][] convert_map;
    private int distance;
    private HashMap<String, Integer> distances = new HashMap<String, Integer>();
    private String[] closed = new String[1];
    private String[] open = new String[1];
    private Array arr = new Array();
    private CoordsConverter coords_cnv = new CoordsConverter();
 
    /**
    * PathFinder Inicializace
    * Nastaví atributy a vytvoří ze získané mapy její interní 
    * zjednodušenou reprezentaci pro ulehčení dalšího zpracování.
    * @param map 2D array integerů reprezentující mapu.
    * @param start_x Souřadnice X startovní pozice.
    * @param start_y Souřadnice Y startovní pozice.
    * @param dest_x Souřadnice X cílové pozice.
    * @param dest_y Souřadnice Y cílové pozice.
    */
    public PathFinder(int[][] map, int start_x, int start_y, int dest_x, int dest_y) {
        this.map = map;
        this.start_x = start_x;
        this.start_y = start_y;
        this.dest_x = dest_x;
        this.dest_y = dest_y;
        this.convertMap();
    }

    // Zpracuje prvek - spočítá jeho vzdálenost od začátku,
    // případně aktualizuje, je-li nová hodnota nižší než původni.
    // Odstraní ze seznamu open, vloží do seznamu closed.
    private void solve(int x_parent, int y_parent, int x, int y) {
        // spocitej klice rodice a noveho prvku
        String coords = this.coords_cnv.convertCoords(x, y);
        String parent = this.coords_cnv.convertCoords(x_parent, y_parent);
        // Jestlize prvek jeste neni zpracovany, zpracuj ho
        if (!this.distances.containsKey(coords)) {
            int dist = this.distances.get(parent);
            this.distances.put(coords, dist+1);
        }
        // jestlize je, zkontroluj, jestli nova cesta neni levnejsi
        else {
            int dist = this.distances.get(parent);
            int key_val = this.distances.get(coords);
            if (dist + 1 < key_val) {
                this.distances.put(coords, dist);
            }
        }

        // Jestli je prvek v open, smaz ho
        int remove = Arrays.asList(this.open).indexOf(coords);
        if (remove != -1) {
            this.open = arr.remove(this.open, remove);
        }

        // Jestli prvek jeste neni v closed, dej ho tam
        int present_in_closed = Arrays.asList(this.closed).indexOf(coords);
        if (present_in_closed == -1) {
            this.closed = arr.append(closed, coords);
        }
    }
    
    /**
    * getPath
    * Metoda pro určení nejkratší cesty ze startu do cíle,
    * @param path Array objektů typu string, do něhož se zapisuje cesta.
    * @param dst Vzdálenost cílového bodu od počatečního bodu.
    * @return Nejkratší cesta mezi 2 body.
    */
    public String[] getPath(String[] path, int dst) {
        // urci klic pro destinaci a start
        String destination = this.coords_cnv.convertCoords(this.dest_x, this.dest_y);
        String start = this.coords_cnv.convertCoords(this.start_x, this.start_y);
        int dist = -1;
        // jestli hledana pozice existuje (a je dostupna), ziskej jeji vzdalenost
        if (this.distances.containsKey(destination)) {
            dist = this.distances.get(destination);
        }
        // jestli neexistuje, je to exception...
        else {
            throw new ArithmeticException("Pozice neexistuje!");
        }

        // Jestlize cilova pozice je startovni pozice, vrat soucasnou pozici (tzn. ze na ni mas zustat)
        if (destination == start) {
            path = arr.append(path, start);
            return path;
        }

        // aktivni prvek
        int[] active = coords_cnv.coordsInt(destination);
        // pridej do cesty destinaci
        path = arr.append(path, destination);
        // dokud nejsi na startu, hledej cestu
        while (dist != 0) {
            // bugfix
            if (dist == 1) {
                break;
            }
            // north
            // zkontroluj, jestli je smer validni
            if (this.isValid(active[0]-1, active[1])) {
                // ziskej klic na kontrolu
                String cmp = this.coords_cnv.convertCoords(active[0]-1, active[1]);
                // jestlize je hodnota klice mensi nez soucasna vzdalenost, klic je dalsi krok na ceste
                if (dist > this.distances.get(cmp)) {
                    path = arr.append(path, cmp);
                    active = this.coords_cnv.coordsInt(cmp);
                    dist = this.distances.get(cmp);
                    continue;
                }
            }

            // south
            if (this.isValid(active[0]+1, active[1])) {
                String cmp = this.coords_cnv.convertCoords(active[0]+1, active[1]);
                if (dist > this.distances.get(cmp)) {
                    path = arr.append(path, cmp);
                    active = this.coords_cnv.coordsInt(cmp);
                    dist = this.distances.get(cmp);
                    continue;
                }
            }

           // east
            if (this.isValid(active[0], active[1]+1)) {
                String cmp = this.coords_cnv.convertCoords(active[0], active[1]+1);
                if (dist > this.distances.get(cmp)) {
                    path = arr.append(path, cmp);
                    active = this.coords_cnv.coordsInt(cmp);
                    dist = this.distances.get(cmp);
                    continue;
                }
            }

            // west
            if (this.isValid(active[0], active[1]-1)) {
                String cmp = this.coords_cnv.convertCoords(active[0], active[1]-1);
                if (dist > this.distances.get(cmp)) {
                    path = arr.append(path, cmp);
                    active = this.coords_cnv.coordsInt(cmp);
                    dist = this.distances.get(cmp);
                    continue;
                }
            }
        }
        // jestlize je cesta dlouha jak ma byt, pridej jeste start pozici pro uplnost
        if (path.length == dst) {
            path = arr.append(path, start);
        }
        return path;
    }

    /**
    * Dijkstra
    * Za pomoci Dijkstrova algoritmu metoda ohodnotí všechny dostupné lokace na mapě.
    * Za pomoci těchto spočtených vzdáleností poté určí nejkratší možnou cestu mezi 2 danými body.
    * @return Array obsahující nejkratší možnou cestu mezi 2 body.
    */
    public String[] Dijkstra() {

        // zpracuj zacatek
        if (this.isValidStart()) {
            String coords = this.coords_cnv.convertCoords(this.start_x, this.start_y);
            int active_counter = 0;
            this.distances.put(coords, 0);  // vloz do hash mapy
            this.closed[0] = coords;        // vloz start do closed
            int remove = Arrays.asList(this.open).indexOf(coords);      // najdi index soucasneho bodu
            this.open = arr.remove(this.open, remove);  // ten bod dej pryc z open
            int[] active = coords_cnv.coordsInt(coords);
            
            // Iteruj, dokud neni open prazdny
            while (this.open.length != 0) {

                // north
                if (this.isValid(active[0]-1, active[1])) {
                    this.solve(active[0], active[1], active[0]-1, active[1]);
                }

                // south
                if (this.isValid(active[0]+1, active[1])) {
                    this.solve(active[0], active[1], active[0]+1, active[1]);
                }

                // east
                if (this.isValid(active[0], active[1]+1)) {
                    this.solve(active[0], active[1], active[0], active[1]+1);
                }

                // west
                if (this.isValid(active[0], active[1]-1)) {
                    this.solve(active[0], active[1], active[0], active[1]-1);
                }

                active_counter++;
                if (active_counter >= this.closed.length) {
                    break;
                }
                // Dalsi aktivni je 2. v closed
                active = coords_cnv.coordsInt(this.closed[active_counter]);
            }
            int dst = this.getDistance(this.coords_cnv.convertCoords(this.dest_x, this.dest_y));

            String[] path = new String[0];
            path = this.getPath(path, dst);
            return path;
        }
        String[] bad = new String[1];
        bad[0] = "error";
        return bad;
    }

    private void convertMap() {
        this.rows = this.map.length;
        boolean flag = true;
        this.cols = this.map[0].length;
        this.convert_map = this.map;
        for (int i = 0; i < this.rows; i++) {
            for (int k = 0; k < this.cols; k++) {
                if (this.map[i][k] == 0 || this.map[i][k] == 1 || this.map[i][k] == 9) {
                    convert_map[i][k] = 0;
                    if (flag) {
                        this.open[0] = this.coords_cnv.convertCoords(i, k);
                    }
                    else {
                        String coords = this.coords_cnv.convertCoords(i, k);
                        this.open = arr.append(open, coords);
                    }
                    flag = false;
                }
                else if (this.map[i][k] == 7) {
                    if (flag) {
                        this.open[0] = this.coords_cnv.convertCoords(i, k);
                    }
                    else {
                        String coords = this.coords_cnv.convertCoords(i, k);
                        this.open = arr.append(open, coords);
                    }
                    flag = false;
                    convert_map[i][k] = 1;
                }
                else {
                    convert_map[i][k] = 9;
                }
            }
        }
    }
    
    private boolean isValidStart() {
        if (this.start_x < this.rows && this.start_x >= 0 && this.start_y < this.cols && this.start_y >= 0) {
            
            return (this.convert_map[this.start_x][this.start_y] == 1);
        }
        return false;
    }

    private boolean isValid(int x, int y) {
        if (x < this.rows && x >= 0 && y < this.cols && y >= 0) {
            return (this.convert_map[x][y] == 0);
        }
        return false;
    }

    /**
    * getDistance
    * Metoda získá vzdálenost mezi počátkem a cílem.
    * @param key Zakódované souřadnice cíle, k němuž vzdálenost chceme.
    * @return Vzdálenost k cíli
    */
    public int getDistance(String key) {
        return this.distances.get(key);
    }


}
