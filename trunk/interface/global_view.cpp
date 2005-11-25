//
// File: global_view.cpp
// Created by: User <Email>
// Created on: Sun Oct 16 20:40:48 2005
//

#include "global_view.h"
#include "engine/track_pattern.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
#include "ui_blocks/helpers.h"
namespace ReShaked {

//*TODO FIX WHEN BLOCKS ARE OUT OF THE AREA
//*TODO FIX PATTERN (audio actualy)
//*TODO MAKE RESIZING
//TOODO MAKE TRACK TYPE CREATION


	struct NewBlock {
		int blocklist;
		Tick pos;
		NewBlock(int p_list=0, Tick p_pos=0) { blocklist=p_list; pos=p_pos; }
	};

/******************* HELPERS *********************/

bool GlobalView::is_block_selected(int p_list_index,int p_block) {

	int id=p_list_index+p_block*MAX_LISTS;
	std::set<int>::iterator I = selection.find(id);
	return (I!=selection.end());
}

void GlobalView::add_block_to_selection(int p_list_index,int p_block) {

	int id=p_list_index+p_block*MAX_LISTS;
	std::set<int>::iterator I = selection.find(id);

	update();

	if (I==selection.end())
		selection.insert(id);

}

void GlobalView::remove_block_from_selection(int p_list_index,int p_block) {

	int id=p_list_index+p_block*MAX_LISTS;
	std::set<int>::iterator I = selection.find(id);

	update();

	if (I!=selection.end())
		selection.erase(I);

}

void GlobalView::clear_selection() {

	selection.clear();
	update();
}

BlockList* GlobalView::get_block_list(int p_idx) {

	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		if (blocks==p_idx)
			return t;
		blocks++;

		for (int j=0;j<t->get_automation_count();j++) {

			if (blocks==p_idx)
				return t->get_automation(j);
			blocks++;
		}
	}

	return NULL;

}
int GlobalView::get_block_list_count() {

	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		blocks++;

		for (int j=0;j<t->get_automation_count();j++) {

			blocks++;
		}
	}

	return blocks;
}
int GlobalView::get_block_list_at_offset(float p_offset) {

	float offset=0;
	int block=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);

		offset+=1;

		if (offset>=p_offset)
			return block;

		block++;

		for (int j=0;j<t->get_automation_count();j++) {

			offset+=0.5;
			if (offset>=p_offset)
				return block;

			block++;

		}
	}


	return -1;
}
float GlobalView::get_block_list_offset(int p_index) {

	float offset=0;
	int block=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		if (p_index==block)
			return offset;
		block++;
		offset+=1;


		for (int j=0;j<t->get_automation_count();j++) {

			if (p_index==block)
				return offset;
			block++;
			offset+=0.5;
		}
	}


	return offset; //total size
}

float GlobalView::get_block_list_width(BlockList *p_block) {

	if (dynamic_cast<Automation*>(p_block))
		return 0.5;
	else
		return 1;

}
QColor GlobalView::get_block_list_color(BlockList *p_block) {

	if (dynamic_cast<Automation*>(p_block))
		return QColor(255,255,0);
	else if (dynamic_cast<Track_Pattern*>(p_block))
		return QColor(89,255,78);
	else
		return QColor(255,55,78);

}

