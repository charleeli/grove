#!/bin/bash
killall AdminModule
killall nginx
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
mkdir -p  /usr/local/grove/fast-cgi/html/
mkdir -p  /usr/local/grove/conf/
cp ../../../depends/bin/AdminModule /usr/local/grove/fast-cgi/
cp ../../conf/html/*.html /usr/local/grove/fast-cgi/html

cp ../../conf/nginx.conf /usr/local/nginx/conf/
/usr/local/nginx/sbin/nginx
../../../depends/bin/spawn-fcgi -a 127.0.0.1 -p 17002 -C 1 -f /usr/local/grove/fast-cgi/AdminModule

#http://127.0.0.1/AdminModule.cgi?command=Login

