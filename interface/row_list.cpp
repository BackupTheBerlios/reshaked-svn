
#include "row_list.h"
#include "pattern_settings.h"
#include <qpainter.h>

namespace ReShaked {
//@ TODO move all this to smoe common place
#define INTERP_COLORVAL(m_from,m_to,m_idx,m_total) \
	(((int)m_from*((m_total-1)-m_idx)/m_total)+((int)m_to*m_idx/m_total))
#define INTERP_COLOR(m_from,m_to,m_idx,m_total) \
	QColor(INTERP_COLORVAL(m_from.red(),m_to.red(),m_idx,m_total),INTERP_COLORVAL(m_from.green(),m_to.green(),m_idx,m_total),INTERP_COLORVAL(m_from.blue(),m_to.blue(),m_idx,m_total))

	
void RowList::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	p.setFont(PatternSettings::get_singleton()->get_font());

	/* paint left borders */
	int margin=PatternSettings::get_singleton()->get_pattern_margin();
	QColor light=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_LIGHT);
	QColor bg=PatternSettings::get_singleton()->get_color(!hasFocus()?PatternSettings::COLOR_PATTERN_BG:PatternSettings::COLOR_PATTERN_BG_FOCUS);
	QColor shadow=PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_SHADOW);

	p.fillRect(0,0,width(),height(),bg);

	for (int i=0;i<margin;i++) {


		QColor actual_light=INTERP_COLOR(light,bg,i,margin);
		QColor actual_dark=INTERP_COLOR(shadow,bg,i,margin);

		/* dark first */
		p.setPen(actual_dark);
		p.drawLine(i,height()-1-i,width()-1-i*2,height()-1-i);
		p.drawLine(width()-1-i,i,width()-1-i,height()-1-i);

		/* light second */
		p.setPen(actual_light);
		p.drawLine(i,i,i,height()-1-i);
		p.drawLine(i,i,width()-i,i);


	}
	int rowsize=PatternSettings::get_singleton()->get_row_size();
	int lines_fit=(height()-margin*2)/rowsize;
	//update cursor, small hack to make code simpler

	QFontMetrics metrics(PatternSettings::get_singleton()->get_font());

	int fontwidth=metrics.maxWidth();
	int fontofs=metrics.ascent()+3;
	int columnwidth=fontwidth*6;

	for (int i=0;i<lines_fit;i++) {

		
		int textofs=PatternSettings::get_singleton()->get_border_margin()+i*rowsize+fontofs;
		int beat=cursor->get_snapped_window_beat(i);
		int subsnap=cursor->get_snapped_window_subsnap(i);
		
		if (subsnap==0) { //beat 
			
			int x=PatternSettings::get_singleton()->get_border_margin();
			int y=PatternSettings::get_singleton()->get_border_margin()+i*rowsize;
			
			p.fillRect(x,y,columnwidth,rowsize,PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_BEAT_HILITE));
			
		}
		
		p.setPen(shadow);
		p.drawLine(width()-PatternSettings::get_singleton()->get_border_margin(),margin+(i+1)*rowsize-1,width(),margin+(i+1)*rowsize-1);
		p.drawLine(0,margin+(i+1)*rowsize-1,PatternSettings::get_singleton()->get_border_margin()-1,margin+(i+1)*rowsize-1);
		p.setPen(light);
		p.drawLine(0,margin+(i+1)*rowsize,width()-PatternSettings::get_singleton()->get_border_margin(),margin+(i+1)*rowsize);
		p.setPen(bg);
		p.drawLine(width()-PatternSettings::get_singleton()->get_border_margin(),margin+(i+1)*rowsize,width(),margin+(i+1)*rowsize);

		p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_FONT));

		char beattext[4]={' ',' ',' ',0};
		if (beat>=100)
			beattext[0]='0'+(beat/100)%10;
		if (beat>=10)
			beattext[1]='0'+(beat/10)%10;
		beattext[2]='0'+beat%10;

		char subsnaptext[3]={' ',' ',0};
		subsnaptext[0]='0'+(subsnap/10)%10;
		subsnaptext[1]='0'+subsnap%10;
				
		if (subsnap==0) {
			p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_VOLUME));
			p.drawText(PatternSettings::get_singleton()->get_border_margin(),textofs,beattext);
		} else {
			p.setPen(PatternSettings::get_singleton()->get_color(PatternSettings::COLOR_PATTERN_FONT));
			p.drawText(PatternSettings::get_singleton()->get_border_margin()+fontwidth,textofs,subsnaptext);
		}

	}
	
	
}
	
	
RowList::RowList(QWidget *p_parent,Cursor *p_cursor) : QWidget(p_parent,"Row List") {

	cursor=p_cursor;
	QFontMetrics metrics(PatternSettings::get_singleton()->get_font());
	int fixedwidth=metrics.maxWidth()*3+PatternSettings::get_singleton()->get_border_margin()*2;
	printf("width = %i\n",fixedwidth);
	
	setFixedWidth(fixedwidth);
	setBackgroundMode(Qt::NoBackground);	
	show();	
};


};

