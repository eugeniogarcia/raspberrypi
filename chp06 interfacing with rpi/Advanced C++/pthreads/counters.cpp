#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>
using namespace std;

//Funcion que usaremos para lanzar ejecutar en el thread
void *counter(void *value){
   int x=0, endCount = *((int *)value);
   cout << "Thread starting counting to " << endCount << endl;
   while(x<endCount){       //while the value of x is less than endCount
      usleep(10);            //sleep for 10us - encourage main thread
      x++;                //increment the value of x by 1
   }
   cout << "Thread finished counting to " << endCount << endl;
   return value;            //return the pointer x (as a void*)
}

int main(int argc, char *argv[]) {
   if(argc!=2){
      cout << "Please pass a valid number between 1 and 99" << endl;
      return -EINVAL;
   }
   //Declara un input stream, y lo informa con el argumento de nuestro programa
   istringstream ss(argv[1]);
   int numCount, endCount=100000;
   //!(ss>>numCount)) daria false si el argumento no fuera un numero
   if((!(ss>>numCount)) || (numCount<1) || (numCount>99)){
      cout << "Please pass a number between 1 and 99" << endl;
      return -EINVAL;
   }
   //Declara un array de threads
   pthread_t thread[numCount];      // array of handles to the threads

   //Para cada uno de los elementos del array...
   for(int i=0; i<numCount; i++){
      // pthread_create() returns 0 on the successful creation of a thread
      //Crea un thread, y lo guarda en el array. EL thread ejecuta la funcion
      //counter, y toma como argumento 100 ms
      if(pthread_create(&thread[i], NULL, &counter, &endCount)!=0){
         cout << "Failed to create the thread" << endl;
         return 1;
      }
      else{
         cout << "Successfully created a thread" << endl;
      }
   }

   //Hacemos un join de todos los threads. Creo que no es la mejor forma de hacerlo
   //porque no solamente esperamos a que todos los thread terminen, sino que
   //ademas estamo esperando que terminen en un orden concreto 
   void* result;
   for(int i=0; i<numCount; i++){
      pthread_join(thread[i], &result);   // allow the pthreads to complete
   }
   int *z = (int *) result;         // cast from void* to int* to get z

   return 0;
}
