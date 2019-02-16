# Varios  
## Version Linux  
Version de Linux que estamos ejecutando:     
```
uname -a  
```

## Variables de entorno  
Lista todas las variables:  
```  
env  
```

Cambia la variable PATH  
```  
export PATH=$PATH:/usr/home/eugenio  
```  

Muestra el valor de una variable de entorno  
```
echo $PATH  
```

## Busca un comando  
```
whereis javac  
```  

## Actualiza la Raspberry Pi  
Configura:  
```  
raspi-config  
```
### Raspbian  
Instala la utilidad:  
```  
apt install rpi-update  
```  

Ejecuta la utilidad:  
```
rpi-update  
```

## Editor  
```
nano  
```  

## Pasa argumentos  
Pasamos una serie de argumentos a otro programa:   
```
echo " arg1 arg2 arg3"> arg.txt  
cat arg.txt|xargs touch  
```

# Usuarios y Grupos  
Quien soy yo:  
```  
whoami  
```  
## Grupos  
Lista grupos:  
```
groupmod (seguido de dos ALT+TAB lista todos los grupos)
```  

Crea grupo:  
```
groupadd  
```  
Añade un usuario a un grupo:    
```
adduser eugenio root
```
## Usuarios  
Crea usuario:    
```
adduser  
```  
Cambia password:  
```
passwd eugenio  
```  
Habilta el usuario root y cambia la contraseña:    
```
passwd root  
```

Lista datos de un usuario:    
```
id eugenio
```  
Modifica un usuario:    
```
usermod (seguido de dos ALT+TAB lista todos los usuarios)

usermod -a -G groupName userName

usermod -a -G root eugenio
```

## sudo  
Abre la sesion de superusuario y la mantiene:    
```
sudo -i  
```
Donde se indica que usuarios pueden utilzar sudo:  
```  
nano /etc/sudoers  
```

# Gestion de Paquetes  
Busca un paquete:  
```
apt-cache search notepad++  
```  
Muestra detalles de un programa:  
```
apt-cache show notepad++
```
## Maneja Paquetes  
Actualiza la lista de Paquetes:  
```  
apt update  
```  

Actualiza cualquier paquete que lo precise:  
```
apt upgrade  
```

Borra cualquier paquete que sea ya obsoleto:  
```
apt autoremove
```  

Instala y elimina un programa:    
```
apt install xxxxx  
apt remove xxxxx  
```  
Descarga un programa:  
```
apt-get download xxxxx  
```
# Servicios  
Version:    
```
systemctl --version  
```  
## Configuracion de arranque  
Raspbian utiliza como gestor de arranque el nuevo systemctl. Frete a initd este nuevo comando introduce la posibilidad de arrancar en paralelo diferentes servicios. Initd sigue estando disponible, para garantizar la compatibilidad. Por ejemplo, podemos ver en que nivel estamos con:  
```
who -r  
```
Arranca al nivel 5:  
```
init 5  
```  

Cual es el target por default:  
```
systemctl get-default  
```  
Muestra todos los targets disponibles:    
```
systemctl list-units -t target  
```  

Cambia el target:  
```
systemctl isolate graphical.target  
```  

## Operaciones  
Lista servicios:  
```
systemctl list-units -t service  
systemctl list-units -t service|grep rdp  
```  

Operaciones con Servicios:  
```
systemctl start xrdp  
systemctl stop xrdp  
systemctl restart xrdp
```  
Configurar que hace el servicio al arrancar el sistema:    
```
systemctl enable xrdp
systemctl disable xrdp  
```
Que copnfiguracion de arranque tiene un servicio:  
```
systemctl is-enabled xrdp
```  
# Gestion de Archivos  

## Filesystem  
### Montaje  
Lista los dispositivos de bloques disponibles en el sistema:  
```
lsblk  
```  
Directorio donde configuramos las unidades que se montaran en el arranque:  
```
/etc/fstab  
```  
Monta todas las unidades definidas en /etc/fstab:  
```
mount -a  
```  
### Montar en una unidad fija  
Si en lugar de que la unidad que se monte en cada directorio no sea predecible, podemos definir en el /etc/fstab una referencia al UUID del dispositivo. Lo primero sera saber cual es el UUID del despositivo:  
```
blkid /dev/sda1  
```  
Podemos usar el UUID para definir la entrada en el /etc/fstab.  

### Formatea una dispositivo  
Crea - formatea - un filesystem en un device:  
```
mkfs /dev/sda1    
mkfs.ext4 /dev/sda1  
```  

Monta un device en un directorio:  
```
mount -t ext4 /dev/sda1 /home/eugenio/midir  
```  

### Unidad temporal
Podemos crear una unidad que se monte en memoria, esto es, en la que el dispositivo sera en memoria. Esto puede ser util para montar unidades en las que se guarden logs. De esta forma evitamos que los logs se guarden tarjetas de memoria - lo que reduciria su tiempo de vida, que esta fijado por el numero de escrituras que se pueden hacer en una tarjeta; El unico inconveniente que esto tiene es que el almacenamiento es efimero, cuando se apague el dispositivo, los datos aqui guardados se pierden.  

```
tempfs  
```  
## Gestion  
 Espacio libre:  
```
df -h  
du -h /home/eugenio  
```  
## Vinculos  
Crea un archivo:  
```
touch miarch.txt  
```  
**Crea un link simbolico/soft**. Creara una nueva entrada en el listado de inodes. Todos los links apunta al mismo archivo fisico. Si borramos el archivo, todos los links se quedaran huerfanos:  
```  
ln -s miarch.txt s1    
```  

