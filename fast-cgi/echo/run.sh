#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
killall EchoModule
spawn-fcgi -a 127.0.0.1 -p 19001 -C 1 -f /usr/local/grove/fast-cgi/EchoModule

http://119.29.81.103/EchoModule.cgi
{
  "uid" : "32faa0581014ec2e16815c9d5be40bdb",
  "command" : "Echo",
  "plaintext" : "{\"foo\":\"ff\"}"
} 
