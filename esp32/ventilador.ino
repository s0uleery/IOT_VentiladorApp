#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

// WiFi
const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

// MQTT
const char* mqtt_server = "192.168.1.100";
const char* topic_control = "ventilador/velocidad/set";
const char* topic_temp = "sensor/ambiente/temperatura";
const char* topic_humedad = "sensor/ambiente/humedad";
const char* topic_gas = "sensor/ambiente/gas";

// Pines de relés
const int rele1 = 18;
const int rele2 = 19;
const int rele3 = 21;

// Sensor MQ2
const int mq2Pin = 34;

// MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// AHT10
Adafruit_AHTX0 aht;

// ================= FUNCIONES ====================

void apagarTodosReles() {
    digitalWrite(rele1, LOW);
    digitalWrite(rele2, LOW);
    digitalWrite(rele3, LOW);
}

void setVelocidad(int nivel) {
    apagarTodosReles();
    switch (nivel) {
        case 1: digitalWrite(rele1, HIGH); break;
        case 2: digitalWrite(rele2, HIGH); break;
        case 3: digitalWrite(rele3, HIGH); break;
        default: break;
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) msg += (char)payload[i];

    Serial.print("⚙️ Control recibido: "); Serial.println(msg);

    int velocidad = msg.toInt();
    setVelocidad(velocidad);
}

void setup_wifi() {
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); Serial.print(".");
    }
    Serial.println("\n✅ Conectado a WiFi");
    Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Reconectando MQTT...");
        if (client.connect("ESP32Client")) {
            Serial.println("Conectado");
            client.subscribe(topic_control);
        } else {
            Serial.print("Error: "); Serial.println(client.state());
            delay(2000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(rele1, OUTPUT);
    pinMode(rele2, OUTPUT);
    pinMode(rele3, OUTPUT);
    apagarTodosReles();

    pinMode(mq2Pin, INPUT);
    Wire.begin();

    if (!aht.begin()) {
        Serial.println("❌ No se detectó AHT10");
        while (1) delay(10);
    } else {
        Serial.println("✅ Sensor AHT10 listo");
    }

    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) reconnect();
    client.loop();

    // Leer sensores
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    int gas = analogRead(mq2Pin);

    Serial.printf("🌡️ %.2f°C | 💧 %.2f%% | 🔥 MQ2: %d\n",
                  temp.temperature, humidity.relative_humidity, gas);

    // Publicar datos
    client.publish(topic_temp, String(temp.temperature, 2).c_str());
    client.publish(topic_humedad, String(humidity.relative_humidity, 2).c_str());
    client.publish(topic_gas, String(gas).c_str());

    delay(5000); // Esperar 5 segundos
}
