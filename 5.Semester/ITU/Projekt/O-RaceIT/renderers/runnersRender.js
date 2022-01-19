/**
 * Stránka zobrazující běžce a jejich informace.
 * @author Vojtěch Malý [xmalyv03]
 */

 const { Runner } = require("../classDefinitions/classRunner");
let runner;
let idOfRunner;
function runnersRender(compClass) {
    // CLear main div and full with html template. 
    mainDiv = document.getElementById("center-Section");
    mainDiv.innerHTML = "";
    mainDiv.innerHTML =
        '				  <div id="inner-div">' +
        '                   <form id="runnerForm" activeRunner="None">' +
		'					  <h2>Závodník:</h2>'+
        '                     <div id="flexFormInputs">' +
        '                       <label for="nameInput">Jméno:</label>' +
        '                       <input id="nameInput" type="text">' +
        '                       <label for="surnameInput">Přijmení:</label>' +
        '                       <input id="surnameInput" type="text">' +
        '                       <label for="cardIDInput">Číslo čipu:</label>' +
        '                       <input id="cardIDInput" type="text"> ' +
        '                       <label for="categorySelect">Kategorie:</label>' +
        '                       <select id="categorySelect">' +
        "                       </select>" +
        '                       <label for="startTime">Startovací čas:</label>' +
        '                       <input id="startTime" type="number" min="0">' +
        '                       <label for"clubInp">Klub:</label>' +
        '                       <input id="clubInp" type="text">' +
        '                       <label for="statusSelect">Status:</label>' +
        '                       <select id="statusSelect">' +
        '			                    <option value="DNF" class="status-option">DNF</option>' +
        '			                    <option value="REG" class="status-option">REG</option>' +
        '			                    <option value="OK" class="status-option">OK</option>' +
        '			                    <option value="None" class="status-option" selected>---</option>' +
        "		                    </select>" +
        "                     </div>" +
        "                   </form>" +
        '                   <button id="saveChanges">Uložit</button>' +
		'					<button id="deleteRunner">Smazat</button>'+
		'					<button id="newRunner">Nový</button>'+
        '                   <div id="left-runner-div">' +
        "                   </div>" +
		'					<div id="control-sequence">'+
		'						<div id="course-control-sequence"></div>'+
		'						<div id="runner-control-sequence"></div>'+
		'					</div>'+
        "                 </div>";

	// add event listeners for buttons
    document.getElementById("saveChanges").addEventListener("click", saveFunction);
	document.getElementById("deleteRunner").addEventListener("click", deleteFunction);
	document.getElementById("newRunner").addEventListener('click', newFunction);


    let leftRunnerDiv = document.getElementById("left-runner-div");

	// Render runners entry in runnes list
    compClass.runners.forEach(renderNames);
    function renderNames(value, index, array) {
        let x = document.createElement("div");
        x.setAttribute("class", "nameSurname");
        x.setAttribute("style", "cursor:pointer");
        x.setAttribute("id", "runner-" + index);
        x.addEventListener("click", renderInfo);
        x.innerHTML = value.name + " " + value.surname;
        leftRunnerDiv.appendChild(x);
    }

    // Category option render
    let categorySelect = document.getElementById("categorySelect");
    for (var ccr = 0; ccr < compClass.courses.length; ccr++) {
        let newOption = document.createElement("option");
        newOption.setAttribute("value", compClass.courses[ccr].name);
        newOption.setAttribute("class", "course-class-selector");
        newOption.textContent = compClass.courses[ccr].name;
        categorySelect.appendChild(newOption);
    }


    // Render new ruuner info
    function renderInfo(event) {
        // Remove all active tags
        document.querySelectorAll(".nameSurname").forEach(function (element) {
            element.classList.remove("runner-active");
        });
        // Add to menu a runner active class selector.
        idOfRunner = event.path[0].id;
        let idOfRunnerINT = idOfRunner.slice(7);
		// Fill active runner
        runner = compClass.getRunnerByIndex(idOfRunnerINT);
        // Change button apperance back to active
        let menuButton = document.getElementById(idOfRunner);
        menuButton.classList.add("runner-active");

        // set actual category
        document.querySelectorAll(".course-class-selector").forEach(function (element) {
            if (element.getAttribute("value") == runner.category) {
                categorySelect.value = runner.category;
            }
        });

		// set inputs to respective values.
		document.getElementById("nameInput").value = runner.name;
        document.getElementById("surnameInput").value = runner.surname;
        document.getElementById("runnerForm").setAttribute("activeRunner", idOfRunnerINT);
        document.getElementById("cardIDInput").value = runner.cardId;
        document.getElementById("startTime").value = runner.startTime;
        document.getElementById("clubInp").value = runner.club;
        document.querySelectorAll(".status-option").forEach(function (value, index) {
            if (value.getAttribute("value") == runner.status) {
                value.selected = true;
            }
        });

		let divSequenceCourse = document.getElementById("course-control-sequence");
		let divSequenceRunner = document.getElementById("runner-control-sequence");	
		divSequenceCourse.innerHTML = '';														// Erase div
		divSequenceRunner.innerHTML = '';														// Erase div
		let courseSequence = compClass.getCourseSequenceByName(runner.category);
		let returnVal = runner.checkRunValidity(compClass);
		let falseBool = false;

		// Render sequence of punches
		for(let cnt1 = 0; cnt1 < courseSequence.length; cnt1++) {
			let div = document.createElement('div');
			div.setAttribute('class', 'course-control-a');
			let numSpan = document.createElement('span');
			numSpan.setAttribute('class', 'numSpan');
			let controlSpanx = document.createElement('span');
			controlSpanx.setAttribute('class', 'controlSpanx');
			numSpan.innerHTML = (cnt1 + 1) + ".";
			controlSpanx.innerHTML = courseSequence[cnt1];
			div.appendChild(numSpan);
			div.appendChild(controlSpanx);
			divSequenceCourse.appendChild(div);

			let runDiv = document.createElement('div');
			if(returnVal.ErrCode == false) {			// Běžec neběžel
				runDiv.setAttribute('class', 'runner-control-a runner-control-empty');
				runDiv.innerHTML = "---";
				divSequenceRunner.appendChild(runDiv);
			} else if (returnVal.ErrCode == true) {		// Bežec doběhl správně
				runDiv.setAttribute('class', 'runner-control-a runner-control-valid');

				let controlNumberSpan = document.createElement('span');
				controlNumberSpan.innerHTML = "[ " + courseSequence[cnt1] + " ]";
				controlNumberSpan.setAttribute('class', 'control-number-span');
				
				let time = (runner.punches[cnt1].timeStamp - (runner.startTime * 60));
				
				let timeStampSpan = document.createElement('span');
				timeStampSpan.innerHTML = Math.floor(time/60)  + ":" + time%60;
				timeStampSpan.setAttribute('class', 'time-span');
				
				runDiv.appendChild(controlNumberSpan);
				runDiv.appendChild(timeStampSpan);
				divSequenceRunner.appendChild(runDiv);
			} else if (returnVal.ErrCode == 10) {
				runDiv.setAttribute('class', 'runner-control-a runner-control-beforeStart');
				let falseTime = document.createElement('span');
				falseTime.innerHTML = "---:--- (Mimo čas startu!)";
				falseTime.setAttribute('class', 'before-time-span');

				let controlSPany = document.createElement('span');
				controlSPany.innerHTML = "[ " + courseSequence[cnt1] + " ]";
				controlSPany.setAttribute('class', 'before-time-span-control');
				runDiv.appendChild(controlSPany);
				runDiv.appendChild(falseTime);
				divSequenceRunner.appendChild(runDiv);
			} else if (returnVal.ErrCode == 20) {
				runDiv.setAttribute('class', 'runner-control-a runner-control-valid');
				let time;
				let controlNumberSpan = document.createElement('span');
				if (runner.punches[cnt1] == undefined) {
					controlNumberSpan.innerHTML = "[ " + "---" + " ]";
					time = 0;
				} else {
					controlNumberSpan.innerHTML = "[ " + runner.punches[cnt1].controlNumber + " ]";
					time = (runner.punches[cnt1].timeStamp - (runner.startTime * 60));
				}
				controlNumberSpan.setAttribute('class', 'control-number-span');

				
				let timeStampSpan = document.createElement('span');
				if (time == 0) {
					timeStampSpan.innerHTML = "00:00";
				} else {
					timeStampSpan.innerHTML = Math.floor(time/60)  + ":" + time%60;
				}

				timeStampSpan.setAttribute('class', 'time-span');
			
				if (returnVal.cnt != cnt1 && falseBool == false) {
					controlNumberSpan.setAttribute('class', 'control-number-span');
				} else {
					if (falseBool == false) {
						controlNumberSpan.setAttribute('class', 'invalid-control-span');
						runDiv.setAttribute('class', 'runner-control-a runner-control-valid redIsTheColor');
					} else {
						controlNumberSpan.setAttribute('class', 'invalid-after-control-span');
					}
					falseBool = true;
				}

				runDiv.appendChild(controlNumberSpan);
				runDiv.appendChild(timeStampSpan);
				divSequenceRunner.appendChild(runDiv);			
			} else if (returnVal.ErrCode == 30) {
				if(returnVal.cnt < cnt1) {											// Před chybou.
					runDiv.setAttribute('class', 'runner-control-a runner-control-valid');

					let controlNumberSpan = document.createElement('span');
					controlNumberSpan.innerHTML = "[ " + courseSequence[cnt1] + " ]";
					controlNumberSpan.setAttribute('class', 'control-number-span');
					
					let time = (runner.punches[cnt1].timeStamp - (runner.startTime * 60));
					
					let timeStampSpan = document.createElement('span');
					timeStampSpan.innerHTML = Math.floor(time/60)  + ":" + time%60;
					timeStampSpan.setAttribute('class', 'time-span');
					
					runDiv.appendChild(controlNumberSpan);
					runDiv.appendChild(timeStampSpan);
					divSequenceRunner.appendChild(runDiv);
				} else {
					runDiv.setAttribute('class', 'runner-control-a runner-control-valid runner-control-none');
					let controlNumberSpan = document.createElement('span');
					controlNumberSpan.innerHTML = "[ " +"---"+ " ]";
					controlNumberSpan.setAttribute('class', 'control-number-span');

					let timeStampSpan = document.createElement('span');
					timeStampSpan.innerHTML = "--:--";
					timeStampSpan.setAttribute('class', 'time-span');

					runDiv.appendChild(controlNumberSpan);
					runDiv.appendChild(timeStampSpan);
					divSequenceRunner.appendChild(runDiv);
				}
			}
		}

		
    }
	
	// Function to save runner changes
	function saveFunction() {
		if (runner == undefined) {
			return;
		} 
		
		let potentional = document.getElementById("cardIDInput").value;
		if (potentional == "") {
			alert("Pozor závodník musí mít čip!");
			runnersRender(window.competition);
			document.getElementById(idOfRunner).click();
		}
		if (runner.cardId != potentional) {												// Pokud se změnilo číslo čipu prověď kontrolu na duplicitu.
			for (let cntx = 0; cntx < window.competition.runners.length; cntx++) {
				if (window.competition.runners[cntx].cardId == potentional) {
					alert("Pozor, tento čip má už přiřazeného závodníka!");
					runnersRender(window.competition);
					document.getElementById(idOfRunner).click();
				}
			}
		}
	
		runner.name = document.getElementById("nameInput").value; 
		runner.surname = document.getElementById("surnameInput").value;
		runner.cardId = potentional;
		runner.category = document.getElementById("categorySelect").value;
		runner.startTime = parseInt(document.getElementById("startTime").value);
		runner.club = document.getElementById("clubInp").value;
		runner.status = document.getElementById("statusSelect").value;
	
		runnersRender(window.competition);
		document.getElementById(idOfRunner).click();
	}
	
	// Function to delete runners
	function deleteFunction() {
		let ans = confirm("Opravdu chcete smazat závodníka "+ runner.name + " "+ runner.surname + "?");
		if (ans) {
			for (let cnty = 0; cnty < window.competition.runners.length; cnty++) {
				if (runner === window.competition.runners[cnty]) {
					window.competition.runners.splice(cnty, 1);
				}
			}
			runnersRender(window.competition);
		}

	}
	
	// Function to append new runner.
}

function newFunction() {
	let newRunner = new Runner("---", "---", "", "---", "0", "REG", "---");
	window.competition.runners.push(newRunner);
	runnersRender(window.competition);
	document.getElementById("runner-" + (window.competition.runners.length - 1)).click()
}



module.exports = { runnersRender, newFunction };
