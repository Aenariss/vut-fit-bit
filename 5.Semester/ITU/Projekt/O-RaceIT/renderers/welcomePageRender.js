/**
 * Funkce implementující zobrazení úvodní stránky-
 * @author Vojtěch Bůbela [xbubel08]
 */

const { runnersRender } = require("./runnersRender");
const { competitionPageRender} = require("./competitionPageRender");
const { aboutUsPageRender } = require("./aboutUsRender");
const { renderTables } = require("./tablePageRender");

/* renders welcome page */
function welcomePageRender() {

    /* get reference to center content of app and empty it */
    mainDiv = document.getElementById("center-Section");
    mainDiv.innerHTML='';

    /* create inner div with grid */
    let innDiv = document.createElement('div');
    innDiv.setAttribute('id', 'welcome-inner-div');

    /* div for image of logo */
    let logoDiv = document.createElement('div');
    logoDiv.setAttribute('id', 'welcome-logo-div');
    
    let logoImg = document.createElement('img');
    logoImg.setAttribute('src', './images/raceitlogo1.png');
    logoImg.setAttribute('class', 'img-fluid');
    logoImg.setAttribute('alt', 'O-RaceIT logo');

    let separatorDiv = document.createElement('div');
    separatorDiv.setAttribute('id', 'welcome-main-separator');
   
    /* create buttons as another way of navigation */
    let raceButton = document.createElement('div');
    raceButton.setAttribute('id', 'welcome-race-button');
    raceButton.setAttribute('class', 'button from-top');
    raceButton.addEventListener('click', loadcompetitionPage); 
    let raceText = document.createElement('div');
    raceText.setAttribute('id', 'text-div');
    raceText.innerHTML = 'Závod';

    let racersButton = document.createElement('div');
    racersButton.setAttribute('id', 'welcome-racers-button');
    racersButton.setAttribute('class', 'button');
    racersButton.addEventListener('click', loggerComp);
    let racersText = document.createElement('div');
    racersText.setAttribute('id', 'text-div');
    racersText.innerHTML = 'Závodníci';

    let tableButton = document.createElement('div');
    tableButton.setAttribute('id', 'welcome-table-button');
    tableButton.setAttribute('class', 'button');
    let tableText = document.createElement('div');
    tableText.setAttribute('id', 'text-div');
    tableText.innerHTML = 'Tabulky';
    tableButton.addEventListener('click', renderTables);

    let aboutButton = document.createElement('div');
    aboutButton.setAttribute('id', 'welcome-about-button');
    aboutButton.setAttribute('class', 'button');
    aboutButton.addEventListener('click', aboutUsPageRender);
    let aboutText = document.createElement('div');
    aboutText.setAttribute('id', 'text-div');
    aboutText.innerHTML = 'O nás';


    aboutButton.appendChild(aboutText);
    tableButton.appendChild(tableText);
    racersButton.appendChild(racersText);
    raceButton.appendChild(raceText);
    logoDiv.appendChild(logoImg);
    innDiv.appendChild(logoDiv);
    innDiv.appendChild(separatorDiv);
    innDiv.appendChild(raceButton);
    innDiv.appendChild(racersButton);
    innDiv.appendChild(tableButton);
    innDiv.appendChild(aboutButton);
    mainDiv.appendChild(innDiv);

}

/* renders runners page */
function loggerComp() {
    console.log("View changed on Runners.");
    runnersRender(window.competition);
}

/* render competition page */
function loadcompetitionPage() {
    console.log("Loading competition page");
    competitionPageRender(window.competition);
}

module.exports = {welcomePageRender};