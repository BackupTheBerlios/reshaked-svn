//
// C++ Interface: piano_keys
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//


enum PianoKey {
	PIANO_KEY_C_0=0,
	PIANO_KEY_Cs0,
	PIANO_KEY_D_0,
	PIANO_KEY_Ds0,
	PIANO_KEY_E_0,
	PIANO_KEY_F_0,
	PIANO_KEY_Fs0,
	PIANO_KEY_G_0,
	PIANO_KEY_Gs0,
	PIANO_KEY_A_0,
	PIANO_KEY_As0,
	PIANO_KEY_B_0,

	PIANO_KEY_C_1,
	PIANO_KEY_Cs1,
	PIANO_KEY_D_1,
	PIANO_KEY_Ds1,
	PIANO_KEY_E_1,
	PIANO_KEY_F_1,
	PIANO_KEY_Fs1,
	PIANO_KEY_G_1,
	PIANO_KEY_Gs1,
	PIANO_KEY_A_1,
	PIANO_KEY_As1,
	PIANO_KEY_B_1,
	
	PIANO_KEY_C_2,
	PIANO_KEY_Cs2,
	PIANO_KEY_D_2,
	PIANO_KEY_Ds2,
	PIANO_KEY_E_2,
	MAX_PIANO_KEYS


};

static const char* piano_key_name[MAX_PIANO_KEYS]={
	
	"C-0",
	"C#0",
	"D-0",
	"D#0",
	"E-0",
	"F-0",
	"F#0",
	"G-0",
	"G#0",
	"A-0",
	"A#0",
	"B-0",

	"C-1",
	"C#1",
	"D-1",
	"D#1",
	"E-1",
	"F-1",
	"F#1",
	"G-1",
	"G#1",
	"A-1",
	"A#1",
	"B-1",
	
	"C-2",
	"C#2",
	"D-2",
	"D#2",
	"E-2"
};



