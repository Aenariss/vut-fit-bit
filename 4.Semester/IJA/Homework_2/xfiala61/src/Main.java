/**
 * Demonstrační třída sloužící k ukázce současné funkcionality programu.
 *
 * @author Vojtěch Fiala <xfiala61>
 */


import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

import src.shelf_manipulation.store.*;
import src.shelf_manipulation.goods.*;
import src.cart.*;
import src.utils.*;
import src.map_manipulation.MapControl;

import java.time.LocalDate;

/**
 * Demonstrační třída sloužící k ukázce současné funkcionality programu.
 */
public class Main {


    public static void main(String[] args) {
        CoordsConverter cnv = new CoordsConverter();
        HashIndexFinder fnd = new HashIndexFinder();
        MapControl map = new MapControl();
        MapPrinter mpp = new MapPrinter();

        // Vytvoreni typu zoozi
        Goods goods1 = new StoreGoods("Kuchynsky stul");
        Goods goods2 = new StoreGoods("Zidle");
        Goods goods3 = new StoreGoods("Skrin");
        Goods goods4 = new StoreGoods("Pohovka");
        Goods goods5 = new StoreGoods("Kreslo");
        Goods goods6 = new StoreGoods("Konferencni stul");
        Goods goods7 = new StoreGoods("Komoda");
        Goods goods8 = new StoreGoods("Postel");

        // Vytvoreni regalu s danym typem zbozi
        GoodsShelf shelf = new StoreShelf(goods1);
        GoodsShelf shelf2 = new StoreShelf(goods2);
        GoodsShelf shelf3 = new StoreShelf(goods3);
        GoodsShelf shelf4 = new StoreShelf(goods4);
        GoodsShelf shelf5 = new StoreShelf(goods5);
        GoodsShelf shelf6 = new StoreShelf(goods6);
        GoodsShelf shelf7 = new StoreShelf(goods7);
        GoodsShelf shelf8 = new StoreShelf(goods8);

        // Vytvoreni noveho kusu zbozi
        GoodsItem itm11 = new StoreGoodsItem(goods1, LocalDate.of(2021, 8, 5));
        GoodsItem itm12 = new StoreGoodsItem(goods1, LocalDate.of(2021, 6, 6));
        GoodsItem itm21 = new StoreGoodsItem(goods2, LocalDate.of(2021, 12, 8));
        GoodsItem itm31 = new StoreGoodsItem(goods3, LocalDate.of(2021, 12, 6));
        GoodsItem itm41 = new StoreGoodsItem(goods4, LocalDate.of(2021, 11, 6));
        GoodsItem itm51 = new StoreGoodsItem(goods5, LocalDate.of(2021, 10, 6));
        GoodsItem itm61 = new StoreGoodsItem(goods6, LocalDate.of(2021, 9, 6));
        GoodsItem itm71 = new StoreGoodsItem(goods7, LocalDate.of(2021, 8, 6));
        GoodsItem itm81 = new StoreGoodsItem(goods8, LocalDate.of(2021, 7, 6));

        // Pridani kusu zbozi do typu
        goods1.addItem(itm11);
        goods1.addItem(itm12);
        goods2.addItem(itm21);
        goods3.addItem(itm31);
        goods4.addItem(itm41);
        goods5.addItem(itm51);
        goods6.addItem(itm61);
        goods7.addItem(itm71);
        goods8.addItem(itm81);

        // Pridani zbozi do regalu
        shelf.put(itm11);
        shelf.put(itm12);
        shelf2.put(itm21);
        shelf3.put(itm31);
        shelf4.put(itm41);
        shelf5.put(itm51);
        shelf6.put(itm61);
        shelf7.put(itm71);
        shelf8.put(itm81);

        // Vytvoreni voziku
        CartControl cart = new CartControl("8.10");
        CartControl cart2 = new CartControl("8.9");

        // Seznam regalu vcetne jejich pozice
        ShelfManipulator regal = new ShelfManipulator("0.0", shelf);
        ShelfManipulator regal2 = new ShelfManipulator("1.0", shelf2);
        ShelfManipulator regal3 = new ShelfManipulator("2.0", shelf3);
        ShelfManipulator regal4 = new ShelfManipulator("3.0", shelf4);
        ShelfManipulator regal5 = new ShelfManipulator("4.0", shelf5);
        ShelfManipulator regal6 = new ShelfManipulator("7.0", shelf6);
        ShelfManipulator regal7 = new ShelfManipulator("8.0", shelf7);
        ShelfManipulator regal8 = new ShelfManipulator("0.3", shelf8);

        // Najdi v seznamu regalu typ zbozi a vrat jeho pozici
        String key = fnd.getIndex(regal.shelf_map, "Kuchynsky stul");
        String key2 = fnd.getIndex(regal.shelf_map, "Zidle");
        String key3 = fnd.getIndex(regal.shelf_map, "Skrin");
        String key4 = fnd.getIndex(regal.shelf_map, "Pohovka");
        String key5 = fnd.getIndex(regal.shelf_map, "Kreslo");
        String key6 = fnd.getIndex(regal.shelf_map, "Konferencni stul");
        String key7 = fnd.getIndex(regal.shelf_map, "Komoda");
        String key8 = fnd.getIndex(regal.shelf_map, "Postel");

        // Ziskej mapu skladu pro zobrazeni
        int[][] warehouse_map = map.getMap();

        System.out.printf("******************************************************\n");
        System.out.printf("Konzolova demonstrace funkcionality programu - Ukol 2\n");
        System.out.printf("******************************************************\n");
        System.out.printf("Mapa skladu, kde:\t5 = regal\t1 = cesta\t9 = vydej\n");
        System.out.printf("\t\t\t0 = start voziku\t\t7 = vozik\n");
        mpp.printMap(warehouse_map);
        System.out.printf("******************************************************\n");
        System.out.printf("Pocet regalu se zbozim: %d. Pocet regalu ve skladu: %d.\n", regal.getNumberOfFullRegals(), regal.getNumberOfAllRegals());
        System.out.printf("******************************************************\n");
        System.out.printf("Typ zbozi v regalu na pozici %s je %s. Pocet kusu: %d.\t", key, regal.shelf_map.get(key).getShelfType(), regal.shelf_map.get(key).size());
        System.out.printf("Typ zbozi v regalu na pozici %s je %s. Pocet kusu: %d.\t\t\n", key2, regal.shelf_map.get(key2).getShelfType(), regal.shelf_map.get(key2).size());
        System.out.printf("Typ zbozi v regalu na pozici %s je %s. Pocet kusu: %d.\t\t", key3, regal.shelf_map.get(key3).getShelfType(), regal.shelf_map.get(key3).size());
        System.out.printf("Typ zbozi v regalu na pozici %s je %s. Pocet kusu: %d.\t\t\n", key4, regal.shelf_map.get(key4).getShelfType(), regal.shelf_map.get(key4).size());
        System.out.printf("Typ zbozi v regalu na pozici %s je %s. Pocet kusu: %d.\t\t", key5, regal.shelf_map.get(key5).getShelfType(), regal.shelf_map.get(key5).size());
        System.out.printf("Typ zbozi v regalu na pozici %s je %s. Pocet kusu: %d.\t\t\n", key6, regal.shelf_map.get(key6).getShelfType(), regal.shelf_map.get(key6).size());
        System.out.printf("Typ zbozi v regalu na pozici %s je %s. Pocet kusu: %d.\t\t", key7, regal.shelf_map.get(key7).getShelfType(), regal.shelf_map.get(key7).size());
        System.out.printf("Typ zbozi v regalu na pozici %s je %s. Pocet kusu: %d.\t\t\n", key8, regal.shelf_map.get(key8).getShelfType(), regal.shelf_map.get(key8).size());
        System.out.printf("******************************************************\n");

        // Najdi v seznamu regalu typ zbozi a vrat pozici, na kterou pro nej ma vozik dojet
        String dest = fnd.getDestination(regal.shelf_map, "Pohovka");
        int[] destination = cnv.coordsInt(dest);

        // Spocitej cestu ke zbozi
        String[] path = cart.findPath(destination[0], destination[1]);

        // Vysli vozik na cestu, vypis vzdalenost a nejkratsi moznou cestu
        int distance = cart.getDistance(dest);
        String start = cart.getStart();

        System.out.printf("Vzdalenost od startu %s k cili %s: %d\n", start, dest, distance);
        System.out.printf("******************************************************\n");
        System.out.printf("Cesta ze startu %s k cili %s je nasledujici: \n", start, dest);
        cart.printPath(path);
    }
}
