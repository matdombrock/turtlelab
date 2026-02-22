#!/bin/bash
set -e

IMAGE_NAME="turtlelab"
CONTAINER_NAME="turtlelab_build"

# Build the container image
podman build -t $IMAGE_NAME ./container

# Remove any existing container
podman rm -f $CONTAINER_NAME || true

# Run the build in the container
podman run --name $CONTAINER_NAME --rm -v "$(pwd)/..:/workspace" $IMAGE_NAME
