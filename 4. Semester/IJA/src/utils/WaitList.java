/**
* WaitList
* WaitList slouží k uložení prvků, co se mají ze skladu odvést
* 
* @author Vojtěch Fiala <xfiala61>
*/

package src.utils;

import java.io.File;  
import java.io.FileNotFoundException;
import java.util.Scanner;

/**
 * WaitList slouží ke spravování seznamu objednávek.
 */
public class WaitList {

    /* WaitList 
     * WaitList obsahující prvky ve frontě
    */
    public static String[] WaitList = new String[0];
    private Array arr = new Array();


    /**
     * WaitList
     * Konstruktor třídy WaitList.
     * Slouží k načtení položek ze souboru s objednávkami
     */
    public WaitList() {
        Scanner reader = null;
        File file = new File("./data/Orders.txt");

        try {
            reader = new Scanner(file);

        } catch (FileNotFoundException e){
            System.out.println("Couldn't load file with orders.");
        }

        while(reader.hasNextLine()) {
            String order = reader.nextLine();
            this.WaitList_Add(order);
        }
    }
    /**
    * WaitList_Add
    * Přidá do WaitListu prvek.
    * @param name Prvek, co se má přidat.
    */
    public void WaitList_Add(String name) {
        WaitList = this.arr.append(this.WaitList, name);
    }

    /**
    * WaitList_Remove_First
    * Odstraní z WaitListu první prvek.
    */
    public void WaitList_Remove_First() {
        WaitList = this.arr.remove(this.WaitList, 0);
    }

    /**
    * WaitList_Len
    * Vrátí délku WaitListu.
    * @return Délka WaitListu
    */
    public int WaitList_Len() {

        if(WaitList == null) {
            return 0;
        } else {
            return WaitList.length ;
        }
    }
}
