/* ========================================
 *  ADClip8 - ADClip8.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ADClip8_H
#include "ADClip8.h"
#endif

void ADClip8::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	//double hardness = 0.618033988749894; // golden ratio
	//double softness = 0.381966011250105; // 1.0 - hardness
	//double refclip = 1.618033988749894; // -0.2dB we're making all this pure raw code
	//refclip*hardness = 1.0  to use ClipOnly as a prefab code-chunk.
	//refclip*softness = 0.618033988749894	Seven decimal places is plenty as it's
	//not related to the original sound much: it's an arbitrary position in softening.
	double inputGain = pow(10.0,(A*18.0)/20.0);
	
	double ceiling = (1.0+(B*0.23594733))*0.5;
	int mode = (int) (C*7.999)+1;
	int stageSetting = mode-2;
	if (stageSetting < 1) stageSetting = 1;
	inputGain = ((inputGain-1.0)/stageSetting)+1.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double overshootL = 0.0;
		double overshootR = 0.0;
		inputSampleL *= 1.618033988749894;
		inputSampleR *= 1.618033988749894;
		
		for (int stage = 0; stage < stageSetting; stage++) {
			if (inputGain != 1.0) {
				inputSampleL *= inputGain;
				inputSampleR *= inputGain;
			}
			if (stage == 0){
				overshootL = fabs(inputSampleL) - 1.618033988749894;
				if (overshootL < 0.0) overshootL = 0.0;
				overshootR = fabs(inputSampleR) - 1.618033988749894;
				if (overshootR < 0.0) overshootR = 0.0;
			}
			if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
			if (inputSampleL - lastSampleL[stage] > 0.618033988749894) inputSampleL = lastSampleL[stage] + 0.618033988749894;
			if (inputSampleL - lastSampleL[stage] < -0.618033988749894) inputSampleL = lastSampleL[stage] - 0.618033988749894;
			if (inputSampleR > 4.0) inputSampleR = 4.0; if (inputSampleR < -4.0) inputSampleR = -4.0;
			if (inputSampleR - lastSampleR[stage] > 0.618033988749894) inputSampleR = lastSampleR[stage] + 0.618033988749894;
			if (inputSampleR - lastSampleR[stage] < -0.618033988749894) inputSampleR = lastSampleR[stage] - 0.618033988749894;
			//same as slew clippage
			
			//begin ClipOnly2 as a little, compressed chunk that can be dropped into code
			if (wasPosClipL[stage] == true) { //current will be over
				if (inputSampleL<lastSampleL[stage]) lastSampleL[stage]=1.0+(inputSampleL*0.381966011250105);
				else lastSampleL[stage] = 0.618033988749894+(lastSampleL[stage]*0.618033988749894);
			} wasPosClipL[stage] = false;
			if (inputSampleL>1.618033988749894) {wasPosClipL[stage]=true;inputSampleL=1.0+(lastSampleL[stage]*0.381966011250105);}
			if (wasNegClipL[stage] == true) { //current will be -over
				if (inputSampleL > lastSampleL[stage]) lastSampleL[stage]=-1.0+(inputSampleL*0.381966011250105);
				else lastSampleL[stage]=-0.618033988749894+(lastSampleL[stage]*0.618033988749894);
			} wasNegClipL[stage] = false;
			if (inputSampleL<-1.618033988749894) {wasNegClipL[stage]=true;inputSampleL=-1.0+(lastSampleL[stage]*0.381966011250105);}
			intermediateL[spacing][stage] = inputSampleL;
			inputSampleL = lastSampleL[stage]; //Latency is however many samples equals one 44.1k sample
			for (int x = spacing; x > 0; x--) intermediateL[x-1][stage] = intermediateL[x][stage];
			lastSampleL[stage] = intermediateL[0][stage]; //run a little buffer to handle this
			//end ClipOnly2 as a little, compressed chunk that can be dropped into code

			//begin ClipOnly2 as a little, compressed chunk that can be dropped into code
			if (wasPosClipR[stage] == true) { //current will be over
				if (inputSampleR<lastSampleR[stage]) lastSampleR[stage]=1.0+(inputSampleR*0.381966011250105);
				else lastSampleR[stage] = 0.618033988749894+(lastSampleR[stage]*0.618033988749894);
			} wasPosClipR[stage] = false;
			if (inputSampleR>1.618033988749894) {wasPosClipR[stage]=true;inputSampleR=1.0+(lastSampleR[stage]*0.381966011250105);}
			if (wasNegClipR[stage] == true) { //current will be -over
				if (inputSampleR > lastSampleR[stage]) lastSampleR[stage]=-1.0+(inputSampleR*0.381966011250105);
				else lastSampleR[stage]=-0.618033988749894+(lastSampleR[stage]*0.618033988749894);
			} wasNegClipR[stage] = false;
			if (inputSampleR<-1.618033988749894) {wasNegClipR[stage]=true;inputSampleR=-1.0+(lastSampleR[stage]*0.381966011250105);}
			intermediateR[spacing][stage] = inputSampleR;
			inputSampleR = lastSampleR[stage]; //Latency is however many samples equals one 44.1k sample
			for (int x = spacing; x > 0; x--) intermediateR[x-1][stage] = intermediateR[x][stage];
			lastSampleR[stage] = intermediateR[0][stage]; //run a little buffer to handle this
			//end ClipOnly2 as a little, compressed chunk that can be dropped into code
		}
		
		switch (mode)
		{
			case 1: break; //Normal
			case 2: inputSampleL /= inputGain; inputSampleR /= inputGain; break; //Gain Match
			case 3: inputSampleL = overshootL; inputSampleR = overshootR; break; //Clip Only
			case 4: break; //Afterburner
			case 5: break; //Explode
			case 6: break; //Nuke
			case 7: break; //Apocalypse
			case 8: break; //Apotheosis
		}
		//this is our output mode switch, showing the effects
		inputSampleL *= ceiling;
		inputSampleR *= ceiling;
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void ADClip8::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	//double hardness = 0.618033988749894; // golden ratio
	//double softness = 0.381966011250105; // 1.0 - hardness
	//double refclip = 1.618033988749894; // -0.2dB we're making all this pure raw code
	//refclip*hardness = 1.0  to use ClipOnly as a prefab code-chunk.
	//refclip*softness = 0.618033988749894	Seven decimal places is plenty as it's
	//not related to the original sound much: it's an arbitrary position in softening.
	double inputGain = pow(10.0,(A*18.0)/20.0);
	
	double ceiling = (1.0+(B*0.23594733))*0.5;
	int mode = (int) (C*7.999)+1;
	int stageSetting = mode-2;
	if (stageSetting < 1) stageSetting = 1;
	inputGain = ((inputGain-1.0)/stageSetting)+1.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double overshootL = 0.0;
		double overshootR = 0.0;
		inputSampleL *= 1.618033988749894;
		inputSampleR *= 1.618033988749894;
		
		for (int stage = 0; stage < stageSetting; stage++) {
			if (inputGain != 1.0) {
				inputSampleL *= inputGain;
				inputSampleR *= inputGain;
			}
			if (stage == 0){
				overshootL = fabs(inputSampleL) - 1.618033988749894;
				if (overshootL < 0.0) overshootL = 0.0;
				overshootR = fabs(inputSampleR) - 1.618033988749894;
				if (overshootR < 0.0) overshootR = 0.0;
			}
			if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
			if (inputSampleL - lastSampleL[stage] > 0.618033988749894) inputSampleL = lastSampleL[stage] + 0.618033988749894;
			if (inputSampleL - lastSampleL[stage] < -0.618033988749894) inputSampleL = lastSampleL[stage] - 0.618033988749894;
			if (inputSampleR > 4.0) inputSampleR = 4.0; if (inputSampleR < -4.0) inputSampleR = -4.0;
			if (inputSampleR - lastSampleR[stage] > 0.618033988749894) inputSampleR = lastSampleR[stage] + 0.618033988749894;
			if (inputSampleR - lastSampleR[stage] < -0.618033988749894) inputSampleR = lastSampleR[stage] - 0.618033988749894;
			//same as slew clippage
			
			//begin ClipOnly2 as a little, compressed chunk that can be dropped into code
			if (wasPosClipL[stage] == true) { //current will be over
				if (inputSampleL<lastSampleL[stage]) lastSampleL[stage]=1.0+(inputSampleL*0.381966011250105);
				else lastSampleL[stage] = 0.618033988749894+(lastSampleL[stage]*0.618033988749894);
			} wasPosClipL[stage] = false;
			if (inputSampleL>1.618033988749894) {wasPosClipL[stage]=true;inputSampleL=1.0+(lastSampleL[stage]*0.381966011250105);}
			if (wasNegClipL[stage] == true) { //current will be -over
				if (inputSampleL > lastSampleL[stage]) lastSampleL[stage]=-1.0+(inputSampleL*0.381966011250105);
				else lastSampleL[stage]=-0.618033988749894+(lastSampleL[stage]*0.618033988749894);
			} wasNegClipL[stage] = false;
			if (inputSampleL<-1.618033988749894) {wasNegClipL[stage]=true;inputSampleL=-1.0+(lastSampleL[stage]*0.381966011250105);}
			intermediateL[spacing][stage] = inputSampleL;
			inputSampleL = lastSampleL[stage]; //Latency is however many samples equals one 44.1k sample
			for (int x = spacing; x > 0; x--) intermediateL[x-1][stage] = intermediateL[x][stage];
			lastSampleL[stage] = intermediateL[0][stage]; //run a little buffer to handle this
			//end ClipOnly2 as a little, compressed chunk that can be dropped into code
			
			//begin ClipOnly2 as a little, compressed chunk that can be dropped into code
			if (wasPosClipR[stage] == true) { //current will be over
				if (inputSampleR<lastSampleR[stage]) lastSampleR[stage]=1.0+(inputSampleR*0.381966011250105);
				else lastSampleR[stage] = 0.618033988749894+(lastSampleR[stage]*0.618033988749894);
			} wasPosClipR[stage] = false;
			if (inputSampleR>1.618033988749894) {wasPosClipR[stage]=true;inputSampleR=1.0+(lastSampleR[stage]*0.381966011250105);}
			if (wasNegClipR[stage] == true) { //current will be -over
				if (inputSampleR > lastSampleR[stage]) lastSampleR[stage]=-1.0+(inputSampleR*0.381966011250105);
				else lastSampleR[stage]=-0.618033988749894+(lastSampleR[stage]*0.618033988749894);
			} wasNegClipR[stage] = false;
			if (inputSampleR<-1.618033988749894) {wasNegClipR[stage]=true;inputSampleR=-1.0+(lastSampleR[stage]*0.381966011250105);}
			intermediateR[spacing][stage] = inputSampleR;
			inputSampleR = lastSampleR[stage]; //Latency is however many samples equals one 44.1k sample
			for (int x = spacing; x > 0; x--) intermediateR[x-1][stage] = intermediateR[x][stage];
			lastSampleR[stage] = intermediateR[0][stage]; //run a little buffer to handle this
			//end ClipOnly2 as a little, compressed chunk that can be dropped into code
		}
		
		switch (mode)
		{
			case 1: break; //Normal
			case 2: inputSampleL /= inputGain; inputSampleR /= inputGain; break; //Gain Match
			case 3: inputSampleL = overshootL; inputSampleR = overshootR; break; //Clip Only
			case 4: break; //Afterburner
			case 5: break; //Explode
			case 6: break; //Nuke
			case 7: break; //Apocalypse
			case 8: break; //Apotheosis
		}
		//this is our output mode switch, showing the effects
		inputSampleL *= ceiling;
		inputSampleR *= ceiling;
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
