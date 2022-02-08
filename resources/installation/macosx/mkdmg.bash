#!/usr/bin/env bash

export DYLD_LIBRARY_PATH="/usr/local/pgsql/lib:/usr/local/lib"

# Warehouse.app
echo "Create Warehouse.app"
rm -Rf Warehouse.app
cp -a ../../../src/modules/warehouse/Warehouse.app .
mkdir ./Warehouse.app/Contents/libs
dylibbundler -od -b -x ./Warehouse.app/Contents/MacOS/Warehouse -d ./Warehouse.app/Contents/libs/
install_name_tool -change build/libds.so  @executable_path/../libs/libds.so ./Warehouse.app/Contents/MacOS/Warehouse
cd ./Warehouse.app/Contents
ln -s libs lib
cd ../../

# WarehouseAdmin.app
echo "Create WarehouseAdmin.app"
rm -Rf WarehouseAdmin.app
cp -a ../../../src/modules/admin/WarehouseAdmin.app .
mkdir ./WarehouseAdmin.app/Contents/libs
dylibbundler -od -b -x ./WarehouseAdmin.app/Contents/MacOS/WarehouseAdmin -d ./WarehouseAdmin.app/Contents/libs/
install_name_tool -change build/libds.so  @executable_path/../libs/libds.so ./WarehouseAdmin.app/Contents/MacOS/WarehouseAdmin
cd ./WarehouseAdmin.app/Contents
ln -s libs lib
cd ../../

# LicenceGenerator.app
echo "Create LicenceGenerator.app"
rm -Rf LicenceGenerator.app
cp -a ../../../../DSLicenceGenerator/src/modules/licencegenerator/LicenceGenerator.app .
dylibbundler -od -b -x ./LicenceGenerator.app/Contents/MacOS/LicenceGenerator -d ./LicenceGenerator.app/Contents/libs/
cd ./LicenceGenerator.app/Contents
ln -s libs lib
cd ../../
