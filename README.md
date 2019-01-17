# MQTT_PRJ_PROBE
Probe Application that read cpu temperature, cpu usage and free disk space on root.
These values are published using MQttClient with a initial sampling time of 1 second.
The application subscribe a command topic to change sampling interval.

This is part of a example test, MQTT_PRJ_DATADISPLAY is the application that interact with the probe.



### Note:
It works on linus system, with Qt5.12.
Mqtt library must be installed.

## Usage
Install a MQTT broker (e.g. mosquitto)

