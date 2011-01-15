#!/bin/bash

export RT_SOURCE=${1}
export RT_DESTINATION=${2}
export RT_WIDTH=${3}
export RT_HEIGHT=${4}
export RT_SIZE=${RT_WIDTH}x${RT_HEIGHT}

if [ ! -n "${RT_SOURCE}" ] || [ ! -n "${RT_DESTINATION}" ] || [ ! -n "${RT_WIDTH}" ] || [ ! -n "${RT_HEIGHT}" ]
then
  echo "Usage: ${0} /path/to/source.img /path/to/dest.img width height"
  echo "Example: rgb ~/camera_images/img021.raw ~/Photos/Me-and-Jeff.jpg 720 480"
  exit 1
fi
