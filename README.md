# sofliq-collector
Data collector for Grünbeck decalcination devices with WLAN and the http: POST interface.
https://www.gruenbeck.de
This models are build about between 2014 and 2018/19
Before 2014 ther were no WLAN interface.
Starting 2018/19 Grünbeck changed to MQTT with direct contact to the Grünbeck cloud server.

Daten Kollektor für Grünbeck Enkalkungsanlagen mit WLAN und dem http: POST Interface.
https://www.gruenbeck.de
Diese Modelle wurden ca. 2014 bis 2018/19 gebaut.
Vor 2014 hatten die Anlagen kein WLAN.
2018/19 stellte Fa. Grünbeck auf MQTT um. Die Anlagen verbinden sich mit dem Grünbeck Cloud Server.

## Features
* Send a http: POST request to the device an read the answer.
* Parse the data.
* Save data to sqlite database
* Send data to mqtt brocker (for integration in other home automation software)
* Direct HTTP integration to Homeassistant, not implemented as i have no Homeassistant server.



## HowTo Build
The RaspberryPi or other linux machine should be running and connected to the internet, ssh sould be available.

* Get root via `sudo -s`, `su` or other ways :smile:

Get the necessary packages (Debian / raspbian)
```shell
$ apt-get update
$ apt-get install git build-essential  libsqlite3-dev sqlite libcjson-dev libcurlpp-dev libpaho-mqtt-dev
```

Other distros: Sorry thats your turn.

Download the source code
```shell
$ mkdir -p ~/gruenbeck
$ cd ~/gruenbeck
$ git clone https://github.com/kutschwo/softliq-collector.git 
```

Sorry actual noch ,configure script or similar.
Please open Makefile, read it and change it.
According to Linux file system hirachy standard the programm will be installed to /usr/local/.....

Compile the data collector service and the included libraries
```shell
$ cd ~/gruenbeck/softliq-collector

$ make

$ sudo make install

```


## Setting up system files

Edit sofliq-collector.json
The hardwar of the Grünbeck devices are very slow.
Grünbeck define that between two requests must be more than 30 seconds.
If you request more often the webserver of the device will hung.

```shell
$ sudo nano /usr/local/etc/sofliq-collector.json
```



## Options file

*vbus-collector* can be controlled using cli options for fast testing but the preferred way is to use an options file.

For production the following file could be used.
```json
{
    "testmode" : true,
    "softlic_url": "http://192.168.0.1/mux_http/",
    "post_string" : "id=62&show=D_A_1_1|D_A_1_2|D_A_2_2|D_A_3_1|D_A_3_2|D_Y_1|D_A_1_3|D_A_2_3|D_Y_5|D_B_1|D_Y_4_1|D_Y_4_2|D_Y_4_3|D_Y_4_4|D_Y_4_5|D_Y_4_6|D_Y_4_7|D_Y_4_8|D_Y_4_9|D_Y_4_10|D_Y_2_1|D_Y_2_2|D_Y_2_3|D_Y_2_4|D_Y_2_5|D_Y_2_6|D_Y_2_7|D_Y_2_8|D_Y_2_9|D_Y_2_10~",
    "loopforever": false,
    "delay": 60,
    "verbose": false,
    "withSql": true,
    "database": "./softliq.db",
    "print_result": true,
    "print_stdout": true,
    "mqtt": {
        "enabled": true,
        "base_topic": "haus/softliq/sensor",
        "server": "tcp://server.dom:1883",
        "client_id": "client",
        "user": "user1234",
        "password": "pw12345"
    },
    "homeassistant": {
        "enabled": false,
        "entity_id_base": "sensor.softliq"
    }
}

```

Usage:
```shell
$ sofliq-collector -c sofliq-collector.json
```

## Homeassistant integration

Sorry could not help a i have no Homeassistant server.
If you want to use this --> adapt source code of main.c and homeassistant.c


Have fun
