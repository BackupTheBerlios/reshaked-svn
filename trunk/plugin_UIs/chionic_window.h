//
// C++ Interface: chionic_window
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCHIONIC_WINDOW_H
#define RESHAKEDCHIONIC_WINDOW_H

#include <Qt/qdialog.h>
#include <Qt/qpixmap.h>
#include <Qt/qstackedwidget.h>

#include "plugins/chionic.h"
#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_button.h"
#include "ui_blocks/property_editors.h"
#include "ui_blocks/key_scaling_editor.h"
#include "ui_blocks/range_level_editor.h"
#include "ui_blocks/panner_editor.h"
#include "ui_blocks/filter_editor.h"
#include "ui_blocks/envelope_editor.h"
#include "ui_blocks/envelope_params_editor.h"
#include "ui_blocks/lfo_params_editor.h"
#include "ui_blocks/pixmap_list.h"
#include "ui_blocks/sample_viewer.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class ChionicWindow : public QDialog, public SoundPlugin_MetaData {
	
	Q_OBJECT
	
			
			
	enum Section {
		SECTION_SOURCE,
		SECTION_GLOBAL,
		SECTION_REGIONS,
		SECTION_PARAMS,
		SECTION_ENVLFO,
	};
	
	PixmapButtonGroup *button_group;
	
	Chionic *chionic;
	CVBox *main_vbox;
	QStackedWidget *main_stack;
	
	struct SourcesPage {
		
		enum Action {
			
			ACTION_LOAD,
			ACTION_SAVE,
			ACTION_REPLACE,
			ACTION_EDIT,
			
			ACTION_NEW_SAMP,
			ACTION_NEW_OSC,
			ACTION_COPY,
			ACTION_PASTE,
			ACTION_CLONE,
			ACTION_REMOVE,					
			
			ACTION_MOVEUP,
			ACTION_MOVEDOWN,
			
		};
		
		CHBox *main_vbox;
		PixmapList *source_list;
		SampleViewer *sample_viewer;
		
	} sources;
	
	struct EnvLFO {
		
		CHBox *main_vbox;
		EnvelopeEditor *envelope_editor;
		EnvelopeParamsEditor *envelope_params;
		LFO_ParamsEditor *lfo_params;
		
	} envlfo;
	
	
	struct ParamsPage {
		
		CHBox *main_vbox;
		CHBox *main_hbox;
		
		PropertyEditor *vol_send;
		PropertyEditor *vol_send_disp;
		
		RangeLevelEditor *vol_range;
		
		PropertyEditor *vol_pitch_scale;
		
		PropertyEditor *vol_random;
		PropertyEditor *vol_random_disp;
		
		PannerEditor *pan_panner;
		
		PropertyEditor *pan_random;
		PropertyEditor *pan_random_disp;
		
		PixmapButton *filter_mode_off;
		PixmapButton *filter_mode_lp;
		PixmapButton *filter_mode_bp;
		PixmapButton *filter_mode_hp;
		PixmapButton *filter_mode_br;
		PixmapButtonGroup *filter_mode_group;
		
		FilterEditor *filter_editor;
		
		PropertyEditor *filter_velsens_cut;
		PropertyEditor *filter_velsens_cut_disp;
		PropertyEditor *filter_velsens_reso;
		PropertyEditor *filter_velsens_reso_disp;
		PropertyEditor *filter_freqtrack;
		
		PropertyEditor *filter_random;
		
		PropertyEditor *time_delay;
		PropertyEditor *time_delay_disp;
		PropertyEditor *time_random;
		PropertyEditor *time_random_disp;
		
		PropertyEditor *tune_fine;
		PropertyEditor *tune_fine_disp;
		PropertyEditor *tune_coarse;
		PropertyEditor *tune_coarse_updown;
		
	} params;
	
	struct Settings {
		
		QPixmap controls_display;
		QColor bg_color;
		QColor main_label_color;
		QColor control_label_color;
		PropertyEditSlider::Skin controls_slider_skin;
		KeyScalingEditor::Skin key_scaling_skin;
		RangeLevelEditor::Skin range_level_skin;
		PannerEditor::Skin panner_skin;
		FilterEditor::Skin filter_editor_skin;
		QPixmap spin_bg;
		PixmapUpDown::Skin updown_skin;
		SkinBox list_bg;
		
		CVBox *current_frame_vb;
		CHBox *current_frame_hb;
	} settings;
	
	struct Layers {
		
		int selected;
		
	} layers;
	
	void init_sources_page();
	void init_params_page();
	void init_envlfo_page();
	
	/* helpers */
	
	void begin_control_frame(QWidget *p_parent);
	QWidget* control_frame_current();
	void end_control_frame();
	
	void add_main_label(QString p_text,QWidget *p_parent);
	void add_control_label(QString p_text,QWidget *p_parent);
	
	PropertyEditor* add_slider_edit_control(QWidget *p_base,Property *p_prop,PropertyEditor** p_with_editbox=NULL);
	PropertyEditor* add_spin_edit_control(QWidget *p_base,Property *p_prop,PropertyEditor** p_updown);
	
	void add_sources_button(QWidget *p_parent, SourcesPage::Action p_action,  char *p_pix[],  char *p_pix_pushed[]);
	
private slots:	
	
	void update_sources_list();
	
	void source_selected(int p_which);
	void sources_action(int p_action);
	
	void filter_mode_select(int p_mode);
	void main_page_select(int p_page);
	
public:
	ChionicWindow(QWidget *p_parent,Chionic *p_chionic);
	~ChionicWindow();
};

}

#endif
