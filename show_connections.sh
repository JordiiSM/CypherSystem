#!/bin/bash
for PORT in $(eval echo {$1..$2}); do
  	timeout 0.2 bash -c "</dev/tcp/127.0.0.1/$PORT &>/dev/null" &&  echo "port $PORT is open"
done




