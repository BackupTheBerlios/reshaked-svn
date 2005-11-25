#ifndef PIANOKEYS_H
#define PIANOKEYS_H


enum PianoKey {
	KEY_C_0,
	KEY_Cs0,
	KEY_D_0,
	KEY_Ds0,
	KEY_E_0,
	KEY_F_0,
	KEY_Fs0,
	KEY_G_0,
	KEY_Gs0,
	KEY_A_0,
	KEY_As0,
	KEY_B_0,

	KEY_C_1,
	KEY_Cs1,
	KEY_D_1,
	KEY_Ds1,
	KEY_E_1,
	KEY_F_1,
	KEY_Fs1,
	KEY_G_1,
	KEY_Gs1,
	KEY_A_1,
	KEY_As1,
	KEY_B_1,
	
	KEY_C_2,
	KEY_Cs2,
	KEY_D_2,
	KEY_Ds2,
	KEY_E_2,
	MAX_KEYS


};

static const char* key_name[MAX_KEYS]={
	
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




#endif
