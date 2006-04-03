//
// C++ Implementation: chionic_window
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chionic_window.h"
#include "ui_blocks/helpers.h"
#include "plugin_UIs/chionic_pixmaps/chionic_bg.xpm"
#include <Qt/qpainter.h>
#include <Qt/qpushbutton.h>
#include <Qt/qlabel.h>
#include <Qt/qfiledialog.h>
#include <Qt/qinputdialog.h>
#include <Qt/qmessagebox.h>

#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/sample_editor.h"
#include "engine/audio_control.h"
#include "dsp/sample_file.h"

#include "plugin_UIs/chionic_pixmaps/frame__bottom_left.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__bottom_right.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__bottom.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__left.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__right.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_begin.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_end.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_envlfo_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_envlfo.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_global_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_global.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_params_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_params.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_regions_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_regions.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_source_active.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__section_source.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__top_left.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__top_right.xpm"
#include "plugin_UIs/chionic_pixmaps/frame__top_separator.xpm"

#include "plugin_UIs/chionic_pixmaps/control_frame__bottom_left.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__bottom_right.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__bottom.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__left.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__right.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__top_left.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__top_right.xpm"
#include "plugin_UIs/chionic_pixmaps/control_frame__top.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__display.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__slider.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__slider_grabber.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__key_scaling_bg.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__range_begin.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__range_end.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__stereo_panner.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__stereo_panner_pos.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__quad_panner.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__quad_panner_pos.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__filter_display.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_bandpass_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_bandpass.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_hipass_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_hipass.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_lowpass_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_lowpass.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_notch_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_notch.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_off_active.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__filter_off.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__spinbox_label.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__spinbox_spin.xpm"

#include "plugin_UIs/chionic_pixmaps/controls__sens_curve.xpm"

#include "plugin_UIs/chionic_pixmaps/envelope__cubic_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__cubic.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__enabled_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__enabled.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__loop_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__loop.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__spin_label.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__susloop_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__susloop.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__scrollbar_grabber.xpm"
#include "plugin_UIs/chionic_pixmaps/controls__scrollbar_bg.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__display.xpm"

#include "plugin_UIs/chionic_pixmaps/envelope__button_cutoff_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_cutoff.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_depth_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_depth.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_pan_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_pan.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_pitch_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_pitch.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_reso_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_reso.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_volume_active.xpm"
#include "plugin_UIs/chionic_pixmaps/envelope__button_volume.xpm"

#include "plugin_UIs/chionic_pixmaps/lfo__sawdown_active.xpm"
#include "plugin_UIs/chionic_pixmaps/lfo__sawdown.xpm"
#include "plugin_UIs/chionic_pixmaps/lfo__sawup_active.xpm"
#include "plugin_UIs/chionic_pixmaps/lfo__sawup.xpm"
#include "plugin_UIs/chionic_pixmaps/lfo__sine_active.xpm"
#include "plugin_UIs/chionic_pixmaps/lfo__sine.xpm"
#include "plugin_UIs/chionic_pixmaps/lfo__square_active.xpm"
#include "plugin_UIs/chionic_pixmaps/lfo__square.xpm"

#include "plugin_UIs/chionic_pixmaps/source__clone_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__clone.xpm"
#include "plugin_UIs/chionic_pixmaps/source__copy_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__copy.xpm"
#include "plugin_UIs/chionic_pixmaps/source__edit_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__edit.xpm"
#include "plugin_UIs/chionic_pixmaps/source__load_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__load.xpm"
#include "plugin_UIs/chionic_pixmaps/source__movedown_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__movedown.xpm"
#include "plugin_UIs/chionic_pixmaps/source__moveup_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__moveup.xpm"
#include "plugin_UIs/chionic_pixmaps/source__new_osc_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__new_osc.xpm"
#include "plugin_UIs/chionic_pixmaps/source__new_samp_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__new_samp.xpm"
#include "plugin_UIs/chionic_pixmaps/source__paste_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__paste.xpm"
#include "plugin_UIs/chionic_pixmaps/source__remove_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__remove.xpm"
#include "plugin_UIs/chionic_pixmaps/source__replace_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__replace.xpm"
#include "plugin_UIs/chionic_pixmaps/source__save_pushed.xpm"
#include "plugin_UIs/chionic_pixmaps/source__save.xpm"

#include "plugin_UIs/chionic_pixmaps/source__list_bg.xpm"
#include "plugin_UIs/chionic_pixmaps/region__notes.xpm"
#include "plugin_UIs/chionic_pixmaps/region__note.xpm"

#include "plugin_UIs/chionic_pixmaps/region__button_add_active.xpm"
#include "plugin_UIs/chionic_pixmaps/region__button_add.xpm"
#include "plugin_UIs/chionic_pixmaps/region__button_remove_active.xpm"
#include "plugin_UIs/chionic_pixmaps/region__button_remove.xpm"
#include "plugin_UIs/chionic_pixmaps/region__button_select_active.xpm"
#include "plugin_UIs/chionic_pixmaps/region__button_select.xpm"
#include "plugin_UIs/chionic_pixmaps/region__blend.xpm"
#include "plugin_UIs/chionic_pixmaps/region__blend_active.xpm"

#include "plugin_UIs/chionic_pixmaps/layer__1_active.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__1.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__2_active.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__2.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__3_active.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__3.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__4_active.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__4.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__5_active.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__5.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__6_active.xpm"
#include "plugin_UIs/chionic_pixmaps/layer__6.xpm"

#include "plugin_UIs/chionic_pixmaps/global__matrix_node_select.xpm"
#include "plugin_UIs/chionic_pixmaps/global__matrix.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_add_active.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_add.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_bottom.xpm"
//#include "plugin_UIs/chionic_pixmaps/global__modulation_empty.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_fm_active.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_fm.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_label.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_off_active.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_off.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_ring_active.xpm"
#include "plugin_UIs/chionic_pixmaps/global__modulation_ring.xpm"


namespace ReShaked {


void ChionicWindow::main_page_select(int p_page) {
	
	
	switch (p_page) {
		
		case SECTION_SOURCE: {
			
			main_stack->setCurrentWidget(sources.main_vbox);			
			layers.layer_vb->hide();
						
		} break;
		case SECTION_GLOBAL: {
			
			main_stack->setCurrentWidget(global.main_vbox);			
			layers.layer_vb->hide();
			
		} break;
		case SECTION_REGIONS: {
			
			main_stack->setCurrentWidget(regions.main_hbox);			
			layers.layer_vb->show();
			
		} break;
		case SECTION_PARAMS: {
			
			main_stack->setCurrentWidget(params.main_vbox);
			layers.layer_vb->show();
		} break;
		case SECTION_ENVLFO: {
			
			main_stack->setCurrentWidget(envlfo.main_vbox);
			layers.layer_vb->show();
	
		} break;
		
	}
}

void ChionicWindow::filter_mode_select(int p_mode) {
	
	ERR_FAIL_INDEX(layers.selected,ChionicParams::MAX_LAYERS);
	
	ChionicParams::Layer::Parameters &p=chionic->get_params()->layer[layers.selected].params;
	p.filter.type.set(p_mode);
	params.filter_editor->update();	
}


void ChionicWindow::init_sources_page() {
	
	sources.main_vbox = new CVBox(main_stack);
	main_stack->addWidget( sources.main_vbox );
	
	CVBox *buttons_vb = new CVBox(sources.main_vbox);
	buttons_vb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed );
	CHBox *hb = new CHBox( buttons_vb );

	
	add_sources_button(hb,SourcesPage::ACTION_LOAD,source__load_xpm,source__load_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_REPLACE,source__replace_xpm,source__replace_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_NEW_SAMP,source__new_samp_xpm,source__new_samp_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_COPY,source__copy_xpm,source__copy_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_CLONE,source__clone_xpm,source__clone_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_MOVEUP,source__moveup_xpm,source__moveup_pushed_xpm);

	hb = new CHBox( buttons_vb );
	
	add_sources_button(hb,SourcesPage::ACTION_SAVE,source__save_xpm,source__save_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_EDIT,source__edit_xpm,source__edit_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_NEW_OSC,source__new_osc_xpm,source__new_osc_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_PASTE,source__paste_xpm,source__paste_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_REMOVE,source__remove_xpm,source__remove_pushed_xpm);
	add_sources_button(hb,SourcesPage::ACTION_MOVEDOWN,source__movedown_xpm,source__movedown_pushed_xpm);
	
	buttons_vb->layout()->setSpacing(5);
	
	(new QWidget(sources.main_vbox))->setFixedHeight(40);
	
