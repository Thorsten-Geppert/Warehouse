#!/usr/bin/env python

import csv

f = open('slider_with_image.tpl', 'r')
wi = f.read()

f = open('slider_without_image.tpl', 'r')
wo = f.read()

f = open('slider_download.tpl', 'r')
do = f.read()

first = True
functions = ""

with open('slider.csv', 'rb') as csvfile:
	lines = csv.reader(csvfile, delimiter=';')
	for line in lines:
		tmp = ""
		if len(line) != 3:
			tmp = do
		else:
			if len(line[1]) == 0:
				tmp = wo
			else:
				tmp = wi
				tmp = tmp.replace('{s:image}', line[1])
			tmp = tmp.replace('{s:headline}', line[0])
			tmp = tmp.replace('{s:text}', line[2])
		if first:
			tmp = tmp.replace('{s:active}', 'active')
			first = False
		else:
			tmp = tmp.replace('{s:active}', '')
		functions += tmp

f = open('index.tpl', 'r')
index = f.read()

index = index.replace('{s:functions}', functions)

f = open('../index.html', 'w')
f.write(index)