SkinBox *GlobalView::get_block_list_skinbox(BlockList *p_block,bool p_selected) {
	
	if (dynamic_cast<Automation*>(p_block))
		return VisualSettings::get_singleton()->get_skin_box( p_selected?SKINBOX_GLOBALVIEW_AUTOMATION_SELECTED:SKINBOX_GLOBALVIEW_AUTOMATION );
	else if (dynamic_cast<Track_Pattern*>(p_block))
		return VisualSettings::get_singleton()->get_skin_box( p_selected?SKINBOX_GLOBALVIEW_PATTERN_SELECTED:SKINBOX_GLOBALVIEW_PATTERN );
	else
		return VisualSettings::get_singleton()->get_skin_box( p_selected?SKINBOX_GLOBALVIEW_PATTERN_SELECTED:SKINBOX_GLOBALVIEW_PATTERN );
	
}
bool GlobalView::get_screen_to_blocklist_and_tick( int p_x, int p_y, int *p_blocklist, Tick *p_tick ) {


	/******* FIND BLOCKLISt ********/
	int which_blocklist=-1;
	int blcount=get_block_list_count();
	float ofs=0;
	for (int i=0;i<blcount;i++) {

		BlockList *blocklist=get_block_list(i);

		float f_from_x=ofs-display.ofs_x*display.zoom_width;
		float block_width=get_block_list_width(blocklist)*display.zoom_width;

		//printf("comparing %i to %i\n",(int)(f_from_x+block_width),p_x);
		if ((int)(f_from_x+block_width)>p_x) {
			which_blocklist=i;
			break;
		}

		ofs+=block_width;
	}


	if (which_blocklist==-1)
		return true; //not fund

	/******* FIND TICK ********/

	double tick=p_y/display.zoom_height+display.ofs_y;
	tick*=(double)TICKS_PER_BEAT;

//	printf("coords %i,%i, got block %i, beat %.2f\n",p_x,p_y,which_blocklist,(float)tick/(float)TICKS_PER_BEAT);

	if (tick<0)
		return true;

	*p_blocklist=which_blocklist;
	*p_tick=(Tick)tick;

	return false;
}

bool GlobalView::get_click_location( int p_x, int p_y, int *p_blocklist, int *p_block ) {

	int blocklist;
	Tick tick;

	if (get_screen_to_blocklist_and_tick(p_x,p_y,&blocklist,&tick))
		return true;

	/* find right block */
	BlockList *bl = get_block_list( blocklist );
	ERR_FAIL_COND_V(bl==NULL,true);

	int block_idx = bl->get_block_idx_at_pos( tick );

	if (block_idx<0)
		return true;

	printf("block %i, tick %.2f - blocks idx %i\n",blocklist,(float)tick/(float)TICKS_PER_BEAT,block_idx);

	*p_blocklist=blocklist;
	*p_block=block_idx;

	return false;

}

GlobalView::MouseData::BlockPositionAction GlobalView::get_block_position_action(int p_blocklist,Tick p_pos) {

	BlockList *bl = get_block_list( p_blocklist );
	if (!bl)
		return MouseData::POS_NOBLOCK;


	for (int i=0;i<bl->get_block_count();i++) {

		Tick begin= bl->get_block_pos( i );
		Tick end= begin+bl->get_block( i )->get_length();

		if ( p_pos>=begin && p_pos<end) {
			//the block we are looking for! now determine what to do..

			double dpixelpos_end=(double)(end-p_pos)/(double)TICKS_PER_BEAT;
			dpixelpos_end*=display.zoom_height;
			int pixelpos_end=(int)dpixelpos_end;

			if (pixelpos_end<=mouse_data.border_resize_grab_pixels)
				return MouseData::POS_RESIZE;
			else
				return MouseData::POS_OVER;

		}
	}


	return MouseData::POS_NOBLOCK;

}


void GlobalView::compute_moving_block_list() {

	std::set<int>::iterator I=selection.begin();
	int last_list;
	moving_block.moving_element_list.clear();
	moving_block.snap_to_beat=false;
	for (;I!=selection.end();I++) {

		int list=*I%MAX_LISTS;
		int block=*I/MAX_LISTS;

		last_list=list;
		BlockList *bl=get_block_list( list );
		if (bl->get_block_type()==BlockList::BLOCK_TYPE_FIXED_TO_BEAT)
			moving_block.snap_to_beat=true;


		MovingBlock::Element e;
		e.block=block;
		e.list=list;
		e.block_ptr=bl->get_block( block );
		moving_block.moving_element_list.push_back(e);
		printf("adding %i,%i\n",block,list);
	}

	moving_block.moving=true;
}


