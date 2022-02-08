#!/usr/bin/env bash

# WarehouseDemoEnabler.app
mkdir ./WarehouseDemoEnabler.app/Contents/libs
dylibbundler -of -b -x ./WarehouseDemoEnabler.app/Contents/MacOS/WarehouseDemoEnabler -d ./WarehouseDemoEnabler.app/Contents/libs/
