/**
 * Funkce pro zobrazení stránky "O nás"
 * @author Vojtěch Fiala [xfiala61]
 */
function aboutUsPageRender() {
    console.log("about us page clicked");
    document.getElementById("center-Section").innerHTML = "";

    let titleText = document.createElement('div');
    titleText.setAttribute('id', 'title-div');
    titleText.innerHTML = "O nás";


    let separatorDiv = document.createElement('div');
    separatorDiv.setAttribute('id', 'title-main-separator');

    let bodyDiv = document.createElement('div');
    bodyDiv.setAttribute('id', 'about-intro-div-container');
    bodyDiv.setAttribute('class', 'about-intro-div-container');

    let authorsBodyDiv = document.createElement('div');
    authorsBodyDiv.setAttribute('id', 'about-authors-div-container');
    authorsBodyDiv.setAttribute('class', 'about-intro-div-container');

    let introDiv = document.createElement('div');
    introDiv.setAttribute('id', 'about-intro-div');
    introDiv.setAttribute('class', 'about-intro-div');
    introDiv.innerHTML = "Aplikace O-RaceIT vznikla jako projekt v rámci předmětu ITU na VUT FIT ve školním roce 2021/2022.";

    let authorsDiv = document.createElement('div');
    authorsDiv.setAttribute('id', 'about-us-authors');
    authorsDiv.setAttribute('class', 'about-intro-div');
    authorsDiv.innerHTML = "Autory aplikace jsou:<br> Vojtěch Bůbela<br>Vojtěch Fiala<br>Vojtěch Malý";

    let sourceDivContainer = document.createElement('div');
    sourceDivContainer.setAttribute('id', 'about-us-sources');
    sourceDivContainer.setAttribute('class', 'about-intro-div-container');

    let sourceDiv = document.createElement('div');
    sourceDiv.setAttribute('id', 'about-us-sources');
    sourceDiv.setAttribute('class', 'about-intro-div');
    sourceDiv.innerHTML = "Zdrojové kódy aplikace jsou veřejně dostupné na githubu:<br>";

    let link = document.createElement('a');
    link.setAttribute('id', 'source-link');
    link.setAttribute('class', 'link-info');
    link.setAttribute('href', 'https://github.com/Feanor60/ITU2021-22');
    link.setAttribute('target', "_blank");
    link.innerHTML = "https://github.com/Feanor60/ITU2021-22";


    sourceDiv.append(link);
    bodyDiv.appendChild(introDiv);
    authorsBodyDiv.appendChild(authorsDiv);
    sourceDivContainer.appendChild(sourceDiv);

    document.getElementById("center-Section").appendChild(titleText);
    document.getElementById("center-Section").appendChild(separatorDiv);
    document.getElementById("center-Section").appendChild(bodyDiv);
    document.getElementById("center-Section").appendChild(authorsBodyDiv);
    document.getElementById("center-Section").appendChild(sourceDivContainer);
}

module.exports = { aboutUsPageRender };