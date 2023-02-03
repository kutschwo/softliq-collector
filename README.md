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

Check if the collector is working (stop with Ctrl+C)
```shell
$ sofliq-collector -c sofliq-collector.json
  System time:13:19, Sensor1 temp:20.7C, Sensor2 temp:21.0C, Sensor3 temp:22.9C, Sensor4 temp:24.0C, Pump speed1:0%, Pump speed2:0%, Hours1:2302, Hours2:2425
  System time:13:19, Sensor1 temp:20.7C, Sensor2 temp:21.0C, Sensor3 temp:22.9C, Sensor4 temp:24.0C, Pump speed1:0%, Pump speed2:0%, Hours1:2302, Hours2:2425
  System time:13:19, Sensor1 temp:20.7C, Sensor2 temp:21.0C, Sensor3 temp:22.9C, Sensor4 temp:24.0C, Pump speed1:0%, Pump speed2:0%, Hours1:2302, Hours2:2425
```

Start the monitor-vbus service, remove the "--mqtt" parameter form the service file if no mqtt server is a available at localhost:1883
```shell
$ systemctl start monitor-vbus
```

Check wether the service is running properly
```shell
$ systemctl status monitor-vbus
  ● monitor-vbus.service - Monitor resol vbus temperatures
     Loaded: loaded (/srv/vbus/collector/monitor-vbus.service; linked; vendor preset: disabled)
     Active: active (running) since Mi 2015-09-02 13:29:23 CEST; 10min ago
   Main PID: 12422 (vbus-collector)
     CGroup: /system.slice/monitor-vbus.service
             └─12422 /srv/vbus/collector/vbus-collector --no-print --delay 60 --db /srv/vbus/collector/data.db /dev/tty_resol
```

Check that data is being written to the sqlite database
```shell
$ sqlite3 /srv/vbus/collector/data.db "SELECT * FROM data ORDER BY id DESC LIMIT 4;"
  174837|2015-09-02 11:28:10|10:24|18.8|20.9|22.6|22.9|0|0|2302|2425
  174836|2015-09-02 11:29:07|10:22|18.9|20.9|22.7|22.9|0|0|2302|2425
  174835|2015-09-02 11:30:05|10:21|18.8|20.9|22.6|22.9|0|0|2302|2425
  174834|2015-09-02 11:31:03|10:20|18.9|20.9|22.6|22.9|0|0|2302|2425
```
> Date/Time values in the sqlite database are stored in UTC.
> To get the correct local time ensure that the timezone on the system is set properly and use:
> ```shell
> $ sqlite3 /srv/vbus/collector/data.db "SELECT datetime(time, 'localtime'),* FROM data;"
> ```

## Options file

*vbus-collector* can be controlled using cli options for fast testing but the preferred way is to use an options file.

For production the following file could be used.
```json
{
    "device": "/dev/serial/by-id/usb-1fef_2018-if00",
    "interval": 60,
    "verbose": false,
    "database": "/srv/vbus/collector/data.db",
    "print_stdout": false,
    "mqtt": {
        "enabled": false,
        "base_topic": "heizung",
        "server": "tcp://localhost:1883",
        "client_id": "vbus",
        "user": null,
        "password": null
    },
    "homeassistant": {
        "enabled": false,
        "entity_id_base": "sensor.heating"
    }
}
```

Usage:
```shell
$ vbus-collector --config options.json
```

## Homeassistant integration

Sorry could not help a i have no Homeassistant server.
If you want to use this --> adapt source code of main.c and homeassistant.c


Have fun
