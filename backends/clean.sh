#!/bin/bash

echo "Removing metabot.scad files"
for i in `find . -name "*.metabot.scad"`; do 
    rm $i;
done
