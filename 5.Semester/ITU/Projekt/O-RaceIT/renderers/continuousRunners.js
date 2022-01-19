/**
 * Funkce pro zobrazení postupně dobíhajících závodníků.
 * @author Vojtěch Fiala [xfiala61]
 */


const { Runner } = require("../classDefinitions/classRunner");
const { RunnerAdder } = require("../classDefinitions/classRunnerAdder");
const { newFunction } = require("./runnersRender");
const { compareArrays } = require("../functions/compareArrays");
const fs = require('fs');

let currentID = 0;
let colorChoosing = 0;
let curr_ID;
let err_curr_ID = 0;

function addRacer() {
    fs.readFile('./static/testResults.json', 'utf8' , (err, data) => {
        if (err) {
          console.error(err);
          return
        }
    
        const dataset = JSON.parse(data);
        var racers_number = Object.keys(dataset).length;    // Load the number of all results in test file
        
        if (currentID < racers_number) {    // Check if I read the whole results file
            let competition = window.competition;
            let totalRuners = competition.runners.length;
            var cardID = dataset[currentID].cardID;
            curr_ID = cardID;
            var runner = competition.getRunnerByID(cardID);

            var punch_sequence;
            var n_of_courses = competition.courses.length;
            
            // Get correct sequence from runner's category
            for (var i = 0; i < n_of_courses; i++) {
                if (competition.courses[i].name == runner.category) {
                    punch_sequence = competition.courses[i].controlsSequence;
                }
            }

            if (runner == false) {  // Handle the incorrect chip number error
                err_curr_ID += currentID++;
                document.getElementById('remaining-runners').innerHTML = "V lese zbývá: " + (totalRuners-currentID);
                console.log("Debug entry: invalid chip number!");
                
                var new_div = document.createElement('div')
                new_div.setAttribute('id', "err-runner-" + err_curr_ID);
                if (colorChoosing++ % 2 == 0)
                    new_div.setAttribute('class', "text-center bg-danger status-dnf background-n-1");
                else 
                    new_div.setAttribute('class', "text-center bg-danger status-dnf background-n-2");

                new_div.innerHTML = "NEZNÁMÁ OSOBA" + " <br>" +  "Čip: " + cardID + "<br>";
                new_div.innerHTML += "<button type='button' id='fix-" + err_curr_ID + "' class='btn btn-warning'>Vyřešit</button>";
                document.getElementById('result-container').appendChild(new_div);
                document.getElementById("fix-" + err_curr_ID).addEventListener('click', function(){ createSearch(dataset) })
                return;
            }


            var finished_racer_data = new RunnerAdder(runner, dataset, currentID);
            runner.setTime();

            // Change remaining runners
            ++currentID;
            document.getElementById('remaining-runners').innerHTML = "V lese zbývá: " + (totalRuners-currentID);


            // If he did get the sequence right, it's OK
            var new_div = document.createElement('div');
            new_div.setAttribute('id', "ok-runner-" + currentID);
            if (compareArrays(runner.getPunchesNumbers(), punch_sequence)) {
                runner.checkRunValidity(competition)
            }
            // Else he is disqualified
            else {
                runner.status = "DNF";
            }

            if (colorChoosing++ % 2 == 0) {
                if (runner.status == "OK") {
                    new_div.setAttribute('class', "text-center status-ok");
                }
                else {
                    new_div.setAttribute('class', "text-center status-dnf background-n-1");
                }
            }
            else {
                if (runner.status == "OK") {
                    new_div.setAttribute('class', "text-center status-ok");
                }
                else {
                    new_div.setAttribute('class', "text-center status-dnf background-n-2");
                }
            }
                
            new_div.innerHTML = runner.name + " " + runner.surname + " (" 
            + runner.category + ")";

            // Solved icon
            var icon_container = document.createElement('span');
            icon_container.setAttribute('id', 'icon-ok-runner-' + currentID);
            var solved_icon = document.createElement('i');
            solved_icon.setAttribute('class', 'solved-icon fas fa-times fa-lg');
            icon_container.appendChild(solved_icon);
            new_div.appendChild(icon_container);
            
            new_div.innerHTML += "<br>" + "Status: " + runner.status + "</div>"

            document.getElementById('result-container').appendChild(new_div);
            document.getElementById('icon-ok-runner-' + currentID).addEventListener('click', function(){ hideDiv(new_div) })
            // Also, add the sequence finished_racer_data he got into the testCompetition file (is only in memory now)

            return;
        }
        else {
            console.log("Debug entry: Read the last of finished racers!");
        }
    });
}

function hideDiv(div) {
    console.log("i'm here");
    div.style.display = "none";
}

