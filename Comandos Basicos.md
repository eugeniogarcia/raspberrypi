# Varios  
### Version Linux  
//Version de Linux que ejecutamos:  
uname -a  

### Variables de entorno  
// Lista todas las variables  
env  

//Cambia la variable PATH  
export PATH=$PATH:/usr/home/eugenio  

//Muestra el valor de una variable de entorno  
echo $PATH  

### Editor  
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
