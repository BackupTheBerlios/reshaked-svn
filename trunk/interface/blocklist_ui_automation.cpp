//
// C++ Implementation: blocklist_ui_automation
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "blocklist_ui_automation.h"
#include "ui_blocks/visual_settings.h"
#include <Qt/QPainter>
#include <Qt/QMouseEvent>
#include <math.h>
#include "ui_blocks/helpers.h"
namespace ReShaked {


void BlockListUI_Automation::paint_name(QPainter &p) {
	
	
	QString aut_name=QStrify( automation->get_property()->get_caption() );
	int font_width=	VisualSettings::get_singleton()->get_pattern_font()->get_width();
	
	p.save(); //push painter data and matrix
	
	QFont f;
	f.setPixelSize(font_width); //will be vertical so..
	f.setBold(true);
	p.setFont(f);
	QFontMetrics m(p.font());
	
	p.rotate(90);
	p.setPen(QColor(255,255,255,100));
	p.drawText(font_width,-(width()-m.ascent()-5),aut_name);
	p.restore();
	
	
}


void BlockListUI_Automation::paint_value_text(QPainter &p,int p_x,int p_y,float p_val) {
	
	QFont f;
	f.setPixelSize(FONT_PIXEL_SIZE);
	f.setBold(true);
	p.setFont(f);
	QFontMetrics m(p.font());
	
	QString s=QStrify( automation->get_property()->get_text_value( automation->get_property()->get_value_from_coeff(p_val) ) );
	QRect brect=m.boundingRect(s);
	int w=brect.width();
	int h=brect.height();
	
	int x=p_x-(w/2);
	
	/* hacks to center it nicer */
	if ( (x+w)> width() )
		x=width()-w-5; 
	if (x<5) // small hack so text isnt directly at the left
		x=5;
	
	int y=p_y-h-10;
	if (y<0)
		y=p_y+10;
	
	y+=m.ascent();
	
	p.setPen(QColor(0,0,0));
	p.drawText(x+2,y+2,s);
	p.drawText(x-2,y-2,s);
	p.setPen(QColor(255,255,255));
	p.drawText(x,y,s);
	
	
	
}

void BlockListUI_Automation::paint_row_lines(QPainter &p) {

	int visible_rows=editor->get_cursor().get_window_size();
	int row_size=VisualSettings::get_singleton()->get_editing_row_height();

	for (int i=0;i<visible_rows;i++) {
		if (i==0) //dont want the first one!
			continue;
		
		Tick tick=editor->get_cursor().get_snapped_window_tick_pos(i);

		if ( (tick % TICKS_PER_BEAT)==0 ) {//beat
			
			int block=automation->get_prev_block_from_idx( tick );
			if ( block!=-1) {
				
				if (automation->get_block_pos(block)==tick)
					continue;
				
				if (automation->get_block_pos(block)+automation->get_block(block)->get_length()==tick)
					continue;
				
			}
			p.fillRect(0,i*row_size,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_BEAT_LINE));
		
		} else
			p.fillRect(0,i*row_size,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE));

		
		//p.drawRect(0,i*row_size,width(),0);


	}

}

void BlockListUI_Automation::paint_frames(QPainter& p,int p_from_row,int p_to_row) {
	
				
	int begin_pos=-1;
	int old_block_idx=-1;
	
	if (p_from_row==-1)
		p_from_row=0;

	if (p_to_row==-1)
		p_to_row=editor->get_cursor().get_window_size();
	
	p.setClipping(true);
	int row_size=VisualSettings::get_singleton()->get_editing_row_height();
	p.setClipRect ( 0,p_from_row*row_size,width(),(p_to_row+1)*row_size );
	
	int visible_rows=(p_to_row-p_from_row)+1;
	
	SkinBox *sb=VisualSettings::get_singleton()->get_skin_box( hasFocus() ? SKINBOX_EDITING_AUTOMATION_SELECTED : SKINBOX_EDITING_AUTOMATION );

	for (int i=0;i<(visible_rows+3);i++) {
		Tick tick=editor->get_cursor().get_snapped_window_tick_pos(p_from_row+i);
		int block_idx=automation->get_block_idx_at_pos(tick);

		bool begin=(block_idx!=-1 && automation->get_block_pos(block_idx)==tick);

		if (block_idx!=-1 && i==0 && !begin) {

			begin_pos=-1;
		}

		if (i==0)
			old_block_idx=block_idx;

		bool end=((old_block_idx!=block_idx || i==(visible_rows+2)) && old_block_idx!=-1);
		old_block_idx=block_idx;

		if (end) {

			int begin_h=begin_pos*row_size+p_from_row*row_size;
			int len_h=(i-begin_pos)*row_size+p_from_row*row_size;

			sb->paint_into( p, 0,begin_h, width(), len_h );

		}

		if (begin)
			begin_pos=i;

	}

	if(hasFocus()) {

		p.setPen( VisualSettings::get_singleton()->get_color( COLORLIST_PATTERN_EDIT_FOCUS_RECT ) );
		p.drawRect(0,0,width()-1,height()-1);
	}

	p.setClipping(false);

}

