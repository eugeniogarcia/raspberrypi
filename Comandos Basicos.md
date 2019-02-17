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
Cual es mi dirección:  
```
hostname -I
```
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

ip link
```  
Levantar la interfaz:  
```
ip link set wlp2s0 up
```
Podemos tambien buscar las redes wifi disponibles:  
```
iw wlp2s0 scan|grep SSID:
```  
### Conectar  
Sin encriptar:  
```
iw dev wlan0 connect your_essid
```
Con WEP. La contraseña va en hexadecimal o en ASCII (el formato se identifica automáticamente porque la clave tiene un tamaño fijo):    
```
iw dev wlan0 connect your_essid key 0:your_key
```
Podemos especificar un set de contraseñas. Aqui estariamos informando la tercera contraseña del set (se empieza a contar desde 0):  
```
iw dev wlan0 connect your_essid key d:2:your_key
```
Para WAP no podemos usar iw. Tenemos que utilizar wpa_supplicant.  

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
Los comandos anteriores actualizan el archivo de configuración /etc/wpa_supplicant.conf. Esto mismo es lo que haría en respberrypi la opcion netowrking del raspi-config, cuando seleccionamos wifi e ingresamos un SSID y la contraseña. Por defecto, eso si, el raspi-config guarda la configuración en /etc/wpa_supplicant/wpa_supplicant.conf; Por este motivo en muchos tutoriales, cuando vemos la configuración de /etc/networking/interfaces se hace referencia a ese directiorio de wpa_supplicant.  

Finalmente configuramos la conexión wifi con wpa_supplicant:  
```
wpa_supplicant -c /etc/wpa_supplicant.conf -i wlp2s0  
```  
To run it in the background instead:  
```
wpa_supplicant -B -c /etc/wpa_supplicant.conf -i wlp2s0
```
Al comando de wpa_supplicant le podemos especificar el Driver con la opción -D. Por ejemplo:  
```
wpa_supplicant -B -c /etc/wpa_supplicant/mywired.conf -i eth0 -D wired
wpa_supplicant -B -c /etc/wpa_supplicant/mywireless.conf -i wlan0 -D wext
```
Los posibles valores que el driver acepta son:  
```
wext = Linux wireless extensions (generic)
nl80211 = Linux nl80211/cfg80211
wired = Wired Ethernet driver
none = no driver (RADIUS server/WPS ER)
```  
Finally, once it is installed (we can check it with ``ifconfig``), we can ask the dhcp client to issue an ip:  
```
dhclient wlp2s0
```  
### Hacer que la configuración se aplique al arrancar    
Si queremos que al arrancar se establezca la configuración por defecto
```
nano /etc/network/interfaces
```
Insertamos en el archivo la configuración. En este ejemplo estamos configurando eth0 con una ip estática, la wifi con dhcp y wpa (que hemos definido con el wpa_supplicant).  

#### Opcion 1. Usando solo el /etc/network/interfaces  
En esta opción no vamos a utilizar el /etc/dhcpcd.conf para especificar la dirección estática:  

```
# interfaces(5) file used by ifup(8) and ifdown(8)

# Please note that this file is written to be used with dhcpcd
# For static IP, consult /etc/dhcpcd.conf and 'man dhcpcd.conf'

#INICIO_CAMBIO_EUGENIO
auto lo
iface lo inet loopback

auto wlan0
allow-hotplug wlan0
iface wlan0 inet dhcp
wpa-driver wext
wpa-conf /etc/wpa_supplicant.conf

auto eth0
allow-hotplug eth0
iface eth0 inet static
address 192.168.1.120
netmask 255.255.255.0
network 192.168.1.0
broadcst 192.168.1.255
gateway 192.168.1.1
dns-nameservers 8.8.8.8  8.8.4.4

#FIN_CAMBIO_EUGENIO

# Include files from /etc/network/interfaces.d:
source-directory /etc/network/interfaces.d
```
#### Opcion 2. Usando /etc/network/interfaces y /etc/dhcpcd.conf     
En esta opción definimos la dirección estática en /etc/dhcpcd.conf (tambien podriamos definir en este archivo un comportamiento hibrido. Hacer que se asigne la dirección con dhcpcd, pero en caso de que falle, que se haga de forma estátical. En esto consiste el fallback):  

```
# interfaces(5) file used by ifup(8) and ifdown(8)

