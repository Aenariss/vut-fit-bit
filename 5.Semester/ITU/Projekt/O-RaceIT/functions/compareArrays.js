/**
 * Modul pro porovnání dvou polí.
 * @author Vojtěch Fiala [xfiala61]
 */
function compareArrays(arr1, arr2) {
    // If one of the arrays was not defined, they're not equal
    if (!arr1 || !arr2)
        return false;

    // If the length differs, they're also not equal
    if (arr1.length != arr2.length) 
        return false;
    
    // If the length is the same, go and iteratively compare each element
    for (var i = 0; i < arr1.length; i++) {
        if (arr1[i] != arr2[i])
            return false;
    }

    return true;

}

module.exports = { compareArrays };