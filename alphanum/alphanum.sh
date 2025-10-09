#!/bin/sh

while IFS= read -r line; do
    trimmed=$(echo "$line")


    if [ -z "$line" ]; then
        echo "it is empty"
        continue
    fi

    if echo "$line" | grep -q '^[ \t]*$'; then
        echo "it is empty"
        continue
    fi
    
    if [ -z "$trimmed" ]; then
        echo "it is empty"
        continue
    fi
    if echo "$trimmed" | grep -q '^[a-zA-Z]*$'; then
        echo "it is a word"
    elif echo "$trimmed" | grep -q '^[0-9]$'; then
        echo "it is a digit"
    elif echo "$trimmed" | grep -q '^[0-9][0-9]*$'; then
        echo "it is a number"
    elif echo "$trimmed" | grep -q '^[a-zA-Z0-9]*$'; then
        echo "it is an alphanum"
    else
        echo "it is too complicated"
        exit 0
    fi
done
