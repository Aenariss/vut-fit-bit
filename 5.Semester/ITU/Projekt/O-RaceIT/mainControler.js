/**
 * Zobrazovač stránek, jejich přepínání a načítání dat. Srdce celého programu.
 * @author Vojtěch Malý [xmalv03], Vojtěch Fiala [xfiala61]
 */
const { loadDatasetFromJSON, competition } = require("./functions/datasetLoad");
const { welcomePageRender} = require("./renderers/welcomePageRender");
const { competitionPageRender} = require("./renderers/competitionPageRender");
const { runnersRender } = require("./renderers/runnersRender");
const { addRacer } = require("./renderers/continuousRunners");
const { saveRaceToFile } = require("./functions/export");
const { importRaceFromFile } = require("./functions/import");
const {settingsPage} = require('./renderers/settingsPageRender');
const { renderTables } = require("./renderers/tablePageRender");
const { timeUpdate } = require("./renderers/timeRender");
const { settingsState } = require("./classDefinitions/classSettings");


function loadMainPage() {
    console.log("Loading welcome page");
    welcomePageRender();
}

function loadcompetitionPage() {
    var doesntWorkOtherwise = window.competition;
    console.log("Loading competition page");
    competitionPageRender(doesntWorkOtherwise);
}

function jsonStartUpLoad() {
    console.log("Start");
    window.OraceITsettings = new settingsState;
    loadDatasetFromJSON();
}

function loggerComp() {
    var doesntWorkOtherwise = window.competition;
    console.log("View changed on Runners.");
    runnersRender(doesntWorkOtherwise);
}

function racerArrived() {
    console.log("A wild runner has appeared!")
    addRacer();
}

function exportRace() {
    console.log("Called export!");
    saveRaceToFile(1);  // 1 means that we want to overwrite the current last file
    alert("Závod úspěšně exportován!");
}

function importRace(file) {
    console.log("Called import!");
    importRaceFromFile(file);
    alert("Závod úspěšně naimportován!");
    document.getElementById('importmenu').value = "";   // Remove value, otherwise the 'change' event wouldnt fire next time the same race is imported
}

function loadTables() {
    console.log("clicked table icon");
    renderTables();
}


//Add listeners to HTMl events on the sidebar
document.getElementById("runners").addEventListener('click', loggerComp);
document.getElementById("new-Racer").addEventListener('click', racerArrived);
document.getElementById("welcome-Page").addEventListener('click', loadMainPage);
document.getElementById("competition").addEventListener('click', loadcompetitionPage);
document.getElementById("export").addEventListener('click', exportRace);
document.getElementById("tables").addEventListener('click', loadTables);
document.getElementById("import").addEventListener('click', function() {document.getElementById('importmenu').click()} );
document.getElementById("importmenu").addEventListener('change', function(){importRace(this.value)});
document.getElementById("settings").addEventListener('click', settingsPage);
window.addEventListener('load', jsonStartUpLoad);
window.addEventListener('load', loadMainPage);

// Updating current race time each 10 seconds
setInterval(timeUpdate, 10000);
