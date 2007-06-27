//
// C++ Implementation: edit_view_frame
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "widgets/label.h"
#include "edit_view_frame.h"
#include "interface/edit_view_automation.h"
#include "interface/edit_view_pattern.h"
#include "gui_custom/rsskin.h"

namespace ReShaked {

	
class BlackWidget : public Widget {
	
	virtual void draw(const Point& p_global,const Size& p_size,const Rect& p_exposed) {
		printf("global pos is %i,%i, desired global is %i,%i\n",get_global_pos().x,get_global_pos().y,p_global.x,p_global.y);
		get_painter()->draw_fill_rect(Point(),p_size,color(COLOR_EDITOR_BG));
	}
public:	
	BlackWidget()  {
		
		set_bg_on_updates(false);
	}
};
	
	
class SideWidget : public Widget {
public:	
	
	enum Type {
		TYPE_PATTERN,
  		TYPE_AUTOMATION
	};
		
	Type type;
	String text;
		
	Size get_minimum_size_internal() {
		
		int width= get_painter()->get_font_height( font( FONT_EDIT_VIEW_SIDE_WIDGET ) );
		
		Size arr_bitmap_size= get_painter()->get_bitmap_size( bitmap( type == TYPE_PATTERN ? BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_PATTERN : BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_AUTOMATION ) );
		
		if ( width< arr_bitmap_size.width )
			width=arr_bitmap_size.width;
		
		return Size(width, 0 );
	}
	
	void draw(const Point& p_global,const Size& p_size,const Rect& p_exposed) {
		 	
		get_painter()->draw_fill_rect(Point(),p_size,color( type == TYPE_PATTERN ? COLOR_PATTERN_BG : COLOR_AUTOMATION_BG ) );
		Size arr_bitmap_size= get_painter()->get_bitmap_size( bitmap( type == TYPE_PATTERN ? BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_PATTERN : BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_AUTOMATION ) );
		
		get_painter()->draw_bitmap( bitmap( type == TYPE_PATTERN ? BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_PATTERN : BITMAP_EDIT_VIEW_SIDE_WIDGET_ARROW_AUTOMATION ), Point((p_size.width-arr_bitmap_size.width)/2, 0 ) );
					    
		
		get_painter()->draw_text( font( FONT_EDIT_VIEW_SIDE_WIDGET ), Point( constant( C_EDIT_VIEW_SIDE_WIDGET_MARGIN), arr_bitmap_size.height + constant( C_EDIT_VIEW_SIDE_WIDGET_MARGIN)*2 ) , text, DOWN, color( type== TYPE_AUTOMATION ? COLOR_EDIT_VIEW_SIDE_WIDGET_AUTOMATION : COLOR_EDIT_VIEW_SIDE_WIDGET_PATTERN ) );
	}
	
public:	
	
