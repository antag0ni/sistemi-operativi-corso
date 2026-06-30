#!/bin/bash
if (($# != 1)); then
  echo "Only one argument is accepted!"
  exit 1
fi

if [[ ! -d "recycle" ]]; then
  mkdir recycle
else
  echo "Warning: The recycling directory already exists!"
fi

cp $1 recycle/
rm $1
