#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/thalercoin.png
ICON_DST=../../src/qt/res/icons/thalercoin.ico
convert ${ICON_SRC} -resize 16x16 thalercoin-16.png
convert ${ICON_SRC} -resize 32x32 thalercoin-32.png
convert ${ICON_SRC} -resize 48x48 thalercoin-48.png
convert thalercoin-16.png thalercoin-32.png thalercoin-48.png ${ICON_DST}

