#!/bin/bash

MYPORT=8080; 
kill -9 `ps -ef |grep SimpleHTTPServer |grep $MYPORT |awk '{print $2}'`
python -m SimpleHTTPServer $MYPORT