#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
//#include "Ultrasonic.h"

//Constantes
#define DHTPIN D5 // Pino DHT
#define DHTTYPE DHT22 // DHT 22(AM2302)
#define DHT22 22
#define UMIDADESOLO A0



const char *ssid = "VIVOFIBRA-6876";
const char *password = "2evzd47Brd";
const String ipPort = "http://192.168.15.7:3000/";

DHT dht(DHTPIN, DHTTYPE); // Inicializando DHT

//criando objeto ultrasonic e definindo as portas digitais
//Ultrasonic ultrasonic(8,4);//do Trigger - 8 - e Echo - 4

//Variaveis
int chk;
float umidadeAr = 0;  //Valor da umidade
float temperatura = 0; //Valor da temperatura
long microsec = 0; // Leitura do ultrasonico
float distanciaCM = 0; //Distancia em CM
float umidadeSolo = 0; // Valor umidade solo

void setup() {
  //======= configurações Iniciais do modulo Wifi ====================//
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password); //Connect to your WiFi router
  Serial.println("");
  
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  //==================================================================//
  Serial.begin(115200);
  dht.begin(); //Inicializa o DHT
}

void loop() {
  float umidadeSolo = lerUmidadeSolo();
  float temp = lerTemperatura();
  float umidadeAr = lerUmidadeAr();
  http(temp, umidadeSolo, umidadeAr); //Funçao que gerencia os pacotes e clientes TCP.
  delay(60000); //Delay 60 sec.
}

float lerUmidadeSolo() {

  //-------------- Umidade Solo -----------------------//

  umidadeSolo = analogRead(UMIDADESOLO); //Conectar o sensor no A0
  Serial.print(umidadeSolo);//mostrar capacitancia do solo.
  Serial.println("% HR ");
  return umidadeSolo;

}

float lerDistanciaUltrassoniico() {
  //-------------- Ultrasonico -----------------------//
  //Convertendo a distĂ¢ncia em CM
  /* microsec = ultrasonic.timing();
    distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM);
    Serial.print(distanciaCM);
    Serial.println(" cm");*/
  return -1;
}

float lerTemperatura() {
  //-------------- Temperatura -----------------------//
  temperatura = dht.readTemperature();
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" ºC ");

  return temperatura;
}
float lerUmidadeAr() {
  // -------------- Umidade do Ar -------------------//
  umidadeAr = dht.readHumidity();
  Serial.print("Umidade do Ar: ");
  Serial.print(umidadeAr);
  Serial.println( " % ");

  return umidadeAr;
}

void http(float temperatura, float umidadeSolo, float umidadeAr) {
  HTTPClient httpClient;    //Declare object of class HTTPClient
  WiFiClient client;

  String temperaturaString = String(temperatura);
  String umidadeSoloString = String(umidadeSolo);
  String umidadeArString = String(umidadeAr);

  //Post Data
  String json = "{ \"umidadeDoSolo\" : \"" + umidadeSoloString + "\" , \"temperaturaDoAr\":" + temperaturaString + ", \"umidade\": " + umidadeArString + "}";

  httpClient.begin(client, ipPort);
  httpClient.addHeader("Content-Type", "application/json");
  
  //Specify content-type header
  Serial.println(json);
  int httpCode = httpClient.POST(json);   //Send the request
  String payload = httpClient.getString();    //Get the response payload
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  httpClient.end();  //Close connection

}
