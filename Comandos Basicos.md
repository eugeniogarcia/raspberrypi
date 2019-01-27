# Varios  
## Version Linux  
//Version de Linux que ejecutamos:  
uname -a  

## Variables de entorno  
// Lista todas las variables  
env  

//Cambia la variable PATH  
export PATH=$PATH:/usr/home/eugenio  

//Muestra el valor de una variable de entorno  
echo $PATH  

## Busca un comando  
whereis javac  

## Actualiza la Raspberry Pi  

//Configura  
raspi-config  

### Raspbian  
//Instala la utilidad  
apt install rpi-update  

//Ejecuta la utilidad  
rpi-update  

## Editor  
nano  

# Usuarios y Grupos  
//Quien soy yo  
whoami  

## Crea usuarios y grupos  
//Crea usuario  
adduser  

//Crea grupo  
groupadd  

## Password
//Cambia password  
passwd eugenio  

### Habilita el root    
//Habilta el usuario root y cambia la contraseña  
passwd root  

## sudo  
//Abre la sesion de superusuario y la mantiene  
sudo -i  

//Donde se indica que usuarios pueden utilziar sudo  
nano /etc/sudoers  

# Gestion de Paquetes  
## Busca Paquetes  

//Busca un paquete  
apt-cache search notepad++  

//Muestra detalles de un programa  
apt-cache show notepad++

## Maneja Paquetes  
// Actualiza la lista de Paquetes  
apt update  

// Actualiza cualquier paquete que lo precise  
apt upgrade  

//Borra cualquier paquete que sea ya obsoleto  
apt autoremove

// Instala y elimina un programa    
apt install xxxxx  
apt remove xxxxx  

//Descarga un programa  
apt-get download xxxxx  

# Servicios  
// Version  
systemctl --version  

## Configuracion de arranque  
Raspbian utiliza como gestor de arranque el nuevo systemctl. Frete a initd este nuevo comando introduce la posibilidad de arrancar en paralelo diferentes servicios. Initd sigue estando disponible, para garantizar la compatibilidad. Por ejemplo, podemos ver en que nivel estamos con:  

who -r  

//Arranca al nivel 5
init 5  

//Cual es el target por default  
systemctl get-default  

//Muestra todos los targets disponibles  
systemctl list-units -t target  

//Cambia el target  
systemctl isolate graphical.target  

## Operaciones  
//Lista servicios  
systemctl list-units -t service  
systemctl list-units -t service|grep rdp  

//Operaciones con Servicios  
systemctl start xrdp  
systemctl stop xrdp  
systemctl restart xrdp

//Configurar que hace el servicio al arrancar el sistema    
systemctl enable xrdp
systemctl disable xrdp  

//Que copnfiguracion de arranque tiene un servicio  
systemctl is-enabled xrdp

# Gestion de Archivos  
## Vinculos  
// Crea un archivo  
touch miarch.txt  

//Crea un link simbolico/soft  
//Creara una nueva entrada en el listado de inodes. Todos los links apunta al mismo //archivo fisico. Si borramos el archivo, todos los links se quedaran huerfanos  
ln -s miarch.txt s1    

//Crea un link duro/hard.
//No se crea un nuevo inode, y si nos fijamos en el archivo, veremos que el contador pasa //de 1 a 2. El contador nos indica el numero total de elementos que apuntan al mismo //archivo (en este caso dos, el propio archivo mas el hard link que hemos creado)  
ln miarch.txt s2  

//Muestra los i-nodes de cada archivo  
ls -ali  

## Permisos
//Diversas formas de cambiar los permisos asociados a un archivo    
chmod 664 arch.txt  
chmod u=rw,g=rw,o=r arch.txt  
chmod g+w arch.txt  

### Mascara
La mascara nos define que permisos se asignaran por defecto cuando se cree un archivo o directorio:  
umask 022  

## Cambia grupos y usuarios  
//Cambio de dueño  
chown eugenio arch.txt  

//Cambio de grupo  
chgrp migrupo arch.txt  