# Please note that this file is written to be used with dhcpcd
# For static IP, consult /etc/dhcpcd.conf and 'man dhcpcd.conf'

#INICIO_CAMBIO_EUGENIO
auto lo
iface lo inet loopback

auto wlan0
allow-hotplug wlan0
iface wlan0 inet manual
wpa-driver wext
wpa-conf /etc/wpa_supplicant.conf

auto eth0
iface eth0 inet manual


#FIN_CAMBIO_EUGENIO

# Include files from /etc/network/interfaces.d:
source-directory /etc/network/interfaces.d
```
Notese como en este caso configuramos como ``manual`` la configuración de eth0 y de wlan0. Esto lo que significa es que no especificamos los datos del adaptador, ip, etc. aqui, pero en otro lugar (el dhcpcd.conf). En el dhcpcd.conf especificaremos una ruta estática para el eth0, y dinámica - el valor por defecto - para el wlan0. Veamos el archivo de configuración de dhcpcd:    
```
nano /etc/dhcpcd.conf  
```
El contenido del archivo es:  
```
# A sample configuration for dhcpcd.
# See dhcpcd.conf(5) for details.

# Allow users of this group to interact with dhcpcd via the control socket.
#controlgroup wheel

# Inform the DHCP server of our hostname for DDNS.
hostname

# Use the hardware address of the interface for the Client ID.
clientid
# or
# Use the same DUID + IAID as set in DHCPv6 for DHCPv4 ClientID as per RFC4361.
# Some non-RFC compliant DHCP servers do not reply with this set.
# In this case, comment out duid and enable clientid above.
#duid

# Persist interface configuration when dhcpcd exits.
persistent

# Rapid commit support.
# Safe to enable by default because it requires the equivalent option set
# on the server to actually work.
option rapid_commit

# A list of options to request from the DHCP server.
option domain_name_servers, domain_name, domain_search, host_name
option classless_static_routes
# Most distributions have NTP support.
option ntp_servers
# Respect the network MTU. This is applied to DHCP routes.
option interface_mtu

# A ServerID is required by RFC2131.
require dhcp_server_identifier

# Generate Stable Private IPv6 Addresses instead of hardware based ones
slaac private

#INICIO_CAMBIO_EUGENIO
# Example static IP configuration:
interface eth0
static ip_address=192.168.1.120/24
static routers=192.168.1.1
static domain_name_servers=192.168.1.1 8.8.8.8 8.8.4.4 212.231.6.7
#FIN_CAMBIO_EUGENIO

# It is possible to fall back to a static IP if DHCP fails:
# define static profile
#profile static_eth0
#static ip_address=192.168.1.23/24
#static routers=192.168.1.1
#static domain_name_servers=192.168.1.1

# fallback to static profile on eth0
#interface eth0
#fallback static_eth0
```  

Ver la configuración del DNS:  
```
cat /etc/resolv.conf  
```  
#### Nota
Podemos repetir el ciclo de configuración manualmente lanzando estos dos escripts:  
```
ifdown wlan0
ifup wlan0
```
Estos escripts se lanzan en el proceso de arranque. Si cambiamos la configuración, podemos desear ejecutar los scripts para ver que la configuración hace lo que debe hacer.  
### Network Management Client
Este cliente esta disponible en Ubuntu por defecto, pero debe configurarse en la raspberrypi:    
```
apt-get install network-manager
```  
Esta utilidad nos permite hacer las operaciones antes descritas de otra forma. Para empezar, podemos ver la lista de conexiones guardadas:  
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

# Curl  
Cuando invocamos una url protegida con https, podemos evitar la validación del certificado con la opción -k

```
curl -k https://kubernetes
```
No obstante, lo mas correcto es validar el certificado. Podemos indicar a curl donde encontrar el certificado de la CA:  
```
curl --cacert /var/run/secrets/kubernetes.io/serviceaccount/ca.crt https://kubernetes
```
Para evitar tener que pasar el parametro -cacert en cada ocasión, podemos crear una variable de entorno:  
```
export CURL_CA_BUNDLE=/var/run/secrets/kubernetes.io/serviceaccount/ca.crt

curl https://kubernetes
```
Usando curl con Autenticación:  
```
TOKEN=$(cat cat /var/run/secrets/kubernetes.io/serviceaccount/token)

curl -H "Authorization: Bearer $TOKEN" https://kubernetes
```
