/**
 * Modul pro načtení dat ze souboru.
 * @author Vojtěch Malý [xmalv03]
 */
const fs = require('fs');
const { competitionClass } = require('../classDefinitions/classCompetition');

let competition;

// Function to load default data from the test competition json file
function loadDatasetFromJSON() {
  fs.readFile('./static/testCompetition.json', 'utf8' , (err, data) => {
    if (err) {
      console.error(err);
      return
    }
    //console.log(data)
    
    const dataset = JSON.parse(data);
    competition = new competitionClass();
    competition.loadJSON(dataset);

    // Bind 
    window.competition = competition;
    //return competition;
  });
}

module.exports = {competition};

module.exports = { loadDatasetFromJSON };

