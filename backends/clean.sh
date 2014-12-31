#!/bin/bash

for i in `find . -name "*.metabot.scad"`; do 
    rm $i;
done