bool GlobalView::get_moving_block_pos_and_status(int p_list,int p_block,int &p_dst_list,Tick &p_dst_tick, float &p_free_x, bool &p_allowed) {

	BlockList *bl = get_block_list( p_list);
	if (!bl)
		return true;
	BlockList::Block *b=bl->get_block( p_block );
	if (!b)
		return true;

	/* Find offsets */

	float xofs=moving_block.current_x-mouse_data.click_x_from;
	float yofs=moving_block.current_y-mouse_data.click_y_from;

	/* find tick */
	Tick tick =(Tick)(yofs*TICKS_PER_BEAT)+bl->get_block_pos( p_block );
	if (moving_block.snap_to_beat)
		tick=tick-(tick%TICKS_PER_BEAT);

	/* find track under */
	float under_find_x=get_block_list_offset(p_list)+xofs+get_block_list_width(bl)/2;
	int list_under=get_block_list_at_offset(under_find_x);
	BlockList *bl_under=NULL;
	bool can_move_to=false;

	if (list_under!=-1)
		bl_under=get_block_list( list_under );
	if (bl_under && bl_under->get_type_name()==bl->get_type_name()) {


		p_free_x=get_block_list_offset(list_under)*display.zoom_width-display.ofs_x;

		if (bl->get_block_type()==BlockList::BLOCK_TYPE_FIXED_TO_BEAT) {
			int current=-1;
			if (list_under==p_list && moving_block.operation==MovingBlock::OP_MOVE)
				current=p_block;

			if (bl_under->block_fits( tick, b->get_length(), current )  )
				can_move_to=true;
		} else
			can_move_to=true;


	} else {

		p_free_x=(get_block_list_offset(p_list)+xofs)*display.zoom_width-display.ofs_x;

	}

	p_allowed=can_move_to;
	if (tick<0)
		p_allowed=false;
	p_dst_list=list_under;
	p_dst_tick=tick;

	return false;
}

void GlobalView::commit_moving_block_list() {

	/* check that everything is movable */
	std::list<MovingBlock::Element>::iterator I=moving_block.moving_element_list.begin();

	for(;I!=moving_block.moving_element_list.end();I++) {

		int dst_list;
		Tick dst_tick;
		float free_x;
		bool allowed;

		if (get_moving_block_pos_and_status(I->list,I->block,dst_list,dst_tick,free_x,allowed))
			return; // Something invalid about this block
		if (!allowed)
			return; // Cant commit changes if a block is not allowed to move

	}

	/* If everythign is valid and movable, then go for it */

	I=moving_block.moving_element_list.begin();

	std::list<NewBlock> new_block_list;
	for(;I!=moving_block.moving_element_list.end();I++) {

		int dst_list;
		Tick dst_tick;
		float free_x;
		bool allowed;
		BlockList *src_bl=get_block_list( I->list );
		//search again as it may have moved!
		int src_block_index=src_bl->get_block_index(I->block_ptr);
		ERR_CONTINUE(src_block_index==-1);
		//would be very fucked up if it fails again
		ERR_FAIL_COND( get_moving_block_pos_and_status(I->list,src_block_index,dst_list,dst_tick,free_x,allowed) );



		switch (moving_block.operation) {

			case MovingBlock::OP_COPY: {

				int current=(I->list==dst_list)?src_block_index:-1;

				BlockList::Block *bl=src_bl->get_block( src_block_index );
				BlockList *dst_bl=get_block_list( dst_list );
				dst_bl->copy_block( bl, dst_tick, current);



			} break;

			case MovingBlock::OP_COPY_LINK: {

				int current=(I->list==dst_list)?src_block_index:-1;
				BlockList::Block *bl=src_bl->get_block( src_block_index );
				BlockList *dst_bl=get_block_list( dst_list );
				dst_bl->copy_block_link( bl, dst_tick, current);


			} break;
			case MovingBlock::OP_MOVE: {

				BlockList *dst_bl=get_block_list( dst_list );
				dst_bl->move_block( src_bl, src_block_index, dst_tick );

			} break;
		};

		new_block_list.push_back( NewBlock( dst_list, dst_tick) );

	}


	/* one last pass to reselect! block index changes for each insert so this hack is needed*/
	clear_selection(); //clear selection
	for (std::list<NewBlock>::iterator J=new_block_list.begin();J!=new_block_list.end();J++) {

		BlockList *bl=get_block_list( J->blocklist );
		ERR_CONTINUE(bl==NULL);
		int bindex=bl->get_block_idx_at_pos( J->pos ); //get back block index
		ERR_CONTINUE( bindex<0 );
		add_block_to_selection( J->blocklist, bindex );
	}
	
	resize_check_consistency();

}