	add_main_label("List of Sources",sources.main_vbox);
	begin_control_frame( sources.main_vbox );
	PixmapList::Skin skin;
	
	skin.skin_bg=&settings.list_bg;
	skin.font_height=16;
	skin.separator=4;
	skin.row_margin=5;
	skin.selected_bg_color=QColor(180,180,180);
	skin.font_color=QColor(230,240,250);
	skin.selected_font_color=QColor(0,0,0);
	skin.separator_color=QColor(180,180,180);
	
	sources.source_list = new PixmapList(control_frame_current(),skin);
	QObject::connect(sources.source_list,SIGNAL(item_selected_signal( int )),this,SLOT(source_selected( int )));
	
	end_control_frame();
	settings.current_frame_vb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	add_main_label("Preview",sources.main_vbox);
	begin_control_frame( sources.main_vbox );
	
	sources.preview_stack = new QStackedWidget(control_frame_current());
	sources.sample_viewer = new SampleViewer(sources.preview_stack);
	sources.preview_stack->addWidget(sources.sample_viewer);
	
	sources.oscil_viewer = new OscillatorDisplay(sources.preview_stack);	
	sources.preview_stack->addWidget(sources.oscil_viewer);
	
	sources.preview_stack->setMinimumHeight(80);
	
	end_control_frame();
	
}

void ChionicWindow::add_sources_button(QWidget *p_parent, SourcesPage::Action p_action,  char *p_pix[],  char *p_pix_pushed[]) {
	
	PixmapButton *pb = new PixmapButton(p_parent,PixmapButton::Skin( QPixmap( (const char**)p_pix ), QPixmap( (const char**)p_pix_pushed) ) );
	connect_bind_int(pb,SIGNAL(mouse_pressed_signal()),this,SLOT(sources_action( int )),p_action);
}

void ChionicWindow::source_selected(int p_which) {
	
	if (p_which<0) {
		
		sources.preview_stack->setCurrentWidget(sources.sample_viewer);
		sources.sample_viewer->set_sample_data(NULL); 
		return;
		
	}
	
	ERR_FAIL_INDEX(p_which,chionic->get_params()->global.sources.size());
	
	if ( chionic->get_params()->global.sources[p_which].type==ChionicParams::Source::TYPE_SAMPLE ) {
		
		sources.preview_stack->setCurrentWidget(sources.sample_viewer);
		sources.sample_viewer->set_sample_data( chionic->get_params()->global.sources[p_which].sample );
		
	} else {
		
		sources.preview_stack->setCurrentWidget(sources.oscil_viewer);
		sources.oscil_viewer->set_oscillator( chionic->get_params()->global.sources[p_which].osc );
		
	}
}

void ChionicWindow::sources_action(int p_action) {
	
	switch (p_action) {
		case SourcesPage::ACTION_LOAD: {
			
			QString file=QFileDialog::getOpenFileName ( this, "Open Sample/OSC",".", "Waveforms (*.wav *.au *.aif *.WAV);;Oscs (*.osc);;All Files (*)");
			
			if (file=="")
				break;
			
			/* try with sample */
			Sample *s=SampleFile::get_singleton()->load_sample( DeQStrify(file) );
			
			QString fname=file;
			
			if (fname.lastIndexOf("/")!=-1) {
		
				fname.remove(0,fname.lastIndexOf("/")+1);
			}

			
			if (!s)
				break; //some messagebox?
			
			
			ChionicParams::Source src;
			
			src.type=ChionicParams::Source::TYPE_SAMPLE;
			src.sample=s;
			src.name=DeQStrify(fname);
			
			AudioControl::mutex_lock();
			
			chionic->get_params()->global.sources.push_back(src);
			
			AudioControl::mutex_unlock();
			
			update_sources_list();
			
			sources.source_list->select_item( sources.source_list->get_item_count() -1 );
			source_selected( sources.source_list->get_item_count() -1 );
			
					
			
		} break;
		
		case SourcesPage::ACTION_SAVE: {
			
			
		} break;
		case SourcesPage::ACTION_REPLACE: {
			
			int source=sources.source_list->get_selected();
			if (source<0 ||	source >=chionic->get_params()->global.sources.size() )
				break;
			
			QString file=QFileDialog::getOpenFileName ( this, "Open Sample/OSC",".", "Waveforms (*.wav *.au *.aif *.WAV);;Oscs (*.osc);;All Files (*)");
			
			if (file=="")
				break;
			
			/* try with sample */
			Sample *s=SampleFile::get_singleton()->load_sample( DeQStrify(file) );
			
			QString fname=file;
			
			if (fname.lastIndexOf("/")!=-1) {
		
				fname.remove(0,fname.lastIndexOf("/")+1);
			}

			
			if (!s)
				break; //some messagebox?
			
			
			ChionicParams::Source src;
			
			src.type=ChionicParams::Source::TYPE_SAMPLE;
			src.sample=s;
			src.name=DeQStrify(fname);
			
			AudioControl::mutex_lock();
			
			chionic->get_params()->global.sources[source].clear();			
			chionic->get_params()->global.sources[source]=src;
			
			AudioControl::mutex_unlock();
			
			update_sources_list();
			
			sources.source_list->select_item( sources.source_list->get_item_count() -1 );
			source_selected( sources.source_list->get_item_count() -1 );
			
				
		} break;
		case SourcesPage::ACTION_EDIT: {
			
			
			int source=sources.source_list->get_selected();
			if (source<0 ||	source >=chionic->get_params()->global.sources.size() )
				break;
			
			ChionicParams::Source &src=chionic->get_params()->global.sources[source];
			
			if (src.type==ChionicParams::Source::TYPE_SAMPLE) {
				
				SampleEditorDialog *sed = new SampleEditorDialog(this,src.sample);
				sed->exec();
				delete sed;
			}
			
			
		} break;
				
		case SourcesPage::ACTION_NEW_SAMP: {
			
			bool ok=false;
			QStringList items;
			items << "Mono" << "Stereo";
			QString chans=QInputDialog::getItem ( this, "Channels", "New Sample Channels"	, items,  0, false,&ok);
			if (!ok)
				break;
			bool stereo=(chans=="Stereo"); //weird
			
			int len = QInputDialog::getInteger ( this, "Length", "Length (frames):", 1024, 1, 16*1024*1024, 1 ,&ok);
			
			if (!ok)
				break;
			
			Sample *s = new Sample(stereo?2:1,len);
			s->set_base_freq( 44100 ); //default
			
			ChionicParams::Source src;
			src.type=ChionicParams::Source::TYPE_SAMPLE;
			src.sample=s;
			src.name="New Sample";
		
			
			AudioControl::mutex_lock();
			
			chionic->get_params()->global.sources.push_back(src);
			
			AudioControl::mutex_unlock();
			
			update_sources_list();
			
			sources.source_list->select_item( sources.source_list->get_item_count() -1 );
			source_selected( sources.source_list->get_item_count() -1 );

			
		} break;
		case SourcesPage::ACTION_NEW_OSC: {
			
			
			Oscillator  *o = new Oscillator;
		
			ChionicParams::Source src;
			src.type=ChionicParams::Source::TYPE_OSC;
			src.osc=o;
			src.name="New Oscillator";
			
			AudioControl::mutex_lock();
			
			chionic->get_params()->global.sources.push_back(src);
			
			AudioControl::mutex_unlock();
			
			update_sources_list();
			
			sources.source_list->select_item( sources.source_list->get_item_count() -1 );
			source_selected( sources.source_list->get_item_count() -1 );

			
		} break;
		case SourcesPage::ACTION_COPY: {
			
			int source=sources.source_list->get_selected();
			if (source<0 ||	source >=chionic->get_params()->global.sources.size() )
				break;
			
			sources.clipboard.copy_from( chionic->get_params()->global.sources[source] );
			
			
		} break;
		case SourcesPage::ACTION_PASTE: {
			
			if (!sources.clipboard.osc && !sources.clipboard.sample)
				break; //empty
			
			int source=sources.source_list->get_selected();
			if (source<0 ||	source >=chionic->get_params()->global.sources.size() )
				break;
			
			AudioControl::mutex_lock();
			
			chionic->get_params()->global.sources[source].copy_from( sources.clipboard );
			
			AudioControl::mutex_unlock();
			
			update_sources_list();
			sources.source_list->select_item( source );
			source_selected( source );
			
		} break;
		case SourcesPage::ACTION_CLONE: {
			
			int source=sources.source_list->get_selected();
			if (source<0 ||	source >=chionic->get_params()->global.sources.size() )
				break;
			
			ChionicParams::Source src;
			src.copy_from( chionic->get_params()->global.sources[source] );
			src.name="Clone of " + src.name;
			
			AudioControl::mutex_lock();
			
			chionic->get_params()->global.sources.push_back(src);
			
			AudioControl::mutex_unlock();
			
			update_sources_list();
			
			sources.source_list->select_item( sources.source_list->get_item_count() -1 );
			source_selected( sources.source_list->get_item_count() -1 );
			
		} break;
		
		case SourcesPage::ACTION_REMOVE: {
			
			int source=sources.source_list->get_selected();
			if (source<0 ||	source >=chionic->get_params()->global.sources.size() )
				break;
			

			if (QMessageBox::question ( this, "Confirmation", "Remove '" +  QStrify(chionic->get_params()->global.sources[source].name) + "' ?",QMessageBox::Ok,QMessageBox::Cancel)!=QMessageBox::Ok)
				break;
			
			AudioControl::mutex_lock();
			
			for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
				
				ChionicParams::Layer::Regions &r=chionic->get_params()->layer[i].regions;
				
				for (int j=0;j<r.map.get_stream_size();j++) {
				
					int src_idx=r.map.get_index_value(j).source_idx;		
					if (src_idx==source) {
						
						r.map.erase_index(j);
						j--;
					} else if (src_idx>source) {
						
						
						r.map.get_index_value_w(j).source_idx--;							
					}
				}
				
			}
						  
			chionic->get_params()->global.sources.erase( chionic->get_params()->global.sources.begin() + source );
			
			AudioControl::mutex_unlock();
						  
			update_sources_list();
			
			source--;
			if (source<0)
				source=0;
			
			if (source>=chionic->get_params()->global.sources.size())
				source=chionic->get_params()->global.sources.size()-1;
				
				
			if (source>=0)
				sources.source_list->select_item( source );
			
			source_selected( source );
			
		} break;
				
		case SourcesPage::ACTION_MOVEUP: {
			
			int source=sources.source_list->get_selected();
			if (source<1 ||	source >=chionic->get_params()->global.sources.size() )
				break;
			
			AudioControl::mutex_lock();
			
			for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
				
				ChionicParams::Layer::Regions &r=chionic->get_params()->layer[i].regions;
				
				for (int j=0;j<r.map.get_stream_size();j++) {
				
					int src_idx=r.map.get_index_value(j).source_idx;		
					if (src_idx==source) {
						
						r.map.get_index_value_w(j).source_idx--;
						j--;
					} else if (src_idx==(source-1)) {
							
						r.map.get_index_value_w(j).source_idx++;
					}
				}
				
			}
			
						  
			ChionicParams::Source aux=chionic->get_params()->global.sources[source-1];
			chionic->get_params()->global.sources[source-1]=chionic->get_params()->global.sources[source];
			
			chionic->get_params()->global.sources[source]=aux;
			
			AudioControl::mutex_unlock();
					
			source--;
			if (source<0)
				source=0;
			
			update_sources_list();
			
			sources.source_list->select_item( source );
			
			source_selected( source );
			
			
		} break;
		case SourcesPage::ACTION_MOVEDOWN: {
			
			int source=sources.source_list->get_selected();
			if (source<0 ||	source >=(chionic->get_params()->global.sources.size()-1) )
				break;
			
			AudioControl::mutex_lock();
			
			for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
				
				ChionicParams::Layer::Regions &r=chionic->get_params()->layer[i].regions;
				
				for (int j=0;j<r.map.get_stream_size();j++) {
				
					int src_idx=r.map.get_index_value(j).source_idx;		
					if (src_idx==source) {
						
						r.map.get_index_value_w(j).source_idx++;
						j--;
					} else if (src_idx==(source+1)) {
							
						r.map.get_index_value_w(j).source_idx--;
					}
				}
				
			}
			
						  
			ChionicParams::Source aux=chionic->get_params()->global.sources[source+1];
			chionic->get_params()->global.sources[source+1]=chionic->get_params()->global.sources[source];
			
			chionic->get_params()->global.sources[source]=aux;
			
			AudioControl::mutex_unlock();
					
			source++;
			
			update_sources_list();
			
			sources.source_list->select_item( source );
			
			source_selected( source );
					
		} break;
	}
	
}

