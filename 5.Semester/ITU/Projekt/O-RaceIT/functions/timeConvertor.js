/**
 * Funkce pro získání českého formátu datumu.
 * @author Vojtěch Fiala [xfiala61]
 */
function getCzechDate(date) {
    let last = 0;
    let day, month, year = 0;
    for(let i=0; i < date.length; i++) {
        if (date[i] == '-') {   // We divide by the - sign (aka 09-11-2001)
            if (!year) {
                year = date.substring(0, i);
                last = i;
            }
            else if (!month && year) {
                month = date.substring(last+1, i);
                day = date.substring(i+1, date.length)
            }
        }
    }

    date_string = day + '. ' + month + '. ' + year;
    return date_string;
}

/* Convert given date to american date cuz of HTML standard */
function getAmericanDate(date) {

    let dateYear = date.split(".").pop();   // Split the date by the dots (aka 25.11.2021) and divide further
    dateYear = dateYear.substring(1);
    let dateDay = date.split(".")[0];
    date = date.substring(date.indexOf('.') + 1);
    let dateMonth = date.split(".")[0];
    dateMonth = dateMonth.substring(1);

    let correctDate = dateYear + '-' + dateMonth + '-' + dateDay;

    return correctDate;
}

/** Convert seconds to 00:00:00 format
 * @author Vojtěch Fiala [xfiala61]
 */
function convertTimeToFormat(time) {
    let timeH = Math.floor(time/3600);  // First divide the whole seconds number by 3600 to get the hours
    
    let timeM = Math.floor(time/60) - timeH*60; // Then divide the number by 60 to get the minutes and deduce the hours

    if(timeM < 10) {    // If given time is less than 10, add 0
        timeM = '0' + timeM;
    }

    if(timeH < 10) {
        timeH = '0' + timeH;
    }

    console.log(timeM)
    let correctTime = timeH + ":" + timeM;  // Add hours + minutes, we don't really needs seconds in ours usecases
    return correctTime;
}

/**
 * Convert time in format 00:00:00 to seconds
 * @author Vojtěch Fiala [xfiala61]
 */
function convertFormatToTime(time) {
    time = time.split(':'); // Split it by the ':'
    if (time.length < 3)    // If we dont have 3 parts, something went wrong
        return 0    // Error

    let seconds = parseInt(time[0]*3600) + parseInt(time[1]*60) + parseInt(time[2]);
    return seconds
}

/* Important: This is the way to export multiple functions */
module.exports = {
    getCzechDate,
    getAmericanDate,
    convertTimeToFormat,
    convertFormatToTime
 }
