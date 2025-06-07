#ifndef WEBPAGE_H
#define WEBPAGE_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      background:rgb(144, 183, 200);
      color: #fff;
    }
    .container {
      background: #1c4966;
      padding: 30px;
      border-radius: 12px;
      box-shadow: 0 8px 16px rgba(0,0,0,0.3);
    }
    h1 { margin-bottom: 20px; font-weight: 300; }
    .button {
      padding: 12px 28px;
      font-size: 18px;
      border: none;
      border-radius: 8px;
      background:rgb(255, 85, 0);
      color: white;
      cursor: pointer;
      transition: 0.3s;
    }
    .button:hover { background:rgb(230, 138, 0); transform: scale(1.1); }
  </style>
</head>
<body>
  <div class="container">
    <h1>ESP Web Server</h1>
    <button class="button" onmousedown="sendRequest('on');" onmouseup="sendRequest('off');">STOP LED</button>
  </div>
  <script>
    function sendRequest(x) { fetch("/" + x); }
  </script>
</body>
</html>
)rawliteral";

#endif
