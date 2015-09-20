#!/bin/bash
killall echo.fcgi
spawn-fcgi -a 127.0.0.1 -p 19008 -C 1 -f /usr/local/grove/fast-cgi/echo.fcgi 
