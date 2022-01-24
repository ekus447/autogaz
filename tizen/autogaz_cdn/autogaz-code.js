function clickHandler() {

    if(document.getElementById("imagehandler").state == "polat") {
      document.getElementById("imagehandler").src = "autogaz_cdn/imgs/before.png";
      document.getElementById("imagehandler").state = "before";
      return;
    }

    document.getElementById("imagehandler").src = "autogaz_cdn/imgs/after.png";
    document.getElementById("imagehandler").state = "after";

    var audio = document.getElementById("music");
    audio.volume = 0.2;
    audio.play();

    audio.onended = function () {
        document.getElementById("imagehandler").src = "autogaz_cdn/imgs/polat.png";
        document.getElementById("imagehandler").state = "polat";
    }
}
