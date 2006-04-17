//
// C++ Implementation: sample
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sample.h"
#include "error_macros.h"

namespace ReShaked {

int FrameData::get_channels() const {
	
	return data.size();
}


float FrameData::get(int p_channel) const {
	
	ERR_FAIL_INDEX_V(p_channel,data.size(),0);
	return data[p_channel];
	
}
void FrameData::set(int p_channel,float p_data) {
	
	ERR_FAIL_INDEX(p_channel,data.size());
	data[p_channel]=p_data;
}
		
FrameData FrameData::operator+(const FrameData& p_rvalue) const {
	
	FrameData aux=*this;
	aux+=p_rvalue;
	return aux;
	
}
FrameData& FrameData::operator+=(const FrameData& p_rvalue) {
	
	ERR_FAIL_COND_V(p_rvalue.data.size()!=data.size(),*this);

	for (int i=0;i<data.size();i++)
		data[i]+=p_rvalue.data[i];
	
	return *this;
	
	
}

FrameData FrameData::operator-(const FrameData& p_rvalue) const {
	
	FrameData aux=*this;
	aux-=p_rvalue;
	return aux;
	
}
FrameData& FrameData::operator-=(const FrameData& p_rvalue) {
	
	ERR_FAIL_COND_V(p_rvalue.data.size()!=data.size(),*this);

	for (int i=0;i<data.size();i++)
		data[i]-=p_rvalue.data[i];
	
	return *this;
	
}

FrameData FrameData::operator*(float p_val) const {
	
	FrameData aux=*this;
	aux*=p_val;
	return aux;
	
}
FrameData& FrameData::operator*=(float p_val) {
	
	for (int i=0;i<data.size();i++)
		data[i]*=p_val;
	
	return *this;
	
	
}

FrameData FrameData::operator+(float p_val) const {
	
	FrameData aux=*this;
	aux+=p_val;
	return aux;
	
}
FrameData& FrameData::operator+=(float p_val) {
	
	for (int i=0;i<data.size();i++)
		data[i]+=p_val;
	
	return *this;
	
}

FrameData FrameData::operator/(float p_val) const {
	
	FrameData aux=*this;
	aux/=p_val;
	return aux;
	
}
FrameData& FrameData::operator/=(float p_val) {
	
	ERR_FAIL_COND_V(p_val==0,*this);
	
	for (int i=0;i<data.size();i++)
		data[i]/=p_val;
	
	return *this;
}

bool FrameData::operator==(const FrameData& p_rvalue) const {
	
	ERR_FAIL_COND_V( p_rvalue.data.size()!=data.size() , false );
	
	for (int i=0;i<data.size();i++)
		if (data[i]!=p_rvalue.data[i])
			return false;
	
	return true;
	
}
bool FrameData::operator!=(const FrameData& p_rvalue) const {
	
	ERR_FAIL_COND_V( p_rvalue.data.size()!=data.size() , true );
	
	return !(*this==p_rvalue);
	
}

FrameData::FrameData(const FrameData& p_framedata) {
	
	data=p_framedata.data;	
}

FrameData::FrameData(int p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	data.resize(p_channels);
	
	for (int i=0;i<data.size();i++)
		data[i]=0;
	
	
}

/****** SAMPLE *******/


int Sample::get_channels() const {
	
	return data.size();
}
int Sample::get_length(){
	
	ERR_FAIL_COND_V(data.size()==0,0);
	
	return data[0].size();
	
}
const float* Sample::get_buffer(int p_channel){
	
	ERR_FAIL_INDEX_V(p_channel,data.size(),NULL);
	
	return &data[p_channel][0];
}

float* Sample::get_buffer_w(int p_channel){
	
	ERR_FAIL_INDEX_V(p_channel,data.size(),NULL);
	
	peakcache_dirty=true;
	return &data[p_channel][0];
}

FrameData Sample::get_frame(int p_pos) const{
	
	ERR_FAIL_COND_V(data.size()==0,FrameData(get_channels()));
	ERR_FAIL_INDEX_V(p_pos,data[0].size(),FrameData(get_channels()));
	
	FrameData frame(get_channels());
	
	for (int i=0;i<data.size();i++) {
		
		frame.set(i,data[i][p_pos]);
	}
	
	return frame;
	
}
void Sample::set_frame(int p_pos,const FrameData& p_data){
	
	ERR_FAIL_COND(p_data.get_channels()!=get_channels());
	ERR_FAIL_COND(data.size()==0);
	ERR_FAIL_INDEX(p_pos,data[0].size());
	
	for (int i=0;i<data.size();i++) {
		
		data[i][p_pos]=p_data.get(i);
	}
	
	peakcache_dirty=true;
	
}

float Sample::get_base_freq() {
	
	return base_freq;	
}
void Sample::set_base_freq(float p_freq) {
	
	base_freq=p_freq;
}

void Sample::set_loop_begin(int p_pos) {
	
	ERR_FAIL_INDEX(p_pos,(get_length()+1));
	loop_begin=p_pos;
}

int Sample::get_loop_begin() {
	
	return loop_begin;
}

void Sample::set_loop_end(int p_pos) {
	
	ERR_FAIL_INDEX(p_pos,(get_length()+1));
	loop_end=p_pos;
}

int Sample::get_loop_end() {
	
	return loop_end;	
}

void Sample::set_loop_type(LoopType p_type) {
	
	
	loop_type=p_type;
}

Sample::LoopType Sample::get_loop_type() {
	
	
	return loop_type;	
}

void Sample::import(const std::vector<FrameData> p_array) {
	
	
}

void Sample::generate_peak_cache(int p_len) {
	
	
	if (data.size()==0 || get_length()==0) {
		
		peakcache.resize(0);
		return;	
	}
	
	peakcache.resize(p_len);
	int peaks_size=peakcache.size();
	double sample_size=get_length();
	int sample_sizei=get_length();
	
	for (int i=0;i<peaks_size;i++) {
		
		int sample_idx=(int)( (double)i*(double)sample_size/peaks_size );
		int subsamples=(int)(sample_size/(double)peaks_size);
		if (subsamples<2)
			subsamples=1;
		else
			subsamples+=1; //a bit more visual-accurate
		
		FrameData max_peak_frame( get_channels() );
		FrameData min_peak_frame( get_channels() );
		for (int ch=0;ch<data.size();ch++) {
			float max_peak=-2.0;
			float min_peak=2.0;
			
			for (int j=sample_idx;j<(sample_idx+subsamples);j++) {
			
				if (j>=sample_sizei)
					break;
				float samp=data[ch][j];
				if (samp>max_peak)
					max_peak=samp;
				if (samp<min_peak)
					min_peak=samp;
						
			}
			
			max_peak_frame.set(ch,max_peak);
			min_peak_frame.set(ch,min_peak);
		}		
		peakcache[i].max=max_peak_frame;
		peakcache[i].min=min_peak_frame;

	}
	
	peakcache_dirty=false;
}

bool Sample::is_peak_cache_dirty() const {
	
	return peakcache_dirty;
}
void Sample::create(int p_channels,int p_length) {
	
	if (p_channels<0)
		p_channels=0;
	data.resize(p_channels);
	
	for (int i=0;i<p_channels;i++) {
		
		data[i].resize(p_length);
		for (int j=0;j<p_length;j++)
			data[i][j]=0;
	}
	
	
	base_freq=1;
	loop_begin=0;
	loop_end=0;
	loop_type=LOOP_NONE;
	
}



Sample::Sample(int p_channels,int p_length) {
	
	peakcache_dirty=true;
	create(p_channels,p_length);
}


Sample::~Sample() {
	
}


}
