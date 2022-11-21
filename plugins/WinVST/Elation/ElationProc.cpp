/* ========================================
 *  Elation - Elation.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Elation_H
#include "Elation.h"
#endif

void Elation::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double wet = A;
	double sqdrive = (B*3.0);
	if (sqdrive > 1.0) sqdrive *= sqdrive;
	sqdrive = sqrt(sqdrive);
	double indrive = C*3.0;
	double compthreshold = (6.0 - indrive)/6.0;
	double recoveryspd = indrive / 32.0;
	if (indrive > 1.0) indrive *= indrive;
	indrive *= (1.0+(0.226*sqdrive));
	//no gain loss of convolution for APIcolypse
	//calibrate this to match noise level with character at 1.0
	//you get for instance 0.819 and 1.0-0.819 is 0.181
	double randy;
	double outlevel = D;
	double threshSample;
	double abSample;
	double outputSample;	
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL *= indrive;
		inputSampleR *= indrive;
		//calibrated to match gain through convolution and -0.3 correction
		
		abSample = fabs(inputSampleL);
		if (abSample > previousL) threshSample = previousL;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compAL = (compAL + (1.0 / threshSample)) / 2.0;
			compBL = (compBL + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSampleL *= compAL;
			threshSample *= compAL;
			if (threshSample > compthreshold)
			{compAL = (compAL + compAL + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compAL < 1.0) compAL += pow((1.0-compAL)*recoveryspd,7);}
		}
		else
		{
			inputSampleL *= compBL;
			threshSample *= compBL;
			if (threshSample > compthreshold)
			{compBL = (compBL + compBL + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compBL < 1.0) compBL += pow((1.0-compBL)*recoveryspd,7);}			
		}
		previousL = abSample;
		
		abSample = fabs(inputSampleR);
		if (abSample > previousR) threshSample = previousR;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compAR = (compAR + (1.0 / threshSample)) / 2.0;
			compBR = (compBR + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSampleR *= compAR;
			threshSample *= compAR;
			if (threshSample > compthreshold)
			{compAR = (compAR + compAR + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compAR < 1.0) compAR += pow((1.0-compAR)*recoveryspd,7);}
		}
		else
		{
			inputSampleR *= compBR;
			threshSample *= compBR;
			if (threshSample > compthreshold)
			{compBR = (compBR + compBR + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compBR < 1.0) compBR += pow((1.0-compBR)*recoveryspd,7);}			
		}
		previousR = abSample;
		
		if (sqdrive > 0.0){
			bL[33] = bL[32]; bL[32] = bL[31]; 
			bL[31] = bL[30]; bL[30] = bL[29]; bL[29] = bL[28]; bL[28] = bL[27]; bL[27] = bL[26]; bL[26] = bL[25]; bL[25] = bL[24]; bL[24] = bL[23]; 
			bL[23] = bL[22]; bL[22] = bL[21]; bL[21] = bL[20]; bL[20] = bL[19]; bL[19] = bL[18]; bL[18] = bL[17]; bL[17] = bL[16]; bL[16] = bL[15]; 
			bL[15] = bL[14]; bL[14] = bL[13]; bL[13] = bL[12]; bL[12] = bL[11]; bL[11] = bL[10]; bL[10] = bL[9]; bL[9] = bL[8]; bL[8] = bL[7]; 
			bL[7] = bL[6]; bL[6] = bL[5]; bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1]; bL[1] = bL[0]; bL[0] = inputSampleL * sqdrive;
			
			inputSampleL -= (bL[1] * (0.25867935358656502  - (0.00045755657070112*fabs(bL[1]))));
			inputSampleL += (bL[2] * (0.11509367290253694  - (0.00017494270657228*fabs(bL[2]))));
			inputSampleL -= (bL[3] * (0.06709853575891785  - (0.00058913102597723*fabs(bL[3]))));
			inputSampleL += (bL[4] * (0.01871006356851681  - (0.00003387358004645*fabs(bL[4]))));
			inputSampleL -= (bL[5] * (0.00794797957360465  - (0.00044224784691203*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.01956921817394220  - (0.00006718936750076*fabs(bL[6]))));
			inputSampleL += (bL[7] * (0.01682120257195205  + (0.00032857446292230*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.03401069039824205  - (0.00013634182872897*fabs(bL[8]))));
			inputSampleL += (bL[9] * (0.02369950268232634  + (0.00023112685751657*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.03477071178117132  - (0.00018029792231600*fabs(bL[10]))));
			inputSampleL += (bL[11] * (0.02024369717958201  + (0.00017337813374202*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.02819087729102172  - (0.00021438538665420*fabs(bL[12]))));
			inputSampleL += (bL[13] * (0.01147946743141303  + (0.00014424066034649*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.01894777011468867  - (0.00021549146262408*fabs(bL[14]))));
			inputSampleL += (bL[15] * (0.00301370330346873  + (0.00013527460148394*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.01067147835815486  - (0.00020960689910868*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.00402715397506384  - (0.00014421582712470*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.00502221703392005  - (0.00019805767015024*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.00808788533308497  - (0.00016095444141931*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.00232696588842683  - (0.00018384470981829*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.00943950821324531  - (0.00017098987347593*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.00193709517200834  - (0.00018151995939591*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.00899713952612659  - (0.00017385835059948*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.00280584331659089  - (0.00017742164162470*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.00780381001954970  - (0.00018002500755708*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.00400370310490333  - (0.00017471691087957*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.00661527728186928  - (0.00018137323370347*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.00496545526864518  - (0.00017681872601767*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.00580728820997532  - (0.00018186220389790*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.00549309984725666  - (0.00017722985399075*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.00542194777529239  - (0.00018486900185338*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.00565992080998939  - (0.00018005824393118*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.00532121562846656  - (0.00018643189636216*fabs(bL[33]))));			
			//we apply the first samples of the impulse- dynamically adjusted.
			
			bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR * sqdrive;
			
			
			inputSampleR -= (bR[1] * (0.25867935358656502  - (0.00045755657070112*fabs(bR[1]))));
			inputSampleR += (bR[2] * (0.11509367290253694  - (0.00017494270657228*fabs(bR[2]))));
			inputSampleR -= (bR[3] * (0.06709853575891785  - (0.00058913102597723*fabs(bR[3]))));
			inputSampleR += (bR[4] * (0.01871006356851681  - (0.00003387358004645*fabs(bR[4]))));
			inputSampleR -= (bR[5] * (0.00794797957360465  - (0.00044224784691203*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.01956921817394220  - (0.00006718936750076*fabs(bR[6]))));
			inputSampleR += (bR[7] * (0.01682120257195205  + (0.00032857446292230*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.03401069039824205  - (0.00013634182872897*fabs(bR[8]))));
			inputSampleR += (bR[9] * (0.02369950268232634  + (0.00023112685751657*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.03477071178117132  - (0.00018029792231600*fabs(bR[10]))));
			inputSampleR += (bR[11] * (0.02024369717958201  + (0.00017337813374202*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.02819087729102172  - (0.00021438538665420*fabs(bR[12]))));
			inputSampleR += (bR[13] * (0.01147946743141303  + (0.00014424066034649*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.01894777011468867  - (0.00021549146262408*fabs(bR[14]))));
			inputSampleR += (bR[15] * (0.00301370330346873  + (0.00013527460148394*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.01067147835815486  - (0.00020960689910868*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.00402715397506384  - (0.00014421582712470*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.00502221703392005  - (0.00019805767015024*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.00808788533308497  - (0.00016095444141931*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.00232696588842683  - (0.00018384470981829*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.00943950821324531  - (0.00017098987347593*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.00193709517200834  - (0.00018151995939591*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.00899713952612659  - (0.00017385835059948*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.00280584331659089  - (0.00017742164162470*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.00780381001954970  - (0.00018002500755708*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.00400370310490333  - (0.00017471691087957*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.00661527728186928  - (0.00018137323370347*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.00496545526864518  - (0.00017681872601767*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.00580728820997532  - (0.00018186220389790*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.00549309984725666  - (0.00017722985399075*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.00542194777529239  - (0.00018486900185338*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.00565992080998939  - (0.00018005824393118*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.00532121562846656  - (0.00018643189636216*fabs(bR[33]))));
			//we apply the first samples of the impulse- dynamically adjusted.
		}
		
		abSample = fabs(inputSampleL);
		if (abSample > previousBL) threshSample = previousBL;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compCL = (compCL + (1.0 / threshSample)) / 2.0;
			compDL = (compDL + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSampleL *= compCL;
			threshSample *= compCL;
			if (threshSample > compthreshold)
			{compCL = (compCL + compCL + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compCL < 1.0) compCL += pow((1.0-compCL)*recoveryspd,7);}
		}
		else
		{
			inputSampleL *= compDL;
			threshSample *= compDL;
			if (threshSample > compthreshold)
			{compDL = (compDL + compDL + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compDL < 1.0) compDL += pow((1.0-compDL)*recoveryspd,7);}			
		}
		previousBL = abSample;
		//another dose of compression, please
		
		abSample = fabs(inputSampleR);
		if (abSample > previousBR) threshSample = previousBR;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compCR = (compCR + (1.0 / threshSample)) / 2.0;
			compDR = (compDR + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSampleR *= compCR;
			threshSample *= compCR;
			if (threshSample > compthreshold)
			{compCR = (compCR + compCR + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compCR < 1.0) compCR += pow((1.0-compCR)*recoveryspd,7);}
		}
		else
		{
			inputSampleR *= compDR;
			threshSample *= compDR;
			if (threshSample > compthreshold)
			{compDR = (compDR + compDR + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compDR < 1.0) compDR += pow((1.0-compDR)*recoveryspd,7);}			
		}
		previousBR = abSample;
		//another dose of compression, please
		
		flip = !flip;
		
		randy = ((double(fpdL)/UINT32_MAX)*0.054);
		outputSample = ((((inputSampleL*(1-randy))+(lastSampleL*randy))*wet)+(drySampleL*(1.0-wet))) * outlevel;		
		lastSampleL = inputSampleL;
		inputSampleL = outputSample;

		randy = ((double(fpdR)/UINT32_MAX)*0.054);
		outputSample = ((((inputSampleR*(1-randy))+(lastSampleR*randy))*wet)+(drySampleR*(1.0-wet))) * outlevel;
		lastSampleR = inputSampleR;
		inputSampleR = outputSample;
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Elation::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double wet = A;
	double sqdrive = (B*3.0);
	if (sqdrive > 1.0) sqdrive *= sqdrive;
	sqdrive = sqrt(sqdrive);
	double indrive = C*3.0;
	double compthreshold = (6.0 - indrive)/6.0;
	double recoveryspd = indrive / 32.0;
	if (indrive > 1.0) indrive *= indrive;
	indrive *= (1.0+(0.226*sqdrive));
	//no gain loss of convolution for APIcolypse
	//calibrate this to match noise level with character at 1.0
	//you get for instance 0.819 and 1.0-0.819 is 0.181
	double randy;
	double outlevel = D;
	double threshSample;
	double abSample;
	double outputSample;	
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL *= indrive;
		inputSampleR *= indrive;
		//calibrated to match gain through convolution and -0.3 correction
		
		abSample = fabs(inputSampleL);
		if (abSample > previousL) threshSample = previousL;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compAL = (compAL + (1.0 / threshSample)) / 2.0;
			compBL = (compBL + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSampleL *= compAL;
			threshSample *= compAL;
			if (threshSample > compthreshold)
			{compAL = (compAL + compAL + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compAL < 1.0) compAL += pow((1.0-compAL)*recoveryspd,7);}
		}
		else
		{
			inputSampleL *= compBL;
			threshSample *= compBL;
			if (threshSample > compthreshold)
			{compBL = (compBL + compBL + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compBL < 1.0) compBL += pow((1.0-compBL)*recoveryspd,7);}			
		}
		previousL = abSample;
		
		abSample = fabs(inputSampleR);
		if (abSample > previousR) threshSample = previousR;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compAR = (compAR + (1.0 / threshSample)) / 2.0;
			compBR = (compBR + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSampleR *= compAR;
			threshSample *= compAR;
			if (threshSample > compthreshold)
			{compAR = (compAR + compAR + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compAR < 1.0) compAR += pow((1.0-compAR)*recoveryspd,7);}
		}
		else
		{
			inputSampleR *= compBR;
			threshSample *= compBR;
			if (threshSample > compthreshold)
			{compBR = (compBR + compBR + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compBR < 1.0) compBR += pow((1.0-compBR)*recoveryspd,7);}			
		}
		previousR = abSample;
		
		if (sqdrive > 0.0){
			bL[33] = bL[32]; bL[32] = bL[31]; 
			bL[31] = bL[30]; bL[30] = bL[29]; bL[29] = bL[28]; bL[28] = bL[27]; bL[27] = bL[26]; bL[26] = bL[25]; bL[25] = bL[24]; bL[24] = bL[23]; 
			bL[23] = bL[22]; bL[22] = bL[21]; bL[21] = bL[20]; bL[20] = bL[19]; bL[19] = bL[18]; bL[18] = bL[17]; bL[17] = bL[16]; bL[16] = bL[15]; 
			bL[15] = bL[14]; bL[14] = bL[13]; bL[13] = bL[12]; bL[12] = bL[11]; bL[11] = bL[10]; bL[10] = bL[9]; bL[9] = bL[8]; bL[8] = bL[7]; 
			bL[7] = bL[6]; bL[6] = bL[5]; bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1]; bL[1] = bL[0]; bL[0] = inputSampleL * sqdrive;
			
			inputSampleL -= (bL[1] * (0.25867935358656502  - (0.00045755657070112*fabs(bL[1]))));
			inputSampleL += (bL[2] * (0.11509367290253694  - (0.00017494270657228*fabs(bL[2]))));
			inputSampleL -= (bL[3] * (0.06709853575891785  - (0.00058913102597723*fabs(bL[3]))));
			inputSampleL += (bL[4] * (0.01871006356851681  - (0.00003387358004645*fabs(bL[4]))));
			inputSampleL -= (bL[5] * (0.00794797957360465  - (0.00044224784691203*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.01956921817394220  - (0.00006718936750076*fabs(bL[6]))));
			inputSampleL += (bL[7] * (0.01682120257195205  + (0.00032857446292230*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.03401069039824205  - (0.00013634182872897*fabs(bL[8]))));
			inputSampleL += (bL[9] * (0.02369950268232634  + (0.00023112685751657*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.03477071178117132  - (0.00018029792231600*fabs(bL[10]))));
			inputSampleL += (bL[11] * (0.02024369717958201  + (0.00017337813374202*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.02819087729102172  - (0.00021438538665420*fabs(bL[12]))));
			inputSampleL += (bL[13] * (0.01147946743141303  + (0.00014424066034649*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.01894777011468867  - (0.00021549146262408*fabs(bL[14]))));
			inputSampleL += (bL[15] * (0.00301370330346873  + (0.00013527460148394*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.01067147835815486  - (0.00020960689910868*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.00402715397506384  - (0.00014421582712470*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.00502221703392005  - (0.00019805767015024*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.00808788533308497  - (0.00016095444141931*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.00232696588842683  - (0.00018384470981829*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.00943950821324531  - (0.00017098987347593*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.00193709517200834  - (0.00018151995939591*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.00899713952612659  - (0.00017385835059948*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.00280584331659089  - (0.00017742164162470*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.00780381001954970  - (0.00018002500755708*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.00400370310490333  - (0.00017471691087957*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.00661527728186928  - (0.00018137323370347*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.00496545526864518  - (0.00017681872601767*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.00580728820997532  - (0.00018186220389790*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.00549309984725666  - (0.00017722985399075*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.00542194777529239  - (0.00018486900185338*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.00565992080998939  - (0.00018005824393118*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.00532121562846656  - (0.00018643189636216*fabs(bL[33]))));			
			//we apply the first samples of the impulse- dynamically adjusted.
			
			bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR * sqdrive;
			
			
			inputSampleR -= (bR[1] * (0.25867935358656502  - (0.00045755657070112*fabs(bR[1]))));
			inputSampleR += (bR[2] * (0.11509367290253694  - (0.00017494270657228*fabs(bR[2]))));
			inputSampleR -= (bR[3] * (0.06709853575891785  - (0.00058913102597723*fabs(bR[3]))));
			inputSampleR += (bR[4] * (0.01871006356851681  - (0.00003387358004645*fabs(bR[4]))));
			inputSampleR -= (bR[5] * (0.00794797957360465  - (0.00044224784691203*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.01956921817394220  - (0.00006718936750076*fabs(bR[6]))));
			inputSampleR += (bR[7] * (0.01682120257195205  + (0.00032857446292230*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.03401069039824205  - (0.00013634182872897*fabs(bR[8]))));
			inputSampleR += (bR[9] * (0.02369950268232634  + (0.00023112685751657*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.03477071178117132  - (0.00018029792231600*fabs(bR[10]))));
			inputSampleR += (bR[11] * (0.02024369717958201  + (0.00017337813374202*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.02819087729102172  - (0.00021438538665420*fabs(bR[12]))));
			inputSampleR += (bR[13] * (0.01147946743141303  + (0.00014424066034649*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.01894777011468867  - (0.00021549146262408*fabs(bR[14]))));
			inputSampleR += (bR[15] * (0.00301370330346873  + (0.00013527460148394*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.01067147835815486  - (0.00020960689910868*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.00402715397506384  - (0.00014421582712470*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.00502221703392005  - (0.00019805767015024*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.00808788533308497  - (0.00016095444141931*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.00232696588842683  - (0.00018384470981829*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.00943950821324531  - (0.00017098987347593*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.00193709517200834  - (0.00018151995939591*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.00899713952612659  - (0.00017385835059948*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.00280584331659089  - (0.00017742164162470*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.00780381001954970  - (0.00018002500755708*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.00400370310490333  - (0.00017471691087957*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.00661527728186928  - (0.00018137323370347*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.00496545526864518  - (0.00017681872601767*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.00580728820997532  - (0.00018186220389790*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.00549309984725666  - (0.00017722985399075*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.00542194777529239  - (0.00018486900185338*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.00565992080998939  - (0.00018005824393118*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.00532121562846656  - (0.00018643189636216*fabs(bR[33]))));
			//we apply the first samples of the impulse- dynamically adjusted.
		}
		
		abSample = fabs(inputSampleL);
		if (abSample > previousBL) threshSample = previousBL;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compCL = (compCL + (1.0 / threshSample)) / 2.0;
			compDL = (compDL + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSampleL *= compCL;
			threshSample *= compCL;
			if (threshSample > compthreshold)
			{compCL = (compCL + compCL + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compCL < 1.0) compCL += pow((1.0-compCL)*recoveryspd,7);}
		}
		else
		{
			inputSampleL *= compDL;
			threshSample *= compDL;
			if (threshSample > compthreshold)
			{compDL = (compDL + compDL + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compDL < 1.0) compDL += pow((1.0-compDL)*recoveryspd,7);}			
		}
		previousBL = abSample;
		//another dose of compression, please
		
		abSample = fabs(inputSampleR);
		if (abSample > previousBR) threshSample = previousBR;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compCR = (compCR + (1.0 / threshSample)) / 2.0;
			compDR = (compDR + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSampleR *= compCR;
			threshSample *= compCR;
			if (threshSample > compthreshold)
			{compCR = (compCR + compCR + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compCR < 1.0) compCR += pow((1.0-compCR)*recoveryspd,7);}
		}
		else
		{
			inputSampleR *= compDR;
			threshSample *= compDR;
			if (threshSample > compthreshold)
			{compDR = (compDR + compDR + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compDR < 1.0) compDR += pow((1.0-compDR)*recoveryspd,7);}			
		}
		previousBR = abSample;
		//another dose of compression, please
		
		flip = !flip;
		
		randy = ((double(fpdL)/UINT32_MAX)*0.054);
		outputSample = ((((inputSampleL*(1-randy))+(lastSampleL*randy))*wet)+(drySampleL*(1.0-wet))) * outlevel;		
		lastSampleL = inputSampleL;
		inputSampleL = outputSample;
		
		randy = ((double(fpdR)/UINT32_MAX)*0.054);
		outputSample = ((((inputSampleR*(1-randy))+(lastSampleR*randy))*wet)+(drySampleR*(1.0-wet))) * outlevel;
		lastSampleR = inputSampleR;
		inputSampleR = outputSample;
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
