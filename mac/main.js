const { app, BrowserWindow } = require('electron');
const path = require("path");

function createWindow () {
    const win = new BrowserWindow({
      width: 640,
      height: 480,
      icon: path.join(__dirname, '/web/assets/icon.png')
    })

    win.setMenu(null);
    win.loadFile('web/gaz.html')
  }

app.whenReady().then(() => {
  createWindow()
})

app.on('window-all-closed', function () {
    app.quit()
})
  