#!/bin/bash

# Try to get a bash shell from a running container.
# If unsuccessful, return a bash from current system.

# Handy to be used as shell replacement in IDEs

CONTAINER_NAME="mosaic_driver"
PROC="/bin/bash"

docker exec -it $CONTAINER_NAME $PROC || /bin/bash