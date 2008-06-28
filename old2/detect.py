import os;
import string;
import sys;


def check_pkg_config():
    
	
	errorval=os.system("pkg-config --version > /dev/null");

	if (errorval):
		print "**Error: cant execute pkg-config, please install pkg-config!"
		sys.exit(1);

def check_pkg( p_env, p_pkg_name, p_pkg_version, p_pkg_vital=False):


	errorval=os.system("pkg-config " + p_pkg_name +" --atleast-version " + p_pkg_version);

	if (errorval):
		print("**Error: " + p_pkg_name + " of at least version " + p_pkg_version + " was not found in pkg-config");
		if (p_pkg_vital):
			sys.exit(1);
		else:
			print("**Error: " + p_pkg_name + " support disabled.");
			return False; # Not detected!

	p_env.ParseConfig("pkg-config --cflags --libs " + p_pkg_name);
	version_found=os.popen("pkg-config --modversion " + p_pkg_name).readlines()[0][:-1];
	
	print("Using: " + p_pkg_name + " " + version_found);
	
	return True; # detected!


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
			stmp += glob.glob(dir + "/*.c")

## add build_dir to cwd for Object
			sources = []
			for src in stmp:
				obj = re.search(r'[/\\]([^/\\]*)$', src)
				file = self.Dir('.').abspath + obj.group(0)
				sources.append(file)
		else:
			sources = glob.glob(dir + "/*.cpp")
			sources += glob.glob(dir + "/*.c")
		
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
	
