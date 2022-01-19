/**
 * Definice hlavní třídy, označující celý závod.
 * @author Vojtěch Malý [xmalyv03]
 */ 

const { courseClass } = require("./classCourse");
const { controlPunch } = require("./classPunch");
const { Runner } = require("./classRunner");

/**
 * Třída pro objektovou reprezentaci celého závodu. 
 */
class competitionClass {

    /**
     * Konstruktor
     * @param {*} date Datum pořádání závodu
     * @param {*} startTime První startovací čas od 00:00 toho dne v minutách. Tzn. 60 = první start v 1:00 toho dne.
     * @param {*} name Název závodu
     */
    constructor(date, startTime, name) {
        this.date = date;
        this.startTime = startTime;
        this.name = name;
        this.runners = [];
        this.courses = [];
    }

    /**
     * Metoda, která nahraje JSON reprezentaci do competitionClass.
     * @param {*} data  JSON reperezentace
     */
    loadJSON(data) {
        this.name = data["name"];
        this.date = data["date"];
        this.startTime = data["startTime"];

        var runnersData = data["runners"];
        for (var key in runnersData) {
            let runner = runnersData[key];
            let person = new Runner(runner["name"], runner["surname"], runner["cardId"], runner["category"], runner["startTime"], runner["status"], runner["club"]);
            let punchEntry = runner["punches"];
            for (var key2 in punchEntry) {
                let pun = new controlPunch(punchEntry[key2].controlNumber, punchEntry[key2].timeStamp, runner["cardId"]);
                person.addPunchSimple(pun);
            }
            if (punchEntry.length != 0)
                person.setTime()
            this.runners.push(person);
        }
        let cou = data["courses"];
        for (let key3 in cou) {
            let cours = new courseClass(cou[key3].name, cou[key3].controlsSequence);
            this.courses.push(cours);
        }
        console.log("Load from JSON succes.");
    }

    /**
     * Vrací běžce v poli běžců podle jeho pozice.
     * @param {*} index Integer v string reprezentaci.
     * @returns Objekt běžce
     */
    getRunnerByIndex(index){
        index = +index;
        return this.runners.at(index);
    }
    
    /**
     * Vrací běžce podle jeho čísla čípu
     * @param {*} id Číslo čipu
     * @returns Objekt běžce
     */
    getRunnerByID(id) {
        var length = this.runners.length;
        for (var i = 0; i < length; i++) {
            if (this.runners[i].cardId == id) {
                return this.runners[i];
            }
        }

        return false;
    }

    /**
     * Vyhledá podle zadaného jména seznam běžců.
     * @param {*} search Jméno a přijmení běžců.
     * @returns Pole běžců, jejichž jméno odpovídá vstupu.
     */
    getRunnersByName(search) {
        let runner_arr = [];
        const reg_name = new RegExp(search);
        var length = this.runners.length;
        for (var i = 0; i < length; i++) {
            let runner_name = this.runners[i].name.toLowerCase() + ' ' + this.runners[i].surname.toLowerCase();
            if (reg_name.test(runner_name)) {
                runner_arr.push(this.runners[i]);
            }
        }

        return runner_arr; 
    }

    /**
     * Setter na jméno závodu.
     * @param {*} name 
     */
    setName(name) {
        this.name = name;
    }

    /**
     * Setter na datum závodu.
     * @param {*} date 
     */
    setDate(date) {
        this.date = date;
    }

    /**
     * Setter na kategorie, které závod má.
     * @param {*} courses 
     */
    setCourses(courses) {
        this.courses = courses;
    }

    /**
     * Vyhledá trať dle jejího názvu
     * @param {*} name 
     * @returns První nebo vyhledaný objekt tratě.
     */
    getCourseSequenceByName(name) {
        for (let cnt = 0; cnt < this.courses.length; cnt++) {
            if (this.courses[cnt].name == name) {
                return this.courses[cnt].controlsSequence;
            }
        }
        return this.courses[0];
    }
}

module.exports.competitionClass = competitionClass;