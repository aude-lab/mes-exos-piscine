#!/bin/sh

if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory>" >&2
    exit 1
fi

if [ ! -d "$1" ]; then
    echo "Error: $1 is not a directory" >&2
    exit 2
fi

for file in "$1"/*.[a-zA-Z][a-zA-Z]; do
    if [ -f "$file" ]; then
        echo "$file"
    fi
done
