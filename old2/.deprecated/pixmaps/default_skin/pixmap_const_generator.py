
import os;
import glob;
import string;

pixmaps = glob.glob("*.xpm");

pixmaps.sort();


for x in pixmaps:
	
	theme_str='THEME_'+x[:-4].upper();
	print '  '+theme_str+',';
