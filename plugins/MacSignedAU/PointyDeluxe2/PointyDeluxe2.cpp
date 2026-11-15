/*
*	File:		PointyDeluxe2.cpp
*	
*	Version:	1.0
* 
*	Created:	11/3/26
*	
*	Copyright:  Copyright © 2026 Airwindows, Airwindows uses the MIT license
* 
*	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
*				consideration of your agreement to the following terms, and your use, installation, modification 
*				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
*				not agree with these terms, please do not use, install, modify or redistribute this Apple 
*				software.
*
*				In consideration of your agreement to abide by the following terms, and subject to these terms, 
*				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
*				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
*				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
*				redistribute the Apple Software in its entirety and without modifications, you must retain this 
*				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
*				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
*				endorse or promote products derived from the Apple Software without specific prior written 
*				permission from Apple.  Except as expressly stated in this notice, no other rights or 
*				licenses, express or implied, are granted by Apple herein, including but not limited to any 
*				patent rights that may be infringed by your derivative works or by other works in which the 
*				Apple Software may be incorporated.
*
*				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
*				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
*				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
*				OR IN COMBINATION WITH YOUR PRODUCTS.
*
*				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
*				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
*				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
*				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
*				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
/*=============================================================================
	PointyDeluxe2.cpp
	
=============================================================================*/
#include "PointyDeluxe2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, PointyDeluxe2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PointyDeluxe2::PointyDeluxe2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PointyDeluxe2::PointyDeluxe2(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
	SetParameter(kParam_F, kDefaultValue_ParamF );
	SetParameter(kParam_G, kDefaultValue_ParamG );
	SetParameter(kParam_H, kDefaultValue_ParamH );
	SetParameter(kParam_I, kDefaultValue_ParamI );
	SetParameter(kParam_J, kDefaultValue_ParamJ );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PointyDeluxe2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PointyDeluxe2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PointyDeluxe2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PointyDeluxe2::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_A:
                AUBase::FillInParameterName (outParameterInfo, kParameterAName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
                break;
            case kParam_D:
                AUBase::FillInParameterName (outParameterInfo, kParameterDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamD;
                break;
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
                break;
            case kParam_F:
                AUBase::FillInParameterName (outParameterInfo, kParameterFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamF;
                break;
            case kParam_G:
                AUBase::FillInParameterName (outParameterInfo, kParameterGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamG;
                break;
            case kParam_H:
                AUBase::FillInParameterName (outParameterInfo, kParameterHName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamH;
                break;
            case kParam_I:
                AUBase::FillInParameterName (outParameterInfo, kParameterIName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamI;
                break;
            case kParam_J:
                AUBase::FillInParameterName (outParameterInfo, kParameterJName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamJ;
                break;
           default:
                result = kAudioUnitErr_InvalidParameter;
                break;
            }
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
    


	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PointyDeluxe2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PointyDeluxe2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PointyDeluxe2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PointyDeluxe2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	PointyDeluxe2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult PointyDeluxe2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____PointyDeluxe2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PointyDeluxe2::PointyDeluxe2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		PointyDeluxe2::PointyDeluxe2Kernel::Reset()
{
	for(int x=0; x<16; x++) {
		for(int y=0; y<16; y++) {
			angS[x][y] = 0.0;angA[x][y] = 0.0;
		}
	}
	for(int y=0; y<16; y++) {
		angG[y] = 0.0;
		for (int x = 0; x < bip_total; x++) bip[x][y] = 0.0;
	}
	for (int x = 0; x < bip_total; x++) bop[x] = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PointyDeluxe2::PointyDeluxe2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		PointyDeluxe2::PointyDeluxe2Kernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	
	angG[0] = 1.0;
	angG[1] = GetParameter( kParam_A )*2.0;
	angG[10] = GetParameter( kParam_J )+0.5;
	double stepValue = (angG[10] - angG[1])*0.1;
	for(int y=1; y < 15; y++) angG[y] = fmax(angG[1]+(stepValue*y),0.0);
	double drive = angG[1];
	angG[2] *= (GetParameter( kParam_B )+0.5);
	if (drive < angG[2]) drive = angG[2];
	angG[3] *= (GetParameter( kParam_C )+0.5);
	if (drive < angG[3]) drive = angG[3];
	angG[4] *= (GetParameter( kParam_D )+0.5);
	if (drive < angG[4]) drive = angG[4];
	angG[5] *= (GetParameter( kParam_E )+0.5);
	if (drive < angG[5]) drive = angG[5];
	angG[6] *= (GetParameter( kParam_F )+0.5);
	if (drive < angG[6]) drive = angG[6];
	angG[7] *= (GetParameter( kParam_G )+0.5);
	if (drive < angG[7]) drive = angG[7];
	angG[8] *= (GetParameter( kParam_H )+0.5);
	if (drive < angG[8]) drive = angG[8];
	angG[9] *= (GetParameter( kParam_I )+0.5);
	if (drive < angG[9]) drive = angG[9];
	double tune = 0.618+(overallscale*0.0055);
	double pad = sqrt(1.0/drive);
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		for (int x = 0; x < 14; x++) {
			inputSample *= drive;
			double bip_delta = inputSample;
			inputSample = fmin(fmax(inputSample,-1.0),1.0);
			inputSample -= inputSample*(inputSample*0.654)*(inputSample*0.654);
			bip[bip_dvA][x] = bip_delta - inputSample; // these are derivatives: raw clip is position
			bip[bip_dvB][x] = bip[bip_pvA][x]-bip[bip_dvA][x]; bip[bip_pvA][x] = bip[bip_dvA][x];//velocity
			bip[bip_dvC][x] = bip[bip_pvB][x]-bip[bip_dvB][x]; bip[bip_pvB][x] = bip[bip_dvB][x];//acceleration
			bip[bip_dvD][x] = bip[bip_pvC][x]-bip[bip_dvC][x]; bip[bip_pvC][x] = bip[bip_dvC][x];//jerk
			double bip_dvE = bip[bip_pvD][x]-bip[bip_dvD][x]; bip[bip_pvD][x] = bip[bip_dvD][x];//snap
			inputSample *= (1.0+(fabs(bip[bip_dvC][x])*0.0618)+(fabs(bip[bip_dvD][x])*-0.05982)+(fabs(bip_dvE)*0.0206));

			double fr = (0.92/overallscale)+(overallscale*0.01);
			double band = inputSample; inputSample = 0.0;
			for (int y = 0; y < 14; y++) {
				angA[x][y] = (angA[x][y]*(1.0-fr)) + ((band-angS[x][y])*fr);
				double temp = band; band = ((angS[x][y]+(angA[x][y]*fr)) * (1.0-fr))+(band*fr);
				angS[x][y] = ((angS[x][y]+(angA[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSample += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSample += band*angG[14];
		}
		if (pad < 1.0) {
			inputSample *= pad;
		}
		
		double bop_delta = inputSample;		
		inputSample = fmin(fmax(inputSample,-M_PI_2),M_PI_2);
		long double X = inputSample; X *= X; //long double for even
		long double temp = inputSample * X; //the initial multiplies
		inputSample -= temp*0.16666666666666666666666666666666666; temp *= X;
		inputSample += temp*0.00833333333333333333333333333333333; temp *= X;
		inputSample -= temp*0.00019841269841269841269841269841269; temp *= X;
		inputSample += temp*0.00000275573192239858906525573192239; temp *= X;
		inputSample -= temp*0.00000002505210838544171877521083854; temp *= X;
		inputSample += temp*0.00000000016059043836821614599392377; temp *= X;
		inputSample -= temp*0.00000000000076471637318198164759011; temp *= X;
		inputSample += temp*0.00000000000000281145725434552076319; temp *= X;
		inputSample -= temp*0.00000000000000000822063524662432971; temp *= X;
		inputSample += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		bop[bip_dvA] = bop_delta - inputSample; // these are derivatives: raw clip is position
		bop[bip_dvB] = bop[bip_pvA]-bop[bip_dvA]; bop[bip_pvA] = bop[bip_dvA];//velocity
		bop[bip_dvC] = bop[bip_pvB]-bop[bip_dvB]; bop[bip_pvB] = bop[bip_dvB];//acceleration
		bop[bip_dvD] = bop[bip_pvC]-bop[bip_dvC]; bop[bip_pvC] = bop[bip_dvC];//jerk
		double bop_dvE = bop[bip_pvD]-bop[bip_dvD]; bop[bip_pvD] = bop[bip_dvD];//snap
		inputSample *= (1.0+(fabs(bop[bip_dvC])*0.0618)+(fabs(bop[bip_dvD])*-0.05982)+(fabs(bop_dvE)*0.0206));
		
		if (pad < 1.0) {
			inputSample *= pad;
		}
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

