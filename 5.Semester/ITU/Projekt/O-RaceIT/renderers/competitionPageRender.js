/**
 * Stránka pro zobrazení závodů.
 * @author Vojtěch Bůbela [xbubel08]
 */
const { courseClass } = require("../classDefinitions/classCourse");
const { competitionClass } = require('../classDefinitions/classCompetition');
const timeModule = require("../functions/timeConvertor");


/* this function renders the competition page */
function competitionPageRender(compClass) {

    /* get reference to center content of app and empty it */
    mainDiv = document.getElementById("center-Section");
    mainDiv.innerHTML='';

    /* create inner div with grid */
    let innDiv = document.createElement('div');
    innDiv.setAttribute('id', 'competition-inner-div');

    let navBar = document.createElement('div');
    navBar.setAttribute('id', 'competition-nav-bar')
    let createComp = document.createElement('button');
    createComp.setAttribute('id', 'competion-create-button');
    createComp.setAttribute('class', 'competition-button btn btn-outline-dark');

    /* when this button is clicked a corresponding page will be rendered in main content of app */
    createComp.addEventListener('click', function() {createNewComp(compContent, compClass)});
    createComp.innerHTML = 'Vytvořit závod';

    let currentComp = document.createElement('button');
    currentComp.setAttribute('id', 'competition-current-button');
    currentComp.setAttribute('class', 'competition-button btn btn-outline-dark');
    currentComp.addEventListener('click', function() {createCurrentComp(compContent, compClass)})
    currentComp.innerHTML = 'Současný závod';

    let compContent = document.createElement('div');

    compContent.setAttribute('id', 'comp-main-content');
    
    
    
    navBar.appendChild(createComp);
    navBar.appendChild(currentComp);
    innDiv.appendChild(navBar);
    innDiv.appendChild(compContent);
    mainDiv.appendChild(innDiv);

    /* by default current competiton will be loaded */
    createCurrentComp(compContent, compClass);
    
    
    
}

/* this function implements rendering of page, that will let user create and save new competiton */
function createNewComp(compContent) {

    newComp = new competitionClass();

    /* delete previous content */
    compContent.innerHTML = '';

    let separatorDiv = document.createElement('div');
    separatorDiv.setAttribute('id', 'comp-main-separator');
    compContent.appendChild(separatorDiv);

    let sectionName = document.createElement('div');
    sectionName.setAttribute('id', 'comp-section-name');
    sectionName.innerHTML = 'Vytvořit nový závod:';
    compContent.appendChild(sectionName);

    let sectionBody = document.createElement('div');
    sectionBody.setAttribute('id', 'comp-section-body');
    compContent.appendChild(sectionBody);

    let nameDiv = document.createElement('div');
    nameDiv.setAttribute('id', 'create-current-name-div');
    nameDiv.setAttribute('class', 'comp-main-content-box');
    nameDiv.innerHTML = 'Název závodu:';
    compContent.appendChild(nameDiv);

    let nameValueDiv = document.createElement('input');
    nameValueDiv.setAttribute('id', 'create-current-name-value-div');
    nameValueDiv.setAttribute('class', 'comp-main-content-box comp-input');
    compContent.appendChild(nameValueDiv);

    let dateDiv = document.createElement('div');
    dateDiv.setAttribute('id', 'create-current-date-div');
    dateDiv.setAttribute('class', 'comp-main-content-box');
    dateDiv.innerHTML = 'Datum konání závodu:';
    compContent.appendChild(dateDiv);

    let dateValueDiv = document.createElement('input');
    dateValueDiv.setAttribute('id', 'create-current-date-value-div');
    dateValueDiv.setAttribute('class', 'comp-main-content-box comp-input');
    dateValueDiv.setAttribute('type', 'date');
    
    compContent.appendChild(dateValueDiv);

    let timeDiv = document.createElement('div');
    timeDiv.setAttribute('id', 'create-current-time-div');
    timeDiv.setAttribute('class', 'comp-main-content-box');
    timeDiv.innerHTML = 'Čas konání závodu:';
    compContent.appendChild(timeDiv);

    let timeValueDiv = document.createElement('input');
    timeValueDiv.setAttribute('id', 'create-current-time-value-div');
    timeValueDiv.setAttribute('class', 'comp-main-content-box comp-input');
    timeValueDiv.setAttribute('type', 'time');
    compContent.appendChild(timeValueDiv);

    let coursesDiv = document.createElement('div');
    coursesDiv.setAttribute('id', 'create-current-courses-div');
    coursesDiv.setAttribute('class', 'comp-main-content-box');
    coursesDiv.innerHTML = 'Kategorie:';
    compContent.appendChild(coursesDiv);

    let coursesValueDiv = document.createElement('input');
    coursesValueDiv.setAttribute('id', 'create-current-courses-value-div');
    coursesValueDiv.setAttribute('class', 'comp-main-content-box comp-input');
    coursesValueDiv.addEventListener('focusout', function() {getCoursesValue(newComp)});
    compContent.appendChild(coursesValueDiv);

    let submitButton = document.createElement('button');
    submitButton.setAttribute('id', 'comp-submit-button');
    submitButton.innerHTML = 'Uložit změny:';
    compContent.appendChild(submitButton);
    submitButton.addEventListener('click', function() {saveNewComp(newComp)});

    let coursesPunches = document.createElement('div');
    coursesPunches.setAttribute('id', 'comp-cour-punch');
    coursesPunches.setAttribute('class', 'comp-main-content-box');
    coursesPunches.innerHTML = 'Kontroly pro kategorii:';
    compContent.appendChild(coursesPunches);
    
    let coursesPunchesValue = document.createElement('select');
    coursesPunchesValue.setAttribute('id', 'comp-cour-punch-val');
    coursesPunchesValue.setAttribute('class', 'comp-main-content-box comp-input');
    coursesPunchesValue.addEventListener('change', function() {loadPunches(newComp)});
    compContent.appendChild(coursesPunchesValue);

    let printPunchesVal = document.createElement('input');
    printPunchesVal.setAttribute('id', 'comp-print-punches-val');
    printPunchesVal.setAttribute('class', 'comp-main-content-box comp-input');
  
    compContent.appendChild(printPunchesVal);

    /* load categories into drop down menu, since this is called over empty category array
       it will only result in "vyberte kategorii" option */
    getCoursesValue(newComp);
}

