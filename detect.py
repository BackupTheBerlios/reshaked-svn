import os;
import string;
import sys;

def check_pkg_config():
    
	
	errorval=os.system("pkg-config --version > /dev/null");

	if (errorval):
		print "Error: cant execute pkg-config, please install pkg-config!"
		sys.exit(1);

def check_pkg( p_env, p_pkg_name, p_pkg_version):


	errorval=os.system("pkg-config " + p_pkg_name +" --atleast-version " + p_pkg_version);

	if (errorval):
		libdata.has_sndfile=0;
		print("Error: " + p_pkg_name + " of at least version " + p_pkg_version + " was not found in pkg-config");
		sys.exit(1);

	p_env.ParseConfig("pkg-config --cflags --libs " + p_pkg_name);
	version_found=os.popen("pkg-config --modversion " + p_pkg_name).readlines()[0][:-1];
	
	print("Using: " + p_pkg_name + " " + version_found);


def try_qtdir(p_dir):

	if (not os.path.isfile(p_dir + "/bin/moc")):
		return False;
	if (not os.path.isfile(p_dir + "/include/qglobal.h")):
		return False;
		
	file_to_check=p_dir + "/include/qglobal.h"

	version=os.popen("cat " + file_to_check + " | grep \"QT_VERSION_STR \"                ").readlines();
	if not len(version):
		return False;

	ver_str=version[0];
	found = 0;
	if ( (ver_str.find("\"3.2")>=0) or (ver_str.find("\"3.3")>=0) or (ver_str.find("\"3.3")>=0) or (ver_str.find("\"3.4")>=0) or (ver_str.find("\"3.5")>=0) or (ver_str.find("\"3.6")>=0)):
		return True;

	return False;

		
	
def check_qtdir():

	if ("QTDIR" in os.environ.keys()):
		if (try_qtdir(os.environ["QTDIR"])):
			return os.environ["QTDIR"]; #QTDIR has been set, use itself

			
	possible_paths=['/usr/share/qt3','/usr/','/usr/local'];

	for x in possible_paths:
		if (try_qtdir(x)):
			return x;

	print("Error: QT 3.2 or above cant be found. Please set the QTDIR environment variable pointing tot he place where it is located");
	sys.exit(1);
	


def find_source_files(self, source_list,filetype):
	import glob;
	import string;
	dir = self.Dir('.').abspath
	sources = glob.glob(dir + "/"+filetype);
	source_list+=sources;
	
	
def make_targets(self, object_list, source_list=[], debug=0):

	import glob
	import string
	import re

	if source_list == []:
		dir = self.Dir('.').abspath
		if self.build_dir:

## strip build_dir from cwd for glob
			edir = '^' + re.escape(self.Dir('#').abspath) + r'[/\\]' + re.escape(self.build_dir)

			dir = re.sub(edir, '', dir)
			dir = self.Dir('#').abspath + dir
			
			stmp = glob.glob(dir + "/*.cpp")

## add build_dir to cwd for Object
			sources = []
			for src in stmp:
				obj = re.search(r'[/\\]([^/\\]*)$', src)
				file = self.Dir('.').abspath + obj.group(0)
				sources.append(file)
		else:
			sources = glob.glob(dir + "/*.cpp")
		
	else:
		sources = []
		for src in source_list:
			sources.append(self.Dir('.').abspath + '/' + src)

	for file in sources:

		sobject = file[:-4] + self['OBJSUFFIX']
		if sobject in object_list:
			continue

#		if (self.make_so):
#			self.SharedObject(file[:-4], file)
#		else:
		self.Object(file[:-4], file)

		object_list.append(sobject)
	