🌿 Jungle Nexus - ESP32 IoT Environmental Monitoring System
🔥 Neon Jungle Cyber-Ecosystem
Welcome to JUNGLE NEXUS, a futuristic IoT environmental monitoring system that transforms sensor data into a vibrant neon jungle cyberpunk experience! This ESP32-based project creates a WiFi access point with a stunning web interface that visualizes environmental data through tribal-themed biome cards.

🎯 Project Vision
Imagine a cyberpunk jungle where technology meets nature. This project creates an immersive dashboard that represents different environmental sensors as mystical jungle biomes - from volcanic cores to misty swamps, each with its own neon aesthetic and tribal guardians.

🌟 Key Features
Sensor Ecosystem
Temperature Monitoring (Volcanic Core Biome) - DHT22 sensor

Humidity Tracking (Misty Swamp Biome) - DHT22 sensor

Air Quality Analysis (Cavern Air Biome) - MQ2 (smoke) & MQ3 (alcohol) sensors

Rain Detection (River Flow Biome) - Raindrop sensor

Motion Sensing (Predator Watch Biome) - PIR sensor

LED Spirit Lights (Canopy Lights Biome) - 4x controlled LEDs

Interactive Controls
Tribal Guardian Mode - Security activation with motion alerts

Waterfall Gate Control - Servo-controlled "waterfall" (0-180°)

Spirit LED System - Four individually controlled "jungle spirits"

Automated Responses - Servo adjusts automatically during rain

Audible Alerts - Buzzer notifications for critical events

🛠️ Technical Stack
Hardware Components
ESP32 Development Board

DHT22 Temperature & Humidity Sensor

MQ2 Smoke/Gas Sensor

MQ3 Alcohol/Gas Sensor

Raindrop Sensor

PIR Motion Sensor

Servo Motor (Simulates waterfall gate)

Buzzer (Audio alerts)

4x LEDs (Jungle spirit lights)

Software Architecture
WiFi Access Point Mode (No internet required)

WebServer with AJAX API

Real-time Dashboard (No page refresh)

JSON Data API for sensor readings

Neon Jungle UI with animated CSS

🎨 UI/UX Features
Visual Design
Neon Jungle Color Palette - Electric greens, deep purples, toxic yellows

Tribal Patterns & Vines - Animated background elements

Biome Cards - Each sensor group in themed containers

Animated Gauges - Circular progress indicators

Smooth Transitions - All interactions without page reload

Interactive Elements
Tribal Toggle Switches - Custom-styled controls

Waterfall Progress Bar - Visual servo position indicator

Status Badges - Dynamic color-coded states

Real-time Updates - 2-second sensor refresh

Mobile Responsive - Works on all devices

📡 Network Setup
The system creates its own WiFi network:

SSID: JUNGLE-NEXUS

Password: neon2024

IP Address: 192.168.4.1

Port: 80

🚀 Setup Instructions
1. Hardware Connections
cpp
MQ2_PIN = 34      // Smoke sensor
MQ3_PIN = 35      // Alcohol sensor
DHT_PIN = 32      // Temperature/Humidity
BUZZER_PIN = 25   // Alert buzzer
RAINDROP_PIN = 33 // Rain sensor
PIR_PIN = 26      // Motion sensor
SERVO_PIN = 27    // Waterfall gate
LED1_PIN = 12     // Fire Spirit
LED2_PIN = 13     // Forest Spirit (Default ON)
LED3_PIN = 14     // Water Spirit
LED4_PIN = 15     // Sun Spirit
2. Installation
Clone the repository

Open in Arduino IDE

Install required libraries:

WiFi.h

WebServer.h

DHT.h

ESP32Servo.h

Upload to ESP32

Connect to JUNGLE-NEXUS WiFi

Navigate to 192.168.4.1

⚙️ System Logic
Automated Behaviors
Rain Detection: Servo opens to 180° during rain, returns to 90° after

Safety Mode: Motion detection triggers buzzer alerts

Air Quality: Gas/smoke thresholds trigger warnings

Temperature/Humidity: Visual status changes based on readings

Manual Controls
Toggle individual LED "spirits"

Activate/deactivate guardian mode

Control servo position manually

Mass LED control (all on/off)

🎮 Dashboard Sections
Volcanic Core - Temperature monitoring with heat gauge

Misty Swamp - Humidity tracking with moisture meter

Cavern Air - Gas levels with toxicity indicator

Predator Watch - Motion detection with guardian toggle

River Flow - Rain sensing with waterfall gate control

Canopy Lights - LED spirit controls with tribal toggles

🔧 API Endpoints
GET /sensorData - JSON sensor readings

POST /toggleSafety - Toggle security mode

POST /toggleLed?led=1-4 - Control individual LEDs

POST /allLedsOn / /allLedsOff - Mass LED control

POST /servoControl?angle=0-180 - Set servo position

🎯 Use Cases
Home Automation - Environmental monitoring with style

Educational Tool - IoT and web development learning

Art Installation - Interactive cyberpunk exhibit

Workshop Demo - Complete IoT system example

Prototype Platform - Extensible sensor framework

📁 Project Structure
text
jungle-nexus/
├── jungle_nexus.ino      # Main ESP32 code
├── libraries/            # Required dependencies
├── images/               # UI assets and screenshots
├── wiring_diagram.png    # Connection schematic
└── README.md            # This file
🤝 Contributing
This project welcomes enhancements! Consider:

Adding more sensor types

Implementing data logging

Creating mobile app companion

Adding sound effects

Extending automation rules

Multi-language support

📜 License
Open-source under MIT License. Tribal rights reserved by the Ancient Tech Council.

🌐 Connect
Experience the neon jungle at 192.168.4.1 - where technology meets tribal mysticism!
