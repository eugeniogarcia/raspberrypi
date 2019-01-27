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

## Manaja Paquetes  
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

//Cambia el nivel de arranque al nivel 5  
init 5  


//Lista targets  
systemctl list-units -t service  

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