void ChionicWindow::update_sources_list() {
	
	sources.source_list->clear();
	regions.source->clear();
	
	for (int i=0;i<chionic->get_params()->global.sources.size();i++) {
		
		QString item = QStrify( chionic->get_params()->global.sources[i].name );
		sources.source_list->add_item( item );
		regions.source->add_item( item );
	}

	update_selected_region_source();
	
	
}


void ChionicWindow::init_global_page() {
	
	global.main_vbox = new CVBox(main_stack);
	main_stack->addWidget( global.main_vbox );
	
	add_main_label( "Modulation Matrix", global.main_vbox);
	
	begin_control_frame( global.main_vbox );
	
	CHBox *base_mod_hb = new CHBox( control_frame_current() );
	CHBox *mod_hb = new CHBox( base_mod_hb );
	mod_hb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	RouteEditor::Skin matrix_skin;
	matrix_skin.bg=QPixmap( (const char**) global__matrix_xpm);
	matrix_skin.cursor_over=QPixmap( (const char**) global__matrix_node_select_xpm);
	matrix_skin.line_bg=QColor(60,70,75);
	matrix_skin.line_connected=QColor(250,240,235);
	matrix_skin.node_size=QSize(20,20);
	
	global.route_edit = new RouteEditor(mod_hb,8,matrix_skin);
	QObject::connect(global.route_edit,SIGNAL(matrix_changed_signal()),this,SLOT(global_matrix_changed_slot()));
	CVBox *mod_vb = new CVBox(mod_hb);
	
	new PixmapLabel(mod_vb,QPixmap( (const char**)global__modulation_label_xpm) );
	
	for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
		
		CHBox * layer_hb = new CHBox(mod_vb);
		
		PixmapButtonGroup *button_group = new PixmapButtonGroup(this);
		global.modulation_modes.push_back(button_group);
		
		button_group->add_button( new PixmapButton(layer_hb, PixmapButton::Skin( QPixmap( (const char**)global__modulation_off_xpm), QPixmap( (const char**)global__modulation_off_active_xpm) ),  PixmapButton::TYPE_TOGGLE ) );
		
		button_group->add_button( new PixmapButton(layer_hb, PixmapButton::Skin( QPixmap( (const char**)global__modulation_add_xpm), QPixmap( (const char**)global__modulation_add_active_xpm) ),  PixmapButton::TYPE_TOGGLE ) );
	
		button_group->add_button( new PixmapButton(layer_hb, PixmapButton::Skin( QPixmap( (const char**)global__modulation_ring_xpm), QPixmap( (const char**)global__modulation_ring_active_xpm) ),  PixmapButton::TYPE_TOGGLE ) );

		button_group->add_button( new PixmapButton(layer_hb, PixmapButton::Skin( QPixmap( (const char**)global__modulation_fm_xpm), QPixmap( (const char**)global__modulation_fm_active_xpm) ),  PixmapButton::TYPE_TOGGLE ) );
		
		button_group->set_selected_index( 0 );
		
		
	}
	
	/* Fuck Qt, needs signal parameter binding, like gtkmm :( */
	QObject::connect(global.modulation_modes[0], SIGNAL(button_selected_signal(int) ),this,SLOT(global_layer_1_mode_selected( int ) ) );
	QObject::connect(global.modulation_modes[1], SIGNAL(button_selected_signal(int) ),this,SLOT(global_layer_2_mode_selected( int ) ) );
	QObject::connect(global.modulation_modes[2], SIGNAL(button_selected_signal(int) ),this,SLOT(global_layer_3_mode_selected( int ) ) );
	QObject::connect(global.modulation_modes[3], SIGNAL(button_selected_signal(int) ),this,SLOT(global_layer_4_mode_selected( int ) ) );
	QObject::connect(global.modulation_modes[4], SIGNAL(button_selected_signal(int) ),this,SLOT(global_layer_5_mode_selected( int ) ) );
	QObject::connect(global.modulation_modes[5], SIGNAL(button_selected_signal(int) ),this,SLOT(global_layer_6_mode_selected( int ) ) );
	
	new PixmapLabel(mod_vb,QPixmap( (const char**)global__modulation_bottom_xpm) );
	
	end_control_frame();
}