**Crea un link duro/hard**. No se crea un nuevo inode, y si nos fijamos en el archivo, veremos que el contador pasa de 1 a 2. El contador nos indica el numero total de elementos que apuntan al mismo archivo (en este caso dos, el propio archivo mas el hard link que hemos creado):  
```
ln miarch.txt s2  
```
Muestra los i-nodes de cada archivo:  
```
ls -ali  
```
## Permisos
Diversas formas de cambiar los permisos asociados a un archivo:    
```
chmod 664 arch.txt  
chmod u=rw,g=rw,o=r arch.txt  
chmod g+w arch.txt  
```  
### Mascara
La mascara nos define que permisos se asignaran por defecto cuando se cree un archivo o directorio:  
```
umask 022  
```  

## Cambia grupos y usuarios  
Cambio de dueño:  
```
chown eugenio arch.txt  
```  
Cambio de grupo:  
```
chgrp migrupo arch.txt  
```  
# Sys (directorio virtual)
El directorio /sys es un directorio virtual, en el que el SSOO mapea informacion sobre los distintos dispositivos que controla. Por ejemplo, en el caso de la raspberry Pi, los LEDs se pueden controlar en:    
```
/sys/class/leds  
```  
Itereactua con el dispositivo por medio del sistema virtual:    
```
sh -c "echo none > trigger"  
```  
# Logs  
```
journalctl --help
```  

Mantiene solo logs por los ultimos dos dias:  
```
journalctl --vacuum-time=2d
```
Retiene solo los ultimos 20MB:  
```
journalctl --vacuum-size=20M
```  
Varios:  
```
journalctl --disk-usage
journalctl --verify
journalctl --rotate
```
Limpia los logs:  
```
journalctl --rotate
journalctl --vacuum-time=1s
```  
Lista logs para un determinado intervalo:  
```
journalctl --since "2019-01-30 14:00:00"
journalctl --since today
```
# Networking  
Get networking information:  
```
ifconfig  
```  
We can see all the network adapters. the ones starting as w* are wifi adapters. The lan* are ethernet adapters. To get the information about the wifi configuration:  
```
wiconfig  
```  
This will give us information about our router. This will come handy if we want to configure manually our network adapter:    
```
route -n  
```  
## Adaptador de red  
Lista la informacion de los adaptadores de red encontrados en el sistema:    
```
rfkill list
```  
Podremos ver el estado de cada adaptador. Si alguno estuviera deshabilitado (bloqueado), lo podriamos habilitar como sigue:    
```
rfkill unblock wifi
```  
Lista redes wifi:  
```
iwconfig

iwlist wlp2s0 scan | grep ESSID
```  
Tambien podemos usar el comando ``iw`` para hacer esto mismo - es equivalente:  
Lista los adaptadores disponibles:  
```
iw dev
```  
Y la información concreta de un determinado adaptador:  
```
iw link wlp2s0
```  
Levantar la interfaz:  
```
ip link set wlp2s0 up
```
Podemos tambien buscar las redes wifi disponibles:  
```
iw wlp2s0 scan|grep SSID:
```  
### Network Management Client (this one works!!!)  
Este cliente nos permite hacer las operaciones antes descritas de otra forma. Para empezar, podemos ver la lista de conexiones guardadas:  
```
nmcli c  
```
Podemos escanear las redes:  
```
nmcli d wifi list
```

Parar y arrancar la wifi:  
```
nmcli r wifi off  

nmcli r wifi on  
```  
Si nos quisieramos conectar a una red:  
```
nmcli dev wifi connect MASMOVIL_xVfu password jzkDP26XhjU3
```
Comprueba la wifi "en verde", la conectada:  
```
nmcli con
```
Desconecta una wifi:  
```
nmcli con down MASMOVIL_xVfu
```  
### Conectarse a una red (wpa suplicant)  
Primero instalamos la utilidad:  
```
sudo apt install wpasupplicant
```  
Esto crea un archivo llamado ``/etc/wpa_supplicant.conf``.  

Para crear la conexion:  
```
wpa_passphrase your-ESSID your-passphrase | sudo tee /etc/wpa_supplicant.conf  

wpa_passphrase MASMOVIL_xVfu jzkDP26XhjU3 | tee /etc/wpa_supplicant.conf
```
Y finalmente:  
```
wpa_supplicant -c /etc/wpa_supplicant.conf -i wlp2s0  
```  
To run it in the background instead:  
```
wpa_supplicant -B -c /etc/wpa_supplicant.conf -i wlp2s0
```  
Finally, once it is installed (we can check it with ``ifconfig``):  
```
dhclient wlp2s0
```  

## Configurar una conexion de red de forma estatica  
Ver la configuración del DNS:  
```
cat /etc/resolv.conf  
```

El archivo /etc/dhcpcd.conf contiene la información de los interfaces de red:  
```
nano /etc/dhcpcd.conf  
```  
Para configurar los interfaces de forma estatica, añadir al final del archivo lo siguiente:
```
interface eth0  
static ip_address=192.168.1.140  
static routers=192.168.1.1  
static domain_name_servers=212.231.6.7 8.8.8.8 192.168.1.1 fe80::1%wlan0  

interface wlan0  
static ip_address=192.168.1.141  
static routers=192.168.1.1  
static domain_name_servers=212.231.6.7 8.8.8.8 192.168.1.1 fe80::1%wlan0  
```  
En domain_name_servers hemos listado las direcciones de los dns, tal y como las recuperamos de cat /etc/resolv.conf. En ip_address colocamos la direccion estatica que queremos usar. En routers la direccion de nuestro gateway router.  

Cuando se hacen cambios hay que hacer un ``reboot``.  
