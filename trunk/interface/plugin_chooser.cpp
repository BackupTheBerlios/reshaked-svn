//
// C++ Implementation: plugin_chooser
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "plugin_chooser.h"
#include "widgets/label.h"
#include "base/painter.h"
#include "containers/center_container.h"
#include "gui_custom/rsskin.h"
namespace ReShaked {


Size PluginChooser::PluginItem::get_minimum_size_internal() {

	Size minsize;
	
	minsize.height+=get_painter()->get_font_height( font(FONT_PLUGIN_CHOOSER_NAME) );
	minsize.height+=get_painter()->get_font_height( font(FONT_PLUGIN_CHOOSER_DESCRIPTION) );
	minsize.height+=constant(C_PLUGIN_CHOOSER_ITEM_MARGIN)*2;
	minsize.height+=constant(C_PLUGIN_CHOOSER_ITEM_INTERNAL_SEPARATION);
	
	return minsize;

}
void PluginChooser::PluginItem::draw(const Point& p_global,const Size& p_size,const Rect& p_exposed) {

	const SoundPluginInfo *info=SoundPluginList::get_singleton()->get_plugin_info( index );
	ERR_FAIL_COND(!info);
	
	if (*current==this)
		get_painter()->draw_fill_rect( Point(), p_size, color(COLOR_PLUGIN_CHOOSER_BG_SELECTED));
	else
		get_painter()->draw_fill_rect( Point(), p_size, color(COLOR_PLUGIN_CHOOSER_BG));
	
	get_painter()->draw_fill_rect( Point(), Size(p_size.width,1), color(COLOR_PLUGIN_CHOOSER_SEPARATOR));
	Point ofs=Point();
	ofs.x+=constant(C_PLUGIN_CHOOSER_ITEM_MARGIN);
	ofs.y+=constant(C_PLUGIN_CHOOSER_ITEM_MARGIN);
	ofs.y+=get_painter()->get_font_ascent( font(FONT_PLUGIN_CHOOSER_NAME ) );
	get_painter()->draw_text( font(FONT_PLUGIN_CHOOSER_NAME ), ofs, info->caption,color(COLOR_PLUGIN_CHOOSER_FONT_NAME) );
	ofs.y+=get_painter()->get_font_height( font(FONT_PLUGIN_CHOOSER_NAME) );
	ofs.y-=get_painter()->get_font_ascent( font(FONT_PLUGIN_CHOOSER_NAME ) );
	ofs.y+=get_painter()->get_font_ascent( font(FONT_PLUGIN_CHOOSER_DESCRIPTION ) );
	ofs.x+=constant(C_PLUGIN_CHOOSER_ITEM_DESCRIPTION_MARGIN);
	get_painter()->draw_text( font(FONT_PLUGIN_CHOOSER_DESCRIPTION ), ofs, info->description, color(COLOR_PLUGIN_CHOOSER_FONT_DESCRIPTION) );

}

void PluginChooser::PluginItem::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	if (!p_press)
		return;
	
	if (*current)
		(*current)->update();
		
	*current=this;
	
	selected_signal.call(this);
		
	update();
		
	
}

PluginChooser::PluginItem::PluginItem( int p_index,PluginItem ** p_current ) {

	index=p_index;
	current=p_current;
}


void PluginChooser::item_selected(PluginItem*p_item) {

	static const char *instance_chan_names[9]={"None","Mono","Stereo","3","Quad","5","6","7","Octo"};

	const SoundPluginInfo *info=SoundPluginList::get_singleton()->get_plugin_info( p_item->get_index() );
	ERR_FAIL_COND(!info);
	
	
	channels->clear();
	int select=-1;
	
	int current_chans = editor->get_song()->get_track(current_track)->get_channels();
	
	if (info->custom_channels.size()) {
	
		for (int i=0;i<info->custom_channels.size();i++) {
		
			if (info->custom_channels[i]==current_chans)
				select=i;
						
			channels->add_string(instance_chan_names[info->custom_channels[i]]);
		}
	} else {
	
		for (int i=1;i<=8;i++) {
		
			if (i==current_chans)
				select=i-1;
						
			channels->add_string(instance_chan_names[i]);
		}
	
	}
	
	if (select!=-1)
		channels->select( select );
}

void PluginChooser::choose_plugin(int p_current_track) {

	current_track=p_current_track;
	if (plugin_list)
		delete plugin_list;
	current=NULL;
		
	plugin_list = scroll_vb->add ( new VBoxContainer );
	plugin_list->set_separation(0);
	
	String old_category;
	for(int i=0;i<SoundPluginList::get_singleton()->get_plugin_count();i++) {
	
		String category = SoundPluginList::get_singleton()->get_plugin_info(i)->category;
		if (old_category!=category) {
		
			plugin_list->add( new Label(category+":"));
			old_category=category;
		}
		
		PluginItem *pi = plugin_list->add( new PluginItem(i,&current) );
		pi->selected_signal.connect(this, &PluginChooser::item_selected);
	}
	
	channels->clear();
	
	show();
}
	

PluginChooser::PluginChooser(Window *p_parent,Editor *p_editor) : Window(p_parent,Window::MODE_POPUP,Window::SIZE_TOPLEVEL_CENTER) {

	editor=p_editor;
	WindowBox *wb = new WindowBox("Sound Plugin List");
	set_root_frame(wb);
	
	scroll_box = wb->add( new ScrollBox, 1 );
	
	scroll_vb = scroll_box->set( new VBoxContainer );
	scroll_box->set_scroll_v(true);
	scroll_box->set_expand_h(true);
	
	
	append_to_out = wb->add( new CheckButton("Auto-Connect to Output"));
	HBoxContainer *hb = wb->add( new HBoxContainer );
	hb->add(new Label("Channels to Instance"),1);
	
	Button *add_b = wb->add(new CenterContainer)->set( new Button("Add Plugin"));
	
	channels = hb->add(new ComboBox);
	current = NULL;
	plugin_list=NULL;
	
}


PluginChooser::~PluginChooser()
{
}


}