void ChionicWindow::global_update_matrix() {
	
	global.route_edit->clear_connections();
	
	for (int i=0;i<(ChionicParams::MAX_LAYERS+1);i++) {
		
		for (int j=0;j<chionic->get_params()->global.modulation_source[i].read_from.size();j++) {
			
			int dest=i+1;
			int read_from=chionic->get_params()->global.modulation_source[i].read_from[j];
			int source=(read_from<ChionicParams::MAX_LAYERS)?(read_from+1):0;
			
			global.route_edit->connect_nodes( source, dest );
			
		}
	}
	
	for (int i=0;i<ChionicParams::MAX_LAYERS;i++) {
		
		global.modulation_modes[i]->set_selected_index( chionic->get_params()->layer[i].modulation_mode );
	}
}

void ChionicWindow::global_layer_mode_selected(int p_layer,int p_mode) {
	
	ChionicParams::Layer::ModulationMode mode=ChionicParams::Layer::MODE_OFF;
	
	switch( p_mode ) {
		case 0: mode=ChionicParams::Layer::MODE_OFF; break;
		case 1: mode=ChionicParams::Layer::MODE_ON_ADD; break;
		case 2: mode=ChionicParams::Layer::MODE_RING; break;
		case 3: mode=ChionicParams::Layer::MODE_FM; break;
	}
	
	ERR_FAIL_INDEX(p_layer,ChionicParams::MAX_LAYERS);
	
	chionic->get_params()->layer[p_layer].modulation_mode=mode;
	
}

/* wish Qt had parameter binding to the signals.. */
void ChionicWindow::global_layer_1_mode_selected(int p_mode) { global_layer_mode_selected(0,p_mode); }
void ChionicWindow::global_layer_2_mode_selected(int p_mode) { global_layer_mode_selected(1,p_mode); }
void ChionicWindow::global_layer_3_mode_selected(int p_mode) { global_layer_mode_selected(2,p_mode); }
void ChionicWindow::global_layer_4_mode_selected(int p_mode) { global_layer_mode_selected(3,p_mode); }
void ChionicWindow::global_layer_5_mode_selected(int p_mode) { global_layer_mode_selected(4,p_mode); }
void ChionicWindow::global_layer_6_mode_selected(int p_mode) { global_layer_mode_selected(5,p_mode); }

void ChionicWindow::global_matrix_changed_slot() {
	
	/* Filling again all the synthesis routes, so this requieres locking */
	
	AudioControl::mutex_lock();
	
	//Clear all
	for (int i=0;i<(ChionicParams::MAX_LAYERS+1);i++) 
		chionic->get_params()->global.modulation_source[i].read_from.clear();
	
	//Fill it again
	for(int i=1;i<(ChionicParams::MAX_LAYERS+2);i++) { //layers, plus inputs/outputs
		
		
		for (int j=0;j<i;j++) {
	
			int layer=i-1;
			int source=(j==0)?ChionicParams::MAX_LAYERS:(j-1);
			
			if (!global.route_edit->are_connected( i,j))
				continue;
			
			chionic->get_params()->global.modulation_source[layer].read_from.push_back(source);
		}
	}	
	AudioControl::mutex_unlock();
	
	
}

