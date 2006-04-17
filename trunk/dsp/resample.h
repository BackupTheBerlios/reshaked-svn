//
// C++ Interface: resample
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef RESAMPLE_H
#define RESAMPLE_H

namespace ReShaked {

#define RESAMPLER_MAX_CHANNELS 4


enum ResampleInterpolationMode {
	
	INTERPOLATION_RAW,
	INTERPOLATION_LINEAR,
	INTERPOLATION_CUBIC,
};

enum ResampleLoopMode {
	
	RESAMPLE_LOOP_NONE,
	RESAMPLE_LOOP_FORWARD,
	RESAMPLE_LOOP_BIDI
};


struct ResampleData {
	
	/* fixed point */
	int fp_increment;
	Sint64 fp_pos;
	Sint64 fp_loop_begin;
	Sint64 fp_loop_end;
	Sint64 fp_size;
	
	/* loop */
	
	ResampleLoopMode loop_mode;
	
	/* volume ramp */
	const float *src_buffers[RESAMPLER_MAX_CHANNELS];
	float *dst_buffers[RESAMPLER_MAX_CHANNELS];
	
	/* volume ramp */
	float src_amps[RESAMPLER_MAX_CHANNELS];
	float incr_amps[RESAMPLER_MAX_CHANNELS];