void GlobalView::get_resizing_block_data(Tick* p_new_len,bool *allowed) {

	double yofs=resizing_block.current_y-mouse_data.click_y_from;
	Tick tick_diff=Tick(yofs*(double)TICKS_PER_BEAT);
	*p_new_len=resizing_block.len+tick_diff;

	if (*p_new_len<0) // zoom to minimum visible size
		*p_new_len=Tick((double)TICKS_PER_BEAT/display.zoom_height);

	BlockList *bl = get_block_list( resizing_block.list );

	if (bl->get_block_type()==BlockList::BLOCK_TYPE_FIXED_TO_BEAT) {

		if (*p_new_len<TICKS_PER_BEAT)
			*p_new_len=TICKS_PER_BEAT;

		*p_new_len=*p_new_len-(*p_new_len%TICKS_PER_BEAT);

	}

	if (bl->get_block_type()==BlockList::BLOCK_TYPE_FREE_MOVING)
		*allowed=true;
	else
		*allowed=bl->block_fits(resizing_block.from,*p_new_len,resizing_block.block);

}

void GlobalView::commit_resizing_block() {

    Tick new_len;
    bool allowed;
    get_resizing_block_data(&new_len,&allowed);
    if (!allowed) //nothing to do if not allowed
        return;

    BlockList *bl = get_block_list( resizing_block.list );
    switch(resizing_block.operation) {
	case ResizingBlock::RESIZE_CREATE_NEW: {

		bl->create_block(resizing_block.from,NULL);
		int idx=bl->get_block_idx_at_pos(resizing_block.from);
		ERR_FAIL_COND(idx<0);
		BlockList::Block *b = bl->get_block(idx);
		b->set_length(new_len);

	} break;
	case ResizingBlock::RESIZE: {

		bl->get_block(resizing_block.block)->set_length(new_len);
	} break;
    }

    resize_check_consistency();

}

/****************** WIDGET SIZE *************/

void GlobalView::resize_check_consistency() {
	
	if (get_pixel_h_offset()>(get_total_pixel_width()-width())) {
		int new_ofs=get_total_pixel_width()-width();
		if (new_ofs>0)
			set_pixel_h_offset( new_ofs );
	}
	
	resized_signal();
}

void GlobalView::resizeEvent( QResizeEvent * event ) {
	
	resize_check_consistency();
	
	
}



/***************** MOUSE *********************/

void GlobalView::mouseMoveEvent ( QMouseEvent * e ) {

	if (e->buttons()&Qt::LeftButton) { //draggingGlobalView(

		if (!mouse_data.selecting)
			return;

		if (moving_block.moving==false && resizing_block.resizing==false) {
			//being block moving
			mouse_data.deselecting=false; //when moving, you cant deselect!

			switch (mouse_data.action_at_selecting) {

				case MouseData::POS_NOBLOCK: {

					//dont bother
					mouse_data.selecting=false;
					return;
				} break;
				case MouseData::POS_OVER: {

					if (selection.empty()) { //
						mouse_data.selecting=false;
						return;

					}
					compute_moving_block_list();
					if (e->modifiers()&Qt::ShiftModifier) { //copy

						if (e->modifiers()&Qt::ControlModifier)
							moving_block.operation=MovingBlock::OP_COPY_LINK;
						else
							moving_block.operation=MovingBlock::OP_COPY;

					} else
						moving_block.operation=MovingBlock::OP_MOVE;
				} break;
				case MouseData::POS_RESIZE: {


					resizing_block.block=mouse_data.clicked_block;
					resizing_block.list=mouse_data.clicked_list;

					resizing_block.operation=ResizingBlock::RESIZE;
					resizing_block.resizing=true;
					resizing_block.len=get_block_list(mouse_data.clicked_list)->get_block(mouse_data.clicked_block)->get_length();
					resizing_block.from=get_block_list(mouse_data.clicked_list)->get_block_pos(mouse_data.clicked_block);
				} break;
			}
		}

		if (moving_block.moving) {

			moving_block.current_x=(float)e->x()/display.zoom_width+display.ofs_x;
			moving_block.current_y=(float)e->y()/display.zoom_height+display.ofs_y;
		}

		if (resizing_block.resizing)
			resizing_block.current_y=(float)e->y()/display.zoom_height+display.ofs_y;

		update();

	} else { //just moving overGlobalView(

		int blocklist=-1;
		Tick tick=-1;

		if (get_screen_to_blocklist_and_tick( e->x(), e->y(),&blocklist,&tick))
			return;
//		printf("travelling block %i, tick %.2f\n",blocklist,(float)tick/(float)TICKS_PER_BEAT);
		MouseData::BlockPositionAction a=get_block_position_action( blocklist , tick );

		switch (a) {

			case MouseData::POS_NOBLOCK:
			case MouseData::POS_OVER: {

				setCursor(Qt::ArrowCursor);

			} break;
			case MouseData::POS_RESIZE: {

				setCursor(Qt::SizeVerCursor);
			} break;

		};

	}

}

