/***
 * perftest.cpp A simple threaded performance test that provides indicative performance
 * results on a single core versus a multi core processor using pthreads.
 * This example uses the C++ chrono library that was introduced in C++11, so please
 * compile using: g++ perftest.cpp -o perftest -pthread -stdc++11
 */

#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
#define NUM_RANDS 5000000

void* thread_function(void*) {       // generate 5M random numbers
   unsigned rand_seed = 0;
   for(int i=0; i<NUM_RANDS; i++)
      rand_r(&rand_seed);
   return 0;
}

void random_generate_no_threads(int numCalls) {
   for(int i=0; i<numCalls; i++){
      thread_function(NULL);
   }
}

void random_generate_with_threads(int numCalls) {
   pthread_t* threads[numCalls];
   for(int i=0; i<numCalls; i++){
      threads[i] = new pthread_t;
   }
   //Al iniciar un thread tenemos que pasarle los atributos - ver https://users.cs.cf.ac.uk/Dave.Marshall/C/node30.html
   //Para ello inicializamos la correspondiente estructura
   pthread_attr_t* attr = new pthread_attr_t;
   pthread_attr_init(attr);
   //Creamos los threads - todos ellos con los mismos parametros
   for(int i=0; i<numCalls; i++){
      pthread_create(threads[i],attr,thread_function,0);
   }
   //Hacemos un join. Esto lo podemos hacer porque en los atributos de creación del
   //thread hemos dejado los valores por defecto, y asi es equivalente a
   //ret = pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_JOINABLE);
   //Si hubieramos especificado lo siguiente:
   //ret = pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
   //El thread se habria creado desatachado, y no podriamos hacer un join con el
   //para ver los resultados

   for(int i=0; i<numCalls; i++){
      pthread_join(*threads[i],0);
   }
   //Es necesario elimiar los atributos
   pthread_attr_destroy(attr);
   //Limpiamos los threads de la memoria
   for(int i=0; i<numCalls; i++){
      delete threads[i];            // cannot used delete[]
   }
   delete attr;
}

int main(int argc, char* argv[]) {
   int numCalls = 1;
   if (argc==2){
      int passed = atoi(argv[1]);
      if (passed==0){
         cout << "Invalid number passed to the program -- exiting!" << endl;
         return -EINVAL;    //invalid argument
      }
      else { numCalls = passed; }
   }
   //Recupera el numero de threads que soportamos con nuestra CPU
   unsigned int numThreads = std::thread::hardware_concurrency();
   cout << "This hardware supports " << numThreads << " concurrent threads." <<endl;

   cout << "Performing test using " << numCalls << " thread enabled function calls" << endl;
   //Empezamos el temporizador
   auto begin = std::chrono::high_resolution_clock::now();
   random_generate_no_threads(numCalls);
   //Terminamos el temporizador
   auto end = std::chrono::high_resolution_clock::now();
   //Calculamos el tiempo que ha tardado
   std::cout << "Real Time: No threads  " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " us" << endl;

   begin = std::chrono::high_resolution_clock::now();
   random_generate_with_threads(numCalls);
   end = std::chrono::high_resolution_clock::now();
   std::cout << "Real Time: With threads " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " us" << endl;
   return 0;
}
