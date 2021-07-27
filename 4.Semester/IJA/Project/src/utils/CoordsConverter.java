/**
* CoordsConverter
* CoordsConverter zajišťuje konverzi z klíče typu string
* na 2 hodnoty typu int a zpět.
* 
* @author Vojtěch Fiala <xfiala61>
*/

package src.utils;

/**
 * * CoordsConverter zajišťuje konverzi z klíče typu string
* na 2 hodnoty typu int a zpět.
 */
public class CoordsConverter {

    /**
    * converCoords
    * Zkonvertuje 2 hodnoty typu int na jejich textovou formu typu String.
    * @param x Hodnota X, která reprezentuje souřadnici X, jež se má zkonvertovat.
    * @param y Hodnota Y, která reprezentuje souřadnici Y, jež se má zkonvertovat.
    * @return Zkonvertovaný string reprezentující souřadnice.
    */
    public String convertCoords(int x, int y) {
        String coords = Integer.toString(x) + "." + Integer.toString(y);
        return coords;
    }

    /**
    * coordsInt
    * Zkonvertuje textovou reprezentaci souřadnic na pole integeru se 2 prvky.
    * @param coords Textová reprezentace souřadnic.
    * @return Array prvků typu int obsahující 2 prvky - souřadnici X a Y.
    */
    public int[] coordsInt(String coords) {
        int x = 0;
        int y = 0;
        if (coords.length() == 3) {   // cislo tecka cislo (5.5) == [5][5]
            x = Integer.parseInt(Character.toString(coords.charAt(0)));
            y = Integer.parseInt(Character.toString(coords.charAt(2)));
        }
        else if (coords.length() == 4) {
            int dot = this.getDot(coords);
            if (dot == 2) {
                String dat = Character.toString(coords.charAt(0)) + Character.toString(coords.charAt(1));
                x = Integer.parseInt(dat);
                y = Integer.parseInt(Character.toString(coords.charAt(3)));
            }
            else {
                String dat = Character.toString(coords.charAt(2)) + Character.toString(coords.charAt(3));
                y = Integer.parseInt(dat);
                x = Integer.parseInt(Character.toString(coords.charAt(0)));
            }
        }
        else if (coords.length() == 5) {
            String dat = Character.toString(coords.charAt(0)) + Character.toString(coords.charAt(1));
            String dat2 = Character.toString(coords.charAt(3)) + Character.toString(coords.charAt(4));
            x = Integer.parseInt(dat);
            y = Integer.parseInt(dat2);
        }
        int[] ret = {x, y};
        return ret;
    }

    private int getDot(String str) {
        for(int i = 0; i < str.length(); i++) {
            if (str.charAt(i) == '.') {
                return i;
            }
        }
        return 0;
    }

}
