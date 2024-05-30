#!/bin/bash

# Start three clients in parallel
for i in 1 2 3; do
    expect client_script.expect $i "안녕하세요 $i" &
done

# Wait for all clients to finish
wait