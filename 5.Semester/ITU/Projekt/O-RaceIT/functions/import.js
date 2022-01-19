/**
 * Funkce pro import dat závodu.
 * @author Vojtěch Fiala [xfiala61]
 */
const fs = require('fs');
const { competitionClass } = require('../classDefinitions/classCompetition');

/* Function to import race from file */
function importRaceFromFile(file) {
    full_file_path = './exports/' + pureFileName(file);   // This is stupid, but i couldnt think of anything better
    loadDatasetFromJSON(full_file_path) // Load the file from path
}

/* Function to get rid of fake filepath and leave only the pure file name */
function pureFileName(file) {
    let length = file.length;
    let i = length-1;
    for(i;i > 0;i--) {
        if (file[i] == '\\')
            break;
    }
    return file.substring(i+1, length)
}

/* Function to load JSON from file and fill the comp class */
function loadDatasetFromJSON(filename) {
    fs.readFile(filename, 'utf8' , (err, data) => {
      if (err) {  // If  user opened something outside of exports file, we cant load it, so it was an error
        console.error("Couldnt read file, it doesnt exist or error picked something outside of export folder");
        return
      }
      
      const dataset = JSON.parse(data);
      competition = new competitionClass();
      competition.loadJSON(dataset);
  
      window.competition = competition; // Set the global variable
    });
}

module.exports = { importRaceFromFile };