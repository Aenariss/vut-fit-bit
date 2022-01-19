/**
 * Třída sloužící k přiřazování sekvence kontrol k závodníkovi na základě výsledků závodu.
 * @author Vojtěch Fiala [xfiala61]
 */

const { controlPunch } = require("./classPunch");
const { Runner } = require("./classRunner");

class RunnerAdder {

    constructor(runner, data, id) {
        this.data = data;
        this.currID = id;
        this.runner = runner;
        this.runner.punches = [];   // Probably not a good idea, but w/e (When adding a new test runner, his puncches get deleted)
        this.addPunches();
    }
    /**
     * Přidání sekvence kontrol závodníkovi.
     * Kontroly jsou přiřazeny z objektového atributu data, které reprezentují výsledky závodu
     */
    addPunches() {
        var punches_number = this.data[this.currID].punches.length;
        for (var i = 0; i < punches_number; i++) {
            let pun = new controlPunch(this.data[this.currID].punches[i].controlNumber, this.data[this.currID].punches[i].timeStamp, this.runner["cardId"]);
            this.runner.addPunchSimple(pun);
        }
    }
}

module.exports.RunnerAdder = RunnerAdder;