void GlobalView::mousePressEvent ( QMouseEvent * e ) {

	if (e->button()==Qt::RightButton) { //cancel any moving stuffGlobalView(

	//clear status!
		mouse_data.deselecting=false;
		mouse_data.selecting=false;
                resizing_block.resizing=false;
		moving_block.moving=false;
		moving_block.moving_element_list.clear();
		update();

		return;
	}


	if (e->button()!=Qt::LeftButton)
		return;

	mouse_data.click_x_from=(float)e->x()/display.zoom_width+display.ofs_x;
	mouse_data.click_y_from=(float)e->y()/display.zoom_height+display.ofs_y;

	mouse_data.clicked_list=-1;
	mouse_data.clicked_block=-1;
	int blocklist=-1,block=-1;

	if (e->modifiers()&Qt::ControlModifier && !(e->modifiers()&Qt::ShiftModifier)) {

		//printf("one\n");
		if (!get_click_location(e->x(),e->y(),&blocklist,&block))
			return; //no location, in this case this is good
		Tick tick;
		//printf("two\n");
		if (get_screen_to_blocklist_and_tick( e->x(), e->y(), &blocklist,&tick))
			return;
		//printf("three\n");
		BlockList *bl=get_block_list(blocklist);
		if (!bl)
			return;
		//printf("four\n");

		switch (bl->get_block_creation_behavior()) {

			case BlockList::BLOCK_CREATE_AND_MOVE: {



			} break;
			case BlockList::BLOCK_CREATE_AND_RESIZE: {


				resizing_block.from=tick;
				if (bl->get_block_type()==BlockList::BLOCK_TYPE_FIXED_TO_BEAT) {
					resizing_block.from=SNAP_TICK_TO_BEAT(resizing_block.from);
				}

				resizing_block.len=TICKS_PER_BEAT;
				resizing_block.list=blocklist;
				resizing_block.block=-1;
				resizing_block.operation=ResizingBlock::RESIZE_CREATE_NEW;
				resizing_block.current_y=(float)e->y()/display.zoom_height+display.ofs_y;
				mouse_data.selecting=true;
				resizing_block.resizing=true;

			} break;
		}

		update();
		return;

	}





	if (get_click_location(e->x(),e->y(),&blocklist,&block))
		return; //no location

	mouse_data.clicked_list=blocklist;
	mouse_data.clicked_block=block;

	if (is_block_selected(blocklist,block)) {

		mouse_data.deselecting=true;
		mouse_data.deselecting_block=block;
		mouse_data.deselecting_list=blocklist;
		mouse_data.shift_when_deselecting=e->modifiers()&Qt::ShiftModifier;
	} else {

		if (! (e->modifiers()&Qt::ShiftModifier))
			clear_selection();
		add_block_to_selection(blocklist,block);


	}

	/* process the selecting stuff */
	int ablocklist=-1;
	Tick atick=-1;

	if (get_screen_to_blocklist_and_tick( e->x(), e->y(),&ablocklist,&atick))
		return;
	MouseData::BlockPositionAction a=get_block_position_action( blocklist,atick);

	mouse_data.selecting=true;
	mouse_data.action_at_selecting=a;

}
void GlobalView::mouseReleaseEvent ( QMouseEvent * e ) {

	if (e->button()!=Qt::LeftButton)
		return;

	if (mouse_data.deselecting) {

		if (mouse_data.shift_when_deselecting) //actual deselect
			remove_block_from_selection( mouse_data.deselecting_list, mouse_data.deselecting_block  );
		else { //select only otherwise!
			clear_selection();
			add_block_to_selection(mouse_data.deselecting_list,mouse_data.deselecting_block);

		}
	}
	if (moving_block.moving) { //apply changes

		commit_moving_block_list();

        }
        if (resizing_block.resizing) {

            commit_resizing_block();
        }
	//clear status!
	mouse_data.deselecting=false;
	mouse_data.selecting=false;
	resizing_block.resizing=false;
	moving_block.moving=false;
	moving_block.moving_element_list.clear();
	update();

//	if (get_click_location(e->x(),e->y(),&blocklist,&block))
//		return; //no location


}

