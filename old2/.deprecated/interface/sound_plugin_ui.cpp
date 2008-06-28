
//
// C++ Implementation: sound_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_ui.h"

#include <Qt/qlayout.h>
#include "ui_blocks/helpers.h"
#include "ui_blocks/pixmap_label.h"
#include "ui_blocks/pixmap_slider.h"
#include "ui_blocks/property_editors.h"
#include "interface/visual_settings.h"
#include <Qt/qmetaobject.h>
namespace ReShaked {



void SoundPluginUI::property_editor_property_edited_callback(void *_this,Property* p_property,double p_old_val) {
	

	SoundPluginUI *instance=(SoundPluginUI*)_this;
	instance->property_edited_signal(p_property, p_old_val); 
	
}

void SoundPluginUI::register_property_editor(PropertyEditorBase* p_editor) {
	
	for (int i=0;i<property_editors.size();i++) {
		
		ERR_FAIL_COND( property_editors[i]==p_editor );
	}
	
	property_editors.push_back(p_editor);
	if (editor_updater)
		editor_updater->add_editor( p_editor );
	
	p_editor->set_changed_by_editor_callback(this,&SoundPluginUI::property_editor_property_edited_callback); 
	
	QWidget *w = dynamic_cast<QWidget*>(p_editor);
	
	if (w) {
		
		//printf("index for %s es is %i\n", QMetaObject::normalizedSignature(SIGNAL(external_edit_signal(Property *) )).data(),w->metaObject()->indexOfSignal( QMetaObject::normalizedSignature(SIGNAL(external_edit_signal(Property *) )).data() ));
		if (w->metaObject()->indexOfSignal( QMetaObject::normalizedSignature("external_edit_signal(Property*)").data() )>=0)
			QObject::connect(w,SIGNAL(external_edit_signal(Property *)),this,SIGNAL(property_options_requested( Property* )));
		/*printf("comparing to others..");
		
		for (int i=0;i<w->metaObject()->methodCount();i++) {
			
			if (!w->metaObject()->method(i).methodType()==QMetaMethod::Signal)
				continue;
			printf(" %i is - %s\n",i,w->metaObject()->method(i).signature());
		} */
	}
	
}

void SoundPluginUI::unregister_property_editor(PropertyEditorBase* p_editor) {
	
	
	
	for (int i=0;i<property_editors.size();i++) {
		
		if (property_editors[i]==p_editor ) {
			property_editors.erase(property_editors.begin()+i);
			break;
		}
			
	}
	
	if (editor_updater)
		editor_updater->remove_editor( p_editor );
	
	p_editor->set_changed_by_editor_callback(NULL,NULL);
}

void SoundPluginUI::set_property_editor_updater(PropertyEditUpdater *p_property_edit_updater) {
	
	if (editor_updater) {
		
		for (int i=0;i<property_editors.size();i++) {
		
			editor_updater->remove_editor( property_editors[i] );
		}
	}
	
	editor_updater=p_property_edit_updater;
	
	for (int i=0;i<property_editors.size();i++) {
		
		editor_updater->add_editor( property_editors[i] );
	}
	
}


void SoundPluginUI::create_editor_for_property(String p_name,QWidget *p_parent) {
	
	int idx=-1;
	for (int i=0;i<_plugin->get_port_count();i++) {
		
		if (_plugin->get_port( i ).get_name()==p_name) {
			
			idx=i;
			break;
		}
	}
	
	ERR_FAIL_COND(idx==-1);
	
	QPixmap value_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VALUE));
	QPixmap label_pixmap(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LABEL));
	PixmapSlider::Skin slider_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_BASE),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_LIGHT),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_SLIDER_GRABBER));
		
	PixmapVU::Skin vu_skin(GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_EMPTY),GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_VU_FILL));
	
	
	CVBox *vb_port = new CVBox(p_parent);
			
	CHBox *hb_port = new CHBox(vb_port);
			
	/* LABEL */
	PixmapLabel * name = new PixmapLabel(hb_port,label_pixmap);
			
	name->set_pos(QPoint(6,label_pixmap.height()-8));
	name->get_font().setPixelSize(10);
	QString name_str=QStrify(_plugin->get_port(idx).get_caption());
	if (_plugin->get_port(idx).get_suffix()!="") {
				
		name_str+=" (" +QStrify(_plugin->get_port(idx).get_suffix()).toLower() +")";
	}
	name->set_text( name_str );
	name->set_angle( -90 );
	name->set_color(QColor(0,0,22));
			
			
	/* SLIDER/VU */
	PropertyEditor *editor;
	if (_plugin->get_port_type(idx)==SoundPlugin::TYPE_WRITE) {
		PropertyEditSlider * slider = new PropertyEditSlider(hb_port,slider_skin);
		slider->set_property(&_plugin->get_port(idx));
		editor=slider;
	} else {
		PropertyEditVU * vu = new PropertyEditVU(hb_port,vu_skin);
		vu->set_property(&_plugin->get_port(idx));
		editor=vu;
	}
			
	register_property_editor( editor );
			
	/* VALUE */
	PropertyEditLabel * value = new PropertyEditLabel( vb_port,value_pixmap );
	value->set_property(&_plugin->get_port(idx));
	value->set_suffix_visible( false );
	value->set_color(QColor(240,240,255));
	value->add_to_group(editor); //share group
			
	register_property_editor( value );
			
	vb_port->layout()->setMargin(0);
	vb_port->layout()->setSpacing(0);
	hb_port->layout()->setMargin(0);
	hb_port->layout()->setSpacing(0);
	
}

CHBox* SoundPluginUI::generate_default_layout(bool p_generate_controls) {
	
	setLayout(new QVBoxLayout(this));
	CHBox *hb_top = new CHBox(this);
	layout()->addWidget(hb_top);
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_LEFT));
	PixmapLabel *title = new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP) ,PixmapLabel::EXPAND_TILE_H);
	title->set_text( QStrify(_plugin->get_caption())) ;
	title->setToolTip( QStrify(_plugin->get_caption())) ;
	new PixmapLabel(hb_top,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_TOP_RIGHT));
	
	
	CHBox *hb = new CHBox(this);
	layout()->addWidget(hb);
	
	new PixmapLabel(hb,GET_QPIXMAP(THEME_EFFECT_PANEL_GENERIC_LEFT));
	//label_name->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	
	CHBox *hb_params = new CHBox(hb);
	
	if (p_generate_controls) {
		
		for (int i=0;i<_plugin->get_port_count();i++) {
			
			if (QStrify(_plugin->get_port(i).get_caption()).indexOf("/")>=0)
				continue;
			
			create_editor_for_property(_plugin->get_port(i).get_name(),hb_params);
			
			
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
	
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	
	return hb_params;
	
}

SoundPluginUI::SoundPluginUI(QWidget *p_parent,SoundPlugin *p_plugin) : QWidget(p_parent) {
	
	editor_updater=NULL;
	_plugin=p_plugin;
	
			
}


SoundPluginUI::~SoundPluginUI() {
	
	for (int i=0;i<property_editors.size();i++) {
		//make disable callback to this
		property_editors[i]->set_changed_by_editor_callback(NULL,NULL);
			
	}
	
	
	
	if (editor_updater) {
		for (int i=0;i<property_editors.size();i++) {
		
			editor_updater->remove_editor( property_editors[i] );
		}
	}
}


}
