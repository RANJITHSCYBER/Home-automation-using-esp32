#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Preferences.h>

// WiFi credentials for Access Point mode
const char* ssid = "HOME-AUTOMATION";
const char* password = "secure2024";

// Pin definitions
#define MQ2_PIN 34
#define MQ3_PIN 35
#define DHT_PIN 32
#define BUZZER_PIN 25
#define RAINDROP_PIN 33
#define PIR_PIN 26
#define SERVO1_PIN 27        // Rain cover servo
#define SERVO2_PIN 12        // Door unlock servo
#define IR_ENTRY_PIN 14      // IR sensor entrance
#define IR_EXIT_PIN 15       // IR sensor exit
#define RFID_SS_PIN 5         // RFID chip select
#define RFID_RST_PIN 22       // RFID reset

// Sensor thresholds
#define MQ2_THRESHOLD 1500
#define MQ3_THRESHOLD 1500
#define RAINDROP_THRESHOLD 2000
#define MAX_CARDS 10

// Variables
WebServer server(80);
DHT dht(DHT_PIN, DHT22);
Servo servo1, servo2;           // Rain servo & Door servo
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
Preferences preferences;

// Global states
bool safetyMode = false;
bool gasAlert = false;
bool smokeAlert = false;
bool rainDetected = false;
bool motionDetected = false;
bool doorUnlocked = false;
float temperature = 0;
float humidity = 0;
int mq2Value = 0;
int mq3Value = 0;
int rainValue = 0;
int servo1Position = 90;
int servo2Position = 0;        // Door servo (0=locked, 90=unlocked)
int peopleCount = 0;
bool lastEntryState = false;
bool lastExitState = false;
unsigned long lastSensorUpdate = 0;
unsigned long lastAlertTime = 0;
bool shouldBeepForRain = false;
bool rainBeepDone = false;
String authorizedCards[MAX_CARDS];
int cardCount = 0;

// Function declarations
void readSensors();
void processSensors();
void beep(int duration);
void handleRoot();
void handleSensorData();
void handleSafetyToggle();
void handleDoorControl();
void handleAddCard();
void handleRemoveCard();
void handleServoControl();
void handleBuzzer();
void handleResetCounter();
void handleClearCards();
void rfidInit();
void rfidCheck();
void loadCards();
void saveCards();
String getSensorJSON();

