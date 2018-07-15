/***************************************************
DFPlayer - A Mini MP3 Player For Arduino
 <https://www.dfrobot.com/index.php?route=product/product&product_id=1121>
 
 ***************************************************
 This example shows the basic function of library for DFPlayer.
 
 Created 2016-12-07
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Arduino.h>
#include <NECIRrcv.h>

#define IRPIN 2   // pin that IR detector is connected to

NECIRrcv ir(IRPIN) ;

SoftwareSerial mySoftwareSerial(10, 11); // TX, RX
DFRobotDFPlayerMini myDFPlayer;

unsigned long ircode ;
unsigned long posicion[10]={3994156545,4161273345,4194696705,4177985025,4094426625, 4127849985, 4111138305,4027579905,4061003265,4044291585};
bool pausa=false;
bool start=false;
bool vol=false;
String val="0";
unsigned int vez=0;
bool fin=false;
unsigned int num=1;
int pos=0;
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
  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  
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

    if(ircode== 4144561665){
      myDFPlayer.next(); //Play next mp3
      //Serial.println("siguente");
      delay(50);
    }
    if(ircode== 4010868225){
       myDFPlayer.previous(); //Play previous mp3
       //Serial.println("Previo");
       delay(50);
    }
    if(ircode== 4278255105){
      myDFPlayer.volumeUp(); //Volume Up
      //Serial.println("Mas volumen");
      delay(50);
    }
    if(ircode== 3810328065){
      myDFPlayer.volumeDown(); //Volume Down   
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
        myDFPlayer.volume(10); 
        vol=false; 
        break;    
      }
      delay(50);
    }
    if (ircode== 3827039745){
      //Serial.println("Introducir un numero");
      fin=false;
      while(fin==false){
        ircode=ir.read();
        for(int i=0; i<10;i++){
         if (ircode== posicion[i]){
            //Serial.print("Numero pulsado: "); Serial.println(i);
            val+=String(i);
           }              
        }
        if (ircode== 4077714945){
            fin=true;
            //Serial.println("fin introducir numeros");
            //Serial.println(val);
            pos=val.toInt();
            myDFPlayer.play(pos);
            vez=0; val="0";
          }
      }
  }
  }
  delay(50);
}
