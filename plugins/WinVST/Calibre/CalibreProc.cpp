/* ========================================
 *  Calibre - Calibre.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Calibre_H
#include "Calibre.h"
#endif

void Calibre::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
	indrive *= (1.0+(0.1935*sqdrive));
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
			
			
			inputSampleL -= (bL[1] * (0.23505923670562212  - (0.00028312859289245*fabs(bL[1]))));
			inputSampleL += (bL[2] * (0.08188436704577637  - (0.00008817721351341*fabs(bL[2]))));
			inputSampleL -= (bL[3] * (0.05075798481700617  - (0.00018817166632483*fabs(bL[3]))));
			inputSampleL -= (bL[4] * (0.00455811821873093  + (0.00001922902995296*fabs(bL[4]))));
			inputSampleL -= (bL[5] * (0.00027610521433660  - (0.00013252525469291*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.03529246280346626  - (0.00002772989223299*fabs(bL[6]))));
			inputSampleL += (bL[7] * (0.01784111585586136  + (0.00010230276997291*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.04394950700298298  - (0.00005910607126944*fabs(bL[8]))));
			inputSampleL += (bL[9] * (0.01990770780547606  + (0.00007640328340556*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.04073629569741782  - (0.00007712327117090*fabs(bL[10]))));
			inputSampleL += (bL[11] * (0.01349648572795252  + (0.00005959130575917*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.03191590248003717  - (0.00008418000575151*fabs(bL[12]))));
			inputSampleL += (bL[13] * (0.00348795527924766  + (0.00005489156318238*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.02198496281481767  - (0.00008471601187581*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.00504771152505089  - (0.00005525060587917*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.01391075698598491  - (0.00007929630732607*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.01142762504081717  - (0.00005967036737742*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.00893541815021255  - (0.00007535697758141*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.01459704973464936  - (0.00005969199602841*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.00694755135226282  - (0.00006930127097865*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.01516695630808575  - (0.00006365800069826*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.00705917318113651  - (0.00006497209096539*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.01420501209177591  - (0.00006555654576113*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.00815905656808701  - (0.00006105622534761*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.01274326525552961  - (0.00006542652857017*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.00937146927845488  - (0.00006051267868722*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.01146573981165209  - (0.00006381511607749*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.01021294359409007  - (0.00005930397856398*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.01065217095323532  - (0.00006371505438319*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.01058751196699751  - (0.00006042857480233*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.01026557827762401  - (0.00006007776163871*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.01060929183604604  - (0.00006114703012726*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.01014533525058528  - (0.00005963567932887*fabs(bL[33]))));
			//we apply the first samples of the Neve impulse- dynamically adjusted.
			
			bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR * sqdrive;
			
		inputSampleR -= (bR[1] * (0.23505923670562212  - (0.00028312859289245*fabs(bR[1]))));
		inputSampleR += (bR[2] * (0.08188436704577637  - (0.00008817721351341*fabs(bR[2]))));
		inputSampleR -= (bR[3] * (0.05075798481700617  - (0.00018817166632483*fabs(bR[3]))));
		inputSampleR -= (bR[4] * (0.00455811821873093  + (0.00001922902995296*fabs(bR[4]))));
		inputSampleR -= (bR[5] * (0.00027610521433660  - (0.00013252525469291*fabs(bR[5]))));
		inputSampleR -= (bR[6] * (0.03529246280346626  - (0.00002772989223299*fabs(bR[6]))));
		inputSampleR += (bR[7] * (0.01784111585586136  + (0.00010230276997291*fabs(bR[7]))));
		inputSampleR -= (bR[8] * (0.04394950700298298  - (0.00005910607126944*fabs(bR[8]))));
		inputSampleR += (bR[9] * (0.01990770780547606  + (0.00007640328340556*fabs(bR[9]))));
		inputSampleR -= (bR[10] * (0.04073629569741782  - (0.00007712327117090*fabs(bR[10]))));
		inputSampleR += (bR[11] * (0.01349648572795252  + (0.00005959130575917*fabs(bR[11]))));
		inputSampleR -= (bR[12] * (0.03191590248003717  - (0.00008418000575151*fabs(bR[12]))));
		inputSampleR += (bR[13] * (0.00348795527924766  + (0.00005489156318238*fabs(bR[13]))));
		inputSampleR -= (bR[14] * (0.02198496281481767  - (0.00008471601187581*fabs(bR[14]))));
		inputSampleR -= (bR[15] * (0.00504771152505089  - (0.00005525060587917*fabs(bR[15]))));
		inputSampleR -= (bR[16] * (0.01391075698598491  - (0.00007929630732607*fabs(bR[16]))));
		inputSampleR -= (bR[17] * (0.01142762504081717  - (0.00005967036737742*fabs(bR[17]))));
		inputSampleR -= (bR[18] * (0.00893541815021255  - (0.00007535697758141*fabs(bR[18]))));
		inputSampleR -= (bR[19] * (0.01459704973464936  - (0.00005969199602841*fabs(bR[19]))));
		inputSampleR -= (bR[20] * (0.00694755135226282  - (0.00006930127097865*fabs(bR[20]))));
		inputSampleR -= (bR[21] * (0.01516695630808575  - (0.00006365800069826*fabs(bR[21]))));
		inputSampleR -= (bR[22] * (0.00705917318113651  - (0.00006497209096539*fabs(bR[22]))));
		inputSampleR -= (bR[23] * (0.01420501209177591  - (0.00006555654576113*fabs(bR[23]))));
		inputSampleR -= (bR[24] * (0.00815905656808701  - (0.00006105622534761*fabs(bR[24]))));
		inputSampleR -= (bR[25] * (0.01274326525552961  - (0.00006542652857017*fabs(bR[25]))));
		inputSampleR -= (bR[26] * (0.00937146927845488  - (0.00006051267868722*fabs(bR[26]))));
		inputSampleR -= (bR[27] * (0.01146573981165209  - (0.00006381511607749*fabs(bR[27]))));
		inputSampleR -= (bR[28] * (0.01021294359409007  - (0.00005930397856398*fabs(bR[28]))));
		inputSampleR -= (bR[29] * (0.01065217095323532  - (0.00006371505438319*fabs(bR[29]))));
		inputSampleR -= (bR[30] * (0.01058751196699751  - (0.00006042857480233*fabs(bR[30]))));
		inputSampleR -= (bR[31] * (0.01026557827762401  - (0.00006007776163871*fabs(bR[31]))));
		inputSampleR -= (bR[32] * (0.01060929183604604  - (0.00006114703012726*fabs(bR[32]))));
		inputSampleR -= (bR[33] * (0.01014533525058528  - (0.00005963567932887*fabs(bR[33]))));
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
		
		
		randy = ((double(fpdL)/UINT32_MAX)*0.042);
		inputSampleL = ((inputSampleL*(1-randy))+(lastSampleL*randy)) * outlevel;
		lastSampleL = inputSampleL;
		
		randy = ((double(fpdR)/UINT32_MAX)*0.042);
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

void Calibre::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
	indrive *= (1.0+(0.1935*sqdrive));
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
			
			
			inputSampleL -= (bL[1] * (0.23505923670562212  - (0.00028312859289245*fabs(bL[1]))));
			inputSampleL += (bL[2] * (0.08188436704577637  - (0.00008817721351341*fabs(bL[2]))));
			inputSampleL -= (bL[3] * (0.05075798481700617  - (0.00018817166632483*fabs(bL[3]))));
			inputSampleL -= (bL[4] * (0.00455811821873093  + (0.00001922902995296*fabs(bL[4]))));
			inputSampleL -= (bL[5] * (0.00027610521433660  - (0.00013252525469291*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.03529246280346626  - (0.00002772989223299*fabs(bL[6]))));
			inputSampleL += (bL[7] * (0.01784111585586136  + (0.00010230276997291*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.04394950700298298  - (0.00005910607126944*fabs(bL[8]))));
			inputSampleL += (bL[9] * (0.01990770780547606  + (0.00007640328340556*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.04073629569741782  - (0.00007712327117090*fabs(bL[10]))));
			inputSampleL += (bL[11] * (0.01349648572795252  + (0.00005959130575917*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.03191590248003717  - (0.00008418000575151*fabs(bL[12]))));
			inputSampleL += (bL[13] * (0.00348795527924766  + (0.00005489156318238*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.02198496281481767  - (0.00008471601187581*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.00504771152505089  - (0.00005525060587917*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.01391075698598491  - (0.00007929630732607*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.01142762504081717  - (0.00005967036737742*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.00893541815021255  - (0.00007535697758141*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.01459704973464936  - (0.00005969199602841*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.00694755135226282  - (0.00006930127097865*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.01516695630808575  - (0.00006365800069826*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.00705917318113651  - (0.00006497209096539*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.01420501209177591  - (0.00006555654576113*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.00815905656808701  - (0.00006105622534761*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.01274326525552961  - (0.00006542652857017*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.00937146927845488  - (0.00006051267868722*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.01146573981165209  - (0.00006381511607749*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.01021294359409007  - (0.00005930397856398*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.01065217095323532  - (0.00006371505438319*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.01058751196699751  - (0.00006042857480233*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.01026557827762401  - (0.00006007776163871*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.01060929183604604  - (0.00006114703012726*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.01014533525058528  - (0.00005963567932887*fabs(bL[33]))));
			//we apply the first samples of the Neve impulse- dynamically adjusted.
			
			bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR * sqdrive;
			
			inputSampleR -= (bR[1] * (0.23505923670562212  - (0.00028312859289245*fabs(bR[1]))));
			inputSampleR += (bR[2] * (0.08188436704577637  - (0.00008817721351341*fabs(bR[2]))));
			inputSampleR -= (bR[3] * (0.05075798481700617  - (0.00018817166632483*fabs(bR[3]))));
			inputSampleR -= (bR[4] * (0.00455811821873093  + (0.00001922902995296*fabs(bR[4]))));
			inputSampleR -= (bR[5] * (0.00027610521433660  - (0.00013252525469291*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.03529246280346626  - (0.00002772989223299*fabs(bR[6]))));
			inputSampleR += (bR[7] * (0.01784111585586136  + (0.00010230276997291*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.04394950700298298  - (0.00005910607126944*fabs(bR[8]))));
			inputSampleR += (bR[9] * (0.01990770780547606  + (0.00007640328340556*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.04073629569741782  - (0.00007712327117090*fabs(bR[10]))));
			inputSampleR += (bR[11] * (0.01349648572795252  + (0.00005959130575917*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.03191590248003717  - (0.00008418000575151*fabs(bR[12]))));
			inputSampleR += (bR[13] * (0.00348795527924766  + (0.00005489156318238*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.02198496281481767  - (0.00008471601187581*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.00504771152505089  - (0.00005525060587917*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.01391075698598491  - (0.00007929630732607*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.01142762504081717  - (0.00005967036737742*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.00893541815021255  - (0.00007535697758141*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.01459704973464936  - (0.00005969199602841*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.00694755135226282  - (0.00006930127097865*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.01516695630808575  - (0.00006365800069826*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.00705917318113651  - (0.00006497209096539*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.01420501209177591  - (0.00006555654576113*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.00815905656808701  - (0.00006105622534761*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.01274326525552961  - (0.00006542652857017*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.00937146927845488  - (0.00006051267868722*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.01146573981165209  - (0.00006381511607749*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.01021294359409007  - (0.00005930397856398*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.01065217095323532  - (0.00006371505438319*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.01058751196699751  - (0.00006042857480233*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.01026557827762401  - (0.00006007776163871*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.01060929183604604  - (0.00006114703012726*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.01014533525058528  - (0.00005963567932887*fabs(bR[33]))));
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
		
		
		randy = ((double(fpdL)/UINT32_MAX)*0.042);
		inputSampleL = ((inputSampleL*(1-randy))+(lastSampleL*randy)) * outlevel;
		lastSampleL = inputSampleL;
		
		randy = ((double(fpdR)/UINT32_MAX)*0.042);
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
