#!/bin/bash

draw_instr () {
    y=20
    ../build/test/palettes | while read NAME; do
        echo "text 15,$y \"$NAME\";"
        y=$(($y+25))
    done
}

convert -font helvetica -fill white -strokewidth 4 -stroke white -pointsize 20 -draw "$(draw_instr)" palettes.ppm palettes_labelled.png
mogrify -font helvetica -fill black -pointsize 20 -draw "$(draw_instr)" palettes_labelled.png