/* implements rendering of current competition, very similar to rendering new competition
   however this function also renders values read from compClass. User can edit these values */
function createCurrentComp(compContent, compClass) {

    compClass = window.competition;

    compContent.innerHTML = '';

    let separatorDiv = document.createElement('div');
    separatorDiv.setAttribute('id', 'comp-main-separator');
    compContent.appendChild(separatorDiv);

    let sectionName = document.createElement('div');
    sectionName.setAttribute('id', 'comp-section-name');
    sectionName.innerHTML = 'Současný závod';
    compContent.appendChild(sectionName);

    let sectionBody = document.createElement('div');
    sectionBody.setAttribute('id', 'comp-section-body');
    compContent.appendChild(sectionBody);

    let nameDiv = document.createElement('div');
    nameDiv.setAttribute('id', 'create-current-name-div');
    nameDiv.setAttribute('class', 'comp-main-content-box');
    nameDiv.innerHTML = 'Název závodu:';
    compContent.appendChild(nameDiv);

    let nameValueDiv = document.createElement('input');
    nameValueDiv.setAttribute('id', 'create-current-name-value-div');
    nameValueDiv.setAttribute('class', 'comp-main-content-box comp-input');
    nameValueDiv.setAttribute('value', compClass.name);
    compContent.appendChild(nameValueDiv);

    let dateDiv = document.createElement('div');
    dateDiv.setAttribute('id', 'create-current-date-div');
    dateDiv.setAttribute('class', 'comp-main-content-box');
    dateDiv.innerHTML = 'Datum konání závodu:';
    compContent.appendChild(dateDiv);

    let dateValueDiv = document.createElement('input');
    dateValueDiv.setAttribute('id', 'create-current-date-value-div');
    dateValueDiv.setAttribute('class', 'comp-main-content-box comp-input');
    dateValueDiv.setAttribute('type', 'date');
    
    let date = compClass.date;
    // convert date read from compClass into correct format fot html type=date
    let correctDate = timeModule.getAmericanDate(date);
    dateValueDiv.setAttribute('value', correctDate);
    compContent.appendChild(dateValueDiv);

    let timeDiv = document.createElement('div');
    timeDiv.setAttribute('id', 'create-current-time-div');
    timeDiv.setAttribute('class', 'comp-main-content-box');
    timeDiv.innerHTML = 'Čas konání závodu:';
    compContent.appendChild(timeDiv);

    let timeValueDiv = document.createElement('input');
    timeValueDiv.setAttribute('id', 'create-current-time-value-div');
    timeValueDiv.setAttribute('class', 'comp-main-content-box comp-input');
    timeValueDiv.setAttribute('type', 'time');
    let time = compClass.startTime;
    // convert start time from seconds to hours:minutes
    let correctTime = timeModule.convertTimeToFormat(time); 
    timeValueDiv.setAttribute('value', correctTime);
    compContent.appendChild(timeValueDiv);

    let coursesDiv = document.createElement('div');
    coursesDiv.setAttribute('id', 'create-current-courses-div');
    coursesDiv.setAttribute('class', 'comp-main-content-box');
    coursesDiv.innerHTML = 'Kategorie:';
    compContent.appendChild(coursesDiv);

    let coursesValueDiv = document.createElement('input');
    coursesValueDiv.setAttribute('id', 'create-current-courses-value-div');
    coursesValueDiv.setAttribute('class', 'comp-main-content-box comp-input');
    let coursesText = '';
    compClass.courses.forEach(getCourses);
    
    coursesValueDiv.setAttribute('value', coursesText);
    coursesValueDiv.addEventListener('focusout', function() {getCoursesValue(compClass)});
    compContent.appendChild(coursesValueDiv);

    // read courses from compClass and print them out for user
    function getCourses(value, index) {
        coursesText += value.name ;
        if(index != compClass.courses.length -1)
            coursesText += ', ';
    }

    let submitButton = document.createElement('button');
    submitButton.setAttribute('id', 'comp-submit-button');
    submitButton.innerHTML = 'Uložit změny';
    compContent.appendChild(submitButton);
    /* rewrite compClass according to user input */
    submitButton.addEventListener('click', function() {saveStuff(compClass)});

    let coursesPunches = document.createElement('div');
    coursesPunches.setAttribute('id', 'comp-cour-punch');
    coursesPunches.setAttribute('class', 'comp-main-content-box');
    coursesPunches.innerHTML = 'Kontroly pro kategorii:';
    compContent.appendChild(coursesPunches);
    
    let coursesPunchesValue = document.createElement('select');
    coursesPunchesValue.setAttribute('id', 'comp-cour-punch-val');
    coursesPunchesValue.setAttribute('class', 'comp-main-content-box comp-input');
    /* whenever user changes dropdown menu of categories, punches for the category refresh accordingly */
    coursesPunchesValue.addEventListener('change', function() {loadPunches(compClass)});
    compContent.appendChild(coursesPunchesValue);

    let printPunchesVal = document.createElement('input');
    printPunchesVal.setAttribute('id', 'comp-print-punches-val');
    printPunchesVal.setAttribute('class', 'comp-main-content-box comp-input');
    compContent.appendChild(printPunchesVal);

    /* initial load of drop down menu categories */
    getCoursesValue(compClass);
}