void ChionicWindow::init_regions_page() {
		
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[0].regions;
	
	regions.main_hbox = new CHBox(main_stack);
	main_stack->addWidget( regions.main_hbox );
	
	
	CVBox *region_map_vbox = new CVBox(regions.main_hbox);
	add_main_label("Region Map",region_map_vbox);
	
	begin_control_frame(region_map_vbox);
	
	settings.current_frame_vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
			
	CVBox *region_map_vbox2 = new CVBox(control_frame_current());
	
	KeyboardRegionsEditor::Skin skin;
	skin.bg=QPixmap((const char**)region__notes_xpm);
	skin.border=QColor(255,255,255,150);
	skin.region=QColor(150,160,170,180);
	skin.region_selected=QColor(190,160,130,180);
	
	CHBox *region_mode_hbox = new CHBox( region_map_vbox2 );
	PixmapButtonGroup *region_mode_group = new PixmapButtonGroup(this);
	region_mode_group->add_button( new PixmapButton(region_mode_hbox, PixmapButton::Skin( QPixmap( (const char**)region__button_select_xpm), QPixmap( (const char**)region__button_select_active_xpm) ),  PixmapButton::TYPE_TOGGLE ) );
	
	region_mode_group->add_button( new PixmapButton(region_mode_hbox, PixmapButton::Skin( QPixmap( (const char**)region__button_add_xpm), QPixmap( (const char**)region__button_add_active_xpm) ),  PixmapButton::TYPE_TOGGLE ) );
	
	region_mode_group->add_button( new PixmapButton(region_mode_hbox, PixmapButton::Skin( QPixmap( (const char**)region__button_remove_xpm), QPixmap( (const char**)region__button_remove_active_xpm) ),  PixmapButton::TYPE_TOGGLE ) );
	
	region_mode_group->set_selected_index( 0 );
	QObject::connect( region_mode_group, SIGNAL(button_selected_signal( int )), this, SLOT(region_mode_changed( int ) ) ); 
	
	regions.region_map_editor = new KeyboardRegionsEditor( region_map_vbox2,skin );
	regions.region_map_editor->set_region_map( &r.map );
	connect( regions.region_map_editor, SIGNAL(mouse_over_note_signal( int ) ), this, SLOT(region_changed_note_over( int )));
	
	connect( regions.region_map_editor, SIGNAL(region_selected_signal( int )), this, SLOT(region_selected_changed(int)));
	
	regions.note_over = new PixmapLabel( region_map_vbox2, QPixmap( (const char**) region__note_xpm  ));
	regions.note_over->set_color( QColor(255,255,255) );
	
	end_control_frame();
		
	(new QWidget(regions.main_hbox))->setFixedWidth(20);
	
	CVBox *region_edit_vbox = new CVBox(regions.main_hbox);
	
	add_main_label("Region Edit",region_edit_vbox);
	
	
	begin_control_frame(region_edit_vbox);
	
	settings.current_frame_vb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	CVBox *region_edit_vbox2 = new CVBox(control_frame_current());
	
	add_control_label("Region",region_edit_vbox2);
	
	CHBox * current_reg_hb=  new CHBox (region_edit_vbox2);
	current_reg_hb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	regions.current_region = new PixmapLabel(current_reg_hb,settings.spin_bg);
	regions.current_region->set_text("---");	
	regions.current_updown = new PixmapUpDown(current_reg_hb,settings.updown_skin);
	QObject::connect( regions.current_updown, SIGNAL(up_pressed_signal()), this, SLOT(region_select_up()));
	QObject::connect( regions.current_updown, SIGNAL(down_pressed_signal()), this, SLOT(region_select_down()));
	
	add_control_label("Base Note",region_edit_vbox2);
	
	current_reg_hb=  new CHBox (region_edit_vbox2);
	current_reg_hb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	regions.current_note = new PixmapLabel(current_reg_hb,settings.spin_bg);
	regions.current_note->set_text("---");	
	regions.current_note_updown = new PixmapUpDown(current_reg_hb,settings.updown_skin);
	QObject::connect( regions.current_note_updown, SIGNAL(up_pressed_signal()), this, SLOT(region_base_note_select_up()));
	QObject::connect( regions.current_note_updown, SIGNAL(down_pressed_signal()), this, SLOT(region_base_note_select_down()));
	
	add_control_label("Source",region_edit_vbox2);
	
	CHBox *src_hb = new CHBox(region_edit_vbox2);
	src_hb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	PixmapList::Skin src_skin;
	
	src_skin.skin_bg=&settings.list_bg;
	src_skin.font_height=12;
	src_skin.separator=4;
	src_skin.row_margin=5;
	src_skin.selected_bg_color=QColor(180,180,180);
	src_skin.font_color=QColor(230,240,250);
	src_skin.selected_font_color=QColor(0,0,0);
	src_skin.separator_color=QColor(180,180,180);
	
	regions.source = new PixmapList(src_hb,src_skin);
	
	PixmapScrollBar::Skin scr_skin;
	scr_skin.bottom=&settings.scroll_bg;
	scr_skin.grabber=&settings.scroll_grab;
	regions.source->set_scrollbar(  new PixmapScrollBar(src_hb,scr_skin,PixmapScrollBar::TYPE_VERTICAL) );
	
	QObject::connect(regions.source,SIGNAL(item_selected_signal( int )),this,SLOT(region_source_selected( int )));
	
	
	end_control_frame();
	
	add_main_label("Blending",region_edit_vbox);
	
	begin_control_frame(region_edit_vbox);
	
	CHBox *blend_hbox = new CHBox( control_frame_current() );
	
	regions.blend_enable = new PixmapButton( blend_hbox, PixmapButton::Skin( QPixmap( (const char**) region__blend_xpm), QPixmap( (const char**) region__blend_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	QObject::connect( regions.blend_enable, SIGNAL(mouse_toggled_signal( bool )), this, SLOT(region_blend_toggle(bool)));
	regions.sens_curve_editor = new SensCurveEditor( blend_hbox, settings.sens_skin );
	regions.sens_curve_editor->set_property( &r.blending.curve );
	
	
	
	end_control_frame();
	
}

void ChionicWindow::region_blend_toggle(bool p_blend) {
	
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[ layers.selected ].regions;

	r.blending.enabled=p_blend;
}
void ChionicWindow::update_selected_region_source() {
	
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[ layers.selected ].regions;
	int current_region=regions.region_map_editor->get_selected_region();
	
		
	if (current_region<0 || current_region>=r.map.get_stream_size())
		return;
	int src_idx = r.map.get_index_value(current_region).source_idx;
	if ( src_idx<0 || src_idx>=regions.source->get_item_count() )
		return;
	regions.source->select_item( src_idx );
		
}

void ChionicWindow::region_source_selected(int p_source) {
	
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[ layers.selected ].regions;
	int current_region=regions.region_map_editor->get_selected_region();
	
	if (current_region<0 || current_region>=r.map.get_stream_size())
		return;
	
	r.map.get_index_value_w(current_region).source_idx=p_source;
	
}

void ChionicWindow::region_mode_changed(int p_to_mode) {
	
	switch (p_to_mode) {
		case 0: regions.region_map_editor->set_mode( KeyboardRegionsEditor::MODE_SELECT ); break;
		case 1: regions.region_map_editor->set_mode( KeyboardRegionsEditor::MODE_ADD ); break;
		case 2: regions.region_map_editor->set_mode( KeyboardRegionsEditor::MODE_REMOVE ); break;
	}
	
}

	
void ChionicWindow::region_selected_changed(int p_to_which) {
		
	if (p_to_which==-1) {
		
		regions.current_region->set_text("---");	
		regions.current_note->set_text("---");
		update_selected_region_source();	
		return;
	}
	
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[ layers.selected ].regions;
	
	ERR_FAIL_INDEX(p_to_which,r.map.get_stream_size());
	
	QString regnum = QString::number(p_to_which) + "/" + QString::number(r.map.get_stream_size()-1) + ":    ";
	if (p_to_which==(r.map.get_stream_size()-1)) {
		regions.current_region->set_text( regnum + get_note_string( r.map.get_index_pos(p_to_which) ) + " - end ");	
		
	} else {
		
		regions.current_region->set_text( regnum + get_note_string( r.map.get_index_pos(p_to_which) ) + " - " + get_note_string( r.map.get_index_pos(p_to_which+1) ) );	
		
	}
	regions.current_note->set_text( get_note_string( r.map.get_index_value(p_to_which).base_note ) );	

	update_selected_region_source();	
	
}

void ChionicWindow::region_select_up() {
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[ layers.selected ].regions;
	int current_region=regions.region_map_editor->get_selected_region();
	
	if (current_region<1 || current_region>=r.map.get_stream_size())
		return;
	
	regions.region_map_editor->set_selected_region( current_region -1 );
	region_selected_changed( current_region -1 );
	
}

void ChionicWindow::region_select_down() {
	
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[ layers.selected ].regions;
	int current_region=regions.region_map_editor->get_selected_region();
	
	if (current_region<0 || current_region>=(r.map.get_stream_size()-1))
		return;
	
	regions.region_map_editor->set_selected_region( current_region +1 );
	region_selected_changed( current_region +1 );
	
}

void ChionicWindow::region_base_note_select_up() {
	
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[ layers.selected ].regions;
	int current_region=regions.region_map_editor->get_selected_region();
	
	if (current_region<0 || current_region>=r.map.get_stream_size())
		return;
	
	
	if (r.map.get_index_value(current_region).base_note>0)
		r.map.get_index_value_w(current_region).base_note--;
	
	regions.current_note->set_text( get_note_string( r.map.get_index_value(current_region).base_note ) );	
	
}

void ChionicWindow::region_base_note_select_down() {
	
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[ layers.selected ].regions;
	int current_region=regions.region_map_editor->get_selected_region();
	
	if (current_region<0 || current_region>=r.map.get_stream_size())
		return;
	
	
	if (r.map.get_index_value(current_region).base_note<119)
		r.map.get_index_value_w(current_region).base_note++;
	
	regions.current_note->set_text( get_note_string( r.map.get_index_value(current_region).base_note ) );	
	
	
}

void ChionicWindow::region_changed_note_over(int p_to_note) {
	
	ChionicParams::Layer::Parameters &p=chionic->get_params()->layer[ layers.selected ].params;
	
	QString str=get_note_string(p_to_note);
	
	regions.note_over->set_text( str );
	
	
}

void ChionicWindow::init_envlfo_page() {
	
	ChionicParams::Layer::Parameters &p=chionic->get_params()->layer[0].params;
		
	envlfo.main_vbox = new CVBox(main_stack);
	main_stack->addWidget( envlfo.main_vbox );
	envlfo.main_vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	add_main_label("Parameter Select",envlfo.main_vbox);
	
	begin_control_frame( envlfo.main_vbox );
	
	CHBox *mode_hb = new CHBox( control_frame_current() );
	
	envlfo.parameter_edited = new PixmapButtonGroup(this);
	QObject::connect( envlfo.parameter_edited, SIGNAL(button_selected_signal( int )), this, SLOT(envlfo_parameter_selected(int)));
	
	envlfo.parameter_edited->add_button( new PixmapButton(mode_hb,PixmapButton::Skin(QPixmap((const char**)envelope__button_volume_xpm),QPixmap((const char**)envelope__button_volume_active_xpm)),PixmapButton::TYPE_TOGGLE ) ) ;
	
	envlfo.parameter_edited->add_button( new PixmapButton(mode_hb,PixmapButton::Skin(QPixmap((const char**)envelope__button_pitch_xpm),QPixmap((const char**)envelope__button_pitch_active_xpm)),PixmapButton::TYPE_TOGGLE ) ) ;
			
	envlfo.parameter_edited->add_button( new PixmapButton(mode_hb,PixmapButton::Skin(QPixmap((const char**)envelope__button_cutoff_xpm),QPixmap((const char**)envelope__button_cutoff_active_xpm)),PixmapButton::TYPE_TOGGLE ) ) ;
			
	envlfo.parameter_edited->add_button( new PixmapButton(mode_hb,PixmapButton::Skin(QPixmap((const char**)envelope__button_reso_xpm),QPixmap((const char**)envelope__button_reso_active_xpm)),PixmapButton::TYPE_TOGGLE ) ) ;

	envlfo.parameter_edited->add_button( new PixmapButton(mode_hb,PixmapButton::Skin(QPixmap((const char**)envelope__button_pan_xpm),QPixmap((const char**)envelope__button_pan_active_xpm)),PixmapButton::TYPE_TOGGLE ) ) ;
	
	PixmapButton *depth_select=new PixmapButton(mode_hb,PixmapButton::Skin(QPixmap((const char**)envelope__button_depth_xpm),QPixmap((const char**)envelope__button_depth_active_xpm)),PixmapButton::TYPE_TOGGLE );
	envlfo.parameter_edited->add_button( depth_select ) ;
	
	if (chionic->get_channels_created()==2)
		depth_select->hide();
		
	end_control_frame();
	
	add_main_label("Envelope",envlfo.main_vbox);
	
	begin_control_frame(envlfo.main_vbox);	
	
	CVBox *controls_vb = new CVBox(control_frame_current());
	
	EnvelopeEditor::Skin editor_skin;
	
	editor_skin.bg=QPixmap( (const char**) envelope__display_xpm );
	editor_skin.line_color=QColor(160,170,190);
	editor_skin.point_color=QColor(215,215,255);
	editor_skin.zero_color=QColor(150,150,150);
	editor_skin.border_size=1;
	
	
	envlfo.envelope_editor = new EnvelopeEditor(controls_vb,editor_skin);
	envlfo.envelope_editor->set_envelope( &p.volume.envelope );
	
	EnvelopeParamsEditor::Skin skin;
	skin.enabled_button = PixmapButton::Skin(QPixmap((const char**)envelope__enabled_xpm),QPixmap((const char**)envelope__enabled_active_xpm));
	skin.cubic_button = PixmapButton::Skin(QPixmap((const char**)envelope__cubic_xpm),QPixmap((const char**)envelope__cubic_active_xpm));
	skin.loop_button = PixmapButton::Skin(QPixmap((const char**)envelope__loop_xpm),QPixmap((const char**)envelope__loop_active_xpm));
	skin.susloop_button = PixmapButton::Skin(QPixmap((const char**)envelope__susloop_xpm),QPixmap((const char**)envelope__susloop_active_xpm));
	
	skin.spin_label=QPixmap((const char**)envelope__spin_label_xpm);
	skin.spin_updown=QPixmap((const char**)controls__spinbox_spin_xpm);
	
	skin.label_col=QColor(255,255,255);
	
	envlfo.envelope_params = new EnvelopeParamsEditor(controls_vb,skin);
	envlfo.envelope_params->set_envelope( &p.volume.envelope );
	
	QObject::connect(envlfo.envelope_editor,SIGNAL(envelopeChanged()),envlfo.envelope_params,SLOT(update_controls()));
	QObject::connect(envlfo.envelope_params,SIGNAL(envelope_changed_signal()),envlfo.envelope_editor,SLOT(update()));
	
	end_control_frame();
	
	add_main_label("LFO",envlfo.main_vbox);
	
	LFO_ParamsEditor::Skin lfo_skin;
	lfo_skin.display=settings.controls_display;
	lfo_skin.label=QColor(255,255,255);
	lfo_skin.slider=settings.controls_slider_skin;
	lfo_skin.button_sine=PixmapButton::Skin( QPixmap( (const char**) lfo__sine_xpm ), QPixmap( (const char**) lfo__sine_active_xpm ) );
	lfo_skin.button_sawup=PixmapButton::Skin( QPixmap( (const char**) lfo__sawup_xpm ), QPixmap( (const char**) lfo__sawup_active_xpm ) );
	lfo_skin.button_sawdown=PixmapButton::Skin( QPixmap( (const char**) lfo__sawdown_xpm ), QPixmap( (const char**) lfo__sawdown_active_xpm ) );
	lfo_skin.button_square=PixmapButton::Skin( QPixmap( (const char**) lfo__square_xpm ), QPixmap( (const char**) lfo__square_active_xpm ) );
	
	
	begin_control_frame(envlfo.main_vbox);	
	
	envlfo.lfo_params = new LFO_ParamsEditor(control_frame_current(),lfo_skin);
	envlfo.lfo_params->set_lfo( &p.volume.lfo );
	
	QObject::connect( envlfo.lfo_params, SIGNAL(lfo_changed_signal()), envlfo.envelope_editor, SLOT(update()) );
	
	envlfo.envelope_editor->set_lfo( &p.volume.lfo );
	
	end_control_frame();
	
	envlfo.selected_param=0;
	envlfo.parameter_edited->set_selected_index( 0 );
	
}

void ChionicWindow::envlfo_parameter_selected(int p_param) {
	
	
	ChionicParams::Layer::Parameters &p=chionic->get_params()->layer[layers.selected].params;
		
	switch (p_param) {
		
		case 0: 
			envlfo.envelope_editor->set_envelope( &p.volume.envelope );
			envlfo.envelope_params->set_envelope( &p.volume.envelope );			
			envlfo.envelope_editor->set_lfo( &p.volume.lfo );			
			envlfo.lfo_params->set_lfo( &p.volume.lfo );
		break;
		case 1: 
			envlfo.envelope_editor->set_envelope( &p.pitch.envelope );
			envlfo.envelope_params->set_envelope( &p.pitch.envelope );			
			envlfo.envelope_editor->set_lfo( &p.pitch.lfo );			
			envlfo.lfo_params->set_lfo( &p.pitch.lfo );
		break;
		case 2: 
			envlfo.envelope_editor->set_envelope( &p.filter.envelope_cutoff );
			envlfo.envelope_params->set_envelope( &p.filter.envelope_cutoff );			
			envlfo.envelope_editor->set_lfo( &p.filter.lfo_cutoff );			
			envlfo.lfo_params->set_lfo( &p.filter.lfo_cutoff );
		break;
		case 3: 
			envlfo.envelope_editor->set_envelope( &p.filter.envelope_resonance );
			envlfo.envelope_params->set_envelope( &p.filter.envelope_resonance );			
			envlfo.envelope_editor->set_lfo( &p.filter.lfo_resonance );			
			envlfo.lfo_params->set_lfo( &p.filter.lfo_resonance );
		break;
		case 4: 
			envlfo.envelope_editor->set_envelope( &p.pan.envelope );
			envlfo.envelope_params->set_envelope( &p.pan.envelope );			
			envlfo.envelope_editor->set_lfo( &p.pan.lfo );			
			envlfo.lfo_params->set_lfo( &p.pan.lfo );
		break;
		case 5: 
			envlfo.envelope_editor->set_envelope( &p.pan.envelope_depth );
			envlfo.envelope_params->set_envelope( &p.pan.envelope_depth );			
			envlfo.envelope_editor->set_lfo( &p.pan.lfo_depth );			
			envlfo.lfo_params->set_lfo( &p.pan.lfo_depth );
		break;
	}
	
	envlfo.selected_param=p_param;
	
}

void ChionicWindow::init_params_page() {
	
	ChionicParams::Layer::Parameters &p=chionic->get_params()->layer[0].params;
	
	
	params.main_vbox = new CVBox(main_stack);
	main_stack->addWidget( params.main_vbox );
	params.main_vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	params.main_hbox = new CHBox(params.main_vbox);
	
	CVBox *left_vbox = new CVBox(params.main_hbox);
	left_vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	add_main_label("Volume",left_vbox);
	
	begin_control_frame(left_vbox);	
			
	params.main_hbox->layout()->setAlignment(left_vbox,Qt::AlignTop);
	CVBox *controls_vb = new CVBox( control_frame_current() );
	
	
	params.vol_send=add_slider_edit_control( controls_vb, &p.volume.send,&params.vol_send_disp );
	
	
	add_control_label("Vel/Exp Range",controls_vb);
	params.vol_range = new RangeLevelEditor(controls_vb,settings.range_level_skin);
	params.vol_range->set_properties(&p.volume.velocity_range_begin_level,&p.volume.velocity_range_end_level,&p.volume.velocity_range_begin,&p.volume.velocity_range_end);
			
	add_control_label( QStrify(p.volume.pitch_scale.get_caption()), controls_vb );
	params.vol_pitch_scale=new KeyScalingEditor(controls_vb,settings.key_scaling_skin);
	params.vol_pitch_scale->set_property(&p.volume.pitch_scale);
			
	params.vol_random=add_slider_edit_control( controls_vb, &p.volume.random,&params.vol_random_disp );
	
	end_control_frame();
	
	add_main_label("Pan",left_vbox);

	begin_control_frame(left_vbox);	
	controls_vb = new CVBox( control_frame_current() );
	
	if (chionic->get_channels_created()==4) {
		
		CHBox *panner_hb = new CHBox(controls_vb);
		add_control_label("Pos ",panner_hb);
		params.pan_panner = new PannerEditor( panner_hb, true, settings.panner_skin );
		params.pan_panner->set_properties(&p.pan.pos,&p.pan.depth);
		
	} else {
		
		add_control_label("Position",controls_vb);
		params.pan_panner = new PannerEditor( controls_vb,false,settings.panner_skin );
		params.pan_panner->set_properties(&p.pan.pos,&p.pan.depth);
		
	}
			
	
	
	params.pan_random=add_slider_edit_control( controls_vb, &p.pan.random,&params.pan_random_disp );
	
	end_control_frame();
	
	/* separate ^^ */
	(new QFrame(params.main_hbox))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	CVBox *right_vbox = new CVBox(params.main_hbox);
	right_vbox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	params.main_hbox->layout()->setAlignment(right_vbox,Qt::AlignTop);
	
	add_main_label("Filter",right_vbox);
	
	begin_control_frame(right_vbox);	
			
	controls_vb = new CVBox( control_frame_current() );
	
	CHBox *filter_mode_hb = new CHBox(controls_vb);
	
	params.filter_mode_group = new PixmapButtonGroup(this);
	QObject::connect( params.filter_mode_group, SIGNAL(button_selected_signal( int )), this, SLOT(filter_mode_select( int )));
	
	params.filter_mode_off = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_off_xpm), QPixmap( (const char**)controls__filter_off_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_off );
		
	params.filter_mode_lp = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_lowpass_xpm), QPixmap( (const char**)controls__filter_lowpass_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_lp );
	
	params.filter_mode_bp = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_bandpass_xpm), QPixmap( (const char**)controls__filter_bandpass_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_bp );
	
	params.filter_mode_hp = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_hipass_xpm), QPixmap( (const char**)controls__filter_hipass_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_hp );
	
	params.filter_mode_br = new PixmapButton( filter_mode_hb, PixmapButton::Skin( QPixmap( (const char**)controls__filter_notch_xpm), QPixmap( (const char**)controls__filter_notch_active_xpm) ), PixmapButton::TYPE_TOGGLE );
	params.filter_mode_group->add_button( params.filter_mode_br );
	
	params.filter_editor = new FilterEditor(controls_vb, settings.filter_editor_skin );
	params.filter_editor->set_properties( &p.filter.type, &p.filter.cutoff, &p.filter.resonance );
	
	params.filter_velsens_cut=add_slider_edit_control( controls_vb, &p.filter.cutoff_velsens,&params.filter_velsens_cut_disp );
	
	params.filter_velsens_reso=add_slider_edit_control( controls_vb, &p.filter.resonance_velsens,&params.filter_velsens_reso_disp );
	
	params.filter_freqtrack=add_slider_edit_control( controls_vb, &p.filter.freq_tracking,NULL );
	
	params.filter_random=add_slider_edit_control( controls_vb, &p.filter.random,NULL );
	
	end_control_frame();
	
	add_main_label("Time",right_vbox);
	
	begin_control_frame(right_vbox);	
			
	controls_vb = new CVBox( control_frame_current() );
	
	params.time_delay=add_slider_edit_control( controls_vb, &p.time.delay,&params.time_delay_disp );
	params.time_random=add_slider_edit_control( controls_vb, &p.time.random,&params.time_random_disp );
	
	end_control_frame();
	
	add_main_label("Tune",params.main_vbox);
	begin_control_frame(params.main_vbox);	
	
	CHBox *controls_hb = new CHBox( control_frame_current() );
	params.tune_fine=add_slider_edit_control( controls_hb, &p.pitch.tune_fine,&params.tune_fine_disp );
	/* separate ^^ */
	(new QFrame(controls_hb))->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	params.tune_coarse=add_spin_edit_control(controls_hb,&p.pitch.tune_coarse,&params.tune_coarse_updown);
	
	end_control_frame();
	
}

