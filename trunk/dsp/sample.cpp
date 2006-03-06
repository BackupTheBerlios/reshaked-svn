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
float* Sample::get_buffer(int p_channel){
	
	ERR_FAIL_INDEX_V(p_channel,data.size(),NULL);
	
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
	
	
}


Sample::Sample(int p_channels,int p_length) {
	
	if (p_channels<0)
		p_channels=0;
	data.resize(p_channels);
	
	for (int i=0;i<p_channels;i++) {
		
		data[i].resize(p_length);
	}
}


Sample::~Sample() {
	
}


}
