//bibliotecas para ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//Bibliotecas para TELEGRAM
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

//Bibliotecas para MQTT
#include <PubSubClient.h>

//PINES de ESP8266
#define boton1 04
#define boton2 05
#define interruptor 15

// funcion Deep Sleep
#define SLEEP_TIME 1800

//Variables de tiempo
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

unsigned long tiempo3 = 0;
unsigned long tiempo4 = 0;

unsigned long tiempo5 = 0;
unsigned long tiempo6 = 0;

unsigned long tiempo7 = 0;
unsigned long tiempo8 = 0;

//variables de diferencias de tiempo
unsigned long diferenciaTiempo=0;
unsigned long diferenciaTiempo2=0;
unsigned long diferenciaTiempo3=0;
unsigned long diferenciaTiempo4=0;

//variables de lectura de botones
int Lboton1=0;
int Lboton2=0;
int sumabotones=0;
int lectura_de_interruptor;

// variable para TELEGRAM
char mensaje;

//constantes de RED
const char* ssid = "Queso54";
const char* password = "quegalle123.z";
//constante de servidor
const char*mqtt_server = "192.168.1.79";

//tokens para CHATBOTS
#define BOTtoken "5513596431:AAGQm7NEc0qF976_0z2-DTrUxPZuBH1AMp4"  // your Bot Token (Get from Botfather)
#define CHAT_ID "1299899272"

#define BOTtoken1 "5386748185:AAEEpAeT7E6_iEmw_55Z7Vf2BjuO4-GUGSk"
#define CHAT_ID1 "1299899272"

#define BOTtoken2 ""  
#define CHAT_ID2 ""

#define BOTtoken3 ""
#define CHAT_ID3 ""

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;

UniversalTelegramBot bot(BOTtoken, client);
UniversalTelegramBot bot1(BOTtoken1, client);
UniversalTelegramBot bot2(BOTtoken2, client);
UniversalTelegramBot bot3(BOTtoken3, client);

//instancias para MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient); // creamos una instancia parcialmete inicializada


void setup() {

  //velocidad del SERIAL
  Serial.begin(115200);
  //Modo de operacion de los PINES
  pinMode(interruptor, INPUT);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);

  //funcion millis
  tiempo1=millis(); //tiempo para sensor 1 
  tiempo3=millis(); //tiempo para sensor 2
  tiempo5=millis(); //tiempo para sensores inactivos  
  tiempo7=millis(); //tiempo para sensores activos
  
  //recolector de DATE
  configTime(0, 0, "pool.ntp.org");

  client.setTrustAnchors(&cert); 

  // conexion WiFi
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //si no se encuentra conectado
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  //si se encuentra conectado
  Serial.println("Dispositivo conectado");
  Serial.println("direccion IP:");
  Serial.println(WiFi.localIP());

  //cliente MQTT
  mqttClient.setServer (mqtt_server, 1883);
  mqttClient.setCallback(callback);
}

