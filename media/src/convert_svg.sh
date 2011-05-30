#!/bin/bash

W=$(grep ^$1 sizes.txt | awk '{print $2}')
H=$(grep ^$1 sizes.txt | awk '{print $3}')

echo Converting $1 to $2 width $W, height $H
inkscape $1 --export-png=$2 -w$W -h$H
