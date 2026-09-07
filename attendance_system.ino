/*
 * =====================================================
 *  ESP8266 College Attendance System
 *  - Web interface (hosted on ESP8266) for marking attendance
 *  - Telegram notification sent to parents of absent students
 * =====================================================
 *
 *  LIBRARIES REQUIRED (Install via Arduino Library Manager):
 *  1. ESP8266WiFi        (comes with ESP8266 board package)
 *  2. ESP8266WebServer   (comes with ESP8266 board package)
 *  3. CTBot              by Stefano Ledda  (for Telegram)
 *     OR use UniversalTelegramBot by Brian Lough
 *
 *  BOARD SETUP:
 *  - Board: "NodeMCU 1.0 (ESP-12E Module)"
 *  - Upload Speed: 115200
 *
 * =====================================================
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>

// ─────────────────────────────────────────────
//  CONFIG — EDIT THESE
// ─────────────────────────────────────────────

const char* WIFI_SSID     = "G-1 IQOO";
const char* WIFI_PASSWORD = "1234567890";

// Telegram Bot Token (from @BotFather)
const String BOT_TOKEN = "8655968984:AAFyXUHFGlWAxyTN2FwkslnBrRr7qxxFeas";

// ─────────────────────────────────────────────
//  STUDENT & PARENT DATA
//  Add/remove students here.
//  chat_id = parent's Telegram chat ID
//  (Parent must send /start to your bot first,
//   then get their chat ID from @userinfobot)
// ─────────────────────────────────────────────

struct Student {
  String name;
  String parentChatId;
};

const int STUDENT_COUNT = 5;
Student students[STUDENT_COUNT] = {
  {"Jeevan J.M",    "5148080737"},
  {"mithun sv",   "222222222"},
  {"akm ghate",     "333333333"},
 
};

// Attendance status: true = Present, false = Absent
bool attendance[STUDENT_COUNT];

// ─────────────────────────────────────────────
//  GLOBALS
// ─────────────────────────────────────────────

ESP8266WebServer server(80);
String lastSubmitResult = "";
bool attendanceSubmitted = false;

// ─────────────────────────────────────────────
//  TELEGRAM — Send message via HTTPS
// ─────────────────────────────────────────────

void sendTelegram(String chatId, String message) {
  WiFiClientSecure client;
  client.setInsecure(); // Skip SSL cert verification (OK for hobby projects)

  const char* host = "api.telegram.org";

  if (!client.connect(host, 443)) {
    Serial.println("[Telegram] Connection failed!");
    return;
  }

  // URL-encode the message (basic: replace spaces with %20)
  message.replace(" ", "%20");
  message.replace("\n", "%0A");

  String url = "/bot" + BOT_TOKEN +
               "/sendMessage?chat_id=" + chatId +
               "&text=" + message;

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(500);
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line); // Debug: print response
  }
  Serial.println("[Telegram] Message sent to " + chatId);
}

// ─────────────────────────────────────────────
//  HTML PAGE — Attendance Form
// ─────────────────────────────────────────────

String buildHTML() {
  String html = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Attendance System</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: 'Segoe UI', sans-serif;
      background: #f0f4f8;
      min-height: 100vh;
      display: flex;
      justify-content: center;
      align-items: flex-start;
      padding: 30px 16px;
    }
    .card {
      background: white;
      border-radius: 16px;
      box-shadow: 0 4px 24px rgba(0,0,0,0.10);
      padding: 32px 28px;
      width: 100%;
      max-width: 480px;
    }
    h1 {
      font-size: 1.5rem;
      color: #1a202c;
      margin-bottom: 4px;
    }
    .subtitle {
      color: #718096;
      font-size: 0.9rem;
      margin-bottom: 24px;
    }
    .student-row {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 12px 16px;
      border-radius: 10px;
      margin-bottom: 10px;
      background: #f7fafc;
      border: 1px solid #e2e8f0;
      transition: background 0.2s;
    }
    .student-row:hover { background: #edf2f7; }
    .student-name {
      font-size: 1rem;
      color: #2d3748;
      font-weight: 500;
    }
    .toggle {
      display: flex;
      gap: 8px;
    }
    .toggle label {
      cursor: pointer;
      padding: 6px 16px;
      border-radius: 20px;
      font-size: 0.85rem;
      font-weight: 600;
      border: 2px solid transparent;
      transition: all 0.2s;
    }
    input[type=radio] { display: none; }
    .present-label { background: #c6f6d5; color: #276749; border-color: #9ae6b4; }
    .absent-label  { background: #fed7d7; color: #9b2335; border-color: #fc8181; }
    input[type=radio].present:checked + .present-label,
    input[type=radio].absent:checked  + .absent-label {
      box-shadow: 0 0 0 3px rgba(66,153,225,0.4);
      transform: scale(1.05);
    }
    .submit-btn {
      width: 100%;
      padding: 14px;
      background: linear-gradient(135deg, #667eea, #764ba2);
      color: white;
      border: none;
      border-radius: 10px;
      font-size: 1rem;
      font-weight: 700;
      cursor: pointer;
      margin-top: 20px;
      letter-spacing: 0.5px;
      transition: opacity 0.2s;
    }
    .submit-btn:hover { opacity: 0.9; }
    .result {
      margin-top: 18px;
      padding: 14px;
      border-radius: 10px;
      background: #ebf8ff;
      border-left: 4px solid #4299e1;
      color: #2b6cb0;
      font-size: 0.95rem;
      white-space: pre-line;
    }
  </style>
</head>
<body>
<div class="card">
  <h1>&#127979; Attendance Portal</h1>
  <p class="subtitle">Mark today's attendance and notify parents</p>

  <form action="/submit" method="POST">
)rawhtml";

  for (int i = 0; i < STUDENT_COUNT; i++) {
    String idx = String(i);
    html += "<div class='student-row'>";
    html += "<span class='student-name'>" + students[i].name + "</span>";
    html += "<div class='toggle'>";

    // Present radio
    html += "<input type='radio' class='present' name='s" + idx + "' id='p" + idx + "' value='present' checked>";
    html += "<label class='present-label' for='p" + idx + "'>Present</label>";

    // Absent radio
    html += "<input type='radio' class='absent' name='s" + idx + "' id='a" + idx + "' value='absent'>";
    html += "<label class='absent-label' for='a" + idx + "'>Absent</label>";

    html += "</div></div>";
  }

  html += "<button type='submit' class='submit-btn'>&#9989; Submit &amp; Notify Parents</button>";
  html += "</form>";

  if (lastSubmitResult != "") {
    html += "<div class='result'>" + lastSubmitResult + "</div>";
  }

  html += "</div></body></html>";
  return html;
}

// ─────────────────────────────────────────────
//  ROUTE: GET /  — Show attendance form
// ─────────────────────────────────────────────

void handleRoot() {
  server.send(200, "text/html", buildHTML());
}

// ─────────────────────────────────────────────
//  ROUTE: POST /submit — Process form
// ─────────────────────────────────────────────

void handleSubmit() {
  int absentCount = 0;
  String resultMsg = "✅ Attendance submitted!\n\n";

  for (int i = 0; i < STUDENT_COUNT; i++) {
    String key = "s" + String(i);
    String val = server.arg(key);

    if (val == "absent") {
      attendance[i] = false;
      absentCount++;

      // Send Telegram notification to parent
      String msg = "📢 Attendance Alert!\n"
                   "Dear Parent,\n"
                   + students[i].name +
                   " was marked ABSENT today.\n"
                   "Please contact the college for details.";

      sendTelegram(students[i].parentChatId, msg);
      resultMsg += "❌ " + students[i].name + " — Absent (Parent notified)\n";
      Serial.println("Absent: " + students[i].name);

    } else {
      attendance[i] = true;
      resultMsg += "✔️ " + students[i].name + " — Present\n";
      Serial.println("Present: " + students[i].name);
    }
  }

  if (absentCount == 0) {
    resultMsg += "\n🎉 All students present today!";
  } else {
    resultMsg += "\n📲 " + String(absentCount) + " parent(s) notified via Telegram.";
  }

  lastSubmitResult = resultMsg;
  server.sendHeader("Location", "/");
  server.send(303);
}

// ─────────────────────────────────────────────
//  SETUP
// ─────────────────────────────────────────────

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n[ESP8266] Attendance System Starting...");

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("[WiFi] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[WiFi] Connected!");
  Serial.print("[WiFi] IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("[INFO] Open this IP in your browser to access the attendance form.");

  // Register routes
  server.on("/",       HTTP_GET,  handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);

  server.begin();
  Serial.println("[Server] Web server started on port 80");
}

// ─────────────────────────────────────────────
//  LOOP
// ─────────────────────────────────────────────

void loop() {
  server.handleClient();
}
