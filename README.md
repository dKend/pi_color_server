# pi_color_server
pi_color_server (pcs) is a project I have been working on for a year or so intended to improve my C programming skills and to be a way to control a RGB color strip attached to a Raspberry Pi Model B using the pigpio C library.
# Requirements
 - Raspberry Pi Model B or newer.
 - pigpio library
   - Installation instructions here: http://abyz.me.uk/rpi/pigpio/download.html
   - pigpio github page: https://github.com/joan2937/pigpio
 - In order for the server to work, the pigpiod daemon must be running.
 - LED strip setup: https://dordnung.de/raspberrypi-ledstrip/#%2F
# Basic Usage
 - Run "pcs-server -d" to start the server daemon
 - Run "pcs-client -setcolor \<RED VALUE\> \<GREEN VALUE\> \<BLUE VALUE\>" to set the color
 - Run "pcs-client -halt" to stop the daemon (the current color is saved and loaded next time the server is run).
