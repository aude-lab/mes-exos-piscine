#!/bin/sh
if [ $# -ne 1 ]; then
    exit 1
fi

seconds="$1"

if [ "$seconds" -lt 0 ]; then
    exit 1
fi

if [ "$seconds" -eq 0 ]; then
    exit 0
fi

SECONDES=0
while true; do
    if [ "$SECONDES" -ge "$seconds" ]; then
        break
    fi
done

exit 0