/* call this function when user tries to save new competition.
   Alerts user that he will overwrite his current competition if he has any */
function saveNewComp(compClass) {
    var answer = window.confirm("Kliknutím na ok smažete svůj starý závod\nPřejete si pokračovat?");
    if (answer) {
        saveStuff(compClass);
        console.log(compClass, window.competition);
        window.competition = compClass;
    }
    else {
        return;
    }
    
}

/* this function read values from  categories and fills drop down menu under it*/
function getCoursesValue() {
    let couArr = document.getElementById('create-current-courses-value-div').value;

    let parsedCat = getCategoriesArr(couArr);
    console.log("parsedCat lenght is: " + parsedCat.length);
    if(parsedCat.length != 0) {
        let selectType = document.getElementById('comp-cour-punch-val');

        removeAllChildNodes(selectType);
        console.log("here");

        let pickCat = document.createElement('option');
        pickCat.setAttribute('value', 'VyberteKategorii');
        pickCat.innerHTML = 'Vyberte kategorii';
        selectType.appendChild(pickCat);

        parsedCat.forEach(setoption);
        function setoption(value, index){
            let x = document.createElement('option');
            x.setAttribute('value', value);
            x.innerHTML = value;
            selectType.appendChild(x);
        }


    } else {
        /* alert user that he inputed wrong category name by changing value of dropwodn menu to error */
        let errorSelect = document.getElementById('comp-cour-punch-val');
        removeAllChildNodes(errorSelect);
        let x = document.createElement('option');
        x.setAttribute('value', 'Špatný název kategorie!');
        x.innerHTML = 'Špatný název kategorie!';
        errorSelect.appendChild(x);
    }

    let punchesDiv = document.getElementById('comp-print-punches-val');
    punchesDiv.setAttribute("value", []);
}

/* according to selected option in drop down menu load its punches */
function loadPunches(compClass) {
    let punchesPrint = document.getElementById('comp-cour-punch-val');
    let text = punchesPrint.options[punchesPrint.selectedIndex].innerHTML;
    let punchesDiv = document.getElementById('comp-print-punches-val');
    punchesDiv.setAttribute("value", []);
    console.log(compClass.courses);
    compClass.courses.forEach(course => {
        if(course.name == text) {
            punchesDiv.parentNode.removeChild(punchesDiv);
            punchesDiv = document.createElement('input');
            punchesDiv.setAttribute('id', 'comp-print-punches-val');
            punchesDiv.setAttribute('class', 'comp-main-content-box comp-input');
            punchesDiv.setAttribute("value", course.controlsSequence);
            document.getElementById('comp-main-content').appendChild(punchesDiv);
        }
    });
}

function removeAllChildNodes(parent) {
    while (parent.firstChild) {
        parent.removeChild(parent.firstChild);
    }
}

function saveStuff(compClass) {
    console.log("Called save function!");
    saveName(compClass);
    saveDate(compClass);
    saveCategories(compClass);
    console.log(compClass.courses);

}

function saveName(compClass) {
    let name = document.getElementById('create-current-name-value-div').value;
    compClass.setName(name);
}

function saveDate(compClass) {
    let date = document.getElementById('create-current-date-value-div').value;
    compClass.setDate(timeModule.getCzechDate(date));
}

function saveCategories(compClass) {
    let categories = document.getElementById('create-current-courses-value-div').value;
    let cat_arr = getCategoriesArr(categories);
    let existing_courses_names = [];
    compClass.courses.forEach(course => {
        existing_courses_names.push(course.name);
    });
    // Create new CourseClass from name & its control sequence; push the object into array and set into global compclass
    if (cat_arr.length < existing_courses_names.length) {
        getCategoriesToDelete(compClass, existing_courses_names, cat_arr);
    }

    cat_arr.forEach(category => {
        if (!existing_courses_names.includes(category)) {
            let course_to_add = new courseClass(category, []);
            compClass.courses.push(course_to_add);
            if (document.getElementById('comp-cour-punch-val').value == category) {
                let controls_to_add = document.getElementById('comp-print-punches-val').value;
                console.log("test ", controls_to_add)
                course_to_add.controlsSequence = createSequence(controls_to_add);
            }
        }
        else {
            compClass.courses.forEach(course => {
                if (course.name == document.getElementById('comp-cour-punch-val').value) {
                    let controls_to_add = document.getElementById('comp-print-punches-val').value;
                    course.controlsSequence = createSequence(controls_to_add);
                }
            });
        }
    });
}

function getCategoriesToDelete(compClass, existing_courses_names, cat_arr) {
    for (let i =0; i < existing_courses_names.length; i++) {
        if (!cat_arr.includes(existing_courses_names[i])) {
            compClass.courses.forEach(course => {
                if (course.name == existing_courses_names[i]) {
                    var index = compClass.courses.indexOf(course);
                    compClass.courses.splice(index, 1);
                }
            });
        }
    }
};

function createSequence(controls) {
    let arr = [];
    let prev = -1;
    for(let i=0; i < controls.length; i++) {
        if (controls[i] == ',') {
            let ctr;
            if (arr.length == 0)
                ctr = controls.substring(0, i);
            else
                ctr = controls.substring(prev+1, i);
            if (isValidControl(ctr)) {
                arr.push(ctr);
            }
            else {
                return [];
            }
            prev = i;
        }
    }
    last_ctr = controls.substring(prev+1, controls.length);
    if (isValidControl(last_ctr))
        arr.push(last_ctr);
    else {
        return []
    }
    return arr;
}

function isValidControl(control) {
    const regex = new RegExp('^[1-9][0-9][0-9]$');

    if (regex.test(control))
        return true;
    return false;
}

function getCategoriesArr(categories) {
    let cat_arr = [];
    let prev = -2;
    for(let i=0; i < categories.length; i++) {
        if (categories[i] == ',') {
            let category;
            if (cat_arr.length == 0)
                category = categories.substring(0, i);
            else
                category = categories.substring(prev+2, i);
            if (isValidCategory(category)) {
                cat_arr.push(category);
            }
            else {
                return [];
            }
            prev = i;
        }
    }
    last_cat = categories.substring(prev+2, categories.length);

    if (isValidCategory(last_cat))
        cat_arr.push(last_cat);
    else {
        return [];
    }

    return cat_arr;
}

function isValidCategory(category) {
    const regex = new RegExp('^[A-Z][0-9][0-9]$');

    if (regex.test(category))
        return true;
    return false;
}

module.exports = {competitionPageRender};
