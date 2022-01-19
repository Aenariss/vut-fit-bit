/**
 * Stránka zobrazující výsledky v tabulce
 * @author Vojtěch Fiala [xfiala61]
 */


const { compareArrays } = require("../functions/compareArrays");
var timesClicked = 0;
var runnersToShow;
var runnersToShowOriginal;

function renderTables() {
    document.getElementById('center-Section').innerHTML = "";

    let titleText = document.createElement('div');
    titleText.setAttribute('id', 'title-div-table');
    titleText.innerHTML = "Průběžné výsledky";

    let separatorDiv = document.createElement('div');
    separatorDiv.setAttribute('id', 'title-main-separator-table');

    let tableContainerDiv = document.createElement('div');
    tableContainerDiv.setAttribute('id', 'tableContainerDiv');

    let filter_container = document.createElement('div');
    filter_container.setAttribute('id', 'table-filter-container');
    filter_container.style.marginTop = "2em";
    filter_container.style.marginLeft = "33.33%";
    filter_container.setAttribute('class', 'col-lg-4')

    let filterBar = document.createElement('input');
    filterBar.setAttribute('id','tables-filter-bar');
    filterBar.setAttribute('class', 'form-control');
    filterBar.setAttribute('placeholder', 'Zapište filtr');
    filterBar.addEventListener('change', function()  {applyFilter(this.value)});

    let filter_display = document.createElement('div');
    filter_display.setAttribute('id', 'filter_display');
    filter_display.style.display = "none";
    filter_display.style.textAlign = "center";

    filter_container.appendChild(filterBar);
    filter_container.appendChild(filter_display);

    tableContainerDiv.appendChild(filter_container);

    let table = document.createElement('table');
    table.setAttribute('class', 'table table-main')

    let thead = document.createElement('thead');
    table.appendChild(thead);

    let headrow = document.createElement('tr');
    thead.appendChild(headrow);

    let number_header = document.createElement('th');
    number_header.setAttribute('id', 'numberSort');
    number_header.setAttribute('scope', 'row');
    number_header.innerHTML = "#";

    let name_header = document.createElement('th');
    name_header.setAttribute('id', 'nameSort');
    name_header.setAttribute('scope', 'row');
    name_header.innerHTML = "Jméno";

    let surname_header = document.createElement('th');
    surname_header.setAttribute('id', 'surnameSort');
    surname_header.setAttribute('scope', 'row');
    surname_header.innerHTML = "Přijmení";

    let cat_header = document.createElement('th');
    cat_header.setAttribute('id', 'catSort');
    cat_header.setAttribute('scope', 'row');
    cat_header.innerHTML = "Kategorie";

    let club_header = document.createElement('th');
    club_header.setAttribute('id', 'clubSort');
    club_header.setAttribute('scope', 'row');
    club_header.innerHTML = "Klub";

    let time_header = document.createElement('th');
    time_header.setAttribute('id', 'timeSort');
    time_header.setAttribute('scope', 'row');
    time_header.innerHTML = "Čas";

    headrow.appendChild(number_header);
    headrow.appendChild(name_header);
    headrow.appendChild(surname_header);
    headrow.appendChild(cat_header);
    headrow.appendChild(club_header);
    headrow.appendChild(time_header);

    runnersToShow = getFinishedRunners();
    runnersToShowOriginal = runnersToShow;

    let tbody = document.createElement('tbody');
    tbody.setAttribute('id', 'tbody')
    tbody = fillTable(tbody);


    table.appendChild(tbody);
    tableContainerDiv.appendChild(table);

    // INFO BOX
    let infoDiv = document.createElement('div');
    infoDiv.setAttribute('id', 'infoDIV');
    infoDiv.innerHTML='<i class="fas fa-question-circle fa-3x"></i>';    
    infoDiv.addEventListener('click', helpMeFun);

    function helpMeFun () {
        alert('Pro použití filtrů zadejte "filtr: hodnota".\n\nDostupné filtry:\n   kategorie:\n');   
    }

    document.getElementById("center-Section").appendChild(titleText);
    document.getElementById("center-Section").appendChild(separatorDiv);
    document.getElementById("center-Section").appendChild(tableContainerDiv);
    document.getElementById('center-Section').appendChild(infoDiv);
}

function getFinishedRunners() {
    let allRunners = window.competition.runners;
    let runnersToShow = [];

    allRunners.forEach(runner => {
        if (runner.punches.length != 0) {  // If runner hasnt finished yet, pop him out
            runnersToShow.push(runner);
        }
    }); 

    return sortByTime(runnersToShow);
}

function applyFilter(value) {
    display = document.getElementById('filter_display');
    display.style.display = "none";
    value = value.toLowerCase();

    let valueRegex = new RegExp('^kategorie: [a-z][0-9][0-9]$');
    if (valueRegex.test(value)) {
        cat = value.split(' ')[1].toUpperCase();

        let new_runners_to_show = [];

        for (let i=0; i < runnersToShow.length; i++) {
            if (runnersToShow[i].category == cat) {
                new_runners_to_show.push(runnersToShow[i])
            }
        }
        runnersToShow = sortByTime(new_runners_to_show);

        tbody = document.getElementById('tbody');
        tbody.innerHTML = "";
        tbody = fillTable(tbody);   // Fill the table with filtered names

        runnersToShow = runnersToShowOriginal;  // But restore the original so that the next search has them all to go through
    }
    else {
        if (value != "") {
            display = document.getElementById('filter_display');
            display.innerHTML = "Uvedený filtr je neplatný! Zkuste to znovu!";
            display.style.display = "block";
        }
    }
}

function sortByTime(runners) {
    let ok = false
    while (true) {

        if (ok == true) {
            break;
        }

        ok = true;

        for(let i = 0; i < runners.length; i++) {
            if (i+1 < runners.length) {
                let time1 = parseInt(runners[i].time);
                if (time1 > parseInt(runners[i+1].time)) {
                    let tmp = runners[i+1];
                    runners[i+1] = runners[i];
                    runners[i] = tmp;
                    ok = false;
                }
            }
        }
    }
    return runners;
}

function fillTable(tbody) {
    let i = 1;
    runnersToShow.forEach(runner => {
        let tRow = document.createElement('tr');
        let th = "<th scope=\"row\">" + i++ + "</th>";
        tRow.innerHTML = th;

        let tds = "<td>" + runner.name + "</td>" +
                  "<td>" + runner.surname + "</td>" +
                  "<td>" + runner.category + "</td>" +
                  "<td>" + runner.club + "</td>" +
                  "<td>" + printableTime(runner.time) + "</td>";
        
                  tRow.innerHTML += tds;

        tbody.appendChild(tRow);

    });
    return tbody;
}

function printableTime(time) {
    let hours = Math.floor(time/3600);
    let minutes = Math.floor((time-hours*3600)/60);
    let seconds = time-hours*3600-minutes*60;

    readable_time = addZero(hours) + ':' + addZero(minutes) + ':' + addZero(seconds);
    return readable_time;
}

function addZero(time) {
    if (time < 0)
        time = -time;
    if (time < 10)
        return "0" + time;
    return time;
}

module.exports = {renderTables}; 