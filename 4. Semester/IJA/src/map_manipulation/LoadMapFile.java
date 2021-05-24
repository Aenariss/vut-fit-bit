/**
* Load Map File
* Load Map File načte mapu skladu ze souboru Warehouse.txt 
* ve složce /data
*
* @author Vojtěch Fiala <xfiala61>
*/

package src.map_manipulation;
import java.io.File;  
import java.io.FileNotFoundException; 
import java.util.Scanner;

/**
 * LoadMapFile načte mapu skladu ze souboru Warehouse.txt 
 * ve složce /data
 */
public class LoadMapFile {
    private int rows = 0;
    private int cols = -5;
    private int map[][];
    private int shelf_count = 0;

    /**
    * LoadMapFile Inicializace
    * Získej počet řádků a sloupců mapy a je-li validní, načti ji.
    */
    public LoadMapFile() {
        this.getMapData();
        this.loadMap();
    }

    /**
    * getRows
    * Vrací počet řádků v mapě.
    * @return Počet řádků v mapě.
    */
    public int getRows() {
        return this.rows;
    }

    /**
    * getCols
    * Vrací počet sloupců v mapě.
    * @return Počet sloupců v mapě.
    */
    public int getCols() {
        return this.cols;
    }

    private void rowValid(String data) {
        if (this.cols == -5) {
            this.cols = data.length();
        }
        else {
            if (data.length() != this.cols) {
                throw new ArithmeticException("Spatne zadana mapa skladu!");
            }
        }
    }

    private void getMapData() {
        try {
            File file = new File("./data/Warehouse.txt");
            Scanner myReader = new Scanner(file);
            while (myReader.hasNextLine()) {
                String data = myReader.nextLine();
                this.rowValid(data);
                this.rows++;
            }
            myReader.close();
        }
        catch (FileNotFoundException e) {
            System.out.println("Couldn't load Warehouse map.");
        }
    }

    private void setMapSize() { 
        this.map = new int[this.rows][this.cols];
    }

     /**
    * getMap
    * Vrací načtenou mapu.
    * @return Načtená mapa jako 2D array prvků typu integer.
    */
    public int[][] getMap() {
        return this.map;
    }

    /**
    * getNumberOfShelves
    * Vrací počet polic ve skladu.
    * @return Počet polic ve skladu.
    */
    public int getNumberOfShelves() {
        return this.shelf_count;
    }

    private void loadMap() {
        this.setMapSize();
        int current_row = 0;
        try {
            File file = new File("./data/Warehouse.txt");
            Scanner myReader = new Scanner(file);
            while (myReader.hasNextLine()) {
                String data = myReader.nextLine();
                for (int i = 0; i < data.length(); i++) {
                    this.map[current_row][i] = Integer.parseInt(Character.toString(data.charAt(i)));
                    if (this.map[current_row][i] == 5) {
                        this.shelf_count++;
                    } 
                }
                current_row++;
            }
            myReader.close();
        }
        catch (FileNotFoundException e) {
            System.out.println("Couldn't load Warehouse map.");
        }
    }
}
