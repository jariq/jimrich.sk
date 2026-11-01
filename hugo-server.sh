#!/bin/bash

set -ex

HUGO=~/dev/hugo/hugo

xdg-open http://localhost:1313

"$HUGO" server \
  --renderToMemory \
  --disableFastRender \
  --buildDrafts \
  --forceSyncStatic \
  --noHTTPCache \
  --source "$(pwd)/input/"