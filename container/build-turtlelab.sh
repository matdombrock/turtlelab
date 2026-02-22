#!/bin/bash
set -e

IMAGE_NAME="turtlelab"
CONTAINER_NAME="turtlelab_run"

# Run the built binary in the container
podman run --name $CONTAINER_NAME --rm -v "$(pwd):/workspace" $IMAGE_NAME ./build.sh
