/**
* WarehouseLoader
* WarehouseLoader načítá úvodní obsah skladu ze souboru
* 
* @author Vojtěch Bůbela <xbubel08>
* @author Vojtěch Fiala <xfiala61>
*/

package src.utils;

import java.io.File;  
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.time.LocalDate;
import java.util.Arrays;

import src.shelf_manipulation.store.*;
import src.shelf_manipulation.goods.*;

/**
 * WarehouseLoader slouží k načítání obsahu skladu.
 */
public class WarehouseLoader {

    /**
     * controller
     * Regál, skrz který je možné přistoupit do seznamu regálů
     */
    public ShelfManipulator controller;
    /**
     * goods_list
     * Seznam typů zboží ve skladu
     */
    public static String[] goods_list = new String[0];
    private boolean controller_set = false;
    private RandomNumberGenerator rng = new RandomNumberGenerator();
    private Array arr = new Array();
    
    /**
     * WarehouseLoader
     * Konstruktor třídy WarehouseLoader.
     */
    public WarehouseLoader() {

        Scanner s = get_file();
        int counter = 0;

        while(s.hasNextLine()) {
            String new_line = s.nextLine();
            String[] split = new_line.split(" ");
            parse_line(split, counter);
            counter++;
        }
    }

    private Scanner get_file() {
        Scanner reader = null;
        File file = new File("./data/Warehouse_goods.txt");

        try {
            reader = new Scanner(file);

        } catch (FileNotFoundException e){
            System.out.println("Couldn't load file with goods.");
        }

        return reader;
    }

    private void parse_line(String[] line, int counter) {
        

        if (line.length != 3) {
            String error = "Chyba na radku" + counter + "ve vstupnim souboru s obsahem skladu!";
            throw new ArithmeticException(error);
        }

        String ware_name = line[0];
        int number_of_wares = Integer.parseInt(line[1]);
        String coords = line[2];

        Goods goods_name = new StoreGoods(ware_name);
        GoodsShelf shelf_name = new StoreShelf(goods_name);

        for (int i = 0; i < number_of_wares; i++) {
            // Nahodne datum
            int year = rng.getRandomNumber(1500, 2021); // Zbozi bude, rekneme, z nahodneho data mezi roky 1500 a 2021
            int month = rng.getRandomNumber(1,12);
            int day = rng.getRandomNumber(1, 28); // Nahodny den v mesici, ale pro jistotu max 28. -- nebudeme riskovat, ze bychom dostali unor a meli 31. den
            GoodsItem itm = new StoreGoodsItem(goods_name, LocalDate.of(year, month, day));
            goods_name.addItem(itm);
            shelf_name.put(itm);
        }

        ShelfManipulator regal = new ShelfManipulator(coords, shelf_name);
        this.goods_list = arr.append(this.goods_list, ware_name);

        if (this.controller_set == false) {
            this.controller_set = true;
            this.controller = regal;
        }
    }
}
