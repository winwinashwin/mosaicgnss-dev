#!/bin/bash

# Try to get a bash shell from a running container.
# If unsuccessful, return a bash from current system.

# Handy to be used as shell replacement in IDEs

CONTAINER_NAME="mosaic_driver"
PROC="/bin/bash"

CONTAINER_ID="$(docker ps -qf name=$CONTAINER_NAME)"

if [ ! $CONTAINER_ID ]; then
    echo "[-] Could not locate container $CONTAINER_NAME"
    echo "[-] Calling system bash instead"
else
    echo "[+] Running $PROC in container $CONTAINER_ID"
    docker exec -it $CONTAINER_NAME $PROC
fi

/bin/bash

exit 0
