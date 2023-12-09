/* ========================================
 *  MSFlipTimer - MSFlipTimer.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __MSFlipTimer_H
#include "MSFlipTimer.h"
#endif

void MSFlipTimer::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
				//0 to 1 from beginning mono to end as direct stereo out
				double mono = (inputSampleL+inputSampleR)*0.5;
				if (crossfade < 0.5) {
					outputSampleL = (inputSampleL*crossfade)+(mono*(1.0-crossfade));
					outputSampleR = (inputSampleR*crossfade)+(mono*(1.0-crossfade));
				} else {
					crossfade = fabs(crossfade-1.0);
					outputSampleL = (mono*crossfade)+(inputSampleL*(1.0-crossfade));
					outputSampleR = (mono*crossfade)+(inputSampleR*(1.0-crossfade));
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
				//0 to 1 from beginning direct to end as mono
				double mono = (inputSampleL+inputSampleR)*0.5;
				if (crossfade < 0.5) {
					outputSampleL = (mono*crossfade)+(inputSampleL*(1.0-crossfade));
					outputSampleR = (mono*crossfade)+(inputSampleR*(1.0-crossfade));
				} else {
					crossfade = fabs(crossfade-1.0);
					outputSampleL = (inputSampleL*crossfade)+(mono*(1.0-crossfade));
					outputSampleR = (inputSampleR*crossfade)+(mono*(1.0-crossfade));
				}
			} else {
				double mono = (inputSampleL+inputSampleR)*0.5;
				outputSampleL = outputSampleR = mono;
				//mono out
			}
		}
				
		*out1 = outputSampleL;
		*out2 = outputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void MSFlipTimer::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
				//0 to 1 from beginning mono to end as direct stereo out
				double mono = (inputSampleL+inputSampleR)*0.5;
				if (crossfade < 0.5) {
					outputSampleL = (inputSampleL*crossfade)+(mono*(1.0-crossfade));
					outputSampleR = (inputSampleR*crossfade)+(mono*(1.0-crossfade));
				} else {
					crossfade = fabs(crossfade-1.0);
					outputSampleL = (mono*crossfade)+(inputSampleL*(1.0-crossfade));
					outputSampleR = (mono*crossfade)+(inputSampleR*(1.0-crossfade));
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
				//0 to 1 from beginning direct to end as mono
				double mono = (inputSampleL+inputSampleR)*0.5;
				if (crossfade < 0.5) {
					outputSampleL = (mono*crossfade)+(inputSampleL*(1.0-crossfade));
					outputSampleR = (mono*crossfade)+(inputSampleR*(1.0-crossfade));
				} else {
					crossfade = fabs(crossfade-1.0);
					outputSampleL = (inputSampleL*crossfade)+(mono*(1.0-crossfade));
					outputSampleR = (inputSampleR*crossfade)+(mono*(1.0-crossfade));
				}
			} else {
				double mono = (inputSampleL+inputSampleR)*0.5;
				outputSampleL = outputSampleR = mono;
				//mono out
			}
		}
				
		*out1 = outputSampleL;
		*out2 = outputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
