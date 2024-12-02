#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqtt_server = ""; 
const char* mqtt_user = "";
const char* mqtt_password = "";

WiFiClient espClient;
PubSubClient client(espClient);

const int pirPin = 11; 
int pirState = LOW;    

void callback(char* topic, byte* payload, unsigned int length) {
  // accuiones MQTT
}

void setup() {
  Serial.begin(115200);
  pinMode(pirPin, INPUT); // Configura el pin PIR como entrada

  // Configuración de Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");

  // Configuración de MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Connected to MQTT broker!");
    } else {
      Serial.print("Failed with state: ");
      Serial.println(client.state());
      delay(2000);
    }
  }

  client.subscribe("raspberry_to_esp32"); // Suscripción a un tema (opcional)
}

void loop() {
  client.loop(); // Mantiene la conexión MQTT activa

  // Lee el estado del PIR
  int currentState = digitalRead(pirPin);
  if (currentState == HIGH && pirState == LOW) {
    Serial.println("Motion detected!");
    pirState = HIGH;

    // Publica un mensaje al tema MQTT
    String message = "Movimiento detectado";
    client.publish("esp32_to_raspberry", message.c_str());
  } else if (currentState == LOW && pirState == HIGH) {
    pirState = LOW; // Reinicia el estado cuando no hay movimiento
  }

  delay(100); // Breve pausa para evitar sobrecarga
}