bool GlobalView::is_block_being_moved(int p_list,int p_block) {


	std::list<MovingBlock::Element>::iterator I=moving_block.moving_element_list.begin();
	for(;I!=moving_block.moving_element_list.end();I++) {
		if ( I->block == p_block && I->list == p_list )
			return true;

	}


	return false;
}

bool GlobalView::is_block_being_resized(int p_list,int p_block) {


	if (!resizing_block.resizing)
		return false;

	return (p_list==resizing_block.list && p_block==resizing_block.block);


}

/****************** PAINTING ********************/


void GlobalView::paint_name(QPainter&p, int p_blocklist,int p_ofs) {
	
	
	QString name;
	
	BlockList *b=editor->get_blocklist( p_blocklist);
	ERR_FAIL_COND(b==NULL);
	float des_width;

	if (dynamic_cast<Track*>(b)) {
		Track *t=dynamic_cast<Track*>(b);
		name=QStrify(t->get_name());		
		des_width=0.45;
	} else if (dynamic_cast<Automation*>(b)) {
		
		Automation * a=dynamic_cast<Automation*>(b);
		name=QStrify( a->get_property()->get_caption() );
		des_width=0.35;
	} else return; // ??
	
	int h=(int)(des_width*display.zoom_width);
	if (h>24)
		h=24; //dont let it be THAT big
	
	p.save(); //push painter data and matrix
	
	QFont f;
	f.setPixelSize(h); //will be vertical so..
	f.setBold(true);
	p.setFont(f);
	QFontMetrics m(p.font());
	
	p.rotate(90);
	p.setPen(QColor(0,0,0,200));
	p.drawText(6,-(p_ofs+m.descent()+1),name);
	if (p_blocklist==editor->get_current_blocklist())
		p.setPen(QColor(255,222,222,240));
	else
		p.setPen(QColor(255,255,255,240));
	p.drawText(5,-(p_ofs+m.descent()),name);
	p.restore();

	
	
}

void GlobalView::paint_block(QPainter& p,int p_x,int p_y,int p_list,int p_block,bool p_drawover,bool p_notpossible,Tick p_len_othersize) {

	int alpha=p_drawover?150:255;
	BlockList *blocklist=get_block_list(p_list);

	Tick block_len=0;
	if (p_len_othersize!=-1)
		block_len=p_len_othersize;
	else
		block_len=blocklist->get_block(p_block)->get_length();

	float f_height=((float)block_len/(float)(TICKS_PER_BEAT))*display.zoom_height;
	float f_width=get_block_list_width(blocklist)*display.zoom_width;

	
	SkinBox *sb;
	
	if (is_block_selected(p_list,p_block))
		sb=get_block_list_skinbox( blocklist, true );
	else
		sb=get_block_list_skinbox( blocklist, false );
		
	//p.fillRect(p_x,p_y,(int)f_width,(int)f_height,col);
	//p.setPen(QColor(255,255,255,alpha));
	//p.drawRect(p_x,p_y,(int)f_width,(int)f_height);
	
	sb->paint_into( p,p_x,p_y,(int)f_width,(int)f_height);
	if (p_notpossible) {

		QBrush b(QColor(255,255,255),Qt::DiagCrossPattern);
		p.fillRect(p_x,p_y,(int)f_width,(int)f_height,b);
	}

}

