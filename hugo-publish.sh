#!/bin/bash

set -ex

HUGO=~/dev/hugo/hugo

rm -rf output
mkdir -p output

"$HUGO" \
  --source "$(pwd)/input/" \
  --destination "$(pwd)/output/"

find output -name .gitkeep -delete
