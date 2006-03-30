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
	
	FrameData operator+(float p_val) const;
	FrameData& operator+=(float p_val);
	
	bool operator==(const FrameData& p_rvalue) const;
	bool operator!=(const FrameData& p_rvalue) const;
	
	FrameData(const FrameData& p_framedata);
	FrameData(int p_channels=0);
			
};

class Sample {
public:	
	
	enum LoopType {
		
		LOOP_NONE,
		LOOP_FORWARD,
		LOOP_BIDI
	};
private:	
	
	//@TODO Realloc and use memory properly, since vector<> takes up a lot more than asked!
	std::vector<std::vector<float> > data;
	
	float base_freq;
		
	int loop_begin;
	int loop_end;
	
	LoopType loop_type;
	
	/* peak cache, stored in-sample */
	
	struct PeakCache {
		
		FrameData max,min;	
	};
	
	std::vector<PeakCache> peakcache;
	bool peakcache_dirty;
	
public:
	
	int get_channels() const;
	int get_length();
	float* get_buffer(int p_channel);
	
	FrameData get_frame(int p_pos) const;
	void set_frame(int p_pos,const FrameData& p_data);

	float get_base_freq();
	void set_base_freq(float p_freq);
	
	void set_loop_begin(int p_pos);
	int get_loop_begin();
	
	void set_loop_end(int p_pos);
	int get_loop_end();
	
	void set_loop_type(LoopType p_type);
	LoopType get_loop_type();
	
	void generate_peak_cache(int p_len=16384);
	inline const std::vector<PeakCache>& get_peak_cache() const { return peakcache; } //fast drawing
	bool is_peak_cache_dirty() const;
	
	void import(const std::vector<FrameData> p_array);
	void create(int p_channels=0,int p_length=0);
	Sample(int p_channels=0,int p_length=0);
	~Sample();
};






}

#endif
