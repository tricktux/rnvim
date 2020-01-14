#!/bin/bash


while :
do
	head /dev/urandom | tr -dc A-Za-z0-9 | head -c 100
	echo ''
	sleep 0.1
done