PropertyEditor* ChionicWindow::add_slider_edit_control(QWidget *p_base,Property *p_prop,PropertyEditor** p_with_editbox) {
	
	CVBox *vb = new CVBox(p_base);
	
	vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	if (p_with_editbox) {
		
		CHBox *hb = new CHBox(vb);
		add_control_label( QStrify( p_prop->get_caption()), hb );
		*p_with_editbox = new PropertyEditLabel(hb,settings.controls_display);
		(*p_with_editbox)->set_property(p_prop);
		
	} else {
		
		add_control_label( QStrify( p_prop->get_caption()), vb );
		
	}
	
	PropertyEditor *spin = new PropertyEditSlider(vb,settings.controls_slider_skin,PixmapSlider::TYPE_HORIZONTAL,1,1);
	if (p_with_editbox)
		(*p_with_editbox)->add_to_group(spin);
	
	spin->set_property(p_prop);
	return spin;
}

PropertyEditor* ChionicWindow::add_spin_edit_control(QWidget *p_base,Property *p_prop,PropertyEditor** p_updown) {
	
	CVBox *vb = new CVBox(p_base);
	vb->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	add_control_label( QStrify( p_prop->get_caption()), vb );
	
	CHBox *hb = new CHBox(vb);
	
	PropertyEditor *label = new PropertyEditLabel(hb,settings.spin_bg);
	label->set_property( p_prop );
	
	
	*p_updown= new PropertyEditUpDown(hb,settings.updown_skin);
	(*p_updown)->add_to_group(label);
	
	(*p_updown)->set_property( p_prop );
	
	
	return label;
	
}

