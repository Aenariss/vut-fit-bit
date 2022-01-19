/**
 * Modul implementující export/uložení aktuálního stavu závodu.
 * @author Vojtěch Fiala [xfiala61]
 */
const fs = require('fs');

/* Function to save current competition into a file */
function saveRaceToFile(overwrite) {
    let competition = window.competition;
    competition = JSON.stringify(competition, null, 4); // Stringify current competition, use spacing of 4 spaces
    let export_dir = './exports';

    // Check if export dir exists
    if (!fs.existsSync(export_dir)) {
        fs.mkdirSync(export_dir);
    }

    // Read files in dir
    let files = fs.readdirSync(export_dir);
    // get last number in file name
    let filename = makeFilename(files, overwrite);
    fs.writeFileSync(export_dir + '/' + filename, competition);

}

/* Function to make a new file name (according to if we want to overwrite ccurrent file or not) */
function makeFilename(files, overwrite) {
    let filename;
    let new_number = 0;


    if (files.length == 0) {    // if there is no file present in the folder, use this as the default name
        return "O-RaceIt-Export-1.json";
    }
    else {  // Else make a new name with the number of the last file -> Last was ..Export-1, the next one will be Export-2 etc.. 
        files.forEach(file => {
            let dot = findDot(file);
            if (dot > 0) {
                let file_number = getFileNumber(file.substring(0, dot));
                let tmp_new_number = parseInt(file_number)+1-overwrite; // If the overwrite parameter was given, we get the same name we already have
                if (tmp_new_number > new_number)
                    new_number = tmp_new_number;
            }
        });
    }
    filename = "O-RaceIt-Export-" + new_number + '.json';
    return filename;
}

// Function to find dot in filename (if there's any)
function findDot(filename) {
    let dot = -1;
    let length = filename.length;
    for(let i=0; i < length; i++) {
        if (filename[i] == '.')
            dot = i;
    }
    return dot;
}

/* Function to get the number from the end of the filename */
function getFileNumber(filename) {
    let length = filename.length;
    let i = length-1;
    for (i; i >= 0; i--) {
        if (!Number.isInteger(filename[i])) // If we found something that was not an integer, we're done
            break;
    }
    return filename.substring(i, length);
}

module.exports = { saveRaceToFile };