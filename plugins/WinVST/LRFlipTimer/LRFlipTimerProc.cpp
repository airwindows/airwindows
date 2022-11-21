/* ========================================
 *  LRFlipTimer - LRFlipTimer.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __LRFlipTimer_H
#include "LRFlipTimer.h"
#endif

void LRFlipTimer::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	uint32_t time = (uint32_t)(((A*9.0)+1.0) * 60.0 * getSampleRate());
	uint32_t transition = (uint32_t)(getSampleRate()*0.1);
	//number of minutes * 60 seconds * samples per second
	//transition: tenth of a second crossfade
	
    while (--sampleFrames >= 0)
    {
		float inputSampleL = *in1;
		float inputSampleR = *in2;
		float outputSampleL = inputSampleL;
		float outputSampleR = inputSampleR;
		
		tick++;
		if (tick > time*2) tick = 0;
		
		if (tick < time) {
			if (tick < transition) {
				double crossfade;
				crossfade = tick;
				crossfade /= transition;
				//0 to 1 from beginning swapped to end as direct stereo out
				if (crossfade < 0.5) {
					outputSampleL = (inputSampleL*crossfade)+(inputSampleR*(1.0-crossfade));
					outputSampleR = (inputSampleR*crossfade)+(inputSampleL*(1.0-crossfade));
				} else {
					crossfade = fabs(crossfade-1.0);
					outputSampleL = (inputSampleR*crossfade)+(inputSampleL*(1.0-crossfade));
					outputSampleR = (inputSampleL*crossfade)+(inputSampleR*(1.0-crossfade));
				}
			} else {
				outputSampleL = inputSampleL;
				outputSampleR = inputSampleR;
				//direct stereo out
			}
		} else {
			if ((tick - time) < transition) {
				double crossfade;
				crossfade = tick-time;
				crossfade /= transition;
				//0 to 1 from beginning direct to end as swapped stereo
				if (crossfade < 0.5) {
					outputSampleL = (inputSampleR*crossfade)+(inputSampleL*(1.0-crossfade));
					outputSampleR = (inputSampleL*crossfade)+(inputSampleR*(1.0-crossfade));
				} else {
					crossfade = fabs(crossfade-1.0);
					outputSampleL = (inputSampleL*crossfade)+(inputSampleR*(1.0-crossfade));
					outputSampleR = (inputSampleR*crossfade)+(inputSampleL*(1.0-crossfade));
				}
			} else {
				outputSampleL = inputSampleR;
				outputSampleR = inputSampleL;
				//swapped stereo out
			}
		}
		
		*out1 = outputSampleL;
		*out2 = outputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void LRFlipTimer::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	uint32_t time = (uint32_t)(((A*9.0)+1.0) * 60.0 * getSampleRate());
	uint32_t transition = (uint32_t)(getSampleRate()*0.1);
	//number of minutes * 60 seconds * samples per second
	//transition: tenth of a second crossfade

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		double outputSampleL = inputSampleL;
		double outputSampleR = inputSampleR;
		
		tick++;
		if (tick > time*2) tick = 0;
		
		if (tick < time) {
			if (tick < transition) {
				double crossfade;
				crossfade = tick;
				crossfade /= transition;
				//0 to 1 from beginning swapped to end as direct stereo out
				if (crossfade < 0.5) {
					outputSampleL = (inputSampleL*crossfade)+(inputSampleR*(1.0-crossfade));
					outputSampleR = (inputSampleR*crossfade)+(inputSampleL*(1.0-crossfade));
				} else {
					crossfade = fabs(crossfade-1.0);
					outputSampleL = (inputSampleR*crossfade)+(inputSampleL*(1.0-crossfade));
					outputSampleR = (inputSampleL*crossfade)+(inputSampleR*(1.0-crossfade));
				}
			} else {
				outputSampleL = inputSampleL;
				outputSampleR = inputSampleR;
				//direct stereo out
			}
		} else {
			if ((tick - time) < transition) {
				double crossfade;
				crossfade = tick-time;
				crossfade /= transition;
				//0 to 1 from beginning direct to end as swapped stereo
				if (crossfade < 0.5) {
					outputSampleL = (inputSampleR*crossfade)+(inputSampleL*(1.0-crossfade));
					outputSampleR = (inputSampleL*crossfade)+(inputSampleR*(1.0-crossfade));
				} else {
					crossfade = fabs(crossfade-1.0);
					outputSampleL = (inputSampleL*crossfade)+(inputSampleR*(1.0-crossfade));
					outputSampleR = (inputSampleR*crossfade)+(inputSampleL*(1.0-crossfade));
				}
			} else {
				outputSampleL = inputSampleR;
				outputSampleR = inputSampleL;
				//swapped stereo out
			}
		}
		
		*out1 = outputSampleL;
		*out2 = outputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