void ChionicWindow::begin_control_frame(QWidget *p_parent) {
	
	settings.current_frame_vb = new CVBox(p_parent);
	settings.current_frame_vb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	CHBox *aux_hb = new CHBox( settings.current_frame_vb );
	
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__top_left_xpm));
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__top_xpm),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__top_right_xpm));
	
	settings.current_frame_hb = new CHBox(settings.current_frame_vb);
	settings.current_frame_hb->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	new PixmapLabel(settings.current_frame_hb,QPixmap((const char**)control_frame__left_xpm),PixmapLabel::EXPAND_TILE_V);
	
	
}
void ChionicWindow::end_control_frame() {
	
	new PixmapLabel(settings.current_frame_hb,QPixmap((const char**)control_frame__right_xpm),PixmapLabel::EXPAND_TILE_V);
	
	CHBox *aux_hb = new CHBox( settings.current_frame_vb );
	
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__bottom_left_xpm));
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__bottom_xpm),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(aux_hb,QPixmap((const char**)control_frame__bottom_right_xpm));
}

QWidget* ChionicWindow::control_frame_current() {
	
	return settings.current_frame_hb;
}

void ChionicWindow::add_main_label(QString p_text,QWidget *p_parent) {
	
	QLabel *l = new QLabel(p_text,p_parent);
	QPalette p = l->palette();
	p.setColor(QPalette::Text,settings.main_label_color);
//	p.setColor(QPalette::WindowText,settings.main_label_color);
	l->setPalette(p);
	QFont f;
	f.setPixelSize(13);
	f.setBold(true);
	l->setFont(f);
	
}

void ChionicWindow::add_control_label(QString p_text,QWidget *p_parent) {
	
	QLabel *l = new QLabel(p_text,p_parent);
	QPalette p = l->palette();
	p.setColor(QPalette::Text,settings.control_label_color);
//	p.setColor(QPalette::WindowText,settings.control_label_color);
	l->setPalette(p);
	QFont f;
	f.setPixelSize(13);
	f.setBold(true);
	l->setFont(f);
	
}

void ChionicWindow::layer_selected_slot(int p_layer) {
	
	ERR_FAIL_INDEX(p_layer,ChionicParams::MAX_LAYERS);
	
	layers.selected=p_layer;
	
	/* REGION MAP */
	
	ChionicParams::Layer::Regions &r=chionic->get_params()->layer[p_layer].regions;
	
	regions.region_map_editor->set_region_map( &r.map );
	regions.region_map_editor->set_selected_region( -1 );
	region_selected_changed( regions.region_map_editor->get_selected_region() );
	
	
	regions.blend_enable->set_pressed( r.blending.enabled );
	regions.sens_curve_editor->set_property( &r.blending.curve );

	/* PARAMS */
	
	ChionicParams::Layer::Parameters &p=chionic->get_params()->layer[p_layer].params;
		
	
	params.vol_send->set_property( &p.volume.send );
	params.vol_send_disp->set_property( &p.volume.send );
	
	params.vol_range->set_properties(&p.volume.velocity_range_begin_level,&p.volume.velocity_range_end_level,&p.volume.velocity_range_begin,&p.volume.velocity_range_end);
	
	params.vol_pitch_scale->set_property(&p.volume.pitch_scale);
	
	params.vol_random->set_property( &p.volume.random );
	params.vol_random_disp->set_property( &p.volume.random );
	
	params.pan_panner->set_properties(&p.pan.pos,&p.pan.depth);
	
	params.pan_random->set_property( &p.pan.random );
	params.pan_random_disp->set_property( &p.pan.random );
	
	params.filter_mode_group->set_selected_index( p.filter.type.get_current() );
	
	params.filter_editor->set_properties( &p.filter.type, &p.filter.cutoff, &p.filter.resonance );
	
	params.filter_velsens_cut->set_property( &p.filter.cutoff_velsens );
	params.filter_velsens_cut_disp->set_property( &p.filter.cutoff_velsens );
	
	params.filter_velsens_reso->set_property( &p.filter.resonance_velsens );
	params.filter_velsens_reso_disp->set_property( &p.filter.resonance_velsens );
	
	params.filter_freqtrack->set_property( &p.filter.freq_tracking );
	
	params.filter_random->set_property( &p.filter.random );
	
	params.time_delay->set_property( &p.time.delay );
	params.time_delay_disp->set_property( &p.time.delay );
	
	params.time_random->set_property( &p.time.random );
	params.time_random_disp->set_property( &p.time.random );
	
	params.tune_fine->set_property( &p.pitch.tune_fine );
	params.tune_fine_disp->set_property( &p.pitch.tune_fine );
	
	params.tune_coarse->set_property( &p.pitch.tune_coarse );
	params.tune_coarse_updown->set_property( &p.pitch.tune_coarse );
	
	/* Envelope */
	
	envlfo_parameter_selected(envlfo.selected_param);
	
}

void ChionicWindow::layer_copy_slot() {
	
		
}
void ChionicWindow::layer_paste_slot() {
	
	
	
}


