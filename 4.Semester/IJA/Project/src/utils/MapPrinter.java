/**
* MapPrinter
* MapPrinter zajišťuje tisknutí mapy.
* 
* @author Vojtěch Fiala <xfiala61>
*/

package src.utils;

/**
 * Array MapPrinter zajišťuje tisknutí mapy.
 */
public class MapPrinter {
    
    /**
    * printMap
    * Vypíše na standardní výstup mapu.
    * @param map 2D Array typu int reprezentující mapu.
    */
    public void printMap(int[][] map) {
        int rows = map.length;
        int cols = map[0].length;
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                System.out.printf("%d", map[i][k]);
            }
            System.out.printf("\n");
        }
    }

}
