//time
// C++ Implementation: bottom_bar_controls
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "bottom_bar_controls.h"

#include "visual_settings.h"
#include "get_time.h"
#include <Qt/qinputdialog.h>
#include "engine/sound_driver_list.h"

namespace ReShaked {


void BottomBarControls::action_notify(String p_action) {
	
	info_label->set_text(QStrify(p_action));
	tmp_msg_time=MSG_NOTIFY_TIME_MS;
}

void BottomBarControls::octave_edit_popup() {
	bool ok;
	
	int o=QInputDialog::getInteger ( this, "Editing Octave", "Octave:", editor->get_editing_octave(), 0, 8,1,&ok);
	if (ok)
		editor->set_editing_octave( o );
	
	octave_changed_slot();
}

void BottomBarControls::octave_raise() {
	
	editor->set_editing_octave( editor->get_editing_octave() + 1 );
	octave_changed_slot();
}
void BottomBarControls::octave_lower() {
	
	editor->set_editing_octave( editor->get_editing_octave() - 1 );
	octave_changed_slot();
	
}


void BottomBarControls::octave_changed_slot() {
	
	label_octave->set_text( QString::number( editor->get_editing_octave() ) );			
}


void BottomBarControls::update_time_label(QString p_str,bool p_play) {
	
	QColor col=p_play?GET_QCOLOR(COLORLIST_TIME_LABEL_PLAY):GET_QCOLOR(COLORLIST_TIME_LABEL);
	
	time_label->set_color(col);
	time_label->set_text(p_str);	
	
}

void BottomBarControls::time_update_slot() {
	
	
	/* CHECK CLOCK */
	
	bool playing;
	unsigned int time;
	if (editor->get_song()->get_song_playback().get_status()==SongPlayback::STATUS_PLAY) {
		
		time=GetTime::get_time_msec()-editor->get_song()->get_song_playback().get_song_playback_time();
		playing=true;
	} else {
		time=GetTime::get_time_msec();
		playing=false;
	}
	
	time/=1000; //seconds
	int secs=time%60;
	int mins=(time/60)%60;
	int hours=time/3600;
	
	char bufdigit[3]={0,0,0};
	QString timestr;
	
	timestr+=QString::number(hours)+":";
	bufdigit[0]='0'+mins/10;
	bufdigit[1]='0'+mins%10;
	timestr+=QString(bufdigit)+":";
	
	bufdigit[0]='0'+secs/10;
	bufdigit[1]='0'+secs%10;
	timestr+=bufdigit;
	
	if (timestr!=last_time)
		update_time_label(timestr,playing);
	
	last_time=timestr;
	
	if (tmp_msg_time>=0) { //cant update msg
		
		tmp_msg_time-=UPDATE_INTERVAL_MS;
		return;
	}
	
	
	QString message="Player Stopped";
	
	if (!SoundDriverList::get_singleton()->is_current_driver_active()) {
		
		message="**AUDIO DRIVER IS DISABLED**";
		
	} else if (playing) {
			
		Tick playpos=editor->get_song()->get_song_playback().get_playback_pos_tick();
		
		int beat=playpos/TICKS_PER_BEAT;
		int bar=editor->get_song()->get_bar_map().get_bar_at_beat( beat );
		int bar_beat=editor->get_song()->get_bar_map().get_bar_beat( beat );
		int mark_idx = editor->get_song()->get_marker_list().get_prev_index( beat );
		QString mark = (mark_idx>=0) ? QStrify( editor->get_song()->get_marker_list().get_index_value( mark_idx ) ) : "";
		
		message="Playing: " +mark + " at " + QString::number(bar) + ":"+ QString::number(bar_beat+1);
		
	}
	
	if (message!=last_msg) {
		
		info_label->set_text( message );
	}
	
	/*
	
	if (playpos_text!=last_msg)
			
	Tick playtick = 
	
	int playback_beat=editor->get_cursor().get_beat();
	int playback_subbeat=editor->get_cursor().get_pos()%editor->get_cursor().get_snap();
	int playback_bar=editor->get_cursor().get_pos()%editor->get_cursor().get_snap();
	
	
	*/
	
}


BottomBarControls::BottomBarControls(QWidget *p_parent,Editor *p_editor) : CHBox(p_parent)
{
	editor=p_editor;
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__BEGIN));
	
	
	rack_toggle = new PixmapButton( this, PixmapButton::Skin( GET_QPIXMAP(THEME_BOTTOM__RACK), GET_QPIXMAP(THEME_BOTTOM__RACK_ACTIVE)), PixmapButton::TYPE_TOGGLE );
	rack_toggle->set_ignore_actions( true );
	 
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__SEPARATOR));
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__INFORMATION_BEGIN));
	
	CVBox *info_vb = new CVBox(this);
	
	new PixmapLabel(info_vb,GET_QPIXMAP(THEME_BOTTOM__INFORMATION_LABEL_TOP), PixmapLabel::EXPAND_TILE_H);

	info_label = new PixmapLabel(info_vb,GET_QPIXMAP(THEME_BOTTOM__INFORMATION_LABEL),PixmapLabel::EXPAND_TILE_H);
	
	new PixmapLabel(info_vb,GET_QPIXMAP(THEME_BOTTOM__INFORMATION_LABEL_BOTTOM),PixmapLabel::EXPAND_TILE_H);
	
	info_vb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__INFORMATION_END));
	
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__SEPARATOR));
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__TIME_BEGIN));
	
	CVBox *time_vb = new CVBox(this);
	
	new PixmapLabel(time_vb,GET_QPIXMAP(THEME_BOTTOM__TIME_LABEL_TOP));

	time_label=new PixmapLabel(time_vb,GET_QPIXMAP(THEME_BOTTOM__TIME_LABEL));
	
	new PixmapLabel(time_vb,GET_QPIXMAP(THEME_BOTTOM__TIME_LABEL_BOTTOM));
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__TIME_END));
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__SEPARATOR));

	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__VPIANO_BEGIN));
	
	CVBox *vpiano_vb = new CVBox(this);
	
	
	new PixmapLabel(vpiano_vb,GET_QPIXMAP(THEME_BOTTOM__VPIANO_TOP),PixmapLabel::EXPAND_TILE_H);
	
	Piano_Keys::Skin skin(GET_QPIXMAP(THEME_BOTTOM__VPIANO_WHITE_KEY),GET_QPIXMAP(THEME_BOTTOM__VPIANO_WHITE_KEY_PRESSED),GET_QPIXMAP(THEME_BOTTOM__VPIANO_BLACK_KEY),GET_QPIXMAP(THEME_BOTTOM__VPIANO_BLACK_KEY_PRESSED));
	vpiano = new Piano_Keys(vpiano_vb, skin,editor);
	
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__VPIANO_OCTAVE_SEPARATOR));
	
	CVBox *octave_vb = new CVBox(this);
	new PixmapLabel(octave_vb,GET_QPIXMAP(THEME_BOTTOM__OCTAVE_TOP));
	CHBox *octave_hb = new CHBox(octave_vb);
	
	label_octave = new PixmapLabel(octave_hb,GET_QPIXMAP(THEME_BOTTOM__OCTAVE_LABEL));
	QObject::connect(label_octave,SIGNAL(clicked_signal()),this,SLOT(octave_edit_popup()));
	updown_octave = new PixmapUpDown(octave_hb,GET_QPIXMAP(THEME_BOTTOM__OCTAVE_SPIN));
	QObject::connect(updown_octave,SIGNAL(up_pressed_signal()),this,SLOT(octave_raise()));
	QObject::connect(updown_octave,SIGNAL(down_pressed_signal()),this,SLOT(octave_lower()));
	octave_changed_slot();
	
	new PixmapLabel(octave_vb,GET_QPIXMAP(THEME_BOTTOM__OCTAVE_BOTTOM));

	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__SEPARATOR));
	
	
	new PixmapLabel(this,GET_QPIXMAP(THEME_BOTTOM__END));
	
	
	
	time_updater = new QTimer(this);
	QObject::connect(time_updater ,SIGNAL(timeout()),this,SLOT(time_update_slot()));
	time_updater->start(80);
	tmp_msg_time=0;	
}


BottomBarControls::~BottomBarControls()
{
}


}
