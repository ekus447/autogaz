function platformHandler() {
  if (navigator.platform != "iPhone") {
    window.location.replace("./autogaz_cdn/iphone/autogaz-error.html");
  }
}

function clickHandler() {

    if(document.getElementById("imagehandler").src == "https://cdn.glitch.com/87229131-2a45-44d2-a487-bfa155b45f62%2Fpolat.png?v=1622458463052") {
      document.getElementById("imagehandler").src = "https://cdn.glitch.com/87229131-2a45-44d2-a487-bfa155b45f62%2Fbefore.png?v=1622458462946"
      return;
    }

    document.getElementById("imagehandler").src = "https://cdn.glitch.com/87229131-2a45-44d2-a487-bfa155b45f62%2Fafter.png?v=1622458463507";

    var audio = document.getElementById("music");
    audio.volume = 0.2;
    audio.play();

    audio.onended = function () {
        document.getElementById("imagehandler").src = "https://cdn.glitch.com/87229131-2a45-44d2-a487-bfa155b45f62%2Fpolat.png?v=1622458463052";
    }
}
