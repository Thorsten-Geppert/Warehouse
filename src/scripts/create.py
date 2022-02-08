#!/usr/bin/env python

import sys
import codecs

def program_help(program):
	print ""
	print "Usage of create"
	print "==============="
	print ""
	print "\t" + program + " [options] filename (without extension)"
	print ""
	print "\tOptions"
	print "\t-------"
	print ""
	print "\t\tcpp\t- Creates c++ header and source file"
	print "\t\t\t\tYou may use as second parameter the derived class"
	print "\t\t\t\tYou may use as third parameter xrc"
	print "\t\txrc\t- Creates xrc file (named .xml)"
	print ""

def options_help():
	print ""
	print "Use one of the following options"
	print "================================"
	print ""
	print "\tcpp\t- Creates c++ header and source file"
	print "\t\t\tYou may use as second parameter the derived class"
	print "\t\t\tYou may use as third parameter xrc"
	print "\txrc\t- Creates xrc file (named .xml)"
	print ""

def write_file(filename, content):
	try:
		f = codecs.open(filename, "w", "utf-8")
		f.write(content)
		f.close()
		return True
	except StandardError, e:
		print e

	return False

def create_xrc(class_name, name):
	xrc  = "<?xml version=\"1.0\"?>\n"
	xrc += "<resource version=\"2.3.0.1\">\n\n"
	xrc += "\t<object class=\"" + class_name + "\" name=\"" + name + "\">\n\n";
	xrc += "\t</object>\n\n"
	xrc += "</resource>"

	if not write_file(name + ".xml", xrc):	
		print "Could not save", name + ".xml"

def create_cpp(class_name, name, xrc):
	hpp  = "#ifndef " + name.upper() + "_H_\n"
	hpp += "#define " + name.upper() + "_H_\n\n"

	if xrc == True:
		hpp += "#include <wx/wx.h>\n"
		hpp += "#include <RuntimeInformationType.h>\n\n"

	hpp += "class " + name
	
	if len(class_name) > 0:
		hpp += " : public " + class_name
	
	hpp += " {\n\n"
	hpp += "\tpublic:\n"

	hpp += "\t\t" + name + "("

	if xrc == True:
		hpp += "\n\t\t\twxWindow *parent,"
		hpp += "\n\t\t\twxWindowID id,"
		hpp += "\n\t\t\tRuntimeInformationType *rit"
		hpp += "\n"
	
	hpp += "\t\t);\n"
		
	hpp += "\t\t~" + name + "();\n\n"

	if xrc == True:
		hpp += "\tprotected:\n"
		hpp += "\t\tDECLARE_EVENT_TABLE()\n\n"
		hpp += "\t\tRuntimeInformationType *rit;\n\n"

	hpp += "};\n\n"

	hpp += "#endif /* " + name.upper() + "_H_ */\n"

	###

	cpp  = "#include \"" + name + ".h\"\n"
	cpp += "#include <wx/xrc/xmlres.h>\n\n"
	cpp += "BEGIN_EVENT_TABLE(" + name + ", " + class_name + ")\n"
	cpp += "END_EVENT_TABLE()\n\n"

	if xrc == False:
		cpp += name + "::" + name

		if len(class_name) > 0 and xrc == False:
			cpp += " : " + class_name

		cpp += "() {\n"
	else:
		cpp += name + "::" + name + "(\n"
		cpp += "\twxWindow *parent,\n"
		cpp += "\twxWindowID id,\n"
		cpp += "\tRuntimeInformationType *rit\n"
		cpp += ") {\n"
		cpp += "\tSetId(id);\n"
		cpp += "\tthis->rit = rit;\n\n"
		cpp += "\twxXmlResource::Get()->Load(rit->GRP(_N(\"gui\"), _N(\"" + name + ".xml\")));\n"
		cpp += "\twxXmlResource::Get()->LoadDialog(this, parent, _N(\"" + name + "\"));\n"
	
	cpp += "}\n\n"

	cpp += name + "::~" + name + "() {\n}\n"

	if not write_file(name + ".cpp", cpp):	
		print "Could not save", name + ".cpp"
	if not write_file(name + ".h", hpp):
		print "Could not save", name + ".h"

argv = sys.argv
argc = len(argv)

if (argc != 3 and argc != 4 and argc != 5) or argv[1] == "--help":
	program_help(argv[0])
	exit(1)

if argv[1] != "cpp" and argv[1] != "xrc":
	options_help()
	exit(2)

name       = ""
class_name = ""
xrc        = False

if argc == 5:
	name       = argv[4]
	class_name = argv[2]
	if argv[3] == "xrc":
		xrc = True
	else:
		xrc = False
elif argc == 4:
	name       = argv[3]
	class_name = argv[2]
else:
	name       = argv[2]

if argv[1] == "xrc":
	create_xrc(class_name, name)
elif argv[1] == "cpp":
	create_cpp(class_name, name, xrc)
else:
	help(argv[0]);
