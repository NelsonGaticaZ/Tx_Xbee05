//Este programa es para poder enviar rafagas de 50 mensajes sin cambio de potencia Tx desde el nodo movil    ///
//Ademas incluye un buzzer para indicar el inicio y termino del envio                                      ///
//Última modificación hecha por Nicolas Roa el jueves 22 de septiembre del 2016                           ///
//Última modificación hecha por Nelson Gatica el jueves 29 de septiembre del 2016 a las 18:01hrs         ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//Serial virtual
#include <SoftwareSerial.h>
// RTC
#include <DS1337.h>
DS1337 RTC;
//I2C
#include <Wire.h>

#define pinLed      13    //pin de led rojo
#define pinInt      8   //pin del pulsador
#define num_mensajes  1   //cantidad de mensajes a enviar por diparo (pulsador)
#define pin_buzzer    12    //pin del buzzer

SoftwareSerial mySerial(6, 7);  // RX, TX

unsigned long   t_sampling=100000;    //en microsegundos
unsigned long   next_fast_sampling;
unsigned long   time_out;
unsigned long   tiempo;         //tiempo RTC
unsigned long   variable;

byte i;               
byte num_disparo=0;         //numero disparo, veces que se presiona el pulsador (1byte)
byte cont;              
byte contador;            
unsigned char buf[9];         

int altura_nm=170;          //altura nodo movil       (2 bytes)
byte version_hardware=0xa1; //version_hardware de dispositivo (HW+SW)(1 byte)
byte potencia=0x03;         //potencia de transmision   (1 byte)

void setup(){
  Serial.begin(57600);      // original nelson->57600
    mySerial.begin(57600);      // original nelson->57600
  RTC.start();          // ensure RTC oscillator is running

    delay(500);
    mySerial.print("Serial Virtual OK");

  pinMode(pinInt,INPUT);
    pinMode(pinLed,OUTPUT);
    pinMode(pin_buzzer,OUTPUT);
}

void loop() { 

  if(digitalRead(pinInt)==1){
    digitalWrite(pinLed,1); //Enciende Led que indica inicio envio de datos
    delay(500);             //Espera para evitar valores del tren de pulsos
        //buzzer(1);      //sonido indicador de inicio
        union{
 int8_t b[4];
 uint32_t w;
}variable;
    tiempo=RTC.getTime();
    variable.w=tiempo;
    num_disparo++;
             
     for (i=1;i<=num_mensajes;i++){  
    next_fast_sampling = micros()+t_sampling;
    
    //Serial.write(tiempo);     //envio de fecha y hora
    //Serial.write(0xe4);         //envio de fecha y hora
    //Serial.write(0x12);         //envio de fecha y hora
    //Serial.write(0x6e);         //envio de fecha y hora
    Serial.write(variable.b[0]);
    Serial.write(variable.b[1]);
    Serial.write(variable.b[2]);
    Serial.write(variable.b[3]);
    Serial.write(num_disparo);        //envio de numero de disparo
    Serial.write(i);            //envio de numero de mensaje
    Serial.write(potencia);         //envio de vivel de potencia de transmision
    Serial.write(version_hardware);         //envio de version_hardware de dispositivo
    Serial.write(altura_nm);        //envio de altura nm
    
                
        //mySerial.write(num_disparo); 
        //mySerial.write(i);  
    /*mySerial.write(variable.b[0]);
    mySerial.write(variable.b[1]);
    mySerial.write(variable.b[2]);
    mySerial.write(variable.b[3]);*/
         
        Smart_delay(); //Espera t_sampling microsegundos antes de enviar otro paquete
     }
  digitalWrite(pinLed,0);  //Apaga Led que indica fin del envio de paquetes
  //buzzer(0);        //sonido indicador de fin, es posible realizar siguiente disparo
  }
}

void Smart_delay(){  
  while (micros()<next_fast_sampling){ } //Para marcar los 100ms entre mensajes
}

void buzzer(byte entrada){
 if (entrada==1){//comienzo del disparo
   for (int n=1;n<10;n++){
    digitalWrite(pin_buzzer,1);
    delay(100);
        digitalWrite(pin_buzzer,0);
        delay(100);
   }  
 }

 if (entrada==0){//termino de la rafaga
   for (int n=1;n<2;n++){
    digitalWrite(pin_buzzer,1);
    delay(1000);
        digitalWrite(pin_buzzer,0);
        delay(1000);
   }  
 }
}


