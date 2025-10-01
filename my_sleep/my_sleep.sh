#!/bin/sh
if [ $# -ne 1 ]; then
    exit 1
fi

arg_seconds="$1"

if [ "$arg_seconds" -eq 0 ]; then
    exit 1
fi


SECONDS=0
while true; do
    if [ "$SECONDS" -ge "$arg_seconds" ]; then
        break
    fi
done

exit 0




SECONDS=0

# Boucle d'attente
while [ "$SECONDS" -lt "$arg_seconds" ]; do
    # Petite pause pour éviter de surcharger le CPU
    # Utilisation de commande built-in quand possible
    :
done

exit 0








if [ $# -ne 1 ]; then
    exit 1
fi

target_seconds="$1"

SECONDS=0

while true; do
    if [ "$SECONDS" -ge "$target_seconds" ]; then
        break
    fi
done