void BlockListUI_Automation::paint_envelopes(QPainter &p,int p_from_row, int p_to_row) {

	
	if (p_from_row==-1)
		p_from_row=0;

	if (p_to_row==-1)
		p_to_row=editor->get_cursor().get_window_size()+1;
	
	Tick from_tick=editor->get_cursor().get_snapped_window_tick_pos(p_from_row);
	Tick to_tick=editor->get_cursor().get_snapped_window_tick_pos(p_to_row);
	Tick ticks=to_tick-from_tick;
	
	int font_width=	VisualSettings::get_singleton()->get_pattern_font()->get_width();
	
	int row_height=VisualSettings::get_singleton()->get_editing_row_height();
	int rows=p_to_row-p_from_row;
	int lines=rows*row_height;
	int line_from=p_from_row*row_height;

	float prev=-1;
	
	QPen pen(VisualSettings::get_singleton()->get_color( COLORLIST_AUTOMATION_ENVELOPE ));
	pen.setWidth(2);
	p.setPen(pen);
	
	//p.setRenderHint(QPainter::Antialiasing,true);
	
	
	//p.setClipping(true);
	//p.setClipRect ( ,4,width()-8,height()-8);
	
	for (int i=0;i<lines;i++) { //paint an extra one, even if not editable
		
		int line=i+line_from;
		Tick tick=(Tick)i*ticks/(Tick)lines;
		tick+=from_tick;
		
		float val=automation->get_tick_val(tick);
		if (val<0) {
			prev=-1;
			continue; //cant be drawn
		}
		
		if (prev==-1)
			prev=val; //not much to do, use the same
		
		int x_ofs=font_width+lrintf(val*(float)(width()-font_width*2));
		int x_ofs_prev=font_width+lrintf(prev*(float)(width()-font_width*2));
		p.drawLine(x_ofs_prev,line-1,x_ofs,line);
	
		prev=val;

	}
	
	//p.setRenderHint(QPainter::Antialiasing,false);
	//p.setClipping(false);
	
	/* Envelope Points */
	
	int from_block,to_block;
	if (automation->get_blocks_in_rage( from_tick, to_tick, &from_block, &to_block))
		return;
			
	QPixmap pointpixmap=VisualSettings::get_singleton()->get_pixmap( PIXMAP_ENVELOPE_POINT );
	for (int i=from_block;i<=to_block;i++) {
			
		Automation::AutomationData *d=automation->get_block( i )->get_data();
		Tick block_tick=automation->get_block_pos(i);
		Tick block_end=block_tick+automation->get_block(i)->get_length();
			
		for (int j=0;j<d->get_stream_size();j++) {
			
			Tick tick=block_tick+d->get_index_pos(j);
			float val=d->get_index_value(j).value;

			if (tick<from_tick || tick > to_tick)
				continue;
			if (tick>block_end)
				continue;
			
			int line=line_from+(tick-from_tick)*(lines)/(to_tick-from_tick); //convert to pixel
			int x_ofs=font_width+lrintf(val*(float)(width()-font_width*2));
			
			
			p.drawPixmap(x_ofs-pointpixmap.width()/2, line-pointpixmap.height()/2 ,pointpixmap);
			
			if (moving_point.moving && i==moving_point.block && j==moving_point.point) {
				/* paint text over it */
				paint_value_text( p, x_ofs, line, val );
				
			}
			
			if (!moving_point.moving && i==point_over.block && j==point_over.point) {
				
					paint_value_text( p, x_ofs, line, val );
				
			}
			
		}
		
	}
	
		
	
	
	

}
void BlockListUI_Automation::paintEvent(QPaintEvent *pe) {
	
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	paint_frames(p); //paint all by default
	paint_envelopes( p );
	paint_row_lines( p );
	paint_name( p);
	
}


/****************************************************************/
/****************************************************************/
/*************************LOGIC**********************************/
/****************************************************************/
/****************************************************************/
			

