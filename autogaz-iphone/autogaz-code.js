function platformHandler() {
    if (navigator.platform != "iPhone") {
      window.location.replace("https://ekush.glitch.me/autogaz-error.html");
    }
  }
  
  function buttonHandler() {
      document.getElementById("imagehandler").src = "https://cdn.glitch.com/87229131-2a45-44d2-a487-bfa155b45f62%2Fafter.png?v=1622458463507";
      var button = document.getElementById("buton");
      button.textContent = "GAZA GELİYORSUN!";
  
      var audio = document.getElementById("music");
      audio.volume = 0.2;
      audio.play();
      
      audio.onended = function () {
          document.getElementById("imagehandler").src = "https://cdn.glitch.com/87229131-2a45-44d2-a487-bfa155b45f62%2Fpolat.png?v=1622458463052";
          button.textContent = "GAZA GELDİN! BİR DAHA İÇİN TIKLA!!";
      }
  }