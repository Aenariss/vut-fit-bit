const { app, BrowserWindow } = require('electron')
// Main function to create window.
const createWindow = () => {
    const win = new BrowserWindow({
        //width: 1280,
        //height: 720,
        autoHideMenuBar: true,
        width: 2000,
        height: 1800,
        minWidth: 1200,
        minHeight:700,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            enableRemoteModule: true
          }
    })

    win.loadFile('index.html')
    
}

// Wait for render of window, then opens new window
app.whenReady().then(() => {
    createWindow()

    //document.getElementById("center-Section").style.backgroundColor = "red";
})

//document.getElementById("center-Section").style.backgroundColor = "red";
// If all windows are closed, quit app.
app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') app.quit()
})