bool BlockListUI_Automation::find_closest_point(int p_x,int p_y,int p_radius, int *r_block, int *r_point) {
	
	
	Tick from_tick=editor->get_cursor().get_snapped_window_tick_pos(0);
	Tick to_tick=editor->get_cursor().get_snapped_window_tick_pos(editor->get_cursor().get_window_size());
	
	int from_block,to_block;
	if (automation->get_blocks_in_rage( from_tick, to_tick, &from_block, &to_block))
		return true;
	
	int row_height=VisualSettings::get_singleton()->get_editing_row_height();
	int rows=editor->get_cursor().get_window_size();
	int theight=rows*row_height;
	int font_width=	VisualSettings::get_singleton()->get_pattern_font()->get_width();
			
	
	for (int i=from_block;i<=to_block;i++) {
			
		Automation::AutomationData *d=automation->get_block( i )->get_data();
		Tick block_tick=automation->get_block_pos(i);
		Tick block_end=block_tick+automation->get_block(i)->get_length();
			
		for (int j=0;j<d->get_stream_size();j++) {
			
			Tick tick=block_tick+d->get_index_pos(j);
			float val=d->get_index_value(j).value;

			if (tick<from_tick || tick > to_tick)
				continue;
			if (tick>block_end)
				continue;
			
			
			int line=(tick-from_tick)*(theight)/(to_tick-from_tick); //convert to pixel
			int x_ofs=font_width+lrintf(val*(float)(width()-font_width*2));
			
			int dist=sqrt(POW2(line-p_y)+POW2(x_ofs-p_x));
			if (dist>p_radius)
				continue;
			
			*r_block=i;
			*r_point=j;
			return false;
		}
		
	}
	
	
	return true;
	
}


bool BlockListUI_Automation::screen_to_tick_and_val(int p_x,int p_y,Tick *p_tick, float *p_val) {
	
	if (p_y<0)
		p_y=0;
	if (p_x<0)
		p_x=0;
	
	/* Find Tick */
	
	int window_len=editor->get_cursor().get_window_size();
	Tick tick_begin=editor->get_cursor().get_snapped_window_tick_pos( 0);
	Tick window_len_ticks= editor->get_cursor().get_snapped_window_tick_pos( window_len ) -  tick_begin ;
	int row_size=VisualSettings::get_singleton()->get_editing_row_height();

	//I suppose this should be the most accurate way?
	*p_tick=(Tick)p_y*(window_len_ticks)/(Tick)(row_size*window_len);
	*p_tick+=tick_begin;
	
	/* Find Val */
	
	int font_width=	VisualSettings::get_singleton()->get_pattern_font()->get_width();
	int range=width()-font_width*2;
	
	*p_val=(float)(p_x-font_width)/(float)range;

	return false;
	
}

void BlockListUI_Automation::compute_point_over(int p_x,int p_y) {
	
	int closest_block,closest_point;
	
	if (find_closest_point(p_x,p_y,CLOSEST_POINT_RADIUS, &closest_block, &closest_point)) {
		
		closest_block=-1;
		closest_point=-1;
		
	}
	
	if (point_over.block!=closest_block || point_over.point!=closest_point) {
		point_over.block=closest_block;
		point_over.point=closest_point;
		update();
	}		
	
	
}

