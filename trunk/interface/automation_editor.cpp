//
// C++ Implementation: automation_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "automation_editor.h"
#include <map>
namespace ReShaked {

	
void AutomationEditor::automation_checked(bool p_checked,int p_which) {
	
	
	if (p_which==-1)
		return;
	
	bool has_auto=track->has_property_visible_automation( p_which );
	
	if (p_checked==has_auto)
		return; //nothing to do
	
	if (p_checked)
		editor->show_automation(p_which,track);
	else
		editor->hide_automation(p_which,track);

		
}

void AutomationEditor::show(Track *p_track) {
	
	
	track=p_track;
	tree->clear();
	
	
	typedef std::map<String,TreeItem*> PathMap;
	PathMap hashmap;
	
	tree->clear();

	
	TreeItem *root = tree->create_item("/",0);
	
	for (int i=0;i<track->get_property_count();i++) {
		
		String path=track->get_property_visual_path( i);
		
		String dir=path.left( path.find_last("/") );
		String name=path.right( path.length()-(path.find_last("/")+1) );
		
		//printf("Dir %s, File %s\n",dir.toAscii().data(),name.toAscii().data());
		
		/* find if we have dir! */
		
		TreeItem * item;
		if (dir=="") { //toplevelAutomationTree( 
			
			item  = tree->create_item(root);

		} else {
			
			// have the path for this dir somewhere?
			PathMap::iterator I =hashmap.find(dir);
			if (I!=hashmap.end()) {
				item = tree->create_item(I->second);
				
			} else {
				
				/* build path */
				int from=1;
				String dir2=dir+"/"; //so it finds the last / too
				TreeItem*parent=NULL;
				TreeItem*current=NULL;
				
				while (true) {
					
					if (from==dir2.length())
						break;
					int idx=dir2.find("/",from);
					if (idx==-1)
						break;
					String auxp=path.left( idx );
					from=idx+1; //so it skips the /
					//printf("searching %s\n",auxp.toAscii().data());
					I=hashmap.find(auxp);
					if (I==hashmap.end()) { //we must create it!
						
						TreeItem *node;
						if (parent) {
							node= tree->create_item(parent);

						} else {
							node= tree->create_item(root);

						}
						

						String nodename=auxp.right( auxp.length()-(auxp.find_last("/")+1) );
						node->set_text(nodename+"/");
						
						parent=node;
						hashmap[auxp]=node;
						//printf("adding %s, nodename: %s\n",auxp.toAscii().data(),nodename.toAscii().data());
						
					} else { //exists
						 
						parent=I->second;
					}
				}
				
				item = tree->create_item(parent);
			}
				
				
		}
		
		String captionstr=track->get_property( i)->get_caption();

		if (captionstr.find_last("/")>=0) {
			
			captionstr=captionstr.right( captionstr.length()-(captionstr.find_last("/")+1));
		}
		
		if (track->get_property_automation(i)->get_block_count()) {
				
			item->set_text( captionstr+"(*)"  );
		
		} else {
		
			item->set_text( captionstr );
		
		}
		
		//item->set_text( QStrify( track->get_property( i)->get_caption() ) );
		/*
		item->setText( 1, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_min() ) ) );
		item->setText( 2, QStrify( track->get_property( i)->get_text_value( track->get_property( i)->get_max() ) ) ); */
		
		item->set_checkable(true);
		item->set_selectable(true);
		item->set_checked(track->has_property_visible_automation(i));
		item->checked_signal.connect( Method1<bool>( Method2<bool,int>(this, &AutomationEditor::automation_checked), i) );
		
	}
	
	Window::show();
}
	
	
AutomationEditor::AutomationEditor(Window *p_parent,Editor *p_editor) : Window(p_parent,MODE_POPUP,SIZE_TOPLEVEL_CENTER)
{
	
	editor=p_editor;
	WindowBox *wb = new WindowBox("Automation Editor");
	set_root_frame(wb);
	track=NULL;
	ScrollBox *sb = wb->add( new ScrollBox,1 );
	tree=sb->set( new Tree );
	sb->set_expand_h(true);
	sb->set_expand_v(true);
	
	
}


AutomationEditor::~AutomationEditor()
{
}


}
