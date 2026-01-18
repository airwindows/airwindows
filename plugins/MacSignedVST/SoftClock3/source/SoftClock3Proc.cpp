/* ========================================
 *  SoftClock3 - SoftClock3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SoftClock3_H
#include "SoftClock3.h"
#endif

void SoftClock3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int bpm = (int)(A*200.99)+40;
	int beatCode = (int)(B*32.99);
	double notes = (double)fmax(((int)(C*16.99))/4.0, 0.125);
	double bpmTarget = (getSampleRate()*60.0)/((double)bpm*notes);
	double triplet = 0.0;
	if (D*1.99 > 0.5) triplet = 1.0;
	double accent = E;
	
	if (bpm != currentBPM) {
		currentBPM = bpm;
		step = 20.0;
		double trial = 250.0;
		while (trial > bpm) {step -= 1.0; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest 'flow' energy state UNDER the real bpm, and will step up more slowly
		while (trial < bpm) {step += 0.01; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest tenth of the energy state and have overshot
		while (trial > bpm) {step -= 0.001; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest hundredth to the correct energy state, and can throw away 'trial'
		step = step-((int)step); //and calculate speed based on the fractional remainder of 'step'
		step = ((0.5-fabs(step-0.5))*2.0) * 0.75;
		swagger = pow((0.5-fabs(step-0.5))*2.0,1.618033988749894) * 0.5 * accent;
		//and turn it into the 'speed' control
		//with step 0.0 being 'flow', 0.25 being 'groove', 0.5 being 'edge' and 0.75 being 'tude'
		//turned into a linear speed control where edge is top step and flow being lowest step.
	}	
	double swing = (triplet+swagger)*bpmTarget*0.33333;
	//swing makes beats hit LATER, so the One is 0.0
	int beatMax = beatTable[beatCode];
	//only some counts are literal, others are ways to do prime grooves with different subrhythms
	for (int x = 0; x < (beatMax+1); x++) {
		beatAccent[x] = ((double)fabs((double)beatMax-((double)x*2.0)))/(double)(beatMax*1.618033988749894);
		if (x % 2 > 0) beatSwing[x] = (swagger*(1.0-beatAccent[x]));
		else beatSwing[x] = swing;
	} //this makes the non-accented beats drop down to quiet and back up to half volume
	//we're also decoupling swing from triplet feel unless actually playing triplets
	//otherwise, it's part of how you hear accents and tempos
	if (beatCode > 0) beatAccent[1] = 0.9; beatSwing[1] = 0.0; //first note is an accent at full crank
	switch (beatCode)
	{
		case 0:	break; //not used
		case 1:	break; //1
		case 2:	break; //2
		case 3:	break; //3
		case 4: beatAccent[3]=0.9;
			beatSwing[3]=0.0; break; //4-22
		case 5:	beatAccent[4]=0.9;
			beatSwing[4]=0.0; break; //5-32
		case 6:	beatAccent[4]=0.9;
			beatSwing[4]=0.0; break; //6-33
		case 7:	beatAccent[5]=0.9;
			beatSwing[5]=0.0; break; //7-43
		case 8:	beatAccent[5]=0.9;
			beatSwing[5]=0.0; break; //8-44
		case 9:	beatAccent[4]=0.9; beatAccent[7]=0.8;
			beatSwing[4]=0.0; beatSwing[7]=0.0; break; //9-333
		case 10: beatAccent[6]=0.9;
			beatSwing[6]=0.0; break; //10-55
		case 11: beatAccent[4]=0.9;	beatAccent[7]=0.8;	beatAccent[10]=0.7;
			beatSwing[4]=0.0; beatSwing[7]=0.0; beatSwing[10]=0.0; break; //11-3332
		case 12: beatAccent[5]=0.9;	beatAccent[9]=0.8;
			beatSwing[5]=0.0; beatSwing[9]=0.0; break; //11-443
		case 13: beatAccent[6]=0.9;	beatAccent[11]=0.8;
			beatSwing[6]=0.0; beatSwing[11]=0.0; break; //11-551
		case 14: beatAccent[7]=0.9;
			beatSwing[7]=0.0; break; //11-65
		case 15: beatAccent[4]=0.9;	beatAccent[7]=0.8;	beatAccent[10]=0.7;
			beatSwing[4]=0.0; beatSwing[7]=0.0; beatSwing[10]=0.0; break; //13-3334
		case 16: beatAccent[9]=0.9;
			beatSwing[9]=0.0; break; //16-88
		case 17: beatAccent[5]=0.9;	beatAccent[9]=0.8;
			beatSwing[5]=0.0; beatSwing[9]=0.0; break; //13-445
		case 18: beatAccent[6]=0.9;	beatAccent[11]=0.8;
			beatSwing[6]=0.0; beatSwing[11]=0.0; break; //13-553
		case 19: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; break; //17-44441
		case 20: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //17-5552
		case 21: beatAccent[8]=0.9;	beatAccent[15]=0.8;
			beatSwing[8]=0.0; beatSwing[15]=0.0; break; //17-773
		case 22: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //17-881
		case 23: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; break; //19-44443
		case 24: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //24-888
		case 25: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //19-5554
		case 26: beatAccent[8]=0.9;	beatAccent[15]=0.8;
			beatSwing[8]=0.0; beatSwing[15]=0.0; break; //19-775
		case 27: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //19-883
		case 28: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;	beatAccent[21]=0.7;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; beatSwing[21]=0.0; break; //23-444443
		case 29: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //23-5558
		case 30: beatAccent[8]=0.9;	beatAccent[15]=0.8;	beatAccent[22]=0.7;
			beatSwing[8]=0.0; beatSwing[15]=0.0; beatSwing[22]=0.0; break; //23-7772
		case 31: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //23-887
		case 32: beatAccent[9]=0.9;	beatAccent[17]=0.8;	beatAccent[25]=0.7;
			beatSwing[9]=0.0; beatSwing[17]=0.0; beatSwing[25]=0.0; break; //32-8888
		default: break;			
	}			
	double chaseSpeed = ((step*0.000125)+0.0000125)/overallscale;
	double rootSpeed = 0.9999-(chaseSpeed*((2.0*step)+2.0));
	double pulseWidth = (0.085+(accent*0.04)-((1.0-step)*0.06))/chaseSpeed;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		barPos += 1.0;
		if (barPos>bpmTarget) {
			barPos=0.0;
			beatPos++;
			if (beatPos>beatMax) beatPos=1;
		}
		if ((barPos < (pulseWidth+beatSwing[beatPos])) && (barPos > beatSwing[beatPos]))
			inc = (((beatAccent[beatPos]*accent)+(1.0-accent))*chaseSpeed)+(inc*(1.0-chaseSpeed));
		else
			inc *= rootSpeed;
		sinePos += inc;
		if (sinePos > 6.283185307179586) sinePos -= 6.283185307179586;
		inputSampleL = inputSampleR = sin(sin(sinePos)*inc*5.0);
		
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

void SoftClock3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int bpm = (int)(A*200.99)+40;
	int beatCode = (int)(B*32.99);
	double notes = (double)fmax(((int)(C*16.99))/4.0, 0.125);
	double bpmTarget = (getSampleRate()*60.0)/((double)bpm*notes);
	double triplet = 0.0;
	if (D*1.99 > 0.5) triplet = 1.0;
	double accent = E;
	
	if (bpm != currentBPM) {
		currentBPM = bpm;
		step = 20.0;
		double trial = 250.0;
		while (trial > bpm) {step -= 1.0; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest 'flow' energy state UNDER the real bpm, and will step up more slowly
		while (trial < bpm) {step += 0.01; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest tenth of the energy state and have overshot
		while (trial > bpm) {step -= 0.001; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest hundredth to the correct energy state, and can throw away 'trial'
		step = step-((int)step); //and calculate speed based on the fractional remainder of 'step'
		step = ((0.5-fabs(step-0.5))*2.0) * 0.75;
		swagger = pow((0.5-fabs(step-0.5))*2.0,1.618033988749894) * 0.5 * accent;
		//and turn it into the 'speed' control
		//with step 0.0 being 'flow', 0.25 being 'groove', 0.5 being 'edge' and 0.75 being 'tude'
		//turned into a linear speed control where edge is top step and flow being lowest step.
	}	
	double swing = (triplet+swagger)*bpmTarget*0.33333;
	//swing makes beats hit LATER, so the One is 0.0
	int beatMax = beatTable[beatCode];
	//only some counts are literal, others are ways to do prime grooves with different subrhythms
	for (int x = 0; x < (beatMax+1); x++) {
		beatAccent[x] = ((double)fabs((double)beatMax-((double)x*2.0)))/(double)(beatMax*1.618033988749894);
		if (x % 2 > 0) beatSwing[x] = (swagger*(1.0-beatAccent[x]));
		else beatSwing[x] = swing;
	} //this makes the non-accented beats drop down to quiet and back up to half volume
	//we're also decoupling swing from triplet feel unless actually playing triplets
	//otherwise, it's part of how you hear accents and tempos
	if (beatCode > 0) beatAccent[1] = 0.9; beatSwing[1] = 0.0; //first note is an accent at full crank
	switch (beatCode)
	{
		case 0:	break; //not used
		case 1:	break; //1
		case 2:	break; //2
		case 3:	break; //3
		case 4: beatAccent[3]=0.9;
			beatSwing[3]=0.0; break; //4-22
		case 5:	beatAccent[4]=0.9;
			beatSwing[4]=0.0; break; //5-32
		case 6:	beatAccent[4]=0.9;
			beatSwing[4]=0.0; break; //6-33
		case 7:	beatAccent[5]=0.9;
			beatSwing[5]=0.0; break; //7-43
		case 8:	beatAccent[5]=0.9;
			beatSwing[5]=0.0; break; //8-44
		case 9:	beatAccent[4]=0.9; beatAccent[7]=0.8;
			beatSwing[4]=0.0; beatSwing[7]=0.0; break; //9-333
		case 10: beatAccent[6]=0.9;
			beatSwing[6]=0.0; break; //10-55
		case 11: beatAccent[4]=0.9;	beatAccent[7]=0.8;	beatAccent[10]=0.7;
			beatSwing[4]=0.0; beatSwing[7]=0.0; beatSwing[10]=0.0; break; //11-3332
		case 12: beatAccent[5]=0.9;	beatAccent[9]=0.8;
			beatSwing[5]=0.0; beatSwing[9]=0.0; break; //11-443
		case 13: beatAccent[6]=0.9;	beatAccent[11]=0.8;
			beatSwing[6]=0.0; beatSwing[11]=0.0; break; //11-551
		case 14: beatAccent[7]=0.9;
			beatSwing[7]=0.0; break; //11-65
		case 15: beatAccent[4]=0.9;	beatAccent[7]=0.8;	beatAccent[10]=0.7;
			beatSwing[4]=0.0; beatSwing[7]=0.0; beatSwing[10]=0.0; break; //13-3334
		case 16: beatAccent[9]=0.9;
			beatSwing[9]=0.0; break; //16-88
		case 17: beatAccent[5]=0.9;	beatAccent[9]=0.8;
			beatSwing[5]=0.0; beatSwing[9]=0.0; break; //13-445
		case 18: beatAccent[6]=0.9;	beatAccent[11]=0.8;
			beatSwing[6]=0.0; beatSwing[11]=0.0; break; //13-553
		case 19: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; break; //17-44441
		case 20: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //17-5552
		case 21: beatAccent[8]=0.9;	beatAccent[15]=0.8;
			beatSwing[8]=0.0; beatSwing[15]=0.0; break; //17-773
		case 22: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //17-881
		case 23: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; break; //19-44443
		case 24: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //24-888
		case 25: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //19-5554
		case 26: beatAccent[8]=0.9;	beatAccent[15]=0.8;
			beatSwing[8]=0.0; beatSwing[15]=0.0; break; //19-775
		case 27: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //19-883
		case 28: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;	beatAccent[21]=0.7;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; beatSwing[21]=0.0; break; //23-444443
		case 29: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //23-5558
		case 30: beatAccent[8]=0.9;	beatAccent[15]=0.8;	beatAccent[22]=0.7;
			beatSwing[8]=0.0; beatSwing[15]=0.0; beatSwing[22]=0.0; break; //23-7772
		case 31: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //23-887
		case 32: beatAccent[9]=0.9;	beatAccent[17]=0.8;	beatAccent[25]=0.7;
			beatSwing[9]=0.0; beatSwing[17]=0.0; beatSwing[25]=0.0; break; //32-8888
		default: break;			
	}			
	double chaseSpeed = ((step*0.000125)+0.0000125)/overallscale;
	double rootSpeed = 0.9999-(chaseSpeed*((2.0*step)+2.0));
	double pulseWidth = (0.085+(accent*0.04)-((1.0-step)*0.06))/chaseSpeed;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		barPos += 1.0;
		if (barPos>bpmTarget) {
			barPos=0.0;
			beatPos++;
			if (beatPos>beatMax) beatPos=1;
		}
		if ((barPos < (pulseWidth+beatSwing[beatPos])) && (barPos > beatSwing[beatPos]))
			inc = (((beatAccent[beatPos]*accent)+(1.0-accent))*chaseSpeed)+(inc*(1.0-chaseSpeed));
		else
			inc *= rootSpeed;
		sinePos += inc;
		if (sinePos > 6.283185307179586) sinePos -= 6.283185307179586;
		inputSampleL = inputSampleR = sin(sin(sinePos)*inc*5.0);
		
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
