#!/bin/sh

if [ $# -ne 1 ]; then
    exit 1
fi

directory="$1"

if [ ! -d "$directory" ]; then
    exit 1
fi

for file in "$directory"/*; do
    if [ -f "$file" ]; then
        file_type=$(file "$file")
        if echo "$file_type" | grep -q "ASCII"; then
            echo "$file_type"
        fi
    fi
done