void BlockListUI_Automation::mouseMoveEvent ( QMouseEvent * e ) {
	
	
	/* track to point over */
	
	compute_point_over(e->x(),e->y());
	
	if (!moving_point.moving)
		return;
	
	Tick new_tick;
	float new_val;
	if (screen_to_tick_and_val(e->x(),e->y(),&new_tick,&new_val))
		return;
	

		
	new_tick-=automation->get_block_pos(moving_point.block); //convert to block relative
	

	
	Automation::AutomationData *d=automation->get_block(moving_point.block)->get_data();
	
	/* Adjust Tick Motion */
	
	if (new_tick<0)
		new_tick=0;
	
		
	if (new_tick>=automation->get_block(moving_point.block)->get_length())
		new_tick=automation->get_block(moving_point.block)->get_length()-1;
	
	
	if (moving_point.point>0 && d->get_index_pos( moving_point.point -1 ) >= new_tick )
		new_tick=d->get_index_pos( moving_point.point -1 )+1;
	

	if (moving_point.point<(d->get_stream_size()-1) && d->get_index_pos( moving_point.point +1 ) <= new_tick )
		new_tick=d->get_index_pos( moving_point.point +1 )-1;


	/* Adjust Value */
	
	if (new_val<0)
		new_val=0;
	if (new_val>1)
		new_val=1;
	
	
	float lfo_depth=d->get_index_value( moving_point.point ).lfo_depth;
	d->erase_index( moving_point.point );
	d->insert( new_tick, Automation::AutomationValue( new_val, lfo_depth ) );

	//dont conflict whenmoving point
	point_over.block=moving_point.block;
	point_over.point=moving_point.point;
	
	update();
	
}
void BlockListUI_Automation::mousePressEvent ( QMouseEvent * e ) {
	
	if (e->button()==Qt::RightButton && moving_point.moving ) {
	
		/* Cancel motion */
		
		Automation::AutomationData *d=automation->get_block(moving_point.block)->get_data();
		float lfo_depth=d->get_index_value( moving_point.point ).lfo_depth;
		d->erase_index( moving_point.point );
		if (!moving_point.adding) //if it was adding a point, simply remove it
			d->insert( moving_point.original_tick , Automation::AutomationValue( moving_point.original_value, lfo_depth ) );
		
		moving_point.moving=false;		

		update();
		return;
	} else if ( !moving_point.moving && ((e->button()==Qt::LeftButton && e->modifiers()&Qt::ControlModifier) || e->button()==Qt::RightButton )) {
		/* delete point */
		
		int closest_block,closest_point;
	
		if (find_closest_point(e->x(),e->y(),CLOSEST_POINT_RADIUS, &closest_block, &closest_point))
			return;
		
		Automation::AutomationData *d=automation->get_block(closest_block)->get_data();
		d->erase_index( closest_point );
		
		update();
		return;		
	} else if (e->button()!=Qt::LeftButton)
		return;
	
	
	int closest_block,closest_point;
	
	if (find_closest_point(e->x(),e->y(),CLOSEST_POINT_RADIUS, &closest_block, &closest_point)) {
		/* Nothing close, ADD! */
		
		
		Tick new_tick;
		float new_val;
		if (screen_to_tick_and_val(e->x(),e->y(),&new_tick,&new_val))
			return;
	
		moving_point.block=automation->get_block_idx_at_pos( new_tick );
		if (moving_point.block==-1)
			return;
		
		new_tick-=automation->get_block_pos(moving_point.block); //convert to block relative
		
		Automation::AutomationData *d=automation->get_block(moving_point.block)->get_data();
		
		while (d->get_exact_index( new_tick )>=0)
			new_tick--;
		
		if (new_tick<0)
			return; //cant add anything here
					
		if (new_val<0)
			new_val=0;
		
		if (new_val>1)
			new_val=1;
		
		
		///<@TODO interpolate LFOs
		d->insert( new_tick, Automation::AutomationValue( new_val, 0 ));
		
		moving_point.point=d->get_exact_index( new_tick );
		ERR_FAIL_COND( moving_point.point<0 ); //WTF?! just checking, should never happen though
		
		moving_point.original_tick=new_tick;		
		moving_point.original_value=new_val;
		moving_point.moving=true;		
		moving_point.adding=true;		
		update();
		
	} else {
	
		/* Point near pointer, GRAB */
		
		moving_point.block=closest_block;
		moving_point.point=closest_point ;
		moving_point.original_tick=automation->get_block(moving_point.block)->get_data()->get_index_pos( moving_point.point );
		moving_point.original_value=automation->get_block(moving_point.block)->get_data()->get_index_value( moving_point.point ).value;
		
		moving_point.moving=true;
		moving_point.adding=false;
				
	}

}
void BlockListUI_Automation::mouseReleaseEvent ( QMouseEvent * e ) {
	
	compute_point_over(e->x(),e->y());
	
	moving_point.moving=false;
	update();	
	
}



BlockListUI_Automation::BlockListUI_Automation(QWidget *p_parent, Editor *p_editor, int p_track,int p_automation) : BlockListUI_Base(p_parent) {
	
	editor = p_editor;
	automation=editor->get_song()->get_track(p_track)->get_automation(p_automation);
	
	setBackgroundRole(QPalette::NoRole);

	int fontwidth=VisualSettings::get_singleton()->get_pattern_font()->get_width();
	
	int awidth=2*fontwidth;
	
	switch(automation->get_display_size()) {
		
		case Automation::DISPLAY_SIZE_SMALL: {
				
			awidth+=fontwidth*3;	
		} break;
		case Automation::DISPLAY_SIZE_MEDIUM: {
			
			awidth+=fontwidth*6;	
			
		} break;
		case Automation::DISPLAY_SIZE_BIG: {
			
			awidth+=fontwidth*9;	
			
		} break;
	}
	
	setFixedWidth(awidth);
	
	moving_point.moving=false;
	setMouseTracking(true);
	point_over.block=-1;
	point_over.point=-1;
	
	
}


BlockListUI_Automation::~BlockListUI_Automation()
{
}


}
