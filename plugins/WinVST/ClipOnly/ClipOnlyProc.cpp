/* ========================================
 *  ClipOnly - ClipOnly.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ClipOnly_H
#include "ClipOnly.h"
#endif

void ClipOnly::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double hardness = 0.7390851332151606; // x == cos(x)
	double softness = 1.0 - hardness;
	double refclip = 0.9549925859; // -0.2dB
	
	float inputSampleL;
	float inputSampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;

		if (inputSampleL > 4.0) inputSampleL = 4.0;
		if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (inputSampleR > 4.0) inputSampleR = 4.0;
		if (inputSampleR < -4.0) inputSampleR = -4.0;
		
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL < lastSampleL) { //next one will NOT be over
				lastSampleL = ((refclip*hardness) + (inputSampleL * softness));
			}
			else { //still clipping, still chasing the target
				lastSampleL = ((lastSampleL*hardness) + (refclip * softness));
			}
		}
		wasPosClipL = false;
		if (inputSampleL > refclip) { //next will be over the true clip level. otherwise we directly use it
			wasPosClipL = true; //set the clip flag
			inputSampleL = ((refclip*hardness) + (lastSampleL * softness));
		}
		
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) { //next one will NOT be -over
				lastSampleL = ((-refclip*hardness) + (inputSampleL * softness));
			}
			else { //still clipping, still chasing the target
				lastSampleL = ((lastSampleL*hardness) + (-refclip * softness));
			}
		}
		wasNegClipL = false;
		if (inputSampleL < -refclip) { //next will be -refclip.  otherwise we directly use it
			wasNegClipL = true; //set the clip flag
			inputSampleL = ((-refclip*hardness) + (lastSampleL * softness));
		}

		if (wasPosClipR == true) { //current will be over
			if (inputSampleR < lastSampleR) { //next one will NOT be over
				lastSampleR = ((refclip*hardness) + (inputSampleR * softness));
			}
			else { //still clipping, still chasing the target
				lastSampleR = ((lastSampleR*hardness) + (refclip * softness));
			}
		}
		wasPosClipR = false;
		if (inputSampleR > refclip) { //next will be over the true clip level. otherwise we directly use it
			wasPosClipR = true; //set the clip flag
			inputSampleR = ((refclip*hardness) + (lastSampleR * softness));
		}
		
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) { //next one will NOT be -over
				lastSampleR = ((-refclip*hardness) + (inputSampleR * softness));
			}
			else { //still clipping, still chasing the target
				lastSampleR = ((lastSampleR*hardness) + (-refclip * softness));
			}
		}
		wasNegClipR = false;
		if (inputSampleR < -refclip) { //next will be -refclip.  otherwise we directly use it
			wasNegClipR = true; //set the clip flag
			inputSampleR = ((-refclip*hardness) + (lastSampleR * softness));
		}
				
		*out1 = lastSampleL;
		*out2 = lastSampleR;
		lastSampleL = inputSampleL;
		lastSampleR = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void ClipOnly::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double hardness = 0.7390851332151606; // x == cos(x)
	double softness = 1.0 - hardness;
	double refclip = 0.9549925859; // -0.2dB
	
	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;

		if (inputSampleL > 4.0) inputSampleL = 4.0;
		if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (inputSampleR > 4.0) inputSampleR = 4.0;
		if (inputSampleR < -4.0) inputSampleR = -4.0;
		
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL < lastSampleL) { //next one will NOT be over
				lastSampleL = ((refclip*hardness) + (inputSampleL * softness));
			}
			else { //still clipping, still chasing the target
				lastSampleL = ((lastSampleL*hardness) + (refclip * softness));
			}
		}
		wasPosClipL = false;
		if (inputSampleL > refclip) { //next will be over the true clip level. otherwise we directly use it
			wasPosClipL = true; //set the clip flag
			inputSampleL = ((refclip*hardness) + (lastSampleL * softness));
		}
		
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) { //next one will NOT be -over
				lastSampleL = ((-refclip*hardness) + (inputSampleL * softness));
			}
			else { //still clipping, still chasing the target
				lastSampleL = ((lastSampleL*hardness) + (-refclip * softness));
			}
		}
		wasNegClipL = false;
		if (inputSampleL < -refclip) { //next will be -refclip.  otherwise we directly use it
			wasNegClipL = true; //set the clip flag
			inputSampleL = ((-refclip*hardness) + (lastSampleL * softness));
		}
		
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR < lastSampleR) { //next one will NOT be over
				lastSampleR = ((refclip*hardness) + (inputSampleR * softness));
			}
			else { //still clipping, still chasing the target
				lastSampleR = ((lastSampleR*hardness) + (refclip * softness));
			}
		}
		wasPosClipR = false;
		if (inputSampleR > refclip) { //next will be over the true clip level. otherwise we directly use it
			wasPosClipR = true; //set the clip flag
			inputSampleR = ((refclip*hardness) + (lastSampleR * softness));
		}
		
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) { //next one will NOT be -over
				lastSampleR = ((-refclip*hardness) + (inputSampleR * softness));
			}
			else { //still clipping, still chasing the target
				lastSampleR = ((lastSampleR*hardness) + (-refclip * softness));
			}
		}
		wasNegClipR = false;
		if (inputSampleR < -refclip) { //next will be -refclip.  otherwise we directly use it
			wasNegClipR = true; //set the clip flag
			inputSampleR = ((-refclip*hardness) + (lastSampleR * softness));
		}
		
		*out1 = lastSampleL;
		*out2 = lastSampleR;
		lastSampleL = inputSampleL;
		lastSampleR = inputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}