#!/bin/sh

stop()
{
    kill $PID1 
    return 0 
}

if [ ! -f "node_modules/.bin/serve" ]; then
    echo "Installing 'serve' locally..."
    npm install serve
fi

DIR1="dist"

PORT=3010

trap stop INT

SERVE="./node_modules/.bin/serve"

$SERVE -s $DIR1 -l $PORT -L &
PID1=$!
echo "Server started on http://localhost:$PORT1 (PID: $PID1)"

wait $PID1
