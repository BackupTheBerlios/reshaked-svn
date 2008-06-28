#ifndef MIDI_PARAMS_EDITOR_H
#define MIDI_PARAMS_EDITOR_H

#include "engine/midi_parameters.h"
#include "ui_blocks/helpers.h"
#include <vector>
#include <Qt/qdialog.h>

namespace ReShaked {


class MidiParamsEditor : public QDialog {


	std::vector<PropertyEditor*> property_editors;
public:

	int get_property_editor_count();
	PropertyEditor* get_property_editor(int p_which);
	
	MidiParamsEditor(QWidget *p_parent,MidiParameters *p_params);

};

}

#endif