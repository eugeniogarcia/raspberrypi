#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

//Funcion que se ejecutara desde el thread. devuelve un puntero a void y
//acepta un puntero a void. Notese que no es un puntero a funcion
void *threadFunction(void *value){
   int *x = (int *)value;    //cast the data passed to an int pointer
   while(*x<5){              //while the value of x is less than 5
      usleep(10);            //sleep for 10us - encourage main thread
      (*x)++;                //increment the value of x by 1
   }
   return x;                 //return the pointer x (as a void*)
}

int main() {
   int x=0, y=0;
   //Estructura con la que se declara un thread
   pthread_t thread;        //this is our handle to the pthread
   // create the thread, pass the reference, address of the function and data
   // pthread_create() returns 0 on the successful creation of a thread
   //Crea el thread, especificando como argumento la funcion que se ejecutara,
   //y pasandole sus argumentos
   //Todos los argumentos de la funcion que crea threads son punteros
   if(pthread_create(&thread, NULL, &threadFunction, &x)!=0){
      cout << "Failed to create the thread" << endl;
      return 1;
   }
   // at this point the thread was created successfully
   while(y<5){              // loop and increment y, displaying values
      cout << "The value of x=" << x << " and y=" << y++ << endl;
      usleep(10);           // encourage the pthread to run
   }
   //Este metodo espera a que el thread haya terminado, y recibe la respuesta
   void* result;            // OPTIONAL: receive data back from pthread
   pthread_join(thread, &result);   // allow the pthread to complete
   int *z = (int *) result;         // cast from void* to int* to get z
   cout << "Final: x=" << x << ", y=" << y << " and z=" << *z << endl;
   return 0;
}
