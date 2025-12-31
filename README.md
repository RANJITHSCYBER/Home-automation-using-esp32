🌿 JUNGLE NEXUS - ESP32 IoT Environmental Monitoring System
<div align="center">
https://img.shields.io/badge/Neon-Jungle%2520Ecosystem-00ff88?style=for-the-badge&logo=arduino&logoColor=white
https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white
https://img.shields.io/badge/WiFi_Access_Point-9d00ff?style=for-the-badge&logo=wifi&logoColor=white
https://img.shields.io/badge/IoT-00ffff?style=for-the-badge&logo=internet-explorer&logoColor=black

A Cyberpunk Jungle-Themed Environmental Monitoring System with Stunning Web Dashboard

https://img.shields.io/badge/Version-2.4.0-brightgreen?style=flat-square
https://img.shields.io/badge/License-MIT-blue?style=flat-square
https://img.shields.io/badge/Arduino-Compatible-00979D?style=flat-square&logo=arduino&logoColor=white

</div>
📸 Dashboard Preview
<div align="center">
https://via.placeholder.com/800x400/001a00/00ff88?text=Neon+Jungle+Dashboard+Preview
Neon Jungle Interface with Real-time Sensor Data

</div>
🎯 About The Project
Jungle Nexus transforms environmental monitoring into an immersive cyberpunk jungle experience. This ESP32-based system creates its own WiFi network with a breathtaking web dashboard that visualizes sensor data through mystical jungle biomes, complete with tribal patterns, neon aesthetics, and interactive spirit controls.

Key Features
🌡️ Multi-sensor monitoring (Temperature, Humidity, Air Quality, Rain, Motion)

🎨 Stunning neon jungle UI with animated elements

📱 Fully responsive dashboard - works on any device

🔄 Real-time updates without page refresh

🎚️ Interactive controls for LEDs, servo, and security

🔊 Audio-visual alerts for critical conditions

🌐 Standalone WiFi Access Point - no internet required

🛠️ Hardware Requirements
Component	Quantity	Purpose
ESP32 Dev Board	1	Main controller
DHT22 Sensor	1	Temperature & Humidity
MQ2 Sensor	1	Smoke/Gas detection
MQ3 Sensor	1	Alcohol detection
Raindrop Sensor	1	Rain/Water detection
PIR Sensor	1	Motion detection
SG90 Servo Motor	1	"Waterfall Gate" control
Active Buzzer	1	Audio alerts
LEDs (4 colors)	4	"Jungle Spirit" indicators
Jumper Wires	-	Connections
Breadboard	1	Prototyping
🔧 Pin Connections
ESP32 Pin	Connected To	Description
GPIO34	MQ2 A0	Smoke sensor analog read
GPIO35	MQ3 A0	Alcohol sensor analog read
GPIO32	DHT22 Data	Temperature/Humidity
GPIO25	Buzzer +	Audio alerts
GPIO33	Raindrop A0	Rain detection
GPIO26	PIR OUT	Motion detection
GPIO27	Servo Signal	Waterfall gate control
GPIO12	LED1	Fire Spirit (Red)
GPIO13	LED2	Forest Spirit (Green)
GPIO14	LED3	Water Spirit (Blue)
GPIO15	LED4	Sun Spirit (Yellow)
3.3V	All Sensors VCC	Power supply
GND	All Sensors GND	Ground
🚀 Quick Start Guide
1. Installation
bash
# Clone the repository
git clone https://github.com/yourusername/jungle-nexus.git

# Open in Arduino IDE
cd jungle-nexus
open jungle_nexus.ino
2. Install Required Libraries
In Arduino IDE, install these libraries via Library Manager:

WiFi (ESP32)

WebServer (ESP32)

DHT sensor library by Adafruit

ESP32Servo by Kevin Harrington

3. Upload Code
Select your ESP32 board in Tools → Board

Choose correct COM port

Click Upload

4. Connect & Access
Power up your ESP32

Look for WiFi network: JUNGLE-NEXUS

Connect with password: neon2024

Open browser and navigate to: http://192.168.4.1

🌈 Dashboard Features
Biome Cards
Each sensor group is represented as a mystical jungle biome:

Biome	Sensor	Theme Color	Icon
Volcanic Core	Temperature	Lava Red	🔥
Misty Swamp	Humidity	Electric Blue	💧
Cavern Air	Air Quality (MQ2/MQ3)	Deep Purple	💨
Predator Watch	Motion (PIR)	Jungle Green	🐾
River Flow	Rain Detection	Cyan	🌧️
Canopy Lights	LED Controls	Toxic Yellow	🌟
Interactive Controls
Tribal Guardian Toggle - Activate security mode

