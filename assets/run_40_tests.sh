#!/usr/bin/env bash

for i in $(seq 1 40); do
    python client.py "$i" &
done

wait
echo "All clients finished!"