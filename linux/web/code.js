
function clickHandler() {
    
    //states = 0: before 1: after 2: polat
    var imagehandler = document.getElementById("imagehandler");

    if(imagehandler.state == "2") {
        imagehandler.src = "assets/before.png"
        imagehandler.state = "0";
        return;
    }

    imagehandler.src = "assets/after.png";
    imagehandler.state = "1";

    var audio = document.getElementById("music");
    audio.volume = 0.593453265345767;
    audio.play();
    
    audio.onended = function () {
        imagehandler.src = "assets/polat.png";
        imagehandler.state = "2"
    }
}