/***********
 * ESP32 LINE Messaging API Library
 * サンプルスケッチ
 * 
 * このサンプルは、ESP32を使用してLINE Messaging APIでメッセージを送信する方法を示します。
 * 使用前に、LINE Developers ConsoleでMessaging APIのチャネルを作成し、
 * アクセストークンを取得してください。
 * 
 * GPIO2に割り込みが入った場合にLINEへメッセージを送ります。
 * 
 * 参考: https://developers.line.biz/ja/services/messaging-api/
 ***********/

#include "ESP32LineMessenger.h"
#include <M5Unified.h>

// WiFiとLINEの設定
const char* ssid = "YOUR_SSID";              // WiFiのSSID
const char* password = "YOUR_PASS";      // WiFiのパスワード

// 以下からLINEチャネルアクセストークンを取得する
// https://developers.line.biz/ja/
const char* accessToken = "YOUR_LINE_CHANNEL_ACCESS_TOKEN";  // LINEチャネルアクセストークン

// デバッグモード設定
#define debug true

// ライブラリインスタンス作成
ESP32LineMessenger line;

// GPIO2用の定数
#define GPIO_INTERRUPT_PIN 2

// 割り込みフラグ
volatile bool interruptOccurred = false;

// 割り込みハンドラ関数
void IRAM_ATTR handleInterrupt() {
  interruptOccurred = true;
}

void setup() {
  Serial.begin(115200);  // デバッグ用シリアル
  delay(100);
  
  // M5Stackの初期化
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // アクセストークン設定
  line.setAccessToken(accessToken);
  
  // デバッグモード設定（オプション）
  line.setDebug(debug);
  
  // WiFi接続
  if (line.connectWiFi(ssid, password, debug)) {
    Serial.println("WiFi connected");
    
    // 起動時のメッセージ送信
    line.sendMessage("M5AtomS3 started. GPIO2 interrupt monitoring is active.", debug);
    
    // GPIO2を割り込みピンとして設定
    pinMode(GPIO_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(GPIO_INTERRUPT_PIN), handleInterrupt, FALLING);
    
    Serial.println("GPIO2 interrupt monitoring is active");
    M5.Lcd.println("GPIO2 interrupt\nmonitoring active");
  } else {
    Serial.println("WiFi connection failed");
    M5.Lcd.println("WiFi connection\nfailed");
  }
}

void loop() {
  M5.update();
  
  // 割り込みが発生した場合
  if (interruptOccurred) {
    Serial.println("Interrupt detected on GPIO2!");
    M5.Lcd.println("Interrupt detected!\nSending LINE msg...");
    
    // LINEにメッセージを送信
    if (line.isWiFiConnected()) {
      line.sendMessage("Interrupt detected on GPIO2!", debug);
      Serial.println("LINE message sent");
      M5.Lcd.println("LINE message sent");
    } else {
      Serial.println("WiFi disconnected. Reconnecting...");
      M5.Lcd.println("WiFi disconnected\nReconnecting...");
      
      // WiFiに再接続
      if (line.connectWiFi(ssid, password, debug)) {
        line.sendMessage("Interrupt detected on GPIO2!", debug);
        Serial.println("LINE message sent after reconnection");
        M5.Lcd.println("LINE message sent\nafter reconnection");
      } else {
        Serial.println("WiFi reconnection failed");
        M5.Lcd.println("WiFi reconnection\nfailed");
      }
    }
    
    // フラグをリセット
    interruptOccurred = false;
    
    // チャタリング防止のための遅延
    delay(500);
  }
  
  // 他の処理を追加する場合はここに記述
  delay(100);
}