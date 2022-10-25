// --- WIFI ---
#include <ESP8266WiFi.h>     
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>      
WiFiClient nodemcuClient;

// --- MQTT ---
#include <PubSubClient.h>
const char* mqtt_Broker = "192.168.0.24"; //ip broker externo ou interno
const char* mqtt_ClientID = "pzem004tdiegopnf"; //nome de indenficacao no broker
PubSubClient client(nodemcuClient);
const char* topicoTensao = "pzem/tensao"; //topicos que o broker vai receber
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

// --- ENVIARMYSQL ---
const char http_site[] = "http://desktop-40l53b37/"; //nome dns do broker
const int http_port = 8090; 
IPAddress server(000,000,000,000); //Endereço IP do servidor - http_site

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("Medidor de Energia");
  client.setServer(mqtt_Broker, 1883); //porta para uso mqtt
}

void loop() {
  reconectarMQTT();
  medirEnergia();
  publicarTensaoNoTopico();
  serialLeitura();
  byte usuario = 1;

  if ( !getPage((int)usuario, (float)tensao,(float)corrente,(float)potencia,(float)potenciahora,(float)frequencia,(float)fatorpotencia)) {
  Serial.println("GET request failed");
  }  
}

void reconectarMQTT() {
  while (!client.connected()) {
    client.connect(mqtt_ClientID);
  }
}

void publicarTensaoNoTopico() {
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

  
bool getPage(int usuario, float tensao,float corrente,float potencia,float potenciahora,float frequencia, float fatorpotencia) {
  if ( !nodemcuClient.connect(server, http_port) ) {
    Serial.println("Falha na conexao com o site ");
    return false;
  }
  String param = "?tensao=" + String(tensao) + "&usuario=" + String(usuario) + "&corrente=" + String(corrente) + "&potencia=" + String(potencia) + "&potenciahora=" + String(potenciahora) + "&frequencia=" + String(frequencia) + "&fatorpotencia=" + String(fatorpotencia); //Parâmetros com as leituras
  Serial.println(param);
  nodemcuClient.println("GET /insert_topico.php" + param + " HTTP/1.0"); //insert_topico.php arquivo na raiz da pasta de site do xamp
  nodemcuClient.println("Host: ");
  nodemcuClient.println(http_site);
  nodemcuClient.println("Connection: close");
  nodemcuClient.println();
  nodemcuClient.println();

    // Informações de retorno do servidor para debug
  while(nodemcuClient.available()){
    String line = nodemcuClient.readStringUntil('\r');
    Serial.print(line);
  }
  return true;
}



  
