//
// C++ Interface: tree_helpers_dsp
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTREE_HELPERS_DSP_H
#define RESHAKEDTREE_HELPERS_DSP_H

#include "tree_loader.h"
#include "tree_saver.h"

class Sample;

namespace ReShaked {

bool load_sample_from_tree(TreeLoader *p_tree,Sample *p_sample);
void save_sample_to_tree(TreeSaver *p_tree,Sample *p_sample);

}

#endif