ChionicWindow::ChionicWindow(QWidget *p_parent,Chionic *p_chionic) : QDialog(p_parent) {
	
	chionic=p_chionic;
	layers.selected=0;
	
	settings.bg_color = QColor(0x59,0x5d,0x72);
	settings.main_label_color = QColor(0xD0,0xD0,0xD0);
	settings.control_label_color = QColor(0xFF,0xFF,0xFF);
	settings.controls_display = QPixmap( (const char **) controls__display_xpm );
	settings.controls_slider_skin = PropertyEditSlider::Skin( QPixmap( (const char **) controls__slider_xpm ), QPixmap( (const char **) controls__slider_xpm ), QPixmap( (const char **) controls__slider_grabber_xpm ) );
	
	if (p_chionic->get_channels_created()==2) { //panner
		settings.panner_skin=PannerEditor::Skin( QPixmap( (const char **)controls__stereo_panner_xpm), QPixmap( (const char **)controls__quad_panner_pos_xpm), 1 );
	} else {
							
		settings.panner_skin=PannerEditor::Skin( QPixmap( (const char **)controls__quad_panner_xpm), QPixmap( (const char **)controls__quad_panner_pos_xpm), 1 );
			
	}
	settings.key_scaling_skin=KeyScalingEditor::Skin(QPixmap( (const char**)controls__key_scaling_bg_xpm),QColor(235,245,255,200),1);
	settings.range_level_skin=RangeLevelEditor::Skin( QPixmap( (const char**)controls__key_scaling_bg_xpm), QPixmap( (const char**) controls__range_begin_xpm), QPixmap( (const char**) controls__range_end_xpm),QColor(235,245,255,200),1);
	settings.filter_editor_skin = FilterEditor::Skin( QPixmap( (const char**) controls__filter_display_xpm ), QPixmap( (const char**) controls__filter_display_xpm ),QColor(235,245,255,200),1);
	settings.updown_skin=PixmapUpDown::Skin( QPixmap ( (const char**)controls__spinbox_spin_xpm ),QPixmap ( (const char**)controls__spinbox_spin_xpm ),QPixmap ( (const char**)controls__spinbox_spin_xpm ) );
	settings.spin_bg=QPixmap ( (const char**)controls__spinbox_label_xpm );
	settings.list_bg.load_from_xpm( (const char**)source__list_bg_xpm, 2,2,2,2);
	settings.list_bg.set_center_color( QColor(0,0,0) );
	settings.scroll_bg.load_from_xpm( (const char**)controls__scrollbar_bg_xpm, 2,2,2,2);
	settings.scroll_grab.load_from_xpm( (const char**)controls__scrollbar_grabber_xpm, 2,2,2,2, true);
	settings.sens_skin.bg=QPixmap( (const char **) controls__sens_curve_xpm );
	settings.sens_skin.border_w=1;
	settings.sens_skin.line_color=QColor(235,245,255,200);
	
		
	
	
	QPalette p=palette();
	p.setColor(QPalette::Background,settings.bg_color);
	setPalette(p);
	
	setLayout(new QVBoxLayout(this));
	
	/* TOP BOX */
	
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,frame__top_left_xpm);
	new PixmapLabel(hb_top,frame__section_begin_xpm);
	//section buttons
	button_group = new PixmapButtonGroup(this);
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_source_xpm),QPixmap((const char**)frame__section_source_active_xpm)),PixmapButton::TYPE_TOGGLE ) ) ;
	
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_global_xpm),QPixmap((const char**)frame__section_global_active_xpm)),PixmapButton::TYPE_TOGGLE ) );
	
	new PixmapLabel(hb_top,frame__top_separator_xpm,PixmapLabel::EXPAND_STRETCH_H);
	
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_regions_xpm),QPixmap((const char**)frame__section_regions_active_xpm)),PixmapButton::TYPE_TOGGLE ) );
	
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_params_xpm),QPixmap((const char**)frame__section_params_active_xpm)),PixmapButton::TYPE_TOGGLE ) );
	
	button_group->add_button( new PixmapButton(hb_top,PixmapButton::Skin(QPixmap((const char**)frame__section_envlfo_xpm),QPixmap((const char**)frame__section_envlfo_active_xpm)),PixmapButton::TYPE_TOGGLE ) );
	
	QObject::connect(button_group,SIGNAL(button_selected_signal( int ) ), this, SLOT(main_page_select( int ) )  );
	
	new PixmapLabel(hb_top,frame__section_end_xpm);
	new PixmapLabel(hb_top,frame__top_right_xpm);
	
	/* MIDDLE BOX */
	CHBox *hb_middle = new CHBox(this);
	layout()->addWidget(hb_middle);
	
	new PixmapLabel(hb_middle,frame__left_xpm,PixmapLabel::EXPAND_STRETCH_V);
	
	main_vbox = new CVBox(hb_middle);
	main_vbox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	
	/* layer stuff here */
	
	layers.layer_vb = new CVBox( main_vbox );
	
	add_main_label("Current Layer ",layers.layer_vb);
	
	begin_control_frame( layers.layer_vb );
	layers.layer_hb = new CHBox( control_frame_current() );

		
	layers.button_group = new PixmapButtonGroup(this);
	
	layers.button_group->add_button( new PixmapButton( layers.layer_hb, PixmapButton::Skin( QPixmap( (const char**) layer__1_xpm), QPixmap( (const char**) layer__1_active_xpm) ), PixmapButton::TYPE_TOGGLE ) );
	layers.button_group->add_button( new PixmapButton( layers.layer_hb, PixmapButton::Skin( QPixmap( (const char**) layer__2_xpm), QPixmap( (const char**) layer__2_active_xpm) ), PixmapButton::TYPE_TOGGLE ) );
	layers.button_group->add_button( new PixmapButton( layers.layer_hb, PixmapButton::Skin( QPixmap( (const char**) layer__3_xpm), QPixmap( (const char**) layer__3_active_xpm) ), PixmapButton::TYPE_TOGGLE ) );
	layers.button_group->add_button( new PixmapButton( layers.layer_hb, PixmapButton::Skin( QPixmap( (const char**) layer__4_xpm), QPixmap( (const char**) layer__4_active_xpm) ), PixmapButton::TYPE_TOGGLE ) );
	layers.button_group->add_button( new PixmapButton( layers.layer_hb, PixmapButton::Skin( QPixmap( (const char**) layer__5_xpm), QPixmap( (const char**) layer__5_active_xpm) ), PixmapButton::TYPE_TOGGLE ) );
	layers.button_group->add_button( new PixmapButton( layers.layer_hb, PixmapButton::Skin( QPixmap( (const char**) layer__6_xpm), QPixmap( (const char**) layer__6_active_xpm) ), PixmapButton::TYPE_TOGGLE ) );
	
	(new QWidget(layers.layer_hb))->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ); 
	
	QObject::connect(layers.button_group,SIGNAL(button_selected_signal( int )), this, SLOT(layer_selected_slot( int ) ));
	
	QObject::connect( new PixmapButton( layers.layer_hb, PixmapButton::Skin( QPixmap( (const char**) source__copy_xpm), QPixmap( (const char**) source__copy_pushed_xpm) ) ), SIGNAL(mouse_pressed_signal()), this, SLOT(layer_copy_slot()));
	QObject::connect( new PixmapButton( layers.layer_hb, PixmapButton::Skin( QPixmap( (const char**) source__paste_xpm), QPixmap( (const char**) source__paste_pushed_xpm) ) ), SIGNAL(mouse_pressed_signal()), this, SLOT(layer_paste_slot()));
	
	end_control_frame();
	
	main_stack = new QStackedWidget(main_vbox);
	
	
	new PixmapLabel(hb_middle,frame__right_xpm,PixmapLabel::EXPAND_STRETCH_V);
	
	/* BOTTOM BOX */
	CHBox *hb_bottom = new CHBox(this);
	layout()->addWidget(hb_bottom);
	
	new PixmapLabel(hb_bottom,frame__bottom_left_xpm);
	new PixmapLabel(hb_bottom,frame__bottom_xpm,PixmapLabel::EXPAND_STRETCH_H);
	new PixmapLabel(hb_bottom,frame__bottom_right_xpm);
	
	/* INIT PAGES */
	
	init_params_page();
	init_envlfo_page();
	init_regions_page();		
	init_sources_page();
	init_global_page();
	main_stack->setCurrentWidget(regions.main_hbox);
	
	
	layers.button_group->set_selected_index( 0 );
	hide();
	
	layout()->setMargin(0);
	layout()->setSpacing(0);
	setWindowTitle("Chionic 2");
}


ChionicWindow::~ChionicWindow()
{
	
	sources.clipboard.clear();
}


}
