import os;
import string;
import sys;

moc_path='/bin/moc';
qglobal_path='/include/Qt/qglobal.h'
q3global_path='/include/qglobal.h'

if (os.name=='nt'):
        #I just use this for myself
        moc_path='\\bin\\moc.exe';
        qglobal_path='\\src\\corelib\\global\\qglobal.h'
        q3global_path='\\include\\qglobal.h'

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


def try_qtdir(p_dir,version_major,version_minor):

        print "trying" + p_dir

        if (not os.path.isfile(p_dir + moc_path)):
		return False;
        print "found moc"
        qglobal_file=q3global_path;
	if (version_major>=4):
                qglobal_file=qglobal_path;

        if (not os.path.isfile(p_dir + qglobal_file)):
		return False;

        print 'found qglobal';
        file_to_check=p_dir + qglobal_file;

        print "trying " + file_to_check;
#	version=os.popen("cat " + file_to_check + " | grep \"QT_VERSION_STR \"                ").readlines();
	try:
                f = open(file_to_check,"rb");
	except:
                print "not found"
		return False; # couldnt open file

	lines = f.readlines();	

        print "has " + str(len(lines)) + " lines";

	if (not len(lines)):
		return False;

	ver_str="";

	for line in lines:
		if (line.find("QT_VERSION_STR") == -1):
			continue;
		
		ver_str=line;
		break;

	if (ver_str == ""):
		return False;		

	ver_str=ver_str[ver_str.find("\"")+1:len(ver_str)];
	ver_str=ver_str[0:ver_str.find("\"")];
	version_numbers=ver_str.split(".");
	major=int(version_numbers[0]);
	minor=int(version_numbers[1]);
	if (major!=version_major): #Major must be the same, for backwards compatibility
		return False;
	if (minor<version_minor): #Minor must be greater than the current version
		return False;
	
	
	print "Found QT Version: " + ver_str;
	
	
	
	return True;

		
	
def check_qtdir(version_major,version_minor):

	if ("QTDIR" in os.environ.keys()):
                if (try_qtdir(os.environ["QTDIR"],version_major,version_minor)):
			return os.environ["QTDIR"]; #QTDIR has been set, use itself

			
	possible_paths=[];
	possible_paths+=['/usr/share/qt4']; # debian
	possible_paths+=['/usr/qt4']; # hackish
	possible_paths+=['/usr/X11R6']; # FreeBSD
	possible_paths+=['/usr/','/usr/local','/opt/qt4/']; # Standard

	for x in possible_paths:
		if (try_qtdir(x,version_major,version_minor)):
			return x;

	print "Error: QT version at least: " + str(version_major) + "." + str(version_minor) + ".0 not found.";
	print "Please set the QTDIR environment variable pointing tot he place where it is located";
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
	
