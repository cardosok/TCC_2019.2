#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
//#include "Ultrasonic.h" 
//Constantes
#define DHTPIN D5 // Pino DHT
#define DHTTYPE DHT22 // DHT 22(AM2302)
#define DHT22 22
#define UMIDADESOLO A0

WiFiServer sv(555);//Cria o objeto servidor na porta 555
WiFiClient cl;//Cria o objeto cliente.
DHT dht(DHTPIN, DHTTYPE); // Inicializando DHT
//Ultrasonic ultrasonic(8,4);//do Trigger - 8 - e Echo - 4

//Variaveis
float umidadeAr = 0;  //Valor da umidade
float temperatura = 0; //Valor da temperatura
long microsec = 0; // Leitura do ultrasonico
float distanciaCM = 0; //Distancia em CM
float umidadeSolo =0; // Valor umidade solo

void setup() {
  //======= configurações Iniciais do modulo Wifi ====================//
    Serial.begin(115200);//Habilita a comm serial.
    WiFi.mode(WIFI_AP);//Define o WiFi como Acess_Point.
    WiFi.softAP("NodeMCU", "");//Cria a rede de Acess_Point.
    sv.begin();//Inicia o servidor TCP na porta declarada no começo.
//==================================================================//  
  dht.begin(); //Inicializa o DHT
}

void loop() {
  float umidade = lerUmidadeSolo();
  float temp = lerTemperaturaUmidadeAr();
  tcp(temp, umidade);//Funçao que gerencia os pacotes e clientes TCP.
  delay(2000); //Delay 2 sec.
}

float lerUmidadeSolo(){
  
//-------------- Umidade Solo -----------------------// 

  umidadeSolo = analogRead(UMIDADESOLO); //Conectar o sensor no A0
  Serial.print(umidadeSolo);//mostrar capacitancia do solo.
  Serial.println("% HR ");
  return umidadeSolo;
  
}

float lerDistanciaUltrassoniico(){
  //-------------- Ultrasonico -----------------------// 
   //Convertendo a distĂ¢ncia em CM
   /* microsec = ultrasonic.timing();
    distanciaCM = ultrasonic.convert(microsec, Ultrasonic::CM); 
    Serial.print(distanciaCM);
    Serial.println(" cm");*/
    return -1;
}

float lerTemperaturaUmidadeAr(){
//-------------- Umidade Ar e Temperatura -----------------------//
    umidadeAr = dht.readHumidity();
    temperatura = dht.readTemperature();
    Serial.print("Umidade do Ar: ");
    Serial.print(umidadeAr);
    Serial.println( " % ");
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" ºC ");

    return temperatura;
}


void tcp(float temperatura,float umidadeSolo){
    String t1 = String(temperatura);
    String t2 = String(umidadeSolo);
    //Detecta se há clientes conectados no servidor.
    if (cl.connected()){
        //Verifica se o cliente conectado tem dados para serem lidos.
        if (cl.available() > 0){
            String req = "";
            while (cl.available() > 0){
                char z = cl.read();//Armazena cada Byte (letra/char) na String para formar a mensagem recebida.
                req += z;
            }
            //Mostra a mensagem recebida do cliente no Serial Monitor.
            Serial.print("\nUm cliente enviou uma mensagem");
            Serial.print("\n...IP do cliente: ");
            Serial.print(cl.remoteIP());
            Serial.print("\n...IP do servidor: ");
            Serial.print(WiFi.softAPIP());
            Serial.print("\n...Mensagem do cliente: " + req + "\n");
            //Envia uma resposta para o cliente
            cl.print("\nO servidor recebeu sua mensagem");
            cl.print("\n...Sua mensagem: " + req + "\n");
            cl.print("\n Sua temperatura atual " + t1 + "\n");
            cl.print("\n Sua Humidade do solo atual " + t2 + "\n");
        }
    }
    else{
      cl = sv.available(); // Disponabiliza o servidor para o cliente se conectar.
      delay(1); // delay de 1 millisegundo para refazer a requisição
    }
}
