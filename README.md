🌿 Jungle Nexus - ESP32 IoT Monitor
🎯 What is This?
A cool ESP32 project that shows sensor data on a neon jungle-themed website. It creates its own WiFi network with a beautiful dashboard.

✨ Features
🌡️ Temperature & Humidity monitoring

💨 Air quality (smoke/alcohol) detection

🌧️ Rain detection with automatic servo

🏃 Motion sensor for security

💡 4 colorful LEDs to control

🎨 Amazing neon jungle website

📱 Works on phone/tablet/computer

📦 Parts Needed
Part	Why
ESP32 Board	Main brain
DHT22 Sensor	Temperature & Humidity
MQ2 Sensor	Smoke detector
MQ3 Sensor	Alcohol detector
Rain Sensor	Rain water detector
PIR Sensor	Motion detector
Servo Motor	Moves like a gate
Buzzer	Makes sound alerts
4 LEDs	Different colors
🔌 How to Connect
ESP32 Pin → Connects To

34 → MQ2 (smoke sensor)

35 → MQ3 (alcohol sensor)

32 → DHT22 (temp/humidity)

25 → Buzzer (sound)

33 → Rain sensor

26 → Motion sensor

27 → Servo motor

12 → LED 1 (Red)

13 → LED 2 (Green)

14 → LED 3 (Blue)

15 → LED 4 (Yellow)

Connect all + to 3.3V and all - to GND

🚀 Quick Setup
Download code to Arduino IDE

Install libraries:

WiFi

WebServer

DHT

ESP32Servo

Upload to ESP32

Connect to WiFi:

Name: JUNGLE-NEXUS

Password: neon2024

Open browser → Go to: 192.168.4.1

🌐 What You'll See
Dashboard Sections:
🔥 Volcanic Core - Temperature gauge

💧 Misty Swamp - Humidity meter

💨 Cavern Air - Gas levels

🐾 Predator Watch - Motion detector

🌧️ River Flow - Rain & servo control

🌟 Canopy Lights - LED controls

You Can:
Turn security mode ON/OFF

Control 4 LED "spirits"

Open/close waterfall gate

See real-time sensor numbers

⚡ Smart Features
Rain detected → Servo opens automatically

Motion + security ON → Buzzer sounds

Bad air quality → Website shows warning

LEDs remember their ON/OFF state

🛠️ If Problems
Problem	Fix
No WiFi	Restart ESP32
No sensor data	Check wiring
Website not loading	Clear browser cache
Servo not moving	Check 5V power
🔧 Customize
Change colors in the HTML code:

css
--jungle-green: #00ff88;   /* Green color */
--neon-pink: #ff00ff;      /* Pink color */
--electric-blue: #00ffff;  /* Blue color */
Change WiFi name:

cpp
const char* ssid = "YOUR-NEW-NAME";
📖 Learn More
Great for learning:

ESP32 programming

Web server creation

Sensor interfacing

CSS/HTML design

IoT basics

🤝 Help Improve
Found a bug? Want new features?

Fork the project

Make changes

Send pull request

📜 License
Free to use and modify (MIT License)


