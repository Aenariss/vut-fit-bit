/**
 * Objekt reprezentující jednoho běžce.
 * @author Více autorů
 */
class Runner {

    /**
     * Konstruktor běžce
     * @author Vojtěch Malý [xmalyv03]
     * @param {*} name 
     * @param {*} surname 
     * @param {*} cardId 
     * @param {*} category 
     * @param {*} startTime 
     * @param {*} status Pokud není nutné, implicitně nastavit na "REG"
     * @param {*} club 
     */
    constructor(name, surname, cardId, category, startTime, status, club) {
        this.name = name;
        this.surname = surname;
        this.cardId = cardId;
        this.category = category;
        this.startTime = startTime;
        this.status = status;
        this.club = club;
        this.punches = [];
        this.time;
    }

    addPunch(Punch) {
        if (this.punches.length < 1) {
            this.punches.push(Punch);
            return;
        }
        for (let i = 0; i < this.punches.length; i++) {
            if (Punch.timeStamp < this.punches[i].timeStamp) {
                this.punches.splice(i,0,Punch);
                return;
            } 
        }
    }

    /**
     * Přídání oražení kontroly
     * @param {*} Punch 
     */
    addPunchSimple(Punch) {
        this.punches.push(Punch);
    }

    getPunchesNumbers() {
        var sequence = [];
        var n_of_punches = this.punches.length;

        for (var i = 0; i < n_of_punches; i++) {
            sequence.push(this.punches[i].controlNumber);
        }

        return sequence;
    }


    /**
     * Metoda vrátí výsledný čas, tedy čas běžce na trati od startu do cíle v SEKUNDÁCH.
     * @author Vojtěch Malý [xmalyv03]
     * @returns Čas v sekundách
     */
    getResultTime() {
        if (this.punches.length < 1) {
            console.log("Doesn't have any punches");
        } else {
            let lastPunch = this.punches[this.punches.length - 1];
            let last = lastPunch.timeStamp;
            let ans = last - (this.startTime * 60);
            return ans;
        }
    }

    /**
     * Kontroluje zda jsou běžcovi kontroly správně. 
     * Pokud ano vrací hodnotu TRUE v ErrCode
     * Pokud běžec zatím nedoběhl vraci hodnotu FALSE v ErrCode
     * 
     * V případě problému vrací strukturu {Číslo označující error (10,20,..), Potřebné informace pro zobrazení}
     * 
     * @param {*} compClass Objekt závodu
     * @author Vojtěch Malý [xmalyv03]
     * @returns 
     */
    checkRunValidity(compClass) {
        let courseSequence = compClass.getCourseSequenceByName(this.category);
        let ErrCode = true;
        let returnMessage = "x";

        if (this.punches.length < 1) {
            returnMessage = "Běžec ještě nevyčetl."
            ErrCode = false;
            return {ErrCode, returnMessage};
        }

        for (let cnt = 0; cnt < courseSequence.length; cnt++) {
            //console.log("REM:" + courseSequence[cnt] + ">>>" + this.punches[cnt].controlNumber);

            if (cnt == 0) {
                if (this.punches[cnt].timeStamp < this.startTime * 60) {
                    returnMessage = "Běžec vyběhl dřív než startoval!";
                    this.status = "DNF";
                    ErrCode = 10;                                                                                     // Vyběhl před startem.
                    return {ErrCode, returnMessage};
                }
            }

            if (courseSequence[cnt] != this.punches[cnt].controlNumber) {
                this.status = "DNF";
                ErrCode = 20;                                                                                         // špatná kontrola.
                return {ErrCode, cnt};
            }

            if (this.punches[cnt] == undefined) {
                this.status = "DNF";
                ErrCode = 30;                                                                                         // Málo kontrol
                return {ErrCode, cnt};
            }
        }
        this.status = "OK";
        return {ErrCode, returnMessage};                                                                             // Správně.
    }

    /**
     * Nastavení výsledného času závodníka
     * 
     * @author Vojtěch Fiala [xfiala61]
     * @returns 
     */
    setTime() {
        if (this.punches.length != 0) {     // Jestlize zavodnik ma vic nez 0 orazenych kontrol, muzu mu na zaklade posledni z nich nastavit cas
            let last_punch = this.punches[this.punches.length-1]
            let last_punch_time = last_punch.timeStamp; 
            this.time = last_punch_time - this.startTime*60;
        }
    }
}

module.exports.Runner = Runner;