# IoT-Based Smart Attendance System with ESP8266 & Telegram Notifications

An IoT-based smart attendance system developed using the **ESP8266** microcontroller. The system provides a web-based attendance interface that allows students to mark their attendance using a smartphone, while automatically sending a real-time attendance notification to parents through **Telegram**.

---

## 📌 Project Overview

Traditional attendance systems often require manual recording or additional identification hardware such as RFID readers and biometric sensors. This project provides a simple alternative using only an **ESP8266**, WiFi connectivity, and a web interface.

The ESP8266 hosts a web page where students can enter their details and submit their attendance. Once the attendance is submitted, the ESP8266 processes the information and sends a notification to the registered parent/guardian through a **Telegram Bot**.

### Key Concept

```text
Student Smartphone
        │
        │ WiFi
        ▼
   ESP8266 Web Server
        │
        │ Attendance Data
        ▼
 Attendance Processing
        │
        │ Telegram Bot API
        ▼
 Parent / Guardian
    Telegram Message
```

---

## ✨ Features

* 📱 Web-based attendance marking
* 📡 WiFi connectivity using ESP8266
* 🌐 Embedded web server
* 👨‍🎓 Student Name and USN input
* ⏱️ Attendance time recording
* 📲 Real-time Telegram notification
* 🔧 No RFID, fingerprint, or other external identification hardware
* 💰 Low-cost implementation
* ⚡ Simple and fast operation
* 🔄 Supports continuous attendance marking

---

## 🛠️ Hardware Requirements

| Component                |  Quantity |
| ------------------------ | --------: |
| ESP8266 NodeMCU          |         1 |
| USB Cable                |         1 |
| Smartphone               | 1 or more |
| WiFi/Internet Connection |  Required |

> **Note:** No external sensors or identification modules are required.

---

## 💻 Software Requirements

* Arduino IDE
* ESP8266 Board Package
* C/C++ programming
* Telegram Bot
* WiFi-enabled smartphone
* Internet connection for Telegram communication

### Required Libraries

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
```

---

## ⚙️ Working Principle

### 1. ESP8266 Initialization

The ESP8266 is programmed using Arduino IDE. During startup, the microcontroller initializes the WiFi functionality and web server.

### 2. WiFi Connection

The ESP8266 establishes a WiFi connection and provides access to the attendance web page.

### 3. Student Access

Students use a smartphone connected to the same network to access the ESP8266 web server.

The attendance page provides fields such as:

* Student Name
* USN

### 4. Attendance Submission

The student enters the required details and clicks the **Mark Attendance** button.

The ESP8266 receives the HTTP request and extracts the submitted information.

### 5. Attendance Processing

The ESP8266 processes the student information and generates an attendance record containing the student details and attendance time.

### 6. Telegram Notification

After successful attendance submission, the ESP8266 communicates with the **Telegram Bot API** and sends a notification to the configured parent/guardian Telegram account.

Example:

```text
📢 Attendance Notification

Student: Jeevan
USN: 4JN22EC001
Status: Present
Time: 09:15 AM

Attendance marked successfully.
```

---

## 🔄 System Workflow

```text
             START
               │
               ▼
      Initialize ESP8266
               │
               ▼
       Connect to WiFi
               │
               ▼
       Start Web Server
               │
               ▼
     Student Opens Web Page
               │
               ▼
       Enter Student Data
               │
               ▼
       Submit Attendance
               │
               ▼
    ESP8266 Processes Data
               │
               ▼
      Record Attendance
               │
               ▼
     Send Telegram Message
               │
               ▼
     Parent Receives Alert
               │
               ▼
          Continue
```

---

## 📲 Telegram Integration

The project uses a Telegram Bot to deliver attendance notifications.

The basic communication process is:

```text
ESP8266
   │
   │ HTTPS Request
   ▼
Telegram Bot API
   │
   ▼
Parent's Telegram
```

To configure Telegram:

1. Create a Telegram bot using **BotFather**.
2. Obtain the bot token.
3. Obtain the required chat ID.
4. Add the credentials to the ESP8266 program.
5. Upload the program to the ESP8266.
6. Test the attendance notification.

> **Security:** Never publish your actual Telegram bot token or private credentials in a public GitHub repository.

---

## 📂 Project Structure

```text
ESP8266-Smart-Attendance/
│
├── Smart_Attendance/
│   └── Smart_Attendance.ino
│
├── README.md
│
└── images/
    ├── system_block_diagram.png
    ├── attendance_webpage.png
    └── telegram_notification.png
```

---

## 🚀 How to Run

### Step 1 — Install Arduino IDE

Install Arduino IDE and add ESP8266 board support.

### Step 2 — Install Required Libraries

Install the required ESP8266 libraries through Arduino IDE.

### Step 3 — Configure WiFi

Update the WiFi credentials in the source code:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

### Step 4 — Configure Telegram

Enter your Telegram bot token and chat ID:

```cpp
String botToken = "YOUR_BOT_TOKEN";
String chatID = "YOUR_CHAT_ID";
```

### Step 5 — Select Board

In Arduino IDE, select the appropriate ESP8266 board, such as:

```text
NodeMCU 1.0 (ESP-12E Module)
```

### Step 6 — Upload

Connect the ESP8266 to the computer using USB and upload the program.

### Step 7 — Access the Attendance Page

Open the ESP8266 IP address in a smartphone browser.

Example:

```text
http://192.168.x.x
```

Enter the student details and submit the attendance.

---

## 📊 Expected Output

### Web Interface

```text
--------------------------------
       SMART ATTENDANCE
--------------------------------

Student Name:
[________________________]

USN:
[________________________]

      [ MARK ATTENDANCE ]

--------------------------------
```

### Telegram Notification

```text
📢 Attendance Notification

Student: Jeevan
USN: 4JN22EC001
Status: Present
Time: 09:15 AM
```

---

## ✅ Advantages

* Low-cost system
* Simple implementation
* No RFID or biometric hardware
* Smartphone-based attendance
* Real-time parent notification
* Easy to deploy
* Compact and portable
* Can be expanded with additional IoT features

---

## ⚠️ Limitations

* Requires network connectivity for Telegram communication
* Basic name/USN-based authentication can allow proxy attendance
* ESP8266 has limited memory and processing resources
* Telegram notification requires internet access

---

## 🔮 Future Enhancements

The system can be further improved by implementing:

* 🔐 OTP-based attendance verification
* 👤 Student authentication/login
* 📍 Location-based attendance verification
* ☁️ Firebase or cloud database integration
* 📊 Attendance dashboard
* 📈 Monthly attendance reports
* 📧 Email notifications
* 📱 Dedicated mobile application
* 🧑‍🏫 Teacher/admin login
* 📑 Automatic attendance report generation

---

## 🎯 Applications

This system can be used in:

* Schools
* Colleges
* Universities
* Coaching institutes
* Training centers
* Workshops
* Small organizations

---

## 🧰 Technologies Used

| Technology          | Purpose                 |
| ------------------- | ----------------------- |
| ESP8266             | Main IoT controller     |
| WiFi                | Wireless communication  |
| Embedded Web Server | Attendance interface    |
| HTML                | Webpage design          |
| C/C++               | ESP8266 programming     |
| Telegram Bot API    | Parent notification     |
| Arduino IDE         | Development environment |

---

## 📚 Project Objectives

The main objectives of this project are:

1. To develop a low-cost IoT-based attendance system.
2. To eliminate the requirement for additional attendance hardware.
3. To provide a simple web interface for attendance marking.
4. To process attendance data using ESP8266.
5. To provide real-time attendance notifications to parents through Telegram.
6. To demonstrate the practical application of IoT in education.

---

## 👨‍💻 Project

**Project:** IoT-Based Smart Attendance System
**Controller:** ESP8266 NodeMCU
**Communication:** WiFi
**Notification:** Telegram Bot API
**Interface:** Web Application
**Domain:** IoT / Embedded Systems / Wireless Communication

---

## 📜 License

This project is intended for educational and academic purposes. You are free to modify and improve the project for learning and development.

---

## ⭐ Support

If you find this project useful, consider giving the repository a ⭐ on GitHub.
