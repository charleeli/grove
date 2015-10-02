#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
killall AdminModule
spawn-fcgi -a 127.0.0.1 -p 19002 -C 1 -f /usr/local/grove/fast-cgi/AdminModule

http://127.0.0.1/AdminModule.cgi?command=Echo

 *  sudo mv AdminModule /usr/local/grove/fast-cgi/
 *  cd /usr/local/grove/fast-cgi/
 *  sudo killall AdminModule
 *  sudo spawn-fcgi -a 127.0.0.1 -p 19002 -C 1 -f /usr/local/grove/fast-cgi/AdminModule 