function createSearch(dataset) {
    
    div_to_check = document.getElementById("err-runner-" + err_curr_ID);

    center_section_save = document.getElementById("center-Section").innerHTML;

    document.getElementById("center-Section").innerHTML = "";

    new_container = document.createElement('div');
    new_container.setAttribute('id', 'newContainer');
    document.getElementById('center-Section').appendChild(new_container);

    new_div = document.createElement('div');
    new_div.setAttribute('id', 'new-div');
    new_div.innerHTML = "Zadejte jméno a přijmení závodníka:";
    

    search_bar = document.createElement('input');
    search_bar.setAttribute('type', 'text');
    search_bar.setAttribute('id', 'searchBar');

    search_bar_container = document.createElement('div');
    search_bar_container.setAttribute('id', 'search_bar_cont');
    search_bar.setAttribute('id', 'searchBar');

    newRunnerBtn = document.createElement('button');
    newRunnerBtn.setAttribute('id', 'newRunnerBtn');
    newRunnerBtn.setAttribute('class', 'btn btn-success');
    newRunnerBtn.innerHTML = 'Přidat nového závodníka';
    newRunnerBtn.addEventListener('click', newFunction);

    
    let wrapper = document.createElement('div');
    wrapper.appendChild(new_div);
    wrapper.appendChild(search_bar_container);
    wrapper.appendChild(newRunnerBtn);
    wrapper.setAttribute('id', 'wrapperDiv');
    new_container.appendChild(wrapper);
    search_bar_container.appendChild(search_bar);

    var runners_div = document.createElement('div');
    runners_div.setAttribute('id', 'runnersDiv');
    new_container.appendChild(runners_div);

    document.getElementById('searchBar').addEventListener('input', function(){ searchRunners(center_section_save, dataset) });

}

function searchRunners(center_section_save, dataset) {
    let competition = window.competition;
    document.getElementById('runnersDiv').innerHTML = "";


    var search_value = document.getElementById('searchBar').value;
    if (search_value != "") {
        var search_results = search_value.split(' ');
        if (search_results.length > 0 && search_results.length < 3) {

            var full_text;
            if (search_results.length == 2) {
                full_text = search_results[0] + ' ' + search_results[1];
            }
            else if (search_results.length == 1) {
                full_text = search_results[0];
            }
            var runners = competition.getRunnersByName(full_text.toLowerCase());
            for(let i=0; i < runners.length; i++) {
                let new_div = document.createElement('div');
                new_div.setAttribute('id', 'search-res-' + i);
                new_div.setAttribute('class', 'runner-div-with-items');
                new_text_div = document.createElement('div');
                new_text_div.innerHTML = runners[i].name + ' ' + runners[i].surname + ' (' + runners[i].category + ')' + ' : ' + runners[i].club;
                new_text_div.innerHTML += "  <button type='button' id='assign-" + i +  "' class='btn btn-success btn-runner'>Priradit cip</button>";
                new_div.appendChild(new_text_div);
                document.getElementById('runnersDiv').appendChild(new_div);
                document.getElementById('assign-' + i).addEventListener('click', function(){ assignChip(runners[i], center_section_save, dataset) });
            }
        }
    }
}

function assignChip(runner, center_section_save, dataset) {
    runner.cardId = curr_ID.toString();

    let competition = window.competition;

    var punch_sequence;
    var n_of_courses = competition.courses.length;
            
    // Get correct sequence from runner's category
    for (var i = 0; i < n_of_courses; i++) {
        if (competition.courses[i].name == runner.category) {
            punch_sequence = competition.courses[i].controlsSequence;
        }
    }

    var finished_racer_data = new RunnerAdder(runner, dataset, err_curr_ID);
    runner.setTime();

    new_div = document.getElementById('err-runner-' + err_curr_ID);
    new_div.setAttribute('id', "ok-runner-" + err_curr_ID);

    if (compareArrays(runner.getPunchesNumbers(), punch_sequence)) {
        runner.checkRunValidity(competition)
    }
    // Else he is disqualified
    else {
        runner.status = "DNF";
    }

    if (colorChoosing++ % 2 == 0) {
        if (runner.status == "OK") {
            new_div.setAttribute('class', "text-center status-ok");
        }
        else {
            new_div.setAttribute('class', "text-center status-dnf");
        }
    }
    else {
        if (runner.status == "OK") {
            new_div.setAttribute('class', "text-center status-ok");
        }
        else {
            new_div.setAttribute('class', "text-center status-dnf");
        }
    }

    new_div.innerHTML = runner.name + " " + runner.surname + " (" 
    + runner.category + ")";

    var icon_container = document.createElement('span');
    icon_container.setAttribute('id', 'icon-ok-runner-fixed-' + err_curr_ID);
    var solved_icon = document.createElement('i');
    solved_icon.setAttribute('class', 'solved-icon fas fa-times fa-lg');
    icon_container.appendChild(solved_icon);
    new_div.appendChild(icon_container);
    
    new_div.innerHTML += "<br>" + "Status: " + runner.status + "</div>"


    document.getElementById('icon-ok-runner-fixed-' + err_curr_ID).addEventListener('click', function(){ hideDiv(new_div) })

    document.getElementById("center-Section").innerHTML = center_section_save;
}

module.exports = { addRacer };