Waterfall Gate - Control servo position (0-180°)

Spirit LEDs - Individual control of 4 jungle spirits

Mass Control - Awaken/Rest all spirits simultaneously

⚙️ System Configuration
Sensor Thresholds
cpp
#define MQ2_THRESHOLD 1500    // Smoke detection threshold
#define MQ3_THRESHOLD 1500    // Alcohol detection threshold
#define RAINDROP_THRESHOLD 2000 // Rain detection threshold
WiFi Settings
cpp
const char* ssid = "JUNGLE-NEXUS";
const char* password = "neon2024";
Update Intervals
Sensor readings: Every 2 seconds

Dashboard update: Every 2 seconds

Rain check: Every 500ms

🔄 Automated Behaviors
Smart Responses
Rain Detection: Servo automatically opens to 180° during rain

Safety Mode: Motion triggers continuous buzzer alerts

Air Quality: Gas/smoke above thresholds triggers warnings

Temperature Alerts: Color-coded status changes

LED Default States
LED1 (Fire Spirit): OFF

LED2 (Forest Spirit): ON (Default jungle theme)

LED3 (Water Spirit): OFF

LED4 (Sun Spirit): OFF

📡 API Endpoints
Endpoint	Method	Description	Response
/	GET	Main dashboard	HTML page
/sensorData	GET	JSON sensor data	{temp, humidity, mq2, mq3, ...}
/toggleSafety	POST	Toggle security mode	{success, safetyMode}
/toggleLed?led=1-4	POST	Toggle specific LED	{success, led, state}
/allLedsOn	POST	Turn all LEDs ON	{success}
/allLedsOff	POST	Turn all LEDs OFF	{success}
/servoControl?angle=0-180	POST	Set servo position	{success, servoPosition}
🎨 UI Customization
The dashboard uses a custom CSS theme with these variables:

css
:root {
  --jungle-green: #00ff88;
  --neon-pink: #ff00ff;
  --toxic-yellow: #ffff00;
  --deep-purple: #9d00ff;
  --electric-blue: #00ffff;
  --lava-red: #ff3300;
  --dark-jungle: #001a00;
}
To customize:

Modify color variables in the HTML

Adjust animations in the CSS section

Update icons from Font Awesome

Modify biome card styles

📱 Mobile Compatibility
The dashboard is fully responsive:

Desktop: Full grid layout with 6 biome cards

Tablet: 2-column layout

Mobile: Single column, optimized touch controls

🐛 Troubleshooting
Issue	Solution
Can't connect to WiFi	Check ESP32 power, reset board
Sensors not reading	Verify wiring, check pin definitions
Dashboard not loading	Clear browser cache, check IP
Servo not moving	Check power supply (needs 5V)
Buzzer not sounding	Check polarity, try different pin
🚀 Enhancement Ideas
Future Features
Data logging to SD card

MQTT integration for cloud monitoring

Mobile app with push notifications

Voice control integration

Weather forecast integration

Multi-language dashboard

Advanced alert scheduling

Export data as CSV/JSON

Sensor Additions
Soil moisture sensor

UV light sensor

Sound level sensor

CO2 sensor

Barometric pressure sensor

📚 Learning Resources
ESP32 Official Documentation

Arduino WebServer Library

DHT Sensor Library

Neon CSS Effects

👥 Contributing
Contributions are welcome! Please feel free to submit a Pull Request.

Fork the repository

Create your feature branch (git checkout -b feature/AmazingFeature)

Commit your changes (git commit -m 'Add some AmazingFeature')

Push to the branch (git push origin feature/AmazingFeature)

Open a Pull Request

📜 License
Distributed under the MIT License. See LICENSE for more information.

🙏 Acknowledgements
Icons: Font Awesome 6.0

Color Palette: Cyberpunk/Neon aesthetic

Inspiration: Tribal patterns and jungle ecosystems

Community: ESP32 Arduino community

📞 Contact & Support
GitHub Issues: Report bugs or request features

Discussions: Join the conversation

<div align="center">
✨ May the jungle spirits guide your sensors! ✨
Connect to JUNGLE-NEXUS and experience the neon jungle at 192.168.4.1

https://img.shields.io/badge/Made%2520with-%E2%9D%A4%EF%B8%8F%2520and%2520ESP32-red?style=for-the-badge
https://img.shields.io/badge/Status-Operational-green?style=for-the-badge

</div>
