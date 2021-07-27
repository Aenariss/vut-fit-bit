/**
* RandomNumberGenerator
* RandomNumberGenerator načítá úvodní obsah skladu ze souboru
* 
* @author Vojtěch Fiala <xfiala61>
*/
package src.utils;

import java.lang.Math;

/**
 * RandomNumberGenerator generuje náhodná čísla v daném rozsahu.
 */
public class RandomNumberGenerator {
    // https://www.baeldung.com/java-generating-random-numbers-in-range
    // math.random umi vratit cisla 0-1

    /**
    * getRandomNumber
    * Vygeneruje náhodné číslo ze zadaného intervalu.
    * @param min Nejnižší hodnota intervalu.
    * @param max Nejvyšší hodnota intervalu.
    * @return Náhodné číslo z intervalu.
    */
    public int getRandomNumber(int min, int max) {
        return (int) ((Math.random() * (max - min)) + min);
    }
}