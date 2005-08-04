
EnsureSConsVersion(0,96);

import detect;
import sys;
import os;

# PRE-detect Qt3 QTDIR
qtdir=detect.check_qtdir();
os.environ["QTDIR"]=qtdir;

env = Environment(CCFLAGS=['-Wall','-g3','-I.','-Itypes/'], tools=['default','qt']);

env["QT_LIB"]="qt-mt";

#Enable the make_targets helper
#env.__class__.make_targets = detect.make_targets
env.build_dir = None

env.__class__.find_source_files = detect.find_source_files
detect.check_pkg_config();

#detect.check_pkg(env,"qt-mt","3.2");
detect.check_pkg(env,"alsa","0.9");

Export('env');

SConscript('types/SCsub');
SConscript('engine/SCsub');
SConscript('editing/SCsub');
SConscript('interface/SCsub');

reshaked_libs = ['#interface/libinterface.a','#editing/libediting.a','#engine/libengine.a'];

env.Program('program/reshaked',['program/reshaked.cpp'] + reshaked_libs);