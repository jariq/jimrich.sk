#!/bin/bash

set -ex

if [ -z "$1" ]; then
  echo "Please provide the post name as an argument."
  exit 1
fi

ARG1="$1"
HUGO=~/dev/hugo/hugo

"$HUGO" new "${ARG1}.md" \
  --source "$(pwd)/input/"

mkdir -p "$(pwd)/input/static/${ARG1}"
touch "$(pwd)/input/static/${ARG1}/.gitkeep"
