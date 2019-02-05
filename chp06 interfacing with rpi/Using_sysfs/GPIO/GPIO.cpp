/*
Clase que permite gestionar la RPI utilizando el sysfs
Cada operación que deseemos hacer con la GPIO se implementara como una escritura
o lectura del filesystem, pero no de un filesystem cualquiera, del fs virtual.

En /sys/class/gpio tenemos el fs virtual de la GPIO. Los ejemoplos que siguen
asumen que estamos en este directorio.

echo 4 > export

Este comando habilita la salida 4 de la GPIO. Al hacer el export se crea un
directorio gpio4

cd gpio4

En el directorio habra varios archivos, cada uno governndo diferentes aspectos de
la entrada/salida, en este caso de la numero 4. Por ejemplo,

cat direction

Nos mostrara si la e/s es entrada o es salida

echo in > direction

Habilitara la gpio 4 como entrada

cat value

Nos retornara cual es el valor de la entrada 4

Esto nos da una idea de como controlar la GPIO con el sysfs. Pues bien, esta
clase empaqueta todas esas posibilidades
 */

#include "GPIO.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstdlib>
#include<cstdio>
#include<fcntl.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<pthread.h>
using namespace std;

namespace exploringRPi {

/**
 *
 * @param number The GPIO number for the RPi
 */
GPIO::GPIO(int number) {
	this->number = number;
	this->debounceTime = 0;
	this->togglePeriod=100;
	this->toggleNumber=-1; //infinite number
	this->callbackFunction = NULL;
	this->threadRunning = false;

	ostringstream s;
	s << "gpio" << number;
	this->name = string(s.str());
	this->path = GPIO_PATH + this->name + "/";
	this->exportGPIO();
	// need to give Linux time to set up the sysfs structure
	usleep(250000); // 250ms delay
}

int GPIO::write(string path, string filename, string value){
   ofstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
	   perror("GPIO: write failed to open file ");
	   return -1;
   }
   fs << value;
   fs.close();
   return 0;
}

//Generico. Lee de un archivo del sysfs
string GPIO::read(string path, string filename){
   ifstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
	   perror("GPIO: read failed to open file ");
    }
   string input;
   getline(fs,input);
   fs.close();
   return input;
}

//Generico. Escribe en un archivo del sysfs
int GPIO::write(string path, string filename, int value){
   stringstream s;
   s << value;
   return this->write(path,filename,s.str());
}

//Activa la e/s de la GPIO
int GPIO::exportGPIO(){
   return this->write(GPIO_PATH, "export", this->number);
}

//Desactiva la e/s de la GPIO
int GPIO::unexportGPIO(){
   return this->write(GPIO_PATH, "unexport", this->number);
}

//Especifica si es entrada o salida
int GPIO::setDirection(GPIO_DIRECTION dir){
   switch(dir){
   case INPUT: return this->write(this->path, "direction", "in");
      break;
   case OUTPUT:return this->write(this->path, "direction", "out");
      break;
   }
   return -1;
}

GPIO_DIRECTION GPIO::getDirection(){
	string input = this->read(this->path, "direction");
	if (input == "in") return INPUT;
	else return OUTPUT;
}

//Asigna un valor a la salida
int GPIO::setValue(GPIO_VALUE value){
   switch(value){
   case HIGH: return this->write(this->path, "value", "1");
      break;
   case LOW: return this->write(this->path, "value", "0");
      break;
   }
   return -1;
}

//Obtiene el valor medido en la e/s
GPIO_VALUE GPIO::getValue(){
	string input = this->read(this->path, "value");
	if (input == "0") return LOW;
	else return HIGH;
}

//Espcifica que tipo de cambio usaremos para disparar nuestros eventos/interrupts
//en esta e/s
int GPIO::setEdgeType(GPIO_EDGE value){
   switch(value){
   case NONE: return this->write(this->path, "edge", "none");
      break;
   case RISING: return this->write(this->path, "edge", "rising");
      break;
   case FALLING: return this->write(this->path, "edge", "falling");
      break;
   case BOTH: return this->write(this->path, "edge", "both");
      break;
   }
   return -1;
}

//Obtiene el tipo de cambio asociado a esta e/s
GPIO_EDGE GPIO::getEdgeType(){
	string input = this->read(this->path, "edge");
	if (input == "rising") return RISING;
	else if (input == "falling") return FALLING;
	else if (input == "both") return BOTH;
	else return NONE;
}

//Especifica si nuestra e/s tendra un pull-down resistor o no
//Cuando tenemos una resistencia pull-down, cuando la e/s no esta activada, el
//voltaje que tendremos sera 0V. Si tenemos un pull-up resistor, el voltaje
//cuando no esta activada la e/s serian 3.3v
int GPIO::setActiveLow(bool isLow){
   if(isLow) return this->write(this->path, "active_low", "1");
   else return this->write(this->path, "active_low", "0");
}

