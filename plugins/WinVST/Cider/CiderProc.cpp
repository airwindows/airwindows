/* ========================================
 *  Cider - Cider.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Cider_H
#include "Cider.h"
#endif

void Cider::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double threshold = A;
	double hardness;
	double breakup = (1.0-(threshold/2.0))*3.14159265358979;
	double bridgerectifier;
	double sqdrive = (B*3.0);
	if (sqdrive > 1.0) sqdrive *= sqdrive;
	sqdrive = sqrt(sqdrive);
	double indrive = C*3.0;
	if (indrive > 1.0) indrive *= indrive;
	indrive *= (1.0-(0.216*sqdrive));
	//correct for gain loss of convolution
	//calibrate this to match noise level with character at 1.0
	//you get for instance 0.819 and 1.0-0.819 is 0.181
	double randy;
	double outlevel = D;
	
	if (threshold < 1) hardness = 1.0 / (1.0-threshold);
	else hardness = 999999999999999999999.0;
	//set up hardness to exactly fill gap between threshold and 0db
	//if threshold is literally 1 then hardness is infinite, so we make it very big
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= indrive;
		inputSampleR *= indrive;
		//calibrated to match gain through convolution and -0.3 correction
		
		
		if (sqdrive > 0.0){
			bL[33] = bL[32]; bL[32] = bL[31]; 
			bL[31] = bL[30]; bL[30] = bL[29]; bL[29] = bL[28]; bL[28] = bL[27]; bL[27] = bL[26]; bL[26] = bL[25]; bL[25] = bL[24]; bL[24] = bL[23]; 
			bL[23] = bL[22]; bL[22] = bL[21]; bL[21] = bL[20]; bL[20] = bL[19]; bL[19] = bL[18]; bL[18] = bL[17]; bL[17] = bL[16]; bL[16] = bL[15]; 
			bL[15] = bL[14]; bL[14] = bL[13]; bL[13] = bL[12]; bL[12] = bL[11]; bL[11] = bL[10]; bL[10] = bL[9]; bL[9] = bL[8]; bL[8] = bL[7]; 
			bL[7] = bL[6]; bL[6] = bL[5]; bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1]; bL[1] = bL[0]; bL[0] = inputSampleL * sqdrive;
			
			inputSampleL += (bL[1] * (0.61283288942201319  + (0.00024011410669522*fabs(bL[1]))));
			inputSampleL -= (bL[2] * (0.24036380659761222  - (0.00020789518206241*fabs(bL[2]))));
			inputSampleL += (bL[3] * (0.09104669761717916  + (0.00012829642741548*fabs(bL[3]))));
			inputSampleL -= (bL[4] * (0.02378290768554025  - (0.00017673646470440*fabs(bL[4]))));
			inputSampleL -= (bL[5] * (0.02832818490275965  - (0.00013536187747384*fabs(bL[5]))));
			inputSampleL += (bL[6] * (0.03268797679215937  + (0.00015035126653359*fabs(bL[6]))));
			inputSampleL -= (bL[7] * (0.04024464202655586  - (0.00015034923056735*fabs(bL[7]))));
			inputSampleL += (bL[8] * (0.01864890074318696  + (0.00014513281680642*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.01632731954100322  - (0.00015509089075614*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.00318907090555589  - (0.00014784812076550*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.00208573465221869  - (0.00015350520779465*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.00907033901519614  - (0.00015442964157250*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.00199458794148013  - (0.00015595640046297*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.00705979153201755  - (0.00015730069418051*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.00429488975412722  - (0.00015743697943505*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.00497724878704936  - (0.00016014760011861*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.00506059305562353  - (0.00016194824072466*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.00483432223285621  - (0.00016329050124225*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.00495100420886005  - (0.00016297509798749*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.00489319520555115  - (0.00016472839684661*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.00489177657970308  - (0.00016791875866630*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.00487900894707044  - (0.00016755993898534*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.00486234009335561  - (0.00016968157345446*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.00485737490288736  - (0.00017180713324431*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.00484106070563455  - (0.00017251073661092*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.00483219429408410  - (0.00017321683790891*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.00482013597437550  - (0.00017392186866488*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.00480949628051497  - (0.00017569098775602*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.00479992055604049  - (0.00017746046369449*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.00478750757986987  - (0.00017745630047554*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.00477828651185740  - (0.00017958043287604*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.00476906544384494  - (0.00018170456527653*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.00475700712413634  - (0.00018099144598088*fabs(bL[33]))));			
			//we apply the first samples of the impulse- dynamically adjusted.
			
			bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR * sqdrive;
			
			inputSampleR += (bR[1] * (0.61283288942201319  + (0.00024011410669522*fabs(bR[1]))));
			inputSampleR -= (bR[2] * (0.24036380659761222  - (0.00020789518206241*fabs(bR[2]))));
			inputSampleR += (bR[3] * (0.09104669761717916  + (0.00012829642741548*fabs(bR[3]))));
			inputSampleR -= (bR[4] * (0.02378290768554025  - (0.00017673646470440*fabs(bR[4]))));
			inputSampleR -= (bR[5] * (0.02832818490275965  - (0.00013536187747384*fabs(bR[5]))));
			inputSampleR += (bR[6] * (0.03268797679215937  + (0.00015035126653359*fabs(bR[6]))));
			inputSampleR -= (bR[7] * (0.04024464202655586  - (0.00015034923056735*fabs(bR[7]))));
			inputSampleR += (bR[8] * (0.01864890074318696  + (0.00014513281680642*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.01632731954100322  - (0.00015509089075614*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.00318907090555589  - (0.00014784812076550*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.00208573465221869  - (0.00015350520779465*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.00907033901519614  - (0.00015442964157250*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.00199458794148013  - (0.00015595640046297*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.00705979153201755  - (0.00015730069418051*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.00429488975412722  - (0.00015743697943505*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.00497724878704936  - (0.00016014760011861*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.00506059305562353  - (0.00016194824072466*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.00483432223285621  - (0.00016329050124225*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.00495100420886005  - (0.00016297509798749*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.00489319520555115  - (0.00016472839684661*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.00489177657970308  - (0.00016791875866630*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.00487900894707044  - (0.00016755993898534*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.00486234009335561  - (0.00016968157345446*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.00485737490288736  - (0.00017180713324431*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.00484106070563455  - (0.00017251073661092*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.00483219429408410  - (0.00017321683790891*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.00482013597437550  - (0.00017392186866488*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.00480949628051497  - (0.00017569098775602*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.00479992055604049  - (0.00017746046369449*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.00478750757986987  - (0.00017745630047554*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.00477828651185740  - (0.00017958043287604*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.00476906544384494  - (0.00018170456527653*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.00475700712413634  - (0.00018099144598088*fabs(bR[33]))));			
			//we apply the first samples of the impulse- dynamically adjusted.
		}
		
		if (fabs(inputSampleL) > threshold)
		{
			bridgerectifier = (fabs(inputSampleL)-threshold)*hardness;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardness;
			//do the sine factor, scale back to proper amount
			if (inputSampleL > 0) inputSampleL = bridgerectifier+threshold;
			else inputSampleL = -(bridgerectifier+threshold);
		}
		//otherwise we leave it untouched by the overdrive stuff
		if (fabs(inputSampleR) > threshold)
		{
			bridgerectifier = (fabs(inputSampleR)-threshold)*hardness;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardness;
			//do the sine factor, scale back to proper amount
			if (inputSampleR > 0) inputSampleR = bridgerectifier+threshold;
			else inputSampleR = -(bridgerectifier+threshold);
		}
		//otherwise we leave it untouched by the overdrive stuff
		
		randy = ((double(fpdL)/UINT32_MAX)*0.057);
		inputSampleL = ((inputSampleL*(1-randy))+(lastSampleL*randy)) * outlevel;
		lastSampleL = inputSampleL;
		
		randy = ((double(fpdR)/UINT32_MAX)*0.057);
		inputSampleR = ((inputSampleR*(1-randy))+(lastSampleR*randy)) * outlevel;
		lastSampleR = inputSampleR;
		
		
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

void Cider::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double threshold = A;
	double hardness;
	double breakup = (1.0-(threshold/2.0))*3.14159265358979;
	double bridgerectifier;
	double sqdrive = (B*3.0);
	if (sqdrive > 1.0) sqdrive *= sqdrive;
	sqdrive = sqrt(sqdrive);
	double indrive = C*3.0;
	if (indrive > 1.0) indrive *= indrive;
	indrive *= (1.0-(0.216*sqdrive));
	//correct for gain loss of convolution
	//calibrate this to match noise level with character at 1.0
	//you get for instance 0.819 and 1.0-0.819 is 0.181
	double randy;
	double outlevel = D;
	
	if (threshold < 1) hardness = 1.0 / (1.0-threshold);
	else hardness = 999999999999999999999.0;
	//set up hardness to exactly fill gap between threshold and 0db
	//if threshold is literally 1 then hardness is infinite, so we make it very big
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= indrive;
		inputSampleR *= indrive;
		//calibrated to match gain through convolution and -0.3 correction
		
		
		if (sqdrive > 0.0){
			bL[33] = bL[32]; bL[32] = bL[31]; 
			bL[31] = bL[30]; bL[30] = bL[29]; bL[29] = bL[28]; bL[28] = bL[27]; bL[27] = bL[26]; bL[26] = bL[25]; bL[25] = bL[24]; bL[24] = bL[23]; 
			bL[23] = bL[22]; bL[22] = bL[21]; bL[21] = bL[20]; bL[20] = bL[19]; bL[19] = bL[18]; bL[18] = bL[17]; bL[17] = bL[16]; bL[16] = bL[15]; 
			bL[15] = bL[14]; bL[14] = bL[13]; bL[13] = bL[12]; bL[12] = bL[11]; bL[11] = bL[10]; bL[10] = bL[9]; bL[9] = bL[8]; bL[8] = bL[7]; 
			bL[7] = bL[6]; bL[6] = bL[5]; bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1]; bL[1] = bL[0]; bL[0] = inputSampleL * sqdrive;
			
			inputSampleL += (bL[1] * (0.61283288942201319  + (0.00024011410669522*fabs(bL[1]))));
			inputSampleL -= (bL[2] * (0.24036380659761222  - (0.00020789518206241*fabs(bL[2]))));
			inputSampleL += (bL[3] * (0.09104669761717916  + (0.00012829642741548*fabs(bL[3]))));
			inputSampleL -= (bL[4] * (0.02378290768554025  - (0.00017673646470440*fabs(bL[4]))));
			inputSampleL -= (bL[5] * (0.02832818490275965  - (0.00013536187747384*fabs(bL[5]))));
			inputSampleL += (bL[6] * (0.03268797679215937  + (0.00015035126653359*fabs(bL[6]))));
			inputSampleL -= (bL[7] * (0.04024464202655586  - (0.00015034923056735*fabs(bL[7]))));
			inputSampleL += (bL[8] * (0.01864890074318696  + (0.00014513281680642*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.01632731954100322  - (0.00015509089075614*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.00318907090555589  - (0.00014784812076550*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.00208573465221869  - (0.00015350520779465*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.00907033901519614  - (0.00015442964157250*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.00199458794148013  - (0.00015595640046297*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.00705979153201755  - (0.00015730069418051*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.00429488975412722  - (0.00015743697943505*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.00497724878704936  - (0.00016014760011861*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.00506059305562353  - (0.00016194824072466*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.00483432223285621  - (0.00016329050124225*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.00495100420886005  - (0.00016297509798749*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.00489319520555115  - (0.00016472839684661*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.00489177657970308  - (0.00016791875866630*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.00487900894707044  - (0.00016755993898534*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.00486234009335561  - (0.00016968157345446*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.00485737490288736  - (0.00017180713324431*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.00484106070563455  - (0.00017251073661092*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.00483219429408410  - (0.00017321683790891*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.00482013597437550  - (0.00017392186866488*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.00480949628051497  - (0.00017569098775602*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.00479992055604049  - (0.00017746046369449*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.00478750757986987  - (0.00017745630047554*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.00477828651185740  - (0.00017958043287604*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.00476906544384494  - (0.00018170456527653*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.00475700712413634  - (0.00018099144598088*fabs(bL[33]))));			
			//we apply the first samples of the impulse- dynamically adjusted.
			
			bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR * sqdrive;
			
			inputSampleR += (bR[1] * (0.61283288942201319  + (0.00024011410669522*fabs(bR[1]))));
			inputSampleR -= (bR[2] * (0.24036380659761222  - (0.00020789518206241*fabs(bR[2]))));
			inputSampleR += (bR[3] * (0.09104669761717916  + (0.00012829642741548*fabs(bR[3]))));
			inputSampleR -= (bR[4] * (0.02378290768554025  - (0.00017673646470440*fabs(bR[4]))));
			inputSampleR -= (bR[5] * (0.02832818490275965  - (0.00013536187747384*fabs(bR[5]))));
			inputSampleR += (bR[6] * (0.03268797679215937  + (0.00015035126653359*fabs(bR[6]))));
			inputSampleR -= (bR[7] * (0.04024464202655586  - (0.00015034923056735*fabs(bR[7]))));
			inputSampleR += (bR[8] * (0.01864890074318696  + (0.00014513281680642*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.01632731954100322  - (0.00015509089075614*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.00318907090555589  - (0.00014784812076550*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.00208573465221869  - (0.00015350520779465*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.00907033901519614  - (0.00015442964157250*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.00199458794148013  - (0.00015595640046297*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.00705979153201755  - (0.00015730069418051*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.00429488975412722  - (0.00015743697943505*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.00497724878704936  - (0.00016014760011861*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.00506059305562353  - (0.00016194824072466*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.00483432223285621  - (0.00016329050124225*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.00495100420886005  - (0.00016297509798749*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.00489319520555115  - (0.00016472839684661*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.00489177657970308  - (0.00016791875866630*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.00487900894707044  - (0.00016755993898534*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.00486234009335561  - (0.00016968157345446*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.00485737490288736  - (0.00017180713324431*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.00484106070563455  - (0.00017251073661092*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.00483219429408410  - (0.00017321683790891*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.00482013597437550  - (0.00017392186866488*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.00480949628051497  - (0.00017569098775602*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.00479992055604049  - (0.00017746046369449*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.00478750757986987  - (0.00017745630047554*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.00477828651185740  - (0.00017958043287604*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.00476906544384494  - (0.00018170456527653*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.00475700712413634  - (0.00018099144598088*fabs(bR[33]))));			
			//we apply the first samples of the impulse- dynamically adjusted.
		}
		
		if (fabs(inputSampleL) > threshold)
		{
			bridgerectifier = (fabs(inputSampleL)-threshold)*hardness;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardness;
			//do the sine factor, scale back to proper amount
			if (inputSampleL > 0) inputSampleL = bridgerectifier+threshold;
			else inputSampleL = -(bridgerectifier+threshold);
		}
		//otherwise we leave it untouched by the overdrive stuff
		if (fabs(inputSampleR) > threshold)
		{
			bridgerectifier = (fabs(inputSampleR)-threshold)*hardness;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardness;
			//do the sine factor, scale back to proper amount
			if (inputSampleR > 0) inputSampleR = bridgerectifier+threshold;
			else inputSampleR = -(bridgerectifier+threshold);
		}
		//otherwise we leave it untouched by the overdrive stuff
		
		randy = ((double(fpdL)/UINT32_MAX)*0.057);
		inputSampleL = ((inputSampleL*(1-randy))+(lastSampleL*randy)) * outlevel;
		lastSampleL = inputSampleL;
		
		randy = ((double(fpdR)/UINT32_MAX)*0.057);
		inputSampleR = ((inputSampleR*(1-randy))+(lastSampleR*randy)) * outlevel;
		lastSampleR = inputSampleR;
		
		
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
