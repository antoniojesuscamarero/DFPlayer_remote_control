#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Arduino.h>
#include <NECIRrcv.h>

#define IRPIN 2   // pin that IR detector is connected to

NECIRrcv ir(IRPIN) ;

SoftwareSerial mySoftwareSerial(10, 11); // TX, RX
DFRobotDFPlayerMini myDFPlayer;

unsigned long ircode ;
//Almacena el codigo de cada uno de los botones desde el 0 al 9
unsigned long posicion[10]={3994156545,4161273345,4194696705,4177985025,4094426625, 4127849985, 4111138305,4027579905,4061003265,4044291585};
bool pausa=false;
bool start=false;
bool vol=false;
String val="0";//Numero cancion
String val2="0";//Numero carpeta

bool fin=false;
unsigned int num=1;
int pos=0;
int pos2=0;
int valvol=20;//Valor inicial del volumen
unsigned int cont=1;
void setup() {
  //Serial.begin(115200);
  ir.begin() ;
  mySoftwareSerial.begin(9600);
  
  pinMode(8,INPUT);
  
 if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    //Serial.println("Error");
   
  while(true);
  //Serial.println("Modulo funcionando");
  }
  myDFPlayer.volume(valvol);  //Set volume value. From 0 to 30. Inicialmente esta en 20.
  
}

void loop() {
  //Serial.println("Inicio");
  boolean busy=digitalRead(8);
  if (start==false){
    myDFPlayer.start();
    start=true;
    delay(50);
  } 
  
  if(busy==true){
    myDFPlayer.next();   
  }
  while (ir.available()) { 
    ircode=ir.read();
    //Serial.println(ircode);

    if(ircode== 4144561665){//CH+
      myDFPlayer.next(); //Play next mp3
      //Serial.println("siguente");
      delay(50);
    }
    if(ircode== 4010868225){//CH-
       myDFPlayer.previous(); //Play previous mp3
       //Serial.println("Previo");
       delay(50);
    }
    if(ircode== 4278255105){//VOL+
      valvol=valvol+5;
      myDFPlayer.volume(valvol); //Volume Up
      //Serial.println("Mas volumen");
      delay(50);
    }
    if(ircode== 3810328065){//VOL-
      valvol=valvol-5;
      myDFPlayer.volume(valvol); //Volume Down   
      //Serial.println("Menos volumen");
      delay(50);
    }
    if(ircode== 3760193025){
      myDFPlayer.pause();  //pause the mp3
     //Serial.println("Pausa");
      pausa=true;
      delay(10);
      while(pausa==true){
        ircode=ir.read();
        if(ircode== 4077714945){
          myDFPlayer.start();
          pausa=false;
          delay(50);
          }
        }
    }
   
    if (ircode== 3843751425){//Boton verde
      //Aumentar 5 posiciones la reproduccion
      myDFPlayer.play(num);
      num=num+5;
      delay(50);
    }
    if (ircode== 3776904705){//Boton azul
      //Retroceder 5 posiciones la reproduccion
      myDFPlayer.play(num);
      num=num-5;
      delay(50);
    }
    if (ircode== 4244831745){//Boton mute
      if (vol==false){
        myDFPlayer.volume(0);
        vol=true;
        break;
      }
      if (vol==true){
        myDFPlayer.volume(valvol); 
        vol=false; 
        break;    
      }
      delay(50);
    }
    if (ircode== 3827039745){//Boton text, seleccion cancion
      cancion();
    }
    if (ircode== 3944021505){//Boton EPG, seleccion carpeta 
      carpeta();
    }
  }
  if (ircode== 3893886465){//Boton audio, cambio ecualizacion
    ecualizador();
  }
  delay(50);
}

void ecualizador(){
  //Serial.println("Introducir un numero");
      fin=false;
      while(fin==false){
        ircode=ir.read();
        for(int i=0; i<6;i++){
         if (ircode== posicion[i]){
            //Serial.print("Numero pulsado: "); Serial.println(i);
            cont=i; break;
           }              
        }
        if (ircode== 4211408385){//Boton EXIT
            fin=true;
            switch (cont){
              case 1: myDFPlayer.EQ(DFPLAYER_EQ_NORMAL); 
              break;
              case 2: myDFPlayer.EQ(DFPLAYER_EQ_POP);
              break;
              case 3: myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
              break;
              case 4: myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
              break;
              case 5: myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
              break;
              case 6: myDFPlayer.EQ(DFPLAYER_EQ_BASS);
              break;
            }  
          }
      }
}

void carpeta(){
   //Serial.println("Introducir un numero");
      fin=false;
      while(fin==false){
        ircode=ir.read();
        for(int i=0; i<10;i++){
         if (ircode== posicion[i]){
            //Serial.print("Numero pulsado: "); Serial.println(i);
            val2+=String(i);   
            break;         
           }              
        }
        if (ircode== 4077714945){//Boton OK
            fin=true;
            //Serial.println("fin introducir numeros");
            //Serial.println(val);
            pos2=val2.toInt();            
            myDFPlayer.playFolder(pos2, 1);  //play specific mp3 in SD:/15/004.mp3; Folder Name(1~99); File Name(1~255)
            val2="0";
          }
      }
}

void cancion(){
   //Serial.println("Introducir un numero");
      fin=false;
      while(fin==false){
        ircode=ir.read();
        for(int i=0; i<10;i++){
         if (ircode== posicion[i]){
            //Serial.print("Numero pulsado: "); Serial.println(i);
            val+=String(i);
            break;
           }              
        }
        if (ircode== 4077714945){//Boton OK
            fin=true;
            //Serial.println("fin introducir numeros");
            //Serial.println(val);
            pos=val.toInt();
            myDFPlayer.playFolder(pos2,pos);
            val="0";
          }
      }
} 
