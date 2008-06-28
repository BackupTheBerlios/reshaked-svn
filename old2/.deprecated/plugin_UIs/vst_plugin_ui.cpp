
#include "vst_plugin_ui.h"
#include "version.h"
#ifdef VST_ENABLED



#include <Qt/qpushbutton.h>
#include <Qt/qlayout.h>
#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_slider.h"
#include "ui_blocks/property_editors.h"
#include "ui_blocks/midi_params_editor.h"
#include "interface/visual_settings.h"
#include "engine/audio_control.h"
#include <Qt/qdialog.h>
#include <Qt/qinputdialog.h>

namespace ReShaked {

class NativeWidget : public QDialog {
	
	int paintEngine() { return 0; }
public:	
	
	NativeWidget(QWidget *p_parent) :QDialog(p_parent) {
		
		setAutoFillBackground(true);
		setAttribute(Qt::WA_OpaquePaintEvent);
		setAttribute(Qt::WA_NoSystemBackground);			
		setAttribute(Qt::WA_PaintOnScreen);
		
	}
};

SoundPluginUI* VST_PluginUI::create_this(SoundPlugin *p_plugin,QWidget *p_parent) {
	
	VST_Plugin *vst = dynamic_cast<VST_Plugin*>(p_plugin);
	if (vst)
		return new VST_PluginUI(p_parent,vst);
	else
		return NULL;
}

void VST_PersistentData::plugin_ui_changed_param(void *instance,int p_param) {
	
	VST_PersistentData* pd = (VST_PersistentData*)instance;
	
	pd->property_changed_signal(p_param);
	
}

void VST_PluginUI::gui_show_slot() {
	
	NativeWidget* nw = new NativeWidget(topLevelOf(this));
	
	HWND win=nw->winId();
	vst->get_ptrPlug()->dispatcher(vst->get_ptrPlug(),effEditOpen,0,0,win,0.0f);
	ERect *rect;
	vst->get_ptrPlug()->dispatcher(vst->get_ptrPlug(),effEditGetRect,0,0,&rect,0.0f);
	QSize size(rect->right-rect->left,rect->bottom-rect->top);

	nw->setFixedSize(rect->right-rect->left,rect->bottom-rect->top);
	nw->setWindowTitle(QStrify(vst->get_info()->caption));
	nw->exec();
	
	vst->get_ptrPlug()->dispatcher(vst->get_ptrPlug(),effEditClose,0,0,NULL,0.0f);
	
	delete nw;
	
	
}
void VST_PluginUI::midi_props_edit_slot() {
	
	if (!pdata->midi_params_editor)
		return;
	
	pdata->midi_params_editor->exec();
	pdata->midi_params_editor->hide();
	
}
void VST_PluginUI::params_show_slot(bool p_show) {
	
	pdata->params_visible=p_show;
	if (p_show)
		hb_params->show();
	else
		hb_params->hide();
}
void VST_PluginUI::presets_choose_slot() {
	
	QStringList sl;
	char progbuff[kVstMaxProgNameLen*2]; //just in case
	
	for (int i=0;i<vst->get_ptrPlug()->numPrograms;i++) {
		
		progbuff[0]=0;
		vst->get_ptrPlug()->dispatcher(vst->get_ptrPlug(),effGetProgramNameIndexed,i,0,progbuff,0.0f);		
		sl << (QString::number(i+1)+" "+QString(progbuff));
		
	}
	
	bool ok;
	int current=vst->get_ptrPlug()->dispatcher(vst->get_ptrPlug(),effGetProgram,0,0,0,0.0f);
	QString selected_name=QInputDialog::getItem( topLevelOf(this), "Preset","Select:",sl,current,false,&ok);
	
	if (!ok)
		return;
	/* This is soooooooooo cheap! */
	for (int i=0;i<vst->get_ptrPlug()->numPrograms;i++) {
		
		progbuff[0]=0;
		vst->get_ptrPlug()->dispatcher(vst->get_ptrPlug(),effGetProgramNameIndexed,i,0,progbuff,0.0f);		
		QString sn=(QString::number(i+1)+" "+QString(progbuff));
		
		if (sn==selected_name) {
			
			AudioControl::mutex_lock();
			
			vst->get_ptrPlug()->dispatcher(vst->get_ptrPlug(),effSetProgram,0,i,NULL,0.0f);
		
			
			AudioControl::mutex_unlock();
			
			break;
			
		}
	}
}

void VST_PluginUI::property_changed_by_editor_slot(int p_which) {
	
	Property *p=&vst->get_port(p_which);
	property_edited_signal(p,p->get()); //undo/redo wont work as well I'm afraid
}

VST_PluginUI::VST_PluginUI(QWidget *p_parent,VST_Plugin *p_plugin) : SoundPluginUI(p_parent,p_plugin) {
	
	vst=p_plugin;
	
	pdata=NULL;
	
	pdata = dynamic_cast<VST_PersistentData*>( p_plugin->get_metadata() );
	if (pdata==NULL) {
		
		pdata= new VST_PersistentData;
		p_plugin->set_metadata( pdata );
		p_plugin->set_property_changed_callback(pdata,&VST_PersistentData::plugin_ui_changed_param);
		if (vst->get_info()->is_synth) {
			pdata->midi_params_editor = new MidiParamsEditor(topLevelOf(p_parent),vst->get_midi_parameters());
		} 
	}
	
	QObject::connect(pdata,SIGNAL(property_changed_signal(int)),this,SLOT(property_changed_by_editor_slot(int)));
	
	
	setLayout(new QVBoxLayout(this));
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_LEFT));
	PixmapLabel *title = new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP) ,PixmapLabel::EXPAND_TILE_H);
	title->set_text( QStrify(p_plugin->get_caption())) ;
	title->setToolTip( QStrify(p_plugin->get_caption())) ;
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_RIGHT));
	
	
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	PixmapLabel *label_name = new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LEFT));
	label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	
	/**** VST ****/
	
	CVBox *vst_vb = new CVBox(hb);
	
	new PixmapLabel(vst_vb,vst->get_info()->is_synth?GET_QPIXMAP(THEME_VST__TOP_VSTI):GET_QPIXMAP(THEME_VST__TOP_VST));
	
	CHBox *vst_hb = new CHBox(vst_vb);
	CVBox *buttons_vb = new CVBox(vst_hb);
	
	if ( vst->get_info()->has_internal_UI ) {
		
		gui_button = new PixmapButton( buttons_vb, PixmapButton::Skin( GET_QPIXMAP(THEME_VST__BUTTON_GUI),GET_QPIXMAP(THEME_VST__BUTTON_GUI_ACTIVE)));
		QObject::connect(gui_button,SIGNAL(mouse_pressed_signal()),this,SLOT(gui_show_slot()));
		gui_button->set_pressed(pdata->ui_visible);
		//TODO: Remember this setting
	} else {
		gui_button=NULL;
		new PixmapLabel(buttons_vb,GET_QPIXMAP(THEME_VST__BUTTON_GUI_INACTIVE));
	}
	
	
	if (vst->get_info()->is_synth) {
		
		QObject::connect( new PixmapButton(buttons_vb,PixmapButton::Skin(GET_QPIXMAP(THEME_VST__BUTTON_MIDI),GET_QPIXMAP(THEME_VST__BUTTON_MIDI_ACTIVE))),SIGNAL(mouse_pressed_signal()),this,SLOT(midi_props_edit_slot()));
		
	} else {
		
		new PixmapLabel(buttons_vb,GET_QPIXMAP(THEME_VST__BUTTON_MIDI_INACTIVE));
	}
	
	if (!vst->get_info()->has_internal_UI) {
			
		params_button = new PixmapButton( buttons_vb, PixmapButton::Skin( GET_QPIXMAP(THEME_VST__BUTTON_PARAMS),GET_QPIXMAP(THEME_VST__BUTTON_PARAMS_ACTIVE)),PixmapButton::TYPE_TOGGLE);
		QObject::connect(params_button,SIGNAL(mouse_toggled_signal(bool)),this,SLOT(params_show_slot(bool)));
		params_button->set_pressed(pdata->params_visible);
	} else {
		
		new PixmapLabel(buttons_vb,GET_QPIXMAP(THEME_VST__BUTTON_PARAMS_INACTIVE));
	}
	QObject::connect( new PixmapButton(buttons_vb,PixmapButton::Skin(GET_QPIXMAP(THEME_VST__BUTTON_PRESETS),GET_QPIXMAP(THEME_VST__BUTTON_PRESETS_ACTIVE))),SIGNAL(mouse_pressed_signal()),this,SLOT(presets_choose_slot()));
			
	new PixmapLabel(buttons_vb,GET_QPIXMAP(THEME_VST__MIDDLE));
	
	CHBox *amp_hb = new CHBox(buttons_vb);
	
	new PixmapLabel(amp_hb,GET_QPIXMAP(THEME_VST__AMP_LABEL_BEGIN));
	
	PropertyEditLabel * amp_value = new PropertyEditLabel( amp_hb, GET_QPIXMAP(THEME_VST__AMP_LABEL) );
	
	amp_value->set_property(&vst->get_port_by_name("vst_plugin_gain"));
	amp_value->set_suffix_visible( false );
	amp_value->set_color(QColor(240,240,255));
	
	
	new PixmapLabel(amp_hb,GET_QPIXMAP(THEME_VST__AMP_LABEL_END));
	
	CVBox *gain_vb = new CVBox(vst_hb);
	
	new PixmapLabel(gain_vb,GET_QPIXMAP(THEME_VST__AMP_SLIDER_BEGIN));
	
	PixmapSlider::Skin amp_slider_skin(GET_QPIXMAP(THEME_VST__AMP_SLIDER),GET_QPIXMAP(THEME_VST__AMP_SLIDER),GET_QPIXMAP(THEME_VST__SLIDER_GRABBER));
	
	PropertyEditSlider *pixmap_slider = new PropertyEditSlider(gain_vb,amp_slider_skin);
	pixmap_slider->set_property(&vst->get_port_by_name("vst_plugin_gain"));
	amp_value->add_to_group(pixmap_slider);
	register_property_editor( amp_value );
	register_property_editor( pixmap_slider );
		
	new PixmapLabel(vst_hb,GET_QPIXMAP(THEME_VST__RIGHT));
	
	new PixmapLabel(vst_vb,GET_QPIXMAP(THEME_VST__BOTTOM));
		
	/***** END OF VST ****/
	
	QPixmap value_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VALUE));
	QPixmap label_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LABEL));
	PixmapSlider::Skin slider_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_BASE),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_LIGHT),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_GRABBER));
	
	PixmapVU::Skin vu_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_EMPTY),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_FILL));
	
	hb_params = new CHBox(hb);
	
	if (!vst->get_info()->has_internal_UI) {
		
		//create standard controls
		for (int i=0;i<p_plugin->get_port_count();i++) {
			
			if (QStrify(p_plugin->get_port(i).get_caption()).indexOf("/")>=0)
				continue;
			
			CVBox *vb_port = new CVBox(hb_params);
			
			CHBox *hb_port = new CHBox(vb_port);
			
			/* LABEL */
			PixmapLabel * name = new PixmapLabel(hb_port,label_pixmap);
			
			name->set_pos(QPoint(6,label_pixmap.height()-8));
			name->get_font().setPixelSize(10);
			QString name_str=QStrify(p_plugin->get_port(i).get_caption());
			if (p_plugin->get_port(i).get_suffix()!="") {
				
				name_str+=" (" +QStrify(p_plugin->get_port(i).get_suffix()).toLower() +")";
			}
			name->set_text( name_str );
			name->set_angle( -90 );
			name->set_color(QColor(0,0,22));
			
			
			/* SLIDER/VU */
			PropertyEditor *editor;
			if (p_plugin->get_port_type(i)==SoundPlugin::TYPE_WRITE) {
				PropertyEditSlider * slider = new PropertyEditSlider(hb_port,slider_skin);
				slider->set_property(&p_plugin->get_port(i));
				editor=slider;
			} else {
				PropertyEditVU * vu = new PropertyEditVU(hb_port,vu_skin);
				vu->set_property(&p_plugin->get_port(i));
				editor=vu;
			}
			
			register_property_editor( editor );
			
			/* VALUE */
			PropertyEditLabel * value = new PropertyEditLabel( vb_port,value_pixmap );
			value->set_property(&p_plugin->get_port(i));
			value->set_suffix_visible( false );
			value->set_color(QColor(240,240,255));
			value->add_to_group(editor); //share group
			
			register_property_editor( value );
			
			vb_port->layout()->setMargin(0);
			vb_port->layout()->setSpacing(0);
			hb_port->layout()->setMargin(0);
			hb_port->layout()->setSpacing(0);
			
		}
	}	
	new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_RIGHT));
	
	CHBox *hb_bottom = new CHBox(this);
	layout()->addWidget(hb_bottom);
	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM_LEFT));
	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM),PixmapLabel::EXPAND_TILE_H);
	new PixmapLabel(hb_bottom,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_BOTTOM_RIGHT));
	
	
	layout()->setMargin(0);
	layout()->setSpacing(0);
	
	hb->layout()->setMargin(0);
	hb->layout()->setSpacing(0);	
	if (!pdata->params_visible)
		hb_params->hide(); 
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	if (pdata->midi_params_editor) {
		for (int i=0;i<pdata->midi_params_editor->get_property_editor_count();i++) {
			
			register_property_editor( pdata->midi_params_editor->get_property_editor(i) );
		}
	}
}


VST_PluginUI::~VST_PluginUI()
{
	
	
	
}


}



#endif
