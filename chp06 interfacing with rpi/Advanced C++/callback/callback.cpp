/* A callback function example */

#include<iostream>
using namespace std;

//definimos un tipo llamado CallbackType, que es un puntero
//a funciones que toman int y devuelven int
typedef int (*CallbackType)(int); // used to tidy up the syntax

int squareCallback(int x){        // callback function that squares
   return x*x;
}

int cubeCallback(int x){          // callback function that cubes
   return x*x*x;
}

//Tiene como argumento nuestro tipo CallbackType
int doMath(int num, CallbackType callback){
   return callback(num);          // call the callback fn passed
}

int main() {
   cout << "Math program -- the value of 5: " << endl;
   //Notese que nuestro tipo es un puntero a funcion, asi que le tenemos que pasar
   //la dirección de la función
   cout << " squared is : " << doMath(5, &squareCallback) << endl;
   cout << " cubed is   : " << doMath(5, &cubeCallback) << endl;
   return 0;
}
