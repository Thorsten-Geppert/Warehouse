#!/usr/bin/env bash

WarehouseApp="Warehouse.app"
AdminApp="WarehouseAdmin.app"
LicenceGeneratorApp="LicenceGenerator.app"


# Warehouse.app
echo "Create Warehouse.app"
rm -Rf Warehouse.app
cp -a ../../../src/modules/warehouse/Warehouse.app .
mkdir ./Warehouse.app/Contents/libs
install_name_tool -change build/libds.so /usr/local/lib/libds.so ./Warehouse.app/Contents/MacOS/Warehouse
install_name_tool -change libpq.5.dylib /usr/local/pgsql/lib/libpq.5.dylib ./Warehouse.app/Contents/MacOS/Warehouse
dylibbundler -of -b -x ./Warehouse.app/Contents/MacOS/Warehouse -d ./Warehouse.app/Contents/libs/

install_name_tool -change /usr/lib/libpq.5.dylib @executable_path/../libs/libpq.5.dylib ./Warehouse.app/Contents/libs/libds.so

cd Warehouse.app/Contents/libs/
ln -s libpq.5.3.dylib libpq.5.dylib
cd ../../..


# WarehouseAdmin.app
echo "Create WarehouseAdmin.app"
rm -Rf "${AdminApp}"
cp -a ../../../src/modules/admin/WarehouseAdmin.app .
mkdir ./WarehouseAdmin.app/Contents/libs
install_name_tool -change build/libds.so /usr/local/lib/libds.so ./WarehouseAdmin.app/Contents/MacOS/WarehouseAdmin
install_name_tool -change libpq.5.dylib /usr/local/pgsql/lib/libpq.5.dylib ./WarehouseAdmin.app/Contents/MacOS/WarehouseAdmin
dylibbundler -of -b -x ./WarehouseAdmin.app/Contents/MacOS/WarehouseAdmin -d ./WarehouseAdmin.app/Contents/libs/


# LicenceGenerator.app
echo "Create LicenceGenerator.app"
rm -Rf LicenceGenerator.app
cp -a ../../../../DSLicenceGenerator/src/modules/licencegenerator/LicenceGenerator.app .
mkdir ./LicenceGenerator.app/Contents/libs
dylibbundler -of -b -x ./LicenceGenerator.app/Contents/MacOS/LicenceGenerator -d ./LicenceGenerator.app/Contents/libs/


source1="pkg_warehouse"
source2="pkg_licencegenerator"
source3="pkg_admin"
source4="pkg_postgresql"
postgresScript="postgresql.sh"
postgresApp="postgresql-9.0.6-1-osx.app"
title="Warehouse"
tmpPKGName="pack.temp.pkg"
finalPKGName="Warehouse.pkg"

# Clean previous make
rm -Rf "${source1}"
rm -Rf "${source2}"
rm -Rf "${source3}"
rm -Rf "${source4}"

# Create pkg_warehouse
echo "Create pkg_warehouse"
mkdir -p "${source1}/Applications"
mkdir -p "${source1}/Resources/PostFlight"
cp -R "${WarehouseApp}" "${source1}/Applications/"
cp "mkDockitemWarehouse.sh" "${source1}/Resources/PostFlight/"
chmod 775 "${source1}"
chmod 775 "${source1}/Applications"
chmod 755 "${source1}/Applications/${WarehouseApp}"
chmod a+rx "${source1}/Resources/PostFlight/mkDockitemWarehouse.sh"

# Create pkg_licencegenerator
echo "Create pkg_licencegenerator"
mkdir -p "${source2}/Applications"
mkdir -p "${source2}/Resources/PostFlight"
cp -R "${LicenceGeneratorApp}" "${source2}/Applications/"
cp "mkDockitemLicenceGenerator.sh" "${source2}/Resources/PostFlight/"
chmod 775 "${source2}"
chmod 775 "${source2}/Applications"
chmod 755 "${source2}/Applications/${LicenceGeneratorApp}"
chmod a+rx "${source2}/Resources/PostFlight/mkDockitemLicenceGenerator.sh"

# Create pkg_admin
echo "Create pkg_admin"
mkdir -p "${source3}/Applications"
mkdir -p "${source3}/Resources/PostFlight"
cp -R "${AdminApp}" "${source3}/Applications/"
cp "mkDockitemWarehouseAdmin.sh" "${source3}/Resources/PostFlight/"
chmod 775 "${source3}"
chmod 775 "${source3}/Applications"
chmod 755 "${source3}/Applications/${AdminApp}"
chmod a+rx "${source3}/Resources/PostFlight/mkDockitemWarehouseAdmin.sh"

# Create pkg_postgresql
echo "Create pkg_postgresql"
mkdir -p "${source4}/Resources/PreFlight"
cp -R "${postgresApp}" "${source4}/Resources/PreFlight/"
cp "${postgresScript}" "${source4}/Resources/PreFlight/"
chmod a+rx "${source4}/Resources/PreFlight/${postgresScript}"


#rm -Rf ${source1}
#rm -Rf "${source2}"
#rm -Rf "${source3}"
rm -Rf "${WarehouseApp}"
rm -Rf "${LicenceGeneratorApp}"
rm -Rf "${AdminApp}"
