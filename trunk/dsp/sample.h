//
// C++ Interface: sample
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSAMPLE_H
#define RESHAKEDSAMPLE_H

#include <vector>

namespace ReShaked {

/**
	@author red <red@killy>
*/

class FrameData {
		
	std::vector<float> data;
public:	
	
	
	int get_channels() const;
	float get(int p_channel) const;
	void set(int p_channel,float p_data);
			
	FrameData operator+(const FrameData& p_rvalue) const;
	FrameData& operator+=(const FrameData& p_rvalue);
	
	FrameData operator-(const FrameData& p_rvalue) const;
	FrameData& operator-=(const FrameData& p_rvalue);
	
	FrameData operator*(float p_val) const;
	FrameData& operator*=(float p_val);
	
	FrameData operator/(float p_val) const;
	FrameData& operator/=(float p_val);
	
	bool operator==(const FrameData& p_rvalue) const;
	bool operator!=(const FrameData& p_rvalue) const;
	
	FrameData(const FrameData& p_framedata);
	FrameData(int p_channels=0);
			
};

class Sample {
	
	std::vector<std::vector<float> > data;
	
public:
	
	int get_channels() const;
	int get_length();
	float* get_buffer(int p_channel);
	
	FrameData get_frame(int p_pos) const;
	void set_frame(int p_pos,const FrameData& p_data);

	
	Sample(int p_channels=0,int p_length=0);
	~Sample();
};






}

#endif
