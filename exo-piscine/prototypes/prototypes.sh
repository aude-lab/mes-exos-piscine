#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <fichier.c>" >&2
    exit 1
fi

sed -n '
/^[[:space:]]*#[[:space:]]*include/d
/^[[:space:]]*struct[[:space:]]\+[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*{/,/^[[:space:]]*}[[:space:]]*;/d
/^[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]\+\**[[:space:]]*[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*(.*)[[:space:]]*;[[:space:]]*$/p
' "$1"