void GlobalView::paintEvent(QPaintEvent *pe) {

	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));

	float ofs=0;
	int blocklist_count=get_block_list_count();
	for (int i=0;i<blocklist_count;i++) {

		BlockList *blocklist=get_block_list(i);
		float f_from_x=ofs-display.ofs_x*display.zoom_width;
		float block_width=get_block_list_width(blocklist);
		float f_width=block_width*display.zoom_width;

		p.setPen(QColor(255,255,255));
		p.drawLine(f_from_x+f_width,0,f_from_x+f_width,height());
		for (int j=0;j<blocklist->get_block_count();j++) {


			if (is_block_being_moved(i,j) && moving_block.operation==MovingBlock::OP_MOVE)
				continue;
			if (is_block_being_resized(i,j))
				continue;
			float f_from_y=((float)blocklist->get_block_pos(j)/(float)(TICKS_PER_BEAT))*display.zoom_height-display.ofs_y*display.zoom_height;

			paint_block(p,(int)f_from_x,(int)f_from_y,i,j,false);
		}

		if (editor->get_current_track()==editor->get_blocklist_track( i) ) {
			/* selected, display it */
			
			p.setPen(QColor(255,0,0));
			p.drawRect((int)f_from_x,0,(int)f_width,height());
		}
		
		paint_name(p,i,(int)f_from_x);
		ofs+=f_width;

	}

	/* PaINT MOVING BLOCKs */
	std::list<MovingBlock::Element>::iterator I=moving_block.moving_element_list.begin();

	for(;I!=moving_block.moving_element_list.end();I++) {

		int dst_list;
		Tick dst_tick;
		float free_x;
		bool allowed;

		if (get_moving_block_pos_and_status(I->list,I->block,dst_list,dst_tick,free_x,allowed))
			continue;
		free_x-=display.ofs_x*display.zoom_width;
		float free_y=((double)dst_tick/(double)TICKS_PER_BEAT)*display.zoom_height-display.ofs_y;

		paint_block(p,(int)free_x,(int)free_y,I->list,I->block,true,!allowed);

	}

	/* PaINT REsIZING BLOCKs */ /* (MY sHIFT Is BROKEN, SERIOUsLY) */

	if (resizing_block.resizing) {

		Tick new_len;
		bool allowed;
		get_resizing_block_data( &new_len, &allowed);
		float ofs=get_block_list_offset( resizing_block.list )*display.zoom_width;
		float f_from_x=ofs-display.ofs_x*display.zoom_width;
		float 	f_from_y=((float)resizing_block.from/(float)(TICKS_PER_BEAT))*display.zoom_height-display.ofs_y*display.zoom_height;

		paint_block(p,(int)f_from_x,(int)f_from_y,resizing_block.list,resizing_block.block,true,!allowed,new_len);

	}

}


int GlobalView::get_total_pixel_width() {
	
	float width_accum=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		width_accum+=get_block_list_width( t);

		for (int j=0;j<t->get_automation_count();j++) {

			width_accum+=get_block_list_width(t->get_automation( j ));
			
		}
	}
	
	width_accum*=display.zoom_width;
	return (int)width_accum;		
}

int GlobalView::get_pixel_h_offset() {
	
	return (int)(display.ofs_x*display.zoom_width);	
}

void GlobalView::set_pixel_h_offset(int p_ofs) {
	
	display.ofs_x=(float)p_ofs/display.zoom_width;
	update();
		
}

int GlobalView::get_total_pixel_height() {
	
	int max_len_pixels=((double)editor->get_song_max_len()/(double)TICKS_PER_BEAT)*display.zoom_height;
	return max_len_pixels;
	
}

int GlobalView::get_pixel_v_offset() {
	
	return (int)(display.ofs_y*display.zoom_height);	
}	

void GlobalView::set_pixel_v_offset(int p_ofs) {
	
	display.ofs_y=(float)p_ofs/display.zoom_height;
	update();
	
}

GlobalView::GlobalView(QWidget *p_widget,Editor *p_editor) : QWidget(p_widget)
{
	// TODO: put constructor code here

	setBackgroundRole(QPalette::NoRole);
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	song=p_editor->get_song();
	editor=p_editor;

	display.zoom_width=40;
	display.zoom_height=10;
	display.ofs_x=0;
	display.ofs_y=0;

	mouse_data.border_resize_grab_pixels=2;
	mouse_data.deselecting=false;
	mouse_data.selecting=false;
	mouse_data.shift_when_deselecting=false;
	setMouseTracking(true);

	moving_block.moving=false;
	resizing_block.resizing=false;
}


GlobalView::~GlobalView()
{

	// TODO: put destructor code here
}


}
