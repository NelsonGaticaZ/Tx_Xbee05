//Este programa es para poder enviar rafagas de 50 mensajes sin cambio de potencia Tx            ///
//Ademas incluye un buzzer para indicar el inicio y termino del envio                            ///
//Última modificación hecha por Nelson Gatica el miercoles 14 de septiembre del 2016, 14:15 horas///
////////////////////////////////////////////////////////////////////////////////////////////////////

//Serial virtual
#include <SoftwareSerial.h>
#define pinLed 13
#define pinInt 8
#define num_mensajes 50
#define pin_buzzer 12 

SoftwareSerial mySerial(6, 7); // RX, TX

unsigned long t_sampling=100000;//en microsegundos
unsigned long next_fast_sampling;
unsigned long time_out;
byte i;
byte num_disparo=0;
byte cont;
byte contador;
unsigned char buf[5]; 

void setup(){
  Serial.begin(57600);
    mySerial.begin(57600);
      delay(500);
        mySerial.print("Serial Virtual OK");
  pinMode(pinInt,INPUT);
    pinMode(pinLed,OUTPUT);
      pinMode(pin_buzzer,OUTPUT);
}

void loop() { 
  if(digitalRead(pinInt)==1){
    digitalWrite(pinLed,1);  //Enciende Led que indica inicio envio de datos
      delay(500);            //Espera para evitar valores del tren de pulsos
        buzzer(1);
    num_disparo++;
             
     for (i=1;i<=num_mensajes;i++){  
      next_fast_sampling = micros()+t_sampling;
         Serial.write(num_disparo);
            Serial.write(i);
            
              mySerial.write(num_disparo); 
                  mySerial.write(i);  
         
         Smart_delay(); //Espera t_sampling microsegundos antes de enviar otro paquete
          
     }
  digitalWrite(pinLed,0);  //Apaga Led que indica fin del envio de paquetes
  buzzer(0);
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


