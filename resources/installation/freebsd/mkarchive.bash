if [ $# != 1 ]
then
	echo "Usage: ${0} version (e.g. ${0} 1.2.3)"
	exit 2
fi

echo "Did you recompile all software?"
echo "	- GuG-IT Warehouse"
echo "	- GuG-IT Warehouse Admin"
echo "	- LicenceGenerator"
echo "	- Reports"
echo "	- Handbook"
echo -n "y/n: "
read answer

if [ "${answer}" != "y" ]
then
	echo "Please recompile all software and rerun this script."
	exit 1
fi

VERSION="${1}"
BINARY_PATH="GuGIT-Warehouse-${VERSION}_FreeBSD_amd64"
LIB_PATH="${BINARY_PATH}/lib"
ARCHIVE="${BINARY_PATH}.tar.bz2"

echo "Remove ${BINARY_PATH} if exists"
rm -Rf "${BINARY_PATH}"

echo "Create directory ${BINARY_PATH}"
mkdir "${BINARY_PATH}"

echo "Copy all needed binaries to ${BINARY_PATH}"
cp ../../../src/modules/warehouse/Warehouse "${BINARY_PATH}"
cp ../../../src/modules/warehouse/resources/doc/Handbook.pdf "${BINARY_PATH}"
cp ../../../src/modules/admin/WarehouseAdmin "${BINARY_PATH}"
cp ../../../../DSLicenceGenerator/src/modules/licencegenerator/LicenceGenerator "${BINARY_PATH}"
cp -R ../../../src/modules/warehouse/resources "${BINARY_PATH}"
cp start_licencegenerator.sh "${BINARY_PATH}"
cp start_warehouse.sh "${BINARY_PATH}"
cp start_warehouse_admin.sh "${BINARY_PATH}"

echo "Adding reports"
mkdir "${BINARY_PATH}/reports"
cp ../../../src/modules/reports/target/reports-0.0.1.jar "${BINARY_PATH}/reports/reports.jar"
cp -R ../../../src/modules/reports/target/lib "${BINARY_PATH}/reports/"
cp -R ../../../src/modules/reports/reports "${BINARY_PATH}/reports/"

echo "Create directory ${LIB_PATH}"
mkdir "${LIB_PATH}"

echo "Copy all needed libraries to ${LIB_PATH}"
cp /usr/local/lib/libds.so "${LIB_PATH}"

echo "Create package ${ARCHIVE}"
tar cjf "${ARCHIVE}" "${BINARY_PATH}"

echo "Remove ${BINARY_PATH}, clean up"
rm -Rf "${BINARY_PATH}"

md5 "${ARCHIVE}"