// HTML Page - Tesla Style Black & White with scrolling effects
const char* htmlPage = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>HOME AUTOMATION</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      background: #000;
      color: #fff;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      overflow-x: hidden;
    }
    @keyframes scrollBg {
      0% { transform: translateY(0); }
      100% { transform: translateY(-100%); }
    }
    .bg-scroll {
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 200%;
      background: linear-gradient(180deg, #000 0%, #1a1a1a 25%, #000 50%, #1a1a1a 75%, #000 100%);
      animation: scrollBg 20s linear infinite;
      z-index: -1;
      opacity: 0.5;
    }
    .container {
      max-width: 1400px;
      margin: 0 auto;
      padding: 40px 20px;
      position: relative;
      z-index: 1;
    }
    header {
      text-align: center;
      margin-bottom: 80px;
      padding: 60px 20px;
      position: relative;
      overflow: hidden;
    }
    header::before {
      content: '';
      position: absolute;
      top: 0;
      left: -100%;
      width: 200%;
      height: 1px;
      background: linear-gradient(90deg, transparent, #fff, transparent);
      animation: scanLine 3s infinite;
    }
    @keyframes scanLine {
      0% { left: -100%; }
      100% { left: 100%; }
    }
    h1 {
      font-size: 3.5rem;
      font-weight: 300;
      letter-spacing: 4px;
      margin-bottom: 15px;
      text-transform: uppercase;
      transition: all 0.3s ease;
    }
    h1:hover { letter-spacing: 6px; opacity: 0.8; }
    .subtitle {
      font-size: 0.95rem;
      letter-spacing: 2px;
      color: #999;
      text-transform: uppercase;
      margin-bottom: 40px;
    }
    .status-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      gap: 20px;
      margin-top: 40px;
      padding-top: 40px;
      border-top: 1px solid #333;
    }
    .status-card {
      padding: 20px;
      border: 1px solid #333;
      background: rgba(255, 255, 255, 0.02);
      text-align: center;
      transition: all 0.4s ease;
      cursor: pointer;
    }
    .status-card:hover {
      border-color: #fff;
      background: rgba(255, 255, 255, 0.08);
      transform: translateY(-5px);
    }
    .status-label {
      font-size: 0.8rem;
      letter-spacing: 1px;
      color: #999;
      text-transform: uppercase;
      margin-bottom: 10px;
    }
    .status-value {
      font-size: 1.8rem;
      font-weight: 300;
      color: #fff;
    }
    .dashboard {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(350px, 1fr));
      gap: 30px;
      margin: 80px 0;
    }
    .sensor-panel {
      border: 1px solid #333;
      padding: 40px;
      background: rgba(255, 255, 255, 0.01);
      transition: all 0.5s cubic-bezier(0.34, 1.56, 0.64, 1);
      position: relative;
      overflow: hidden;
    }
    .sensor-panel::before {
      content: '';
      position: absolute;
      top: 0;
      left: -100%;
      width: 100%;
      height: 100%;
      background: rgba(255, 255, 255, 0.05);
      transition: left 0.5s ease;
    }
    .sensor-panel:hover {
      border-color: #fff;
      background: rgba(255, 255, 255, 0.05);
      transform: translateY(-10px);
    }
    .sensor-panel:hover::before { left: 100%; }
    .sensor-title {
      font-size: 1.2rem;
      letter-spacing: 2px;
      text-transform: uppercase;
      margin-bottom: 25px;
      border-bottom: 1px solid #333;
      padding-bottom: 15px;
      color: #ccc;
    }
    .sensor-content {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin: 20px 0;
    }
    .sensor-value {
      font-size: 2.5rem;
      font-weight: 300;
      letter-spacing: 1px;
    }
    .sensor-unit {
      font-size: 0.9rem;
      color: #999;
      letter-spacing: 1px;
      text-transform: uppercase;
    }
    .progress-bar {
      width: 100%;
      height: 2px;
      background: #222;
      margin: 20px 0;
      overflow: hidden;
    }
    .progress-fill {
      height: 100%;
      background: #fff;
      transition: width 0.8s ease;
    }
    .alert-status {
      margin-top: 20px;
      padding-top: 20px;
      border-top: 1px solid #333;
      font-size: 0.9rem;
      letter-spacing: 1px;
    }
    .status-normal { color: #fff; }
    .status-warning { color: #ffaa00; }
    .status-danger { color: #ff4444; }
    .controls-section {
      margin: 80px 0;
      padding: 40px;
      border: 1px solid #333;
      background: rgba(255, 255, 255, 0.01);
    }
    .controls-title {
      font-size: 1.5rem;
      letter-spacing: 2px;
      text-transform: uppercase;
      margin-bottom: 40px;
      border-bottom: 1px solid #333;
      padding-bottom: 20px;
    }
    .controls-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      gap: 20px;
    }
    .btn {
      padding: 15px 30px;
      background: transparent;
      border: 1px solid #333;
      color: #fff;
      cursor: pointer;
      font-size: 0.9rem;
      letter-spacing: 1px;
      text-transform: uppercase;
      transition: all 0.4s ease;
      font-weight: 300;
    }
    .btn:hover {
      border-color: #fff;
      background: rgba(255, 255, 255, 0.08);
      transform: scale(1.02);
    }
    .btn:active { transform: scale(0.98); }
    .btn.danger:hover {
      border-color: #ff4444;
      color: #ff4444;
      box-shadow: 0 0 20px rgba(255, 68, 68, 0.3);
    }
    .btn.success:hover {
      border-color: #00ff88;
      color: #00ff88;
      box-shadow: 0 0 20px rgba(0, 255, 136, 0.3);
    }
    .toggle {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 30px;
      margin: 20px 0;
    }
    .toggle input {
      opacity: 0;
      width: 0;
      height: 0;
    }
    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #333;
      transition: .3s;
      border: 1px solid #555;
    }
    .slider:before {
      position: absolute;
      content: "";
      height: 24px;
      width: 24px;
      left: 3px;
      bottom: 2px;
      background-color: #fff;
      transition: .3s;
    }
    input:checked + .slider {
      background-color: #000;
      border-color: #fff;
    }
    input:checked + .slider:before {
      transform: translateX(30px);
    }
    .people-counter {
      font-size: 3rem;
      font-weight: 300;
      text-align: center;
      margin: 30px 0;
      letter-spacing: 2px;
    }
    .counter-label {
      font-size: 0.9rem;
      color: #999;
      letter-spacing: 1px;
      text-align: center;
    }
    .rfid-section {
      background: rgba(255, 255, 255, 0.02);
      padding: 30px;
      border: 1px solid #333;
      margin: 40px 0;
    }
    .rfid-title {
      font-size: 1.2rem;
      letter-spacing: 2px;
      text-transform: uppercase;
      margin-bottom: 20px;
      border-bottom: 1px solid #333;
      padding-bottom: 15px;
    }
    .card-list {
      list-style: none;
      margin: 20px 0;
      max-height: 300px;
      overflow-y: auto;
    }
    .card-item {
      padding: 10px;
      border-bottom: 1px solid #222;
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-size: 0.9rem;
      letter-spacing: 1px;
    }
    .card-item:hover {
      background: rgba(255, 255, 255, 0.05);
    }
    .remove-btn {
      background: #ff4444;
      border: none;
      color: #000;
      padding: 5px 10px;
      cursor: pointer;
      font-size: 0.8rem;
      transition: all 0.3s ease;
    }
    .remove-btn:hover { background: #ff6666; }
    .door-section {
      text-align: center;
      padding: 40px;
      border: 1px solid #333;
      margin: 40px 0;
      background: rgba(255, 255, 255, 0.01);
    }
    .door-status {
      font-size: 2rem;
      margin: 20px 0;
      letter-spacing: 2px;
      font-weight: 300;
    }
    .door-locked { color: #ff4444; }
    .door-unlocked { color: #00ff88; }
    footer {
      text-align: center;
      margin-top: 100px;
      padding: 40px 0;
      border-top: 1px solid #333;
      color: #666;
      font-size: 0.85rem;
      letter-spacing: 1px;
    }
    @media (max-width: 768px) {
      h1 { font-size: 2.5rem; }
      .dashboard { grid-template-columns: 1fr; }
      .sensor-panel { padding: 30px 20px; }
      .controls-grid { grid-template-columns: 1fr; }
    }
  </style>
</head>
<body>
  <div class="bg-scroll"></div>
  <div class="container">
    <header>
      <h1>HOME AUTOMATION</h1>
      <p class="subtitle">Smart Home Monitoring System</p>
      <div class="status-grid">
        <div class="status-card">
          <div class="status-label">System Status</div>
          <div class="status-value" id="sysStatus">ACTIVE</div>
        </div>
        <div class="status-card">
          <div class="status-label">Last Update</div>
          <div class="status-value" id="lastUpdate">--:--:--</div>
        </div>
        <div class="status-card">
          <div class="status-label">IP Address</div>
          <div class="status-value">192.168.4.1</div>
        </div>
      </div>
    </header>
    <div class="dashboard">
      <div class="sensor-panel">
        <div class="sensor-title">Temperature</div>
        <div class="sensor-content">
          <div><div class="sensor-value" id="tempVal">0.0</div><div class="sensor-unit">°C</div></div>
          <div style="text-align: right;"><div class="progress-bar"><div class="progress-fill" id="tempBar"></div></div><div id="tempStatus" class="alert-status status-normal">NORMAL</div></div>
        </div>
      </div>
      <div class="sensor-panel">
        <div class="sensor-title">Humidity</div>
        <div class="sensor-content">
          <div><div class="sensor-value" id="humidVal">0</div><div class="sensor-unit">%</div></div>
          <div style="text-align: right;"><div class="progress-bar"><div class="progress-fill" id="humidBar"></div></div><div id="humidStatus" class="alert-status status-normal">NORMAL</div></div>
        </div>
      </div>
      <div class="sensor-panel">
        <div class="sensor-title">Smoke (MQ2)</div>
        <div class="sensor-content">
          <div><div class="sensor-value" id="mq2Val">0</div><div class="sensor-unit">PPM</div></div>
          <div style="text-align: right;"><div id="mq2Status" class="alert-status status-normal">SAFE</div></div>
        </div>
      </div>
      <div class="sensor-panel">
        <div class="sensor-title">Gas (MQ3)</div>
        <div class="sensor-content">
          <div><div class="sensor-value" id="mq3Val">0</div><div class="sensor-unit">PPM</div></div>
          <div style="text-align: right;"><div id="mq3Status" class="alert-status status-normal">SAFE</div></div>
        </div>
      </div>
      <div class="sensor-panel">
        <div class="sensor-title">Motion (PIR)</div>
        <div class="sensor-content">
          <div><div class="sensor-value" id="motionVal">NO</div><div class="sensor-unit">Motion</div></div>
          <div style="text-align: right;"><div id="motionStatus" class="alert-status status-normal">CALM</div></div>
        </div>
      </div>
      <div class="sensor-panel">
        <div class="sensor-title">Rain Detector</div>
        <div class="sensor-content">
          <div><div class="sensor-value" id="rainVal">DRY</div><div class="sensor-unit">Status</div></div>
          <div style="text-align: right;"><div id="rainStatus" class="alert-status status-normal">NO RAIN</div></div>
        </div>
      </div>
    </div>
    <div class="sensor-panel" style="grid-column: 1/-1; text-align: center;">
      <div class="sensor-title">People Count</div>
      <div class="people-counter" id="peopleCount">0</div>
      <div class="counter-label">Total People Inside</div>
    </div>
    <div class="controls-section">
      <div class="controls-title">System Controls</div>
      <div class="controls-grid">
        <button class="btn danger" onclick="toggleSafety()"><span id="safetyBtnText">Activate Safety Mode</span></button>
        <button class="btn success" onclick="ringBuzzer()">Alert Sound</button>
        <button class="btn" onclick="resetCounter()">Reset Counter</button>
      </div>
      <div style="margin-top: 30px; border-top: 1px solid #333; padding-top: 30px;">
        <h3 style="letter-spacing: 2px; text-transform: uppercase; margin-bottom: 20px;">Safety Mode</h3>
        <div style="display: flex; align-items: center; gap: 20px;">
          <span>Enabled:</span>
          <label class="toggle">
            <input type="checkbox" id="safetyToggle" onchange="toggleSafety()">
            <span class="slider"></span>
          </label>
          <span id="safetyText" style="color: #666;">INACTIVE</span>
        </div>
      </div>
    </div>
    <div class="door-section">
      <div class="sensor-title">Smart Door Lock</div>
      <div class="door-status" id="doorStatus">LOCKED</div>
      <button class="btn success" onclick="controlDoor()" style="margin-top: 20px;">Unlock Door (5s)</button>
    </div>
    <div class="rfid-section">
      <div class="rfid-title">RFID Card Management</div>
      <p style="font-size: 0.9rem; color: #999; margin-bottom: 20px;">Tap RFID card to add. Max 10 cards.</p>
      <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 20px; margin-bottom: 30px;">
        <button class="btn success" onclick="addCardMode()">Add New Card</button>
        <button class="btn danger" onclick="clearAllCards()">Clear All Cards</button>
      </div>
      <div style="margin-top: 20px;">
        <h4 style="letter-spacing: 1px; margin-bottom: 15px;">Authorized Cards:</h4>
        <ul class="card-list" id="cardList">
          <li class="card-item" style="color: #666;">No cards registered</li>
        </ul>
      </div>
    </div>
    <div class="controls-section">
      <div class="controls-title">Servo Controls</div>
      <div class="controls-grid">
        <div>
          <h4 style="margin-bottom: 15px; letter-spacing: 1px;">Rain Cover Servo</h4>
          <div class="progress-bar"><div class="progress-fill" id="servo1Bar" style="width: 50%;"></div></div>
          <div style="display: flex; gap: 10px; margin-top: 10px;">
            <button class="btn" onclick="controlServo1(0)" style="flex: 1;">Close</button>
            <button class="btn" onclick="controlServo1(180)" style="flex: 1;">Open</button>
          </div>
        </div>
      </div>
    </div>
    <footer>
      <p>HOME AUTOMATION SYSTEM | 2024</p>
      <p style="margin-top: 10px;">Connected via 192.168.4.1 | AJAX Update Rate: 2s</p>
    </footer>
  </div>
  <script>
    let sensorData = {};
    let isUpdating = false;
    function initDashboard() {
      updateDashboard();
      setInterval(updateDashboard, 2000);
    }
    function updateDashboard() {
      if (isUpdating) return;
      isUpdating = true;
      fetch('/sensorData').then(r => r.json()).then(data => {
        sensorData = data;
        updateDisplay(data);
        updateTime();
        isUpdating = false;
      }).catch(e => {
        console.error('Error:', e);
        isUpdating = false;
      });
    }
    function updateDisplay(data) {
      document.getElementById('tempVal').textContent = data.temperature.toFixed(1);
      document.getElementById('tempBar').style.width = Math.min(data.temperature / 50 * 100, 100) + '%';
      let tempStatus = 'NORMAL';
      if (data.temperature > 35) tempStatus = 'HOT';
      if (data.temperature < 5) tempStatus = 'COLD';
      document.getElementById('tempStatus').textContent = tempStatus;
      document.getElementById('tempStatus').className = 'alert-status ' + (data.temperature > 35 ? 'status-danger' : 'status-normal');
      document.getElementById('humidVal').textContent = Math.round(data.humidity);
      document.getElementById('humidBar').style.width = data.humidity + '%';
      let humidStatus = 'NORMAL';
      if (data.humidity > 80) humidStatus = 'HIGH';
      if (data.humidity < 30) humidStatus = 'LOW';
      document.getElementById('humidStatus').textContent = humidStatus;
      document.getElementById('humidStatus').className = 'alert-status ' + (data.humidity > 80 || data.humidity < 30 ? 'status-warning' : 'status-normal');
      document.getElementById('mq2Val').textContent = data.mq2;
      document.getElementById('mq2Status').className = 'alert-status ' + (data.mq2 > 1500 ? 'status-danger' : 'status-normal');
      document.getElementById('mq2Status').textContent = data.mq2 > 1500 ? 'ALERT!' : 'SAFE';
      document.getElementById('mq3Val').textContent = data.mq3;
      document.getElementById('mq3Status').className = 'alert-status ' + (data.mq3 > 1500 ? 'status-danger' : 'status-normal');
      document.getElementById('mq3Status').textContent = data.mq3 > 1500 ? 'ALERT!' : 'SAFE';
      document.getElementById('motionVal').textContent = data.motionDetected ? 'YES' : 'NO';
      document.getElementById('motionStatus').className = 'alert-status ' + (data.motionDetected ? 'status-warning' : 'status-normal');
      document.getElementById('motionStatus').textContent = data.motionDetected ? 'DETECTED' : 'CALM';
      document.getElementById('rainVal').textContent = data.rainDetected ? 'RAINING' : 'DRY';
      document.getElementById('rainStatus').textContent = data.rainDetected ? 'RAINING' : 'NO RAIN';
      document.getElementById('peopleCount').textContent = data.peopleCount;
      document.getElementById('servo1Bar').style.width = (data.servo1Position / 180 * 100) + '%';
      document.getElementById('doorStatus').textContent = data.doorUnlocked ? 'UNLOCKED' : 'LOCKED';
      document.getElementById('doorStatus').className = 'door-status ' + (data.doorUnlocked ? 'door-unlocked' : 'door-locked');
      document.getElementById('safetyToggle').checked = data.safetyMode;
      document.getElementById('safetyText').textContent = data.safetyMode ? 'ACTIVE' : 'INACTIVE';
      document.getElementById('safetyBtnText').textContent = data.safetyMode ? 'Deactivate Safety Mode' : 'Activate Safety Mode';
      if (data.cards) {
        const cardList = document.getElementById('cardList');
        cardList.innerHTML = '';
        if (data.cards.length === 0) {
          cardList.innerHTML = '<li class="card-item" style="color: #666;">No cards registered</li>';
        } else {
          data.cards.forEach((card) => {
            const item = document.createElement('li');
            item.className = 'card-item';
            item.innerHTML = `<span>${card}</span><button class="remove-btn" onclick="removeCard('${card}')">Remove</button>`;
            cardList.appendChild(item);
          });
        }
      }
    }
    function updateTime() {
      const now = new Date();
      document.getElementById('lastUpdate').textContent = now.toLocaleTimeString('en-US', {hour12: false});
    }
    function toggleSafety() {
      fetch('/toggleSafety').then(r => r.json()).then(() => updateDashboard());
    }
    function ringBuzzer() {
      fetch('/buzzer').then(r => r.json()).then(() => updateDashboard());
    }
    function resetCounter() {
      fetch('/resetCounter').then(r => r.json()).then(() => updateDashboard());
    }
    function controlDoor() {
      fetch('/doorControl').then(r => r.json()).then(() => updateDashboard());
    }
    function controlServo1(angle) {
      fetch('/servoControl?angle=' + angle).then(r => r.json()).then(() => updateDashboard());
    }
    function addCardMode() {
      alert('Bring RFID card close to reader...');
    }
    function removeCard(cardId) {
      fetch('/removeCard?id=' + cardId).then(r => r.json()).then(() => updateDashboard());
    }
    function clearAllCards() {
      if (confirm('Clear all authorized cards?')) {
        fetch('/clearCards').then(r => r.json()).then(() => updateDashboard());
      }
    }
    document.addEventListener('DOMContentLoaded', initDashboard);
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize pins
  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ3_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RAINDROP_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(IR_ENTRY_PIN, INPUT);
  pinMode(IR_EXIT_PIN, INPUT);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(90);
  servo2.write(0);
  servo1Position = 90;
  servo2Position = 0;
  
  // Initialize RFID
  rfidInit();
  
  // Load RFID cards from preferences
  loadCards();
  
  // Initialize outputs
  digitalWrite(BUZZER_PIN, LOW);
  
  // Start WiFi Access Point
  WiFi.softAP(ssid, password);
  Serial.println("\n╔═════════════════════════════════════════════════╗");
  Serial.println("║      HOME AUTOMATION SYSTEM BOOTING             ║");
  Serial.println("║      TESLA-STYLE BLACK & WHITE UI               ║");
  Serial.println("╚═════════════════════════════════════════════════╝");
  Serial.print("Network: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("═══════════════════════════════════════════════════");
  
  // Setup server routes
  server.on("/", handleRoot);
  server.on("/sensorData", handleSensorData);
  server.on("/toggleSafety", handleSafetyToggle);
  server.on("/doorControl", handleDoorControl);
  server.on("/servoControl", handleServoControl);
  server.on("/buzzer", handleBuzzer);
  server.on("/resetCounter", handleResetCounter);
  server.on("/removeCard", handleRemoveCard);
  server.on("/clearCards", handleClearCards);
  
  server.begin();
  Serial.println("HTTP server started on port 80");
  Serial.println("Access at http://192.168.4.1");
  Serial.println("═══════════════════════════════════════════════════\n");
  
  // Boot sequence
  beep(100);
  delay(100);
  beep(100);
  delay(100);
  beep(300);
}

void loop() {
  server.handleClient();
  readSensors();
  processSensors();
  rfidCheck();
  delay(100);
}

void readSensors() {
  // Read DHT22 every 2 seconds
  if (millis() - lastSensorUpdate > 2000) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    
    // Read MQ sensors
    mq2Value = analogRead(MQ2_PIN);
    mq3Value = analogRead(MQ3_PIN);
    
    // Read rain sensor
    rainValue = analogRead(RAINDROP_PIN);
    
    // Read PIR sensor
    motionDetected = digitalRead(PIR_PIN);
    
    // Read IR sensors
    bool entryState = digitalRead(IR_ENTRY_PIN);
    bool exitState = digitalRead(IR_EXIT_PIN);
    
    // People counting logic
    if (entryState && !lastEntryState) {
      peopleCount++;
      Serial.println("Entry detected. People: " + String(peopleCount));
      lastEntryState = entryState;
    }
    if (exitState && !lastExitState) {
      if (peopleCount > 0) peopleCount--;
      Serial.println("Exit detected. People: " + String(peopleCount));
      lastExitState = exitState;
    }
    lastEntryState = entryState;
    lastExitState = exitState;
    
    // Check for NaN values from DHT
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("DHT22 sensor error!");
      temperature = 0;
      humidity = 0;
    }
    
    lastSensorUpdate = millis();
  }
}

void processSensors() {
  static bool lastRainState = false;
  static unsigned long lastRainCheck = 0;
  
  // Process MQ2 (Smoke)
  smokeAlert = (mq2Value > MQ2_THRESHOLD);
  
  // Process MQ3 (Alcohol/Gas)
  gasAlert = (mq3Value > MQ3_THRESHOLD);
  
  // Check rain every 500ms
  if (millis() - lastRainCheck > 500) {
    bool newRainDetected = (rainValue < RAINDROP_THRESHOLD);
    
    // If rain is just detected
    if (newRainDetected && !lastRainState) {
      Serial.println("Rain detected! Opening cover...");
      shouldBeepForRain = true;
      rainBeepDone = false;
      // Move servo to 180 degrees smoothly
      for (int pos = servo1Position; pos <= 180; pos += 5) {
        servo1.write(pos);
        servo1Position = pos;
        delay(30);
      }
      servo1Position = 180;
    }
    // If rain stops
    else if (!newRainDetected && lastRainState) {
      Serial.println("Rain stopped. Closing cover...");
      // Move servo back to 90 degrees smoothly
      for (int pos = servo1Position; pos >= 90; pos -= 5) {
        servo1.write(pos);
        servo1Position = pos;
        delay(30);
      }
      servo1Position = 90;
    }
    
    lastRainState = newRainDetected;
    rainDetected = newRainDetected;
    lastRainCheck = millis();
  }
  
  // Beep for rain detection (only once when rain starts)
  if (shouldBeepForRain && !rainBeepDone) {
    beep(200);
    delay(200);
    beep(200);
    rainBeepDone = true;
    shouldBeepForRain = false;
  }
  
  // Process PIR sensor with safety mode
  if (safetyMode && motionDetected) {
    // Continuous beep when motion detected in safety mode
    digitalWrite(BUZZER_PIN, HIGH);
    if (millis() - lastAlertTime > 5000) {
      Serial.println("Motion detected - Safety Mode Active!");
      lastAlertTime = millis();
    }
  } else {
    // Check for gas/smoke alerts
    if ((gasAlert || smokeAlert)) {
      // Beep for gas/smoke detection
      digitalWrite(BUZZER_PIN, HIGH);
      if (millis() - lastAlertTime > 5000) {
        Serial.println("Gas/Smoke alert!");
        lastAlertTime = millis();
      }
    } else {
      // Turn off buzzer if no alerts
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void beep(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void rfidInit() {
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("RFID initialized");
}

void rfidCheck() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck < 500) return;
  lastCheck = millis();
  
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }
  
  // Get card UID
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  
  Serial.println("Card detected: " + uid);
  
  // Check if card is authorized
  bool found = false;
  for (int i = 0; i < cardCount; i++) {
    if (authorizedCards[i] == uid) {
      found = true;
      Serial.println("Card authorized! Unlocking door...");
      // Unlock door for 5 seconds
      servo2.write(90);
      doorUnlocked = true;
      beep(100);
      delay(100);
      beep(100);
      delay(5000);
      servo2.write(0);
      doorUnlocked = false;
      Serial.println("Door locked");
      break;
    }
  }
  
  if (!found) {
    Serial.println("Unknown card");
    beep(500);
  }
  
  rfid.PICC_HaltA();
}

void loadCards() {
  preferences.begin("rfid", true);
  cardCount = preferences.getInt("count", 0);
  for (int i = 0; i < cardCount && i < MAX_CARDS; i++) {
    authorizedCards[i] = preferences.getString(("card" + String(i)).c_str(), "");
  }
  preferences.end();
  Serial.println("Loaded " + String(cardCount) + " RFID cards");
}

void saveCards() {
  preferences.begin("rfid", false);
  preferences.putInt("count", cardCount);
  for (int i = 0; i < cardCount; i++) {
    preferences.putString(("card" + String(i)).c_str(), authorizedCards[i]);
  }
  preferences.end();
}

String getSensorJSON() {
  String json = "{";
  json += "\"temperature\":" + String(temperature, 1) + ",";
  json += "\"humidity\":" + String(humidity, 0) + ",";
  json += "\"mq2\":" + String(mq2Value) + ",";
  json += "\"mq3\":" + String(mq3Value) + ",";
  json += "\"motionDetected\":" + String(motionDetected ? "true" : "false") + ",";
  json += "\"rainDetected\":" + String(rainDetected ? "true" : "false") + ",";
  json += "\"servo1Position\":" + String(servo1Position) + ",";
  json += "\"doorUnlocked\":" + String(doorUnlocked ? "true" : "false") + ",";
  json += "\"safetyMode\":" + String(safetyMode ? "true" : "false") + ",";
  json += "\"peopleCount\":" + String(peopleCount) + ",";
  json += "\"gasAlert\":" + String(gasAlert ? "true" : "false") + ",";
  json += "\"smokeAlert\":" + String(smokeAlert ? "true" : "false") + ",";
  json += "\"cards\":[";
  for (int i = 0; i < cardCount; i++) {
    json += "\"" + authorizedCards[i] + "\"";
    if (i < cardCount - 1) json += ",";
  }
  json += "]";
  json += "}";
  return json;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleSensorData() {
  server.send(200, "application/json", getSensorJSON());
}

void handleSafetyToggle() {
  safetyMode = !safetyMode;
  if (safetyMode) {
    Serial.println("Safety Mode ACTIVATED");
  } else {
    Serial.println("Safety Mode DEACTIVATED");
    digitalWrite(BUZZER_PIN, LOW);
  }
  server.send(200, "application/json", "{\"success\":true}");
}

void handleDoorControl() {
  // Unlock for 5 seconds
  servo2.write(90);
  doorUnlocked = true;
  beep(100);
  delay(100);
  beep(100);
  delay(5000);
  servo2.write(0);
  doorUnlocked = false;
  Serial.println("Door unlocked via web");
  server.send(200, "application/json", "{\"success\":true}");
}

void handleServoControl() {
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    angle = constrain(angle, 0, 180);
    
    // Move servo smoothly
    if (angle > servo1Position) {
      for (int pos = servo1Position; pos <= angle; pos += 5) {
        servo1.write(pos);
        delay(30);
      }
    } else {
      for (int pos = servo1Position; pos >= angle; pos -= 5) {
        servo1.write(pos);
        delay(30);
      }
    }
    servo1Position = angle;
    Serial.println("Servo1 moved to " + String(angle) + "°");
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    server.send(400, "application/json", "{\"error\":\"No angle specified\"}");
  }
}

void handleBuzzer() {
  beep(200);
  server.send(200, "application/json", "{\"success\":true}");
}

void handleResetCounter() {
  peopleCount = 0;
  Serial.println("People counter reset");
  server.send(200, "application/json", "{\"success\":true}");
}

void handleRemoveCard() {
  if (server.hasArg("id")) {
    String cardId = server.arg("id");
    for (int i = 0; i < cardCount; i++) {
      if (authorizedCards[i] == cardId) {
        // Shift array
        for (int j = i; j < cardCount - 1; j++) {
          authorizedCards[j] = authorizedCards[j + 1];
        }
        cardCount--;
        saveCards();
        Serial.println("Card removed: " + cardId);
        server.send(200, "application/json", "{\"success\":true}");
        return;
      }
    }
  }
  server.send(400, "application/json", "{\"error\":\"Card not found\"}");
}

void handleClearCards() {
  cardCount = 0;
  saveCards();
  Serial.println("All RFID cards cleared");
  server.send(200, "application/json", "{\"success\":true}");
}
