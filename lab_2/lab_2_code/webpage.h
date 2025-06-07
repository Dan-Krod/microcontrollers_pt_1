#ifndef WEBPAGE_H
#define WEBPAGE_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; text-align: center; background:rgb(144, 183, 200); color: #fff; }
    .container { background:rgb(15, 51, 75); padding: 30px; border-radius: 12px; box-shadow: 0 8px 16px rgba(0,0,0,0.3); }
    h1 { margin-bottom: 20px; }
    .button { padding: 12px 28px; font-size: 18px; background:rgb(255, 85, 0); color: white; cursor: pointer; }
    .button:hover { background:rgb(230, 138, 0); transform: scale(1.1); }
    .led-container { display: flex; justify-content: center; gap: 15px; margin-bottom: 20px; }
    .led { width: 40px; height: 40px; background: #444; border-radius: 20%; }
    .on1 { background: white; }
    .on2 { background: orange; }
    .on3 { background: blue; }
  </style>
</head>
<body>
  <div class="container">
    <h1>ESP Web Server</h1>
    <div class="led-container">
        <div id="led1" class="led"></div>
        <div id="led2" class="led"></div>
        <div id="led3" class="led"></div>
    </div>
    <button class="button" onmousedown="sendRequest('on');" onmouseup="sendRequest('off');">STOP LED</button>
    <br><br>
    <button class="button" onclick="increaseSpeed();">Increase Speed</button>
  </div>
  <script>
    var connection = new WebSocket('ws://' + window.location.hostname + '/ws');
    connection.onmessage = function (event) {
        var data = JSON.parse(event.data);
        document.getElementById("led1").className = "led " + (data.led1 === "ON" ? "on1" : "");
        document.getElementById("led2").className = "led " + (data.led2 === "ON" ? "on2" : "");
        document.getElementById("led3").className = "led " + (data.led3 === "ON" ? "on3" : "");
    };
    function sendRequest(x) { fetch("/" + x); }
    function increaseSpeed(){
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/increaseSpeed", true);
        xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

#endif
