#!/usr/bin/env bash

DEPLOY="/server/web/rechnungen/www"
#DEPLOY="/tmp/rechnungen"
USER="www"
GROUP="www"
FILES=("images" "styles" "toolkits" "index.html" "sitemap.xml" "robots.txt")

if [ ! -d "${DEPLOY}" ]
then
	mkdir -p "${DEPLOY}"
	if [ $? != 0 ]
	then
		echo "Could not create DEPLOY '${DEPLOY}'"
		exit 1
	fi
fi

./slider.py
if [ $? != 0 ]
then
	echo "Could not exec ./slider.py. Abort."
	exit 2
fi

rm -Rf "${DEPLOY}/*"

for FILE in ${FILES[@]}
do
	cp -rf "../${FILE}" "${DEPLOY}/"
	if [ $? != 0 ]
	then
		echo "Could not copy '../${FILE}' to '${DEPLOY}'. Abort."
		exit 3
	fi
done

chown -R "${USER}:${GROUP}" "${DEPLOY}"
if [ $? != 0 ]
then
	echo "Could not chown to '${USER}:${GROUP}'"
	exit 4
fi