//Establece un pull-up en la e/s
int GPIO::setActiveHigh(){
   return this->setActiveLow(false);
}

//Ausiliares para manejar el stream
int GPIO::streamOpen(){
	stream.open((path + "value").c_str());
	return 0;
}
int GPIO::streamWrite(GPIO_VALUE value){
	stream << value << std::flush;
	return 0;
}
int GPIO::streamClose(){
	stream.close();
	return 0;
}

//Cambia de e->s y viceversa
int GPIO::toggleOutput(){
	this->setDirection(OUTPUT);
	if ((bool) this->getValue()) this->setValue(LOW);
	else this->setValue(HIGH);
    return 0;
}

int GPIO::toggleOutput(int time){ return this->toggleOutput(-1, time); }

//Interesante.
int GPIO::toggleOutput(int numberOfTimes, int time){
	//Hacemos que sea una salida
	this->setDirection(OUTPUT);
	this->toggleNumber = numberOfTimes;
	this->togglePeriod = time;
	this->threadRunning = true;
	//Crea un thread. Se pasa el puntero al thread que queremos que se cree,
	//y la funcion que se debe ejecutar en el thread. El ultimo argumento de la
	//funcion son los argumentos a pasar a la funcion que ejecuta el thread; En
	//este caso le pasamos this, el puntero a esta clase. Observar como hacemos
	//un cast a void*
  if(pthread_create(&this->thread, NULL, &threadedToggle, static_cast<void*>(this))){
  	perror("GPIO: Failed to create the toggle thread");
		//Si no se pudo crear el thread, el estado es false
		this->threadRunning = false;
  	return -1;
  }
  return 0;
}

// This thread function is a friend function of the class
//Notase como la funcion no pertenece a la clase, no es GPIO::
void* threadedToggle(void *value){
	//El argumento es un GPIO*, asi que hacemos un cast
	GPIO *gpio = static_cast<GPIO*>(value);
	//Obtenemos el valor
	bool isHigh = (bool) gpio->getValue(); //find current value
	//Mientras el estado sea running, hara eso una y otra vez:
	while(gpio->threadRunning){
		//Si la salida es activa, nos aseguramos de que lo siga siendo
		if (isHigh)	gpio->setValue(HIGH);
		else gpio->setValue(LOW);
		//Esperamos un rato
		usleep(gpio->togglePeriod * 500);
		//Cambiamos el signo de la salida
		isHigh=!isHigh;
		//Iteramos una vez mas
		if(gpio->toggleNumber>0) gpio->toggleNumber--;
		if(gpio->toggleNumber==0) gpio->threadRunning=false;
	}
	return 0;
}

// Blocking Poll - based on the epoll socket code in the epoll man page
int GPIO::waitForEdge(){
	this->setDirection(INPUT); // must be an input pin to poll its value
	int fd, i, epollfd, count=0;
	struct epoll_event ev;
	//Crea un epoll
	epollfd = epoll_create(1);
		//Comprueba si funciono
    if (epollfd == -1) {
	   perror("GPIO: Failed to create epollfd");
	   return -1;
    }
		//Abre el archivo que contiene el valor de la entrada, para solo lectura y no exclusivo
    if ((fd = open((this->path + "value").c_str(), O_RDONLY | O_NONBLOCK)) == -1) {
       perror("GPIO: Failed to open file");
       return -1;
    }

    //ev.events = read operation | edge triggered | urgent data
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
    ev.data.fd = fd;  // attach the file file descriptor

    //Register the file descriptor on the epoll instance, see: man epoll_ctl
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
       perror("GPIO: Failed to add control interface");
       return -1;
    }
	while(count<=1){  // ignore the first trigger
		i = epoll_wait(epollfd, &ev, 1, -1);
		if (i==-1){
			perror("GPIO: Poll Wait fail");
			count=5; // terminate loop
		}
		else {
			count++; // count the triggers up
		}
	}
    close(fd);
    if (count==5) return -1;
	return 0;
}

// This thread function is a friend function of the class
void* threadedPoll(void *value){
	GPIO *gpio = static_cast<GPIO*>(value);
	while(gpio->threadRunning){
		gpio->callbackFunction(gpio->waitForEdge());
		usleep(gpio->debounceTime * 1000);
	}
	return 0;
}

int GPIO::waitForEdge(CallbackType callback){
	this->threadRunning = true;
	this->callbackFunction = callback;
    // create the thread, pass the reference, address of the function and data
    if(pthread_create(&this->thread, NULL, &threadedPoll, static_cast<void*>(this))){
    	perror("GPIO: Failed to create the poll thread");
    	this->threadRunning = false;
    	return -1;
    }
    return 0;
}

//Destructor. Desactiva la e/s
GPIO::~GPIO() {
	this->unexportGPIO();
}

} /* namespace exploringRPi */
