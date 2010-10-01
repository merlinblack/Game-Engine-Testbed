#! /bin/bash

convert()
{
    svg=$1
    width=$2
    height=$3
    png=$(basename $1 .svg).png
    echo Converting $svg to $png Width:$width Height:$height
    inkscape $svg --export-png=$png -w$width -h$height
}

convert icon.chk.green.svg 16 16 
convert icon.chk.red.svg 16 16 
convert icon.exit.svg 16 16 
convert icon.info.svg 16 16 
convert button.hover.svg 80 24
convert button.normal.svg 80 24
convert button.pressed.svg 80 24
