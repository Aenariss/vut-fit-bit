/**
 * Stránka s nastavením závodu.
 * @author Vojtěch Malý [xmalyv03]
 */

const { saveRaceToFile } = require("../functions/export");

function settingsPage() {
    let settings = window.OraceITsettings;

    mainDiv = document.getElementById("center-Section");
    mainDiv.innerHTML='';

    mainDiv.innerHTML = 
    '<div id="mainInnerDiv">'+
    '<div id="settings-section-name">Nastavení</div>'+
    '<div id="separator-settings"></div>'+
    '   <div id="automaticSave">'+
    '       <div><label for="autoCheck">Automatické ukládání:</label>'+
    '       <input type="checkbox" id="autoCheck"></div>'+
    '       <div><label for="autoSave">Interval uložení:</label>'+
    '       <input type="number" id="autoSave" placeholder="v minutách" disabled></div>'+
    '      <div> <label for="newOrOver">Vytvořit vždy nový soubor:</label>'+
    '       <input type="checkbox" id="newOrOver"></div>'+
    '       <div><button id="startSaving">Ukládat</button></div>'+
    '   </div>'+
    '</div>';

    document.getElementById('autoCheck').addEventListener('click', checkClicked);
    document.getElementById('startSaving').addEventListener('click', startSaveNow);
    document.getElementById('newOrOver').addEventListener('click', function () {
        settings.toggleNew();
    });

    document.getElementById('autoCheck').checked = settings.getAutosave();
    document.getElementById('autoSave').value = settings.setAndGetTime() / 1000 / 60;
    if (settings.getAutosave()) {
        document.getElementById('autoSave').disabled = false;
    }
    document.getElementById('newOrOver').checked = settings.getNewFilesBool();

    function checkClicked() {
        if(document.getElementById('autoCheck').checked) {
            document.getElementById('autoSave').disabled = false;
            settings.setEnable();
        } else {
            document.getElementById('autoSave').disabled = true;
            settings.setDisable();
            console.log("END SAVe NOW");
            endSaveNow();
        }
    }


    function startSaveNow () {
        let timeVal = parseInt(document.getElementById('autoSave').value);
        if(document.getElementById('autoCheck').checked) {
            if (isNaN(timeVal)) {
                timeVal = 5 * 60 *1000;
            } else {
                timeVal = timeVal * 60 * 1000;
            }
            if (timeVal == 0) {
                timeVal = 1 * 60 *1000;
            }
            settings.setAndGetTime(timeVal);
            let intervalX = setInterval(autoSaveFunction, timeVal);
            settings.setIntervalSave(intervalX);
           // console.log(settings.getIntervalSave());
        }

    }


    function endSaveNow() {
        clearInterval(settings.getIntervalSave());
    }


    document.getElementById('newOrOver').checked = settings.getNewFilesBool();
    

    function autoSaveFunction() {
        console.log("Saved");
        if(settings.getNewFilesBool) {
            saveRaceToFile(0);
        } else {
            saveRaceToFile(1);
        }
    }
}

module.exports = {settingsPage};