void loop() {

if (!mqttClient.connected()){
reconnect();
}



     //lectura de sensores
     Lboton1=digitalRead(boton1);
     Lboton2=digitalRead(boton2);
     lectura_de_interruptor=digitalRead(interruptor);
     sumabotones=Lboton1+Lboton2;

     //Rutina

     //si el sensor 1 detecta estado alto
     if(Lboton1==1){
 
              tiempo2=millis(); //inicia tiempo2
              diferenciaTiempo=tiempo2/1000 - tiempo1/1000;
              Serial.println(diferenciaTiempo);

     if(diferenciaTiempo==20){//si han pasado 20 min
              
             mqttClient.subscribe("maquinaCortadora");
             mqttClient.publish("maquinaCortadora", "Alarma nivel 1");
             Serial.println(" han pasado 20min de inactividad "); 
             bot.sendMessage(CHAT_ID, "OptiCut sin operación, Alarma nivel 1, favor de atender");//mandar mensaje a chatbot 1
     }
     if(diferenciaTiempo==40){ //si han pasado 40 min

            
              mqttClient.subscribe("maquinaCortadora");
              mqttClient.publish("maquinaCortadora", "Alarma nivel 2");
              Serial.println("2 han pasado 40min de inactividad ");
              bot1.sendMessage(CHAT_ID1, "OptiCut sin operación, Alarma nivel 2, favor de atender");//mandar mensaje a chatbot 2
     }
     if(diferenciaTiempo==3600){// si ha pasado 1 hora

      
            mqttClient.subscribe("maquinaCortadora");
            mqttClient.publish("maquinaCortadora", "Alarma nivel 3");
            Serial.println("3 han pasado 60min de inactividad ");
            bot2.sendMessage(CHAT_ID2, "OptiCut sin operación, Alarma nivel 3, favor de atender");//mandar mensaje a chatbot 3
     }
     if(diferenciaTiempo==4800){//si ha pasado 1hr con 20min

      
            mqttClient.subscribe("maquinaCortadora");
            mqttClient.publish("maquinaCortadora", "Alarma nivel 4");
            Serial.println("4 han pasado 1hr con 20min de inactividad ");
            bot3.sendMessage(CHAT_ID3, "OptiCut sin operación, Alarma nivel 4, URGENTE");//mandar mensaje a chatbot 4
     }
     
     }
     else{
     tiempo1=millis();
     }

     /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
    // si el sensor 2 es activado contar tabla
    if(Lboton2==1){
      
        tiempo4=millis();
        diferenciaTiempo2=tiempo4/1000 -tiempo3/1000;
        Serial.println(diferenciaTiempo2);

        mqttClient.loop();

        mqttClient.subscribe("maquinaCortadora");
        mqttClient.publish("maquinaCortadora", "tabla contada");
        Serial.println("tabla contada");

        //tiempo de espera  entre tabla y tabla
        delay (10000);
    }
 /*<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>*/
    
    if(diferenciaTiempo2==20){

          
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 1");
           delay (1000);
           Serial.println("Han pasado 20 min con unicamente el sensor 2 activo");
           bot.sendMessage(CHAT_ID, "han pasado 20 min con el sensor dos activo");//mandar mensaje a chatbot 1
    }
    
    if(diferenciaTiempo2==40){

      
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 2");
           delay (1000);
           Serial.println("Han pasado 40 min con unicamente el sensor 1 activo");
           bot1.sendMessage(CHAT_ID1, "han pasado 40 min con unicamente el sensor2 activo");//mandar mensaje a chatbot 2
    }
    if(diferenciaTiempo2==3600){
           
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 3");
        
           Serial.println("Han pasado 60 segundos con unicamente el sensor 1 activo");
           bot2.sendMessage(CHAT_ID2, "han pasado 60 min con unicamente el sensor2 activo");//mandar mensaje a chatbot 2
    }
    if(diferenciaTiempo==4800){//si ha pasado 1hr con 20min

            mqttClient.subscribe("maquinaCortadora");
            mqttClient.publish("maquinaCortadora", "Alarma nivel 4");
            Serial.println("4 han pasado 1hr con 20min de inactividad ");
            bot3.sendMessage(CHAT_ID3, "han pasado 1hr con 20 min con unicamente el sensor2 activo");//mandar mensaje a chatbot 4
    }
    
    

    else{
    tiempo3=millis();  
    }

    
    if(sumabotones==0){
      
          tiempo6=millis();
          diferenciaTiempo3=tiempo6/1000-tiempo5/1000;
          Serial.print("tiempo transcurrido inactivo");
          Serial.println(diferenciaTiempo3);

    if(diferenciaTiempo3==20){

            
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 1");
           Serial.println("Primera notificacion 20 min con ambos sensores inactivos");
           bot.sendMessage(CHAT_ID, "OptiCut sin operación, Alarma nivel 1, favor de atender");//mandar mensaje a chatbot 1
           //modificacion 4 de junio 2022
          tiempo8=millis();
        
          diferenciaTiempo4=tiempo8/1000-tiempo7/1000;
    }
    if(diferenciaTiempo3==40){

           
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 2");
           Serial.println("Segunda notificacion, demasiado tiempo con los sensores inactivos");
           bot1.sendMessage(CHAT_ID1, "OptiCut sin operación, Alarma nivel 2, favor de atender");//mandar mensaje a chatbot 2

    }
    if(diferenciaTiempo3==3600){
         
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 3");
           Serial.println("Tercera notificacion, demasiado tiempo con los sensores inactivos");
           bot2.sendMessage(CHAT_ID2, "OptiCut sin operación, Alarma nivel 3, favor de atender");//mandar mensaje a chatbot 3
    }
    if(diferenciaTiempo3==4800){
          
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 4");
           Serial.println("Cuarta notificacion, demasiado tiempo con los sensores inactivos");
           bot3.sendMessage(CHAT_ID3, "OptiCut sin operación, Alarma nivel 4, URGENTE");//mandar mensaje a chatbot 4
    }
    }
    else{
      tiempo5=millis();     
    }
  
    if(sumabotones==2){
      tiempo8=millis();
      diferenciaTiempo4=tiempo8/1000-tiempo7/1000;
      Serial.print("tiempo transcurrido con los dos sensores activos");
      Serial.println(diferenciaTiempo4);

    if(diferenciaTiempo4==20){
            
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 1");
           Serial.println("Primera notificacion, ambos sensores han pasado mucho tiempo activos");
           bot.sendMessage(CHAT_ID, "OptiCut sin operación, Alarma nivel 1, favor de atender");//mandar mensaje a chatbot 1

    }
    if(diferenciaTiempo==40){
      
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 2");
           Serial.println("Segunda notificación, ambos sensores han pasado mucho tiempo activos");
           bot1.sendMessage(CHAT_ID1, "OptiCut sin operación, Alarma nivel 2, favor de atender");//mandar mensaje a chatbot 2

    }
    if(diferenciaTiempo==3600){
          
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarama nivel 3");
           Serial.println("Tercera Notificacion, ambos sensores han pasado 3 segundos activos");
           bot2.sendMessage(CHAT_ID2, "OptiCut sin operación, Alarma nivel 3, favor de atender");//mandar mensaje a chatbot 3
    }
    if(diferenciaTiempo==4800){

      
           mqttClient.subscribe("maquinaCortadora");
           mqttClient.publish("maquinaCortadora", "Alarma nivel 4");
           Serial.println("Cuarta Notificacion, ambos sensores han pasado mucho tiempo activos");
           bot3.sendMessage(CHAT_ID3, "OptiCut sin operación, Alarma nivel 4, URGENTE");//mandar mensaje a chatbot 2
    }
    else{
        tiempo7=millis();
    }
    }

    if(lectura_de_interruptor==1){
          
          mqttClient.subscribe("maquinaCortadora");
          mqttClient.publish("maquinaCortadora", "Descanso/Hora de comida");
          Serial.println("durmiendo");
          bot.sendMessage(CHAT_ID,   "Descanso/Hora de comida");//mandar mensaje a chatbot 1
          bot1.sendMessage(CHAT_ID1, "Descanso/Hora de comida");//mandar mensaje a chatbot 2
          bot2.sendMessage(CHAT_ID2, "Descanso/Hora de comida");//mandar mensaje a chatbot 3
          bot3.sendMessage(CHAT_ID3, "Descanso/Hora de comida");//mandar mensaje a chatbot 4
          delay(3000);
   
          ESP.deepSleep(SLEEP_TIME * 1000000, WAKE_RF_DEFAULT);
          Serial.println("durmiendo");
          }
          else{ 
          }
}

          




          void callback (char* topic, byte* payload, unsigned int length){
              Serial.print("mensaje cargado [");
              Serial.print(topic);
              Serial.print("]");
              for (int i=0; i<length; i++){
              Serial.print((char)payload[i]);
                  }
                Serial.println();
              }

           //reconectar al cliente mqtt
            void reconnect(){
            while (!mqttClient.connected()){
            if (mqttClient.connect("testgs")){
      
            mqttClient.subscribe("maquinaCortadora");
              }else{
            Serial.print("fallo, reconectando ...");
            Serial.print(mqttClient.state());
            mqttClient.subscribe("maquinaCortadora");
            mqttClient.publish ("maquinaCortadora", "falló dispositivo y se reconecto");
            Serial.println ("volviendo a conectar ");
            delay (1000);
            }
            }
            }
