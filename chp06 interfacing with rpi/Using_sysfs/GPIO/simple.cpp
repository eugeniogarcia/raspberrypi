/* Aplicacion que demuestra el uso del GPIO
 */

#include<iostream>
#include<unistd.h>  // for the usleep() function
#include"GPIO.h"
using namespace exploringRPi;
using namespace std;

int main(){
   //Objetos GPIO. Trabajamos con los pins 17 y 27
   GPIO outGPIO(17), inGPIO(27);    // pin 11 and pin 13

   //Indica que es una salida
   outGPIO.setDirection(OUTPUT);    // basic output example
   //Crea una curva con un escalon que dura 0.5 segundos
   for (int i=0; i<10; i++){        // flash the LED 10 times
      outGPIO.setValue(HIGH);       // turn the LED on
      usleep(500000);               // sleep for 0.5 seconds
      outGPIO.setValue(LOW);        // turn the LED off
      usleep(500000);               // sleep for 0.5 seconds
   }

   //determina que se trata de una entrada
   inGPIO.setDirection(INPUT);      // basic input example
   cout << "The input state is: "<< inGPIO.getValue() << endl;

   //brimos un stream, para escribir valores
   outGPIO.streamOpen();            // fast write example
   for (int i=0; i<1000000; i++){   // write 1 million cycles
      outGPIO.streamWrite(HIGH);    // high
      outGPIO.streamWrite(LOW);     // immediately low, repeat
   }
   outGPIO.streamClose();           // close the stream
   return 0;
}
