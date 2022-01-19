/**
 * Funkce pro aktualizaci času závodu.
 * @author Vojtěch Fiala [xfiala61]
 */

let startTime = new Date() / 60000;    // Current time in unix epoch time in minutes

/* Function to update time since the race start */
function timeUpdate() {
    console.log("Time update");
    // Check if the current competition is set
    if (typeof window.competition.startTime !== 'undefined') {
        currentTime = new Date() / 60000;
        
        // Deduct the start time from the current time
        timePassed = Math.floor(currentTime-startTime);

        document.getElementById('time-since-start').innerHTML = "Aktuální čas: " + timePassed;
    }
}

module.exports = { timeUpdate };
