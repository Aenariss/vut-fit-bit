/**
 * Objekt prezentující jedno oražení kontroly.
 * @author Vojtěch Malý [xmalyv03]
 */

class controlPunch {
    constructor(controlNumber, timeStamp, cardID) {
        this.controlNumber = controlNumber;
        this.timeStamp = timeStamp;
        this.cardID = cardID;
    }
}

module.exports.controlPunch = controlPunch;