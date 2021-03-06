//
// C++ Implementation: add_node_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "add_node_dialog.h"
#include "gui_common/common_skin.h"
#include "bundles/scroll_box.h"
#include "bundles/window_box.h"
#include "bundles/margin_group.h"
#include "widgets/button.h"
#include "containers/center_container.h"
#include "engine/audio_node_registry.h"

#include "editor/edit_commands.h"


static const char* _chan_names[8]={"Mono","Stereo","3","Quad","5","6","7","Octo"};

void AddNodeDialog::NodeInfoItem::mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

	if (!p_press || p_button!=GUI::BUTTON_LEFT)
		return;
		
	selected.call(this);
}


GUI::Size AddNodeDialog::NodeInfoItem::get_minimum_size_internal() {


	GUI::Size minsize;
	
	minsize.height+=get_painter()->get_font_height( font(FONT_NODE_CHOOSER_NAME) );
	minsize.height+=get_painter()->get_font_height( font(FONT_NODE_CHOOSER_DESCRIPTION) );
	minsize.height+=constant(C_NODE_CHOOSER_ITEM_MARGIN)*3;
	
	
	if (icon>=0) { 
	 
	 	GUI::BitmapID iconid=get_window()->get_skin()->get_bitmap( icon );
	 	if (get_painter()->get_bitmap_size(iconid).height>minsize.height)
			minsize.height=get_painter()->get_bitmap_size(iconid).height;
		
	}
	
	minsize.height+=constant(C_NODE_CHOOSER_ITEM_INTERNAL_SEPARATION);
	
	return minsize;
}


void AddNodeDialog::NodeInfoItem::draw(const GUI::Point& p_global,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	if (current &&  *current==this)
		get_painter()->draw_fill_rect( GUI::Point(), p_size, color(COLOR_NODE_CHOOSER_BG_SELECTED));
	else
		get_painter()->draw_fill_rect( GUI::Point(), p_size, color(COLOR_NODE_CHOOSER_BG));
	
	get_painter()->draw_fill_rect( GUI::Point(0,p_size.height-1), GUI::Size(p_size.width,1), color(COLOR_NODE_CHOOSER_SEPARATOR));
	GUI::Point ofs=GUI::Point();
	ofs.x+=constant(C_NODE_CHOOSER_ITEM_MARGIN);
	ofs.y+=constant(C_NODE_CHOOSER_ITEM_MARGIN);
	if (icon>=0) {
	
		 	GUI::BitmapID iconid=get_window()->get_skin()->get_bitmap( icon );
			get_painter()->draw_bitmap(iconid,ofs);
			ofs.x+=constant(C_NODE_CHOOSER_ITEM_MARGIN)+get_painter()->get_bitmap_size(iconid).width;
	}
	
	ofs.y+=get_painter()->get_font_ascent( font(FONT_NODE_CHOOSER_NAME ) );
	get_painter()->draw_text( font(FONT_NODE_CHOOSER_NAME ), ofs, info->caption,color(COLOR_NODE_CHOOSER_FONT_NAME) );
	ofs.y+=get_painter()->get_font_height( font(FONT_NODE_CHOOSER_NAME) );
	ofs.y-=get_painter()->get_font_ascent( font(FONT_NODE_CHOOSER_NAME ) );
	ofs.y+=get_painter()->get_font_ascent( font(FONT_NODE_CHOOSER_DESCRIPTION ) );
	ofs.x+=constant(C_NODE_CHOOSER_ITEM_DESCRIPTION_MARGIN);
	get_painter()->draw_text( font(FONT_NODE_CHOOSER_DESCRIPTION ), ofs, info->description, color(COLOR_NODE_CHOOSER_FONT_DESCRIPTION) );	
}

void AddNodeDialog::NodeInfoItem::set_in_window() {

	icon=get_window()->get_skin()->find_bitmap( "bitmap_"+info->icon_string );

}

AddNodeDialog::NodeInfoItem::NodeInfoItem(const AudioNodeInfo *p_node_info) {

	info=p_node_info;
	current=NULL;
	icon=GUI::INVALID_BITMAP_ID;
}


class _BGFILL : public GUI::Widget {
public:
	void draw(const GUI::Point& p_global,const GUI::Size& p_size,const GUI::Rect& p_exposed) {
	
		get_painter()->draw_fill_rect( GUI::Point(), p_size, color(COLOR_NODE_CHOOSER_BG) );
	}

};

/******************/

void AddNodeDialog::fix_name() {

	if (!selected)
		return;
	
	String base_name = selected->info->short_caption;
	
	int attempt=1;
	while(true) {
	
		bool exists=false;
		String intended_name=attempt==1?base_name:base_name+" "+String::num(attempt);
		
		for (int i=0;i<song->get_audio_graph()->get_node_count();i++) {
		
			if (song->get_audio_graph()->get_node(i)->get_name()==intended_name) {
				
				exists=true;
				break;
			}
		}
		
		if (exists) {
		
			attempt++;
			continue;
		}
	
		name->set_text( intended_name );
		break;	
	}

}

void AddNodeDialog::node_selected_callback(NodeInfoItem *p_item) {

	NodeInfoItem *prev=selected;
	selected=p_item;
	if (prev)
		prev->update();
	if (selected)
		selected->update();
		
	channels->clear();		
	
	if (!selected)
		return;
		
	updating_default_channels=true;	
	
	int default_select=-1;
	for (int i=1;i<=8;i++) {
	
		bool add=false;
		if (!selected->info->custom_channels.size())
			add=true;
		else {
			for (int j=0;j<selected->info->custom_channels.size();j++) {
				if (selected->info->custom_channels[j]==i) {
					add=true;
					break;
				}
				if (selected->info->custom_channels[j]==0) {
					break;
				}
			}
		}
		if (add) {
		
			channels->add_string(_chan_names[i-1]);
			if (default_channels==i)
				default_select=channels->get_size()-1;
			
		}
		
	}
	
	if (default_select!=-1)
		channels->select( default_select );
			
	updating_default_channels=false;;
	
	/* update name */
	
	fix_name();	
}


void AddNodeDialog::default_channels_changed(int p_to) {


	if (!selected)
		return;
	if (updating_default_channels)
		return;
			
	if (selected->info->custom_channels.size()) {
		ERR_FAIL_INDEX( p_to, selected->info->custom_channels.size() );
		default_channels=selected->info->custom_channels[p_to];
	} else {
	
		default_channels=p_to+1;
	}		
}

void AddNodeDialog::create_node() {

	if (!selected)
		return;
		
	int chans = -1;
			
	String sel_str = channels->get_line_edit()->get_text();
	
	for (int i=1;i<8;i++) {
	
		if (sel_str==_chan_names[i-1])
			chans=i;
	}
		
	if (chans==-1)
		return;
		
	AudioNode *anode = selected->info->creation_func( chans, selected->info );
	
	/* make sure it doesn't appear overlappign another node */
	
	int x=20,y=20;
	
	while(true) {
		
		bool overlaps=false;
	
		for (int i=0;i<song->get_audio_graph()->get_node_count();i++) {
		
			int nx,ny;
			AudioNode *an=song->get_audio_graph()->get_node(i);
			nx=an->get_x();
			ny=an->get_y();
		
			if (x==nx && y==ny) {
			
				overlaps=true;
				break;
			}			
		}
		
		if (overlaps) {
		
			x+=20;
			y+=20;
			continue;
		}
		
		break;
	}
	
	anode->set_x(x);
	anode->set_y(y);
	anode->set_name( name->get_text() );
	
	EditCommands::get_singleton()->audio_graph_add_node( song->get_audio_graph(), anode );

	hide();
}

void AddNodeDialog::filter_changed(int p_to_which) {

	ERR_FAIL_INDEX(p_to_which,filters.size());
	
	String filter = filters[p_to_which];
	
	GUI::ChildIterator I = vbox->first_child();
	
	while(!I.end()) {
	
		NodeInfoItem *nii = dynamic_cast<NodeInfoItem*>( I.get() );
		
		if (nii) {
		
			if (filter=="All" || nii->info->category==filter) {
				
				nii->show();
			} else {
			
				nii->hide();			
			}
		}
		
		I=vbox->next_child(I);
	}

}


String AddNodeDialog::get_current_category() {

	ERR_FAIL_INDEX_V( filter_group.get_current(), filters.size(), "All");
	return filters[ filter_group.get_current() ];
}

void AddNodeDialog::show(String p_pre_type) {

	for (int i=0;i<filters.size();i++) {
	
		if (filters[i]==p_pre_type) {
		
			filter_group.set_current(i);
			break;
		}
	}

	Window::show();
	fix_name();
}

AddNodeDialog::AddNodeDialog(GUI::Window *p_parent,Song *p_song) : GUI::Window(p_parent,GUI::Window::MODE_POPUP,GUI::Window::SIZE_TOPLEVEL_CENTER) {

	song=p_song;
	selected=NULL;
	
	GUI::WindowBox *wb = new GUI::WindowBox("Add Node");
	set_root_frame(wb);
	
	GUI::MarginGroup *mg = wb->add( new GUI::MarginGroup("Audio Node Type:"),1 );
			
	GUI::HBoxContainer *filter_hb=mg->add( new GUI::CenterContainer )->set( new GUI::HBoxContainer );
	GUI::ScrollBox *scroll_box = mg->add( new GUI::ScrollBox,1 );
	scroll_box->set_expand_h(true);
	scroll_box->set_expand_v(true);
	vbox = scroll_box->set( new GUI::VBoxContainer );
	vbox->set_separation(0);
				
	filters.push_back("All");
					
	for (int i=0;i<AudioNodeRegistry::get_node_info_count();i++) {
	
		String category=AudioNodeRegistry::get_node_info(i)->category;
		
		bool has_filter=false;
		for (int j=0;j<filters.size();j++) {
		
			if (filters[j]==category) {
			
				has_filter=true;
				break;
			}
		}
		
		
		if (!has_filter) {
		
			filters.push_back(category);
		}
		
		NodeInfoItem *nii = new NodeInfoItem( AudioNodeRegistry::get_node_info(i) );
		nii->current=&selected;
		vbox->add( nii );
		nii->selected.connect( this, &AddNodeDialog::node_selected_callback );
	}
				
	
	vbox->add( new _BGFILL, 1 );
					
					
	for (int i=0;i<filters.size();i++) {
	
		filter_group.add_button( filter_hb->add( new GUI::Button(filters[i]) ) )->set_focus_mode( GUI::FOCUS_NONE );
	}
				
	filters.push_back("Favorites");
	
	GUI::BaseButton *fave_button = filter_group.add_button( filter_hb->add( new GUI::Button ) );
	fave_button->add_bitmap_override( GUI::BITMAP_BUTTON_DEFAULT_ICON, BITMAP_ICON_FAVORITE );
	fave_button->set_focus_mode( GUI::FOCUS_NONE );
				
	filter_group.current_button_changed_signal.connect( this, &AddNodeDialog::filter_changed );
	GUI::HBoxContainer * hbc = wb->add( new GUI::HBoxContainer );
				
	mg = hbc->add( new GUI::MarginGroup("Name:"), 1 );
	
	name = mg->add( new GUI::LineEdit );
	
	channels = hbc->add( new GUI::MarginGroup("Channels"),1)->add( new GUI::ComboBox );
	channels->selected_signal.connect(this, &AddNodeDialog::default_channels_changed );
	
	default_channels=2;
	
	GUI::Button *add = wb->add( new GUI::CenterContainer )->set( new GUI::Button("Add") );
	add->set_tooltip("Creates and Adds Selected Node to Graph");
	
	add->pressed_signal.connect( this, &AddNodeDialog::create_node );
	
}


AddNodeDialog::~AddNodeDialog()
{
}


