#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

//Informações do WiFi
#define STASSID "rede wifi"
#define STAPSK  "senha wifi"

//Informações do Servidor MQTT
const char* mqttserver = "ip mqtt";
int mqttserverport = 1883;
const char* mqttuser = "usuario mqtt";
const char* mqttpass = "senha mqtt";

//Variáveis
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

SoftwareSerial rylr(13, 15);

void reconnect() {
  mqttClient.setServer(mqttserver, mqttserverport);
  while (!mqttClient.connected()) {
    Serial.println("Conectando ao Broker MQTT");
    mqttClient.connect("esp8266lora",mqttuser,mqttpass);
    delay(3000);
  }
  Serial.println("MQTT conectado");
}

void reconnectwifi() {
  Serial.println();
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  IPAddress ip(192, 168, 6, 220);
  IPAddress gateway(192, 168, 6, 254);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(192, 168, 2, 254);
  WiFi.config(ip, dns, gateway, subnet);
  WiFi.begin(STASSID, STAPSK);
  delay(5000);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.println("");
  Serial.print("Conectado! IP address: ");
  Serial.println(WiFi.localIP());  
}

void setup(){
  Serial.begin(115200);

  reconnectwifi();

  if (!mqttClient.connected()) {
    reconnect();
  }
  
  rylr.begin(115200);


  //Configura Sensores através do MQTT Discovery do Home Assistant
  
  String lora_temp = "{\"expire_after\": \"7200\", \"device_class\": \"temperature\", \"unit_of_measurement\": \"°C\", \"name\": \"Temperatura Lora\", \"unique_id\": \"temp_lora\", \"state_topic\": \"esp8266lora/dht11/temperatura/state\"}";
  String lora_umid = "{\"expire_after\": \"7200\", \"device_class\": \"humidity\", \"unit_of_measurement\": \"%\", \"name\": \"Umidade Lora\", \"unique_id\": \"umid_lora\", \"state_topic\": \"esp8266lora/dht11/umidade/state\"}";
  
  mqttClient.publish("homeassistant/sensor/dht11lora/temperatura/config",lora_temp.c_str(),true);
  mqttClient.publish("homeassistant/sensor/dht11lora/umidade/config",lora_umid.c_str(),true);

}

void loop(){

  String leitura = rylr.readString();
  Serial.println(leitura);
  int i1 = leitura.indexOf(',');
  int i2 = leitura.indexOf(',', i1+1);
  int i3 = leitura.indexOf(',', i2+1);
  String parte1 = leitura.substring(0, i1);
  String parte2 = leitura.substring(i1 + 1, i2);
  String dados = leitura.substring(i2 + 1, i3);
  int i11 = dados.indexOf(';');
  int i22 = dados.indexOf(';', i1+1);
  String temp = dados.substring(0, i11);
  String umid = dados.substring(i11 + 1);
  String tempreal = temp.substring(5);
  String umidreal = umid.substring(5);
  Serial.println("Temperatura: " + tempreal);
  Serial.println("Umidade: " + umidreal);

  if (tempreal == 0) {
    Serial.println("Vazio, temperatura nao enviada!");
    } else {
      if (WiFi.status() != WL_CONNECTED) {
        reconnectwifi();
      }
      if (!mqttClient.connected()) {
        reconnect();
      }
      mqttClient.publish("esp8266lora/dht11/temperatura/state",tempreal.c_str());
      Serial.println("Temperatura Enviada!");
      Serial.println(tempreal);
    }
  if (umidreal == 0) {
    Serial.println("Vazio, umidade nao enviada!");
    } else {
      if (WiFi.status() != WL_CONNECTED) {
        reconnectwifi();
      }
      if (!mqttClient.connected()) {
        reconnect();
      }
      mqttClient.publish("esp8266lora/dht11/umidade/state",umidreal.c_str());
      Serial.println("Umidade Enviada!");
      Serial.println(umidreal);
    }
}
