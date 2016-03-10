#!/bin/bash

if [ ! -e "$1" ]; then
    echo "Directory backends not existing, linking it...."
    ln -s "$2" "$1"
fi