	SideWidget(Type p_type, String p_string) {
		 
		type=p_type;
		text=p_string;
	}
	
};
	
	
void EditViewFrame::blocklist_ui_under_cursor_request_slot(EditViewBlockList* p_blocklist) {
		
	
}
	
void EditViewFrame::ensure_cursor_visible() {
		
	
}
	
void EditViewFrame::cursor_changed_blocklist() {
		

	int current_blocklist=editor->get_current_blocklist();
	if (current_blocklist<0 || current_blocklist>=edit_blocklists.size())
		return; //cursot cant be displayed.

	edit_blocklists[current_blocklist]->get_focus();

	ensure_cursor_visible();
	
}

void EditViewFrame::edit_window_moved_slot() {
	
	repaint(); // TODO SCROLL
}
	
void EditViewFrame::repaint() {
	
	for (int i=0;i<edit_blocklists.size();i++) {
		
		edit_blocklists[i]->update();		
	}
}
	
void EditViewFrame::repaint_blocklists(const std::list<int>& p_which) {
		
	std::list<int>::const_iterator I=p_which.begin();
	
	
	for (;I!=p_which.end();I++) {
		
		int bl=*I;
		if (bl<0 || bl>=edit_blocklists.size())
			continue;
			
		edit_blocklists[ bl ]->update();
	}
}
	
void EditViewFrame::rebuild() {
		
	printf("rebuilding\n");
	
	if (hbc)
		delete hbc;
	
	edit_blocklists.clear();
	
	hbc = hbc_base->add( new HBoxContainer,1 );
	hbc->hide();
	hbc->set_separation(0);
		
	
	
	for (int i=0;i<editor->get_song()->get_global_track().get_visible_automation_count();i++) {
						
		
		
		Automation *a=editor->get_song()->get_global_track().get_visible_automation(i);
		
		SideWidget * sw = hbc->add(new SideWidget(SideWidget::TYPE_AUTOMATION, a->get_property()->get_caption() ));
		
		EditViewAutomation *au_wg=hbc->add( new EditViewAutomation(editor,a) );
		edit_blocklists.push_back( au_wg );
		au_wg->blocklist_ui_under_cursor_request_signal.connect(this, &EditViewFrame::blocklist_ui_under_cursor_request_slot); 
			

	}
	
	for (int i=0;i<editor->get_song()->get_track_count();i++) {

		
		//hb->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
		
			
		EditViewBlockList *bui;
		
		SWITCH( editor->get_song()->get_track(i)->get_type_name() )

			CASE("pattern") {

			
				Track_Pattern *pattern=dynamic_cast<Track_Pattern *>(editor->get_song()->get_track(i));
				ERR_CONTINUE(pattern==NULL);
			
				SideWidget * sw = hbc->add( new SideWidget(SideWidget::TYPE_PATTERN, pattern->get_name() ));
			
				bui=hbc->add(new EditViewPattern(editor,i));
				edit_blocklists.push_back( bui );
				
				

				/*
				new PixmapLabel(cvb,GET_QPIXMAP(PIXMAP_TRACK_SLIDER_VU_ICON));
				PropertyEditSliderVU *slider_vu = new PropertyEditSliderVU(cvb,amp_slider_vu_skin);
				slider_vu->set_property( &pattern->volume() );
				slider_vus.push_back(slider_vu);
				property_ui_updater->add_editor(slider_vu);
				property_editors.push_back(slider_vu);
				slider_vu->set_changed_by_editor_callback(this,&BlockListUIList::property_editor_property_edited_callback);
				*/
				
			}
		END_SWITCH
				
		bui->blocklist_ui_under_cursor_request_signal.connect(this, &EditViewFrame::blocklist_ui_under_cursor_request_slot); 
										
		for (int j=0;j<editor->get_song()->get_track(i)->get_visible_automation_count();j++) {

			
			Automation *a=editor->get_song()->get_track(i)->get_visible_automation(j);
			
			SideWidget * sw = hbc->add( new SideWidget(SideWidget::TYPE_AUTOMATION, a->get_property()->get_caption() ) );
			
			EditViewAutomation *au_wg=hbc->add( new EditViewAutomation(editor,a) );
			edit_blocklists.push_back( au_wg );
			au_wg->blocklist_ui_under_cursor_request_signal.connect(this, &EditViewFrame::blocklist_ui_under_cursor_request_slot); 
			
			//new BlockList_Separator(track_hb,QStrify(a->get_property()->get_caption()));
		}
	}	
	hbc->add( new BlackWidget, 1);
	hbc->show();
	
}
	
EditViewFrame::EditViewFrame(GUI_UpdateNotify *p_notifier,Editor *p_editor) {
	
	editor=p_editor;
	
	
	
	toolbar = add( new EditViewToolbar(p_notifier,p_editor) );
	
	viewport = add( new ScrollBox,1  );
	
	
	viewport->set_scroll_h(true);
	viewport->set_scroll_v(false);
	viewport->set_expand_h(true);
	viewport->set_expand_v(true);
	
	hbc_base = viewport->set( new HBoxContainer );
	//hbc_base = add( new HBoxContainer,1 );
	hbc=NULL;
	
	p_notifier->edit_window_moved_signal.connect(this,&EditViewFrame::edit_window_moved_slot);
	p_notifier->edit_window_changed_signal.connect(this,&EditViewFrame::repaint);
	p_notifier->block_layout_changed_signal.connect(this,&EditViewFrame::repaint);
	p_notifier->update_blocklist_list_signal.connect(this,&EditViewFrame::repaint_blocklists);
	p_notifier->cursor_changed_blocklist_signal.connect(this,&EditViewFrame::cursor_changed_blocklist);
	
}


EditViewFrame::~EditViewFrame()
{
}


}
