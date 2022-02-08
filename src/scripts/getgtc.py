#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib

URL   = "http://www.gug-it.de/seiten/Unternehmen/AGB.html"
BEGIN = "<!-- Content -->"
END   = "<!-- /Content -->"
FILE  = "../modules/warehouse/Configuration.h"
FIND  = "#define GTC"

website = urllib.urlopen(URL)
html = website.read()
website.close()
html = "<html><head><title>AGB</title></head><body>" + html[html.find(BEGIN) + len(BEGIN) : html.find(END)] + "</body></html>";
html = html.replace("\r", "").replace("\n", "").replace("\t", "").replace('"', '\\"');

f = open(FILE, "r")
lines = f.readlines()
f.close()

newf = ""
for line in lines:
	line = line[0:-1]
	if line[0:len(FIND)] == FIND:
		newf += FIND + " _N(\"" + html + "\")"
	else:
		newf += line
	newf += "\n"

f = open(FILE, "w")
f.write(newf)
f.close()
