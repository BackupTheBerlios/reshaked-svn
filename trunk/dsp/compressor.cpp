//
// C++ Implementation: compressor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "compressor.h"


void CompressorBase::set_attack(float p_attack) {

	attack=p_attack;
	recompute_coeffs();
}
float CompressorBase::get_attack() const {

	return attack;
}

void CompressorBase::set_release(float p_release) {

	release=p_release;
	recompute_coeffs();
}
float CompressorBase::get_release() const {

	return release;
}

void CompressorBase::set_threshold(float p_threshold) {

	threshold=p_threshold;
}
float CompressorBase::get_threshold() const {

	return threshold;
}

void CompressorBase::set_ratio(float p_ratio) {

	ratio=1.0/p_ratio;
}
float CompressorBase::get_ratio() const {

	return 1.0/ratio;
}

void CompressorBase::recompute_coeffs() {

	c_attack=expf( -1/(attack*mix_rate) );
	c_release=expf( -1/(release*mix_rate) );
}

void CompressorBase::set_mix_rate(float p_mix_rate) {

	mix_rate=p_mix_rate;
	recompute_coeffs();
}
float CompressorBase::get_mix_rate() const {

	return mix_rate;
}


CompressorBase::CompressorBase() {

	mix_rate=44100;
	attack=0.1;
	release=0.4;
	recompute_coeffs();
	threshold=0;
	ratio=0;
	state=0;
}
