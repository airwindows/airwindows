/*
*	File:		ChimeyGuitar.cpp
*	
*	Version:	1.0
* 
*	Created:	5/8/25
*	
*	Copyright:  Copyright © 2025 Airwindows, Airwindows uses the MIT license
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
	ChimeyGuitar.cpp
	
=============================================================================*/
#include "ChimeyGuitar.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, ChimeyGuitar)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChimeyGuitar::ChimeyGuitar
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ChimeyGuitar::ChimeyGuitar(AudioUnit component)
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
//	ChimeyGuitar::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ChimeyGuitar::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChimeyGuitar::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ChimeyGuitar::GetParameterInfo(AudioUnitScope		inScope,
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
//	ChimeyGuitar::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ChimeyGuitar::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChimeyGuitar::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ChimeyGuitar::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ChimeyGuitar::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ChimeyGuitar::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ChimeyGuitarEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChimeyGuitar::ChimeyGuitarKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ChimeyGuitar::ChimeyGuitarKernel::Reset()
{
	for(int x=0; x<21; x++) {
		for(int y=0; y<11; y++) {
			angS[x][y] = 0.0;angA[x][y] = 0.0;
		}
	}
	for(int y=0; y<11; y++) {
		angG[y] = 0.0;
		for (int x = 0; x < bez_total; x++) {bezComp[x][y] = 0.0;}
		bezComp[bez_cycle][y] = 1.0;
	}
	bezRezA = bezRezB = 0.0002;
	
	for(int count = 0; count < 36; count++) {
		iirHPosition[count] = 0.0;
		iirHAngle[count] = 0.0;
		iirBPosition[count] = 0.0;
		iirBAngle[count] = 0.0;
	}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChimeyGuitar::ChimeyGuitarKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ChimeyGuitar::ChimeyGuitarKernel::Process(	const Float32 	*inSourceP,
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
	
	int poles = (int)(pow(GetParameter( kParam_A ),2)*20.0);
	angG[0] = sqrt(GetParameter( kParam_B )*2.0);
	angG[2] = sqrt(GetParameter( kParam_C )*2.0);
	angG[4] = sqrt(GetParameter( kParam_D )*2.0);
	angG[6] = sqrt(GetParameter( kParam_E )*2.0);
	angG[8] = sqrt(GetParameter( kParam_F )*2.0);
	angG[1] = (angG[0]+angG[2])*0.5;
	angG[3] = (angG[2]+angG[4])*0.5;
	angG[5] = (angG[4]+angG[6])*0.5;
	angG[7] = (angG[6]+angG[8])*0.5;
	angG[9] = angG[8];
	double hFreq = pow(GetParameter( kParam_G ),overallscale);
	double lFreq = pow(GetParameter( kParam_H ),overallscale+3.0);
	bezRezA = bezRezB; bezRezB = ((pow(GetParameter( kParam_I ),4.0)*0.5)+0.0002)/overallscale;
	if (bezRezB > 1.0) bezRezB = 1.0;
	double output = GetParameter( kParam_J );
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		double temp = (double)nSampleFrames/inFramesToProcess;
		double bezRez = (bezRezA*temp)+(bezRezB*(1.0-temp));
		
		double CBAL = 0.0;
		for (int x = 0; x < poles; x++) {
			double fr = 0.9/overallscale;
			double band = inputSample; inputSample = 0.0;
			for (int y = 0; y < 9; y++) {
				angA[x][y] = (angA[x][y]*(1.0-fr)) + ((band-angS[x][y])*fr);
				double temp = band; band = ((angS[x][y]+(angA[x][y]*fr)) * (1.0-fr))+(band*fr);
				angS[x][y] = ((angS[x][y]+(angA[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSample += ((temp-band)*angG[y]);
				fr *= 0.618033988749894;
				bezComp[bez_cycle][y] += bezRez;
				bezComp[bez_SampL][y] += (fabs(inputSample) * bezRez);
				if (bezComp[bez_cycle][y] > 1.0) {
					bezComp[bez_cycle][y] -= 1.0;
					bezComp[bez_CL][y] = bezComp[bez_BL][y];
					bezComp[bez_BL][y] = bezComp[bez_AL][y];
					bezComp[bez_AL][y] = bezComp[bez_SampL][y];
					bezComp[bez_SampL][y] = 0.0;
				}
				double CBL = (bezComp[bez_CL][y]*(1.0-bezComp[bez_cycle][y]))+(bezComp[bez_BL][y]*bezComp[bez_cycle][y]);
				double BAL = (bezComp[bez_BL][y]*(1.0-bezComp[bez_cycle][y]))+(bezComp[bez_AL][y]*bezComp[bez_cycle][y]);
				CBAL += (bezComp[bez_BL][y]+(CBL*(1.0-bezComp[bez_cycle][y]))+(BAL*bezComp[bez_cycle][y]))*0.5;
				inputSample *= 1.0-(fmin(CBAL*0.01,1.0));
			}
			inputSample += (band*angG[9]);
		}
		inputSample = sin(inputSample);
				
		double lowSample = inputSample;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirBAngle[count] = (iirBAngle[count]*(1.0-lFreq))+((lowSample-iirBPosition[count])*lFreq);
			lowSample = ((iirBPosition[count]+(iirBAngle[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirBPosition[count] = ((iirBPosition[count]+(iirBAngle[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			inputSample -= (lowSample * (1.0/(3.0+(lFreq*32.0))) );
		}
		
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngle[count] = (iirHAngle[count]*(1.0-hFreq))+((inputSample-iirHPosition[count])*hFreq);
			inputSample = ((iirHPosition[count]+(iirHAngle[count]*hFreq))*(1.0-hFreq))+(inputSample*hFreq);
			iirHPosition[count] = ((iirHPosition[count]+(iirHAngle[count]*hFreq))*(1.0-hFreq))+(inputSample*hFreq);
		} //the lowpass	
		inputSample *= output;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

