#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";
const char* mqtt_server = "192.168.1.100"; // IP del broker Mosquitto

WiFiClient espClient;
PubSubClient client(espClient);

const int rele1 = 18;
const int rele2 = 19;
const int rele3 = 21;

void setup_wifi() {
    delay(10);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado");
    Serial.println(WiFi.localIP());
}

void apagarTodo() {
    digitalWrite(rele1, LOW);
    digitalWrite(rele2, LOW);
    digitalWrite(rele3, LOW);
}

void callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) msg += (char)payload[i];

    Serial.println("Mensaje MQTT: " + msg);
    apagarTodo();

    if (msg == "1") digitalWrite(rele1, HIGH);
    else if (msg == "2") digitalWrite(rele2, HIGH);
    else if (msg == "3") digitalWrite(rele3, HIGH);
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Intentando conexión MQTT...");
        if (client.connect("ESP32Client")) {
            Serial.println("Conectado");
            client.subscribe("ventilador/velocidad/set");
        } else {
            Serial.print("falló, rc=");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(rele1, OUTPUT);
    pinMode(rele2, OUTPUT);
    pinMode(rele3, OUTPUT);
    apagarTodo();
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