	int write_pos;
};

#define LINTERP_MACRO(m_res,m_val,m_next,m_coeff) \
	m_res=m_val+(m_next-m_val)*m_coeff;

template<int FRAC_BITS,int src_channels,int dst_channels,ResampleInterpolationMode interp_mode>
static inline void resample(ResampleData& rd,int p_frames) {
	
	float val[RESAMPLER_MAX_CHANNELS];
		
	while (p_frames--) {
	
		/* Determine integer and fractional sample position */
		int pos=rd.fp_pos >> FRAC_BITS;
		int frac=rd.fp_pos&((1<<FRAC_BITS)-1);
		// int -> float conversion is very fast
		float frac_c=(float)frac/(float)(1<<FRAC_BITS);
	
		/* INTERPOLATION CHOOSE */
		if (interp_mode==INTERPOLATION_RAW) {
			
			if (src_channels>0)
				val[0]=rd.src_buffers[0][pos];
			
			if (src_channels>1)
				val[1]=rd.src_buffers[1][pos];
			
			if (src_channels>2)
				val[2]=rd.src_buffers[2][pos];
			
			if (src_channels>3)
				val[3]=rd.src_buffers[3][pos];
			
		} if (interp_mode==INTERPOLATION_LINEAR) {
						 
			if (src_channels>0) {
				LINTERP_MACRO(val[0],rd.src_buffers[0][pos],rd.src_buffers[0][pos+1],frac_c);
			}
			
			if (src_channels>1) {
				LINTERP_MACRO(val[1],rd.src_buffers[1][pos],rd.src_buffers[1][pos+1],frac_c);
			}
			
			if (src_channels>2) {
				LINTERP_MACRO(val[2],rd.src_buffers[2][pos],rd.src_buffers[2][pos+1],frac_c);
			}
			
			if (src_channels>3) {
				LINTERP_MACRO(val[3],rd.src_buffers[3][pos],rd.src_buffers[3][pos+1],frac_c);
			}
			
		}
						 
		/* now depending on channel remapping, do what asked for */
		
		if (src_channels==1 && dst_channels==1) {
			// MONO -> MONO			
			rd.dst_buffers[0][rd.write_pos]+=val[0]*rd.src_amps[0];
			rd.src_amps[0]+=rd.incr_amps[0];
			
		} if (src_channels==1 && dst_channels==2) {
			// MONO -> STEREO
			rd.dst_buffers[0][rd.write_pos]+=val[0]*rd.src_amps[0];
			rd.dst_buffers[1][rd.write_pos]+=val[0]*rd.src_amps[1];
			rd.src_amps[0]+=rd.incr_amps[0];
			rd.src_amps[1]+=rd.incr_amps[1];
			
		} if (src_channels==2 && dst_channels==2) {
			// STEREO -> STEREO
			rd.dst_buffers[0][rd.write_pos]+=val[0]*rd.src_amps[0];
			rd.dst_buffers[1][rd.write_pos]+=val[1]*rd.src_amps[1];
			rd.src_amps[0]+=rd.incr_amps[0];
			rd.src_amps[1]+=rd.incr_amps[1];
			
		} if (src_channels==1 && dst_channels==4) {
			// MONO -> QUAD
			rd.dst_buffers[0][rd.write_pos]+=val[0]*rd.src_amps[0];
			rd.dst_buffers[1][rd.write_pos]+=val[0]*rd.src_amps[1];
			rd.dst_buffers[2][rd.write_pos]+=val[0]*rd.src_amps[2];
			rd.dst_buffers[3][rd.write_pos]+=val[0]*rd.src_amps[3];
			rd.src_amps[0]+=rd.incr_amps[0];
			rd.src_amps[1]+=rd.incr_amps[1];
			rd.src_amps[2]+=rd.incr_amps[2];
			rd.src_amps[3]+=rd.incr_amps[3];
		} if (src_channels==2 && dst_channels==4) {
			// STEREO -> QUAD
			rd.dst_buffers[0][rd.write_pos]+=val[0]*rd.src_amps[0];
			rd.dst_buffers[1][rd.write_pos]+=val[0]*rd.src_amps[1];
			rd.dst_buffers[2][rd.write_pos]+=val[1]*rd.src_amps[2];
			rd.dst_buffers[3][rd.write_pos]+=val[1]*rd.src_amps[3];
			rd.src_amps[0]+=rd.incr_amps[0];
			rd.src_amps[1]+=rd.incr_amps[1];
			rd.src_amps[2]+=rd.incr_amps[2];
			rd.src_amps[3]+=rd.incr_amps[3];
			
		} if (src_channels==4 && dst_channels==4) {
			// QUAD -> QUAD
			rd.dst_buffers[0][rd.write_pos]+=val[0]*rd.src_amps[0];
			rd.dst_buffers[1][rd.write_pos]+=val[1]*rd.src_amps[1];
			rd.dst_buffers[2][rd.write_pos]+=val[2]*rd.src_amps[2];
			rd.dst_buffers[3][rd.write_pos]+=val[3]*rd.src_amps[3];
			rd.src_amps[0]+=rd.incr_amps[0];
			rd.src_amps[1]+=rd.incr_amps[1];
			rd.src_amps[2]+=rd.incr_amps[2];
			rd.src_amps[3]+=rd.incr_amps[3];
		}

		/* increment integer and fixed point read/write positions */
		
		rd.write_pos++;
		rd.fp_pos+=rd.fp_increment;
	}
	
}


template<int FRAC_BITS,int src_channels,int dst_channels,ResampleInterpolationMode interp_mode>
static inline int resample_with_loop(ResampleData& rd,int p_frames) {

	int todo=p_frames;
		
	while(todo) {

		if ( rd.fp_increment< 0 ) {
			/* The sample is playing in reverse */

			if( ( rd.loop_mode!=RESAMPLE_LOOP_NONE )&&(rd.fp_pos<rd.fp_loop_begin) ) {
				/* the sample is looping and has reached the loopstart index */
				if ( rd.loop_mode==RESAMPLE_LOOP_BIDI ) {
					/* sample is doing bidirectional loops, so 'bounce' the
					current index against the rd.fp_loop_begin */
					rd.fp_pos = rd.fp_loop_begin+(rd.fp_loop_begin-rd.fp_pos);
					rd.fp_increment = -rd.fp_increment;
				} else
					/* normal backwards looping, so set the current position to
					loopend index */
					rd.fp_pos=rd.fp_loop_end-(rd.fp_loop_begin-rd.fp_pos);
			} else {
				/* the sample is not looping, so check if it reached index 0 */
				if(rd.fp_pos < 0) {
					/* playing index reached 0, so stop playing this sample */
					rd.fp_pos=0;

					break;
				}
			}
		} else {
			/* The sample is playing forward */
			if ( (rd.loop_mode!=RESAMPLE_LOOP_NONE) && (rd.fp_pos >= rd.fp_loop_end)) {
				/* the sample is looping, check the loopend index */
				if( rd.loop_mode==RESAMPLE_LOOP_BIDI ) {
					/* sample is doing bidirectional loops, so 'bounce' the
					current index against the rd.fp_loop_end */
					rd.fp_increment = -rd.fp_increment;
					rd.fp_pos = rd.fp_loop_end-(rd.fp_pos-rd.fp_loop_end);
				} else
					/* normal backwards looping, so set the current position
					to loopend index */
					rd.fp_pos=rd.fp_loop_begin+(rd.fp_pos-rd.fp_loop_end);
			} else {
				/* sample is not looping, so check if it reached the last
				position */
				if(rd.fp_pos >= rd.fp_size) {
					/* yes, so stop playing this sample */
					rd.fp_pos=0;
					break;
				}
			}
		}

		/* Set limit, to determine amount of frames in this mix */
		Sint64 end;
		//if loop
		if (rd.loop_mode!=RESAMPLE_LOOP_NONE) {
			
			if ( rd.fp_increment< 0 ) //if backwards
				end=rd.fp_loop_begin;
			else
				end=rd.fp_loop_end;
		} else {
			
			if ( rd.fp_increment < 0 )
				end=0;
			else
				end=rd.fp_size;
		}
		
		// frames mixable until end is reached
		int room_to_end=int ( (end-rd.fp_pos)/rd.fp_increment+1 );
		int to_mix=(room_to_end<todo)?room_to_end:todo;
		
		if ( to_mix<=0 ) { // I think this should never happen, but anyway
			break;
		}

		resample<FRAC_BITS,src_channels,dst_channels,interp_mode>(rd,to_mix);

		todo-=to_mix;

	}

	
	
	return p_frames-todo; //return how much was actually mixed	
	
}

}

#endif