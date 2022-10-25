// --- WIFI ---
#include <ESP8266WiFi.h>     
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>      
WiFiClient nodemcuClient;

// --- MQTT ---
#include <PubSubClient.h>
const char* mqtt_Broker = "192.168.0.24";
const char* mqtt_ClientID = "pzem004tdiegopnf";
PubSubClient client(nodemcuClient);
const char* topicoTensao = "pzem/tensao";
const char* topicoCorrente = "pzem/corrente";
const char* topicoPotencia = "pzem/potencia";
const char* topicoFrequencia = "pzem/frequencia";
const char* topicoFatorpotencia = "pzem/fatorpotencia";
const char* topicoPotenciahora = "pzem/potenciahora";

// --- PZEM004T ---
#include <PZEM004Tv30.h>
PZEM004Tv30 pzem(D5, D6);
float tensao;
float corrente;
float potencia;
float frequencia;
float fatorpotencia;
float potenciahora;

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Medidor de Energia");
  client.setServer(mqtt_Broker, 1883);
}

void loop() {
  reconectarMQTT();
  medirEnergia();
  publicarEnergiaNoTopico();
  serialLeitura();
}

void reconectarMQTT() {
  while (!client.connected()) {
    client.connect(mqtt_ClientID);
  }
}

void publicarEnergiaNoTopico() {
  client.publish(topicoTensao, String(tensao).c_str(), true);
  client.publish(topicoCorrente, String(corrente).c_str(), true);
  client.publish(topicoPotencia, String(potencia).c_str(), true);
  client.publish(topicoFrequencia, String(frequencia).c_str(), true);
  client.publish(topicoFatorpotencia, String(fatorpotencia).c_str(), true);
  client.publish(topicoPotenciahora, String(potenciahora).c_str(), true);
}

void medirEnergia(){
  tensao = pzem.voltage();
  corrente = pzem.current();
  potencia = pzem.power();
  frequencia = pzem.frequency();
  fatorpotencia = pzem.pf();
  potenciahora = pzem.energy();
  delay(5000);
}

void serialLeitura(){
  Serial.print("Tensao (V): ");
  Serial.println(tensao);
  
  Serial.print("Corrente (A): ");
  Serial.println(corrente);
  
  Serial.print("Potencia (W): ");
  Serial.println(potencia);
  
  Serial.print("Potencia/hora (KWh): ");
  Serial.println(potenciahora);
  
  Serial.print("Frquencia (Hz): ");
  Serial.println(frequencia);
  
  Serial.print("Fator de potencia: ");
  Serial.println(fatorpotencia);
  Serial.println(" ");
  
}
