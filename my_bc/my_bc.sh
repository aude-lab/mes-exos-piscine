#!/bin/sh

if [ $# -ge 1 ]; then
    echo "$(($@))"
else
    while read line; do
        if [ -n "$line" ]; then
            echo "$(($line))"
        fi
    done
fi
