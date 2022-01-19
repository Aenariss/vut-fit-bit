/**
 * Objekt pro uchvání stavu nastavení.
 * @author Vojtěch Malý [xmalyv03]
 */
class settingsState {
    constructor() {
        this.autoSaveBool = false;
        this.time = 1 * 60 * 1000;
        this.newFiles = false;
        this.intervalHolder;
    }

    /**
     * Povolí intervalové zalohování.
     */
    setEnable() {
        this.autoSaveBool = true;
    }

    /**
     * Zakáže intervalové zalohování
     */
    setDisable() {
        this.autoSaveBool = false;
    }

    getAutosave() {
        return this.autoSaveBool;
    }

    /**
     * Pokud je argument čas, nastaví ho jako interval pro zalohování, jinak vrátí aktuální.
     * @param {*} timeIn Čas v minutách
     * @returns 
     */
    setAndGetTime(timeIn) {
        if(timeIn == undefined) {
            return this.time;
        } else {
            this.time = timeIn;
            return this.time;
        }
    }

    /**
     * Změní nastavení zda je ukládání do stejného či nového souboru.
     */
    toggleNew() {
        if(this.newFiles) {
            this.newFiles = false;
        } else {
            this.newFiles = true;
        }
    }

    /**
     * Vraci informaci zda je povoleno ukládat do nového souboru.
     * @returns 
     */
    getNewFilesBool() {
        return this.newFiles;
    }

    setIntervalSave(inter) {
        this.intervalHolder = inter;
    }

    getIntervalSave() {
        return this.intervalHolder;
    }
}

module.exports.settingsState = settingsState;