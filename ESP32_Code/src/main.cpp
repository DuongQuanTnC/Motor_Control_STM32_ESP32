#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi & MQTT
const char* ssid = "Danh Quân";
const char* password = "19042003";
const char* mqtt_server = "192.168.1.102";

WiFiClient espClient;
PubSubClient client(espClient);
HardwareSerial stm32Serial(1);  // UART1: RX = GPIO16, TX = GPIO17

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Received MQTT message: ");
    Serial.print(topic);
    Serial.print(" -> ");
    
    String message;
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Serial.println(message);
    
    // Nếu nhận đúng dạng "<Speed:XXX>"
    if (message.startsWith("<") && message.endsWith("\n")) {
        Serial.println("Valid speed command received!");
        stm32Serial.println(message);  // Gửi qua UART đến STM32
    }
}

void setup() {
    Serial.begin(115200);
    stm32Serial.begin(115200, SERIAL_8N1, 16, 17); // UART1, RX=16, TX=17

    // Kết nối WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    // Kết nối MQTT
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    while (!client.connected()) {
        if (client.connect("ESP32Client")) {
            Serial.println("Connected to MQTT!");
            client.subscribe("motor/control");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying...");
            delay(5000);
        }
    }
}

void loop() {
    client.loop(); // Lắng nghe MQTT

    // Nhận dữ liệu từ STM32
    if (stm32Serial.available()) {
        String receivedData = stm32Serial.readStringUntil('\n');  // Đọc đến ký tự xuống dòng
        receivedData.trim();  // Xóa khoảng trắng đầu/cuối

        if (receivedData.startsWith("<Speed:") && receivedData.endsWith(">")) {
            Serial.print("Received from STM32: ");
            Serial.println(receivedData);
            client.publish("motor/response", receivedData.c_str());  // Gửi lên MQTT
        }
    }
}
