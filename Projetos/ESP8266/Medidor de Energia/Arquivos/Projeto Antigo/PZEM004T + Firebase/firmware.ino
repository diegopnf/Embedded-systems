#include <ESP8266WiFi.h>     
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>    
#include <FirebaseESP8266.h>
#include <PZEM004Tv30.h>
#include <Wire.h> 

#define FIREBASE_HOST "teste-ae6f3.firebaseio.com/"
#define FIREBASE_AUTH "sCXHsnIrdZoBwfpbmBINRa2u4VIIYbxhcvVaagaC"

PZEM004Tv30 pzem(D5, D6);
FirebaseData firebaseData;

void setup()
{
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  wifiManager.autoConnect("ESP8266");
  Serial.println("Conectado :)");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.print("Reiniciar Energia");
  pzem.resetEnergy();
}

void loop() {
  //energymeter 2
  
  float tensao = pzem.voltage();
  Serial.print("Tensao (V): ");
  Serial.println(tensao);
  
  float corrente = pzem.current();
  Serial.print("Corrente (A): ");
  Serial.println(corrente);
  
  float potencia = pzem.power();
  Serial.print("Potencia (W): ");
  Serial.println(potencia);
  
  float potenciahora = pzem.energy();
  Serial.print("Potencia/hora (KWh): ");
  Serial.println(potenciahora);
  
  float frequencia = pzem.frequency();
  Serial.print("Frquencia (Hz): ");
  Serial.println(frequencia);
  
  float fatorpotencia = pzem.pf();
  Serial.print("Fator de potencia: ");
  Serial.println(fatorpotencia);
  Serial.println(" ");

  Firebase.setFloat(firebaseData, "/ESP8266_APP/TENSAO", tensao);
  Firebase.setFloat(firebaseData, "/ESP8266_APP/CORRENTE", corrente);
  Firebase.setFloat(firebaseData, "/ESP8266_APP/POTENCIA", potencia);
  Firebase.setFloat(firebaseData, "/ESP8266_APP/POTENCIAHORA", potenciahora);
  Firebase.setFloat(firebaseData, "/ESP8266_APP/FATOR DE POTENCIA", fatorpotencia);
  
  delay(10000); // 10 segundos
}
