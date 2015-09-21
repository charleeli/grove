#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
killall EchoModule
spawn-fcgi -a 127.0.0.1 -p 19008 -C 1 -f /usr/local/grove/fast-cgi/EchoModule 
