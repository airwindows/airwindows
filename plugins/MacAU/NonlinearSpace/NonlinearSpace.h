/*
*	File:		NonlinearSpace.h
*	
*	Version:	1.0
* 
*	Created:	12/21/14
*	
*	Copyright:  Copyright © 2014 Airwindows, Airwindows uses the MIT license
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
#include "AUEffectBase.h"
#include "NonlinearSpaceVersion.h"

#if AU_DEBUG_DISPATCHER
#include "AUDebugDispatcher.h"
#endif


#ifndef __NonlinearSpace_h__
#define __NonlinearSpace_h__


#pragma mark ____NonlinearSpace Parameters

// parameters
static const float kDefaultValue_ParamTwo = 0.5;
static const float kDefaultValue_ParamThree = 0.5;
static const float kDefaultValue_ParamFour = 0.5;
static const float kDefaultValue_ParamFive = 0.0;
static const float kDefaultValue_ParamSix = 1.0;

static CFStringRef kParameterOneName = CFSTR("Sample Rate");
static const int k16 = 1;
static const int k32 = 2;
static const int k44 = 3;
static const int k48 = 4;
static const int k64 = 5;
static const int k88 = 6;
static const int k96 = 7;
static const int kDefaultValue_ParamOne = k44;

static CFStringRef kParameterTwoName = CFSTR("Liveness");
static CFStringRef kParameterThreeName = CFSTR("Treble");
static CFStringRef kParameterFourName = CFSTR("Bass");
static CFStringRef kParameterFiveName = CFSTR("Nonlin");
static CFStringRef kParameterSixName = CFSTR("Dry/Wet");
//Alter the name if desired, but using the plugin name is a start

static CFStringRef kMenuItem_16 = CFSTR ("16K");
static CFStringRef kMenuItem_32 = CFSTR ("32K");
static CFStringRef kMenuItem_44 = CFSTR ("44.1K");
static CFStringRef kMenuItem_48 = CFSTR ("48K");
static CFStringRef kMenuItem_64 = CFSTR ("64K");
static CFStringRef kMenuItem_88 = CFSTR ("88.2K");
static CFStringRef kMenuItem_96 = CFSTR ("96K");


enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	kParam_Six =5,
	//Add your parameters here...
	kNumberOfParameters=6
};

#pragma mark ____NonlinearSpace
class NonlinearSpace : public AUEffectBase
{
public:
	NonlinearSpace(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~NonlinearSpace () { delete mDebugDispatcher; }
#endif
	
	virtual ComponentResult Reset(AudioUnitScope inScope, AudioUnitElement inElement);
	
	virtual OSStatus ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags, 
										const AudioBufferList & inBuffer, AudioBufferList & outBuffer, 
										UInt32 inFramesToProcess);
	
	virtual	ComponentResult		GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	ComponentResult		GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
	virtual UInt32 SupportedNumChannels(const AUChannelInfo ** outInfo);
	
	virtual ComponentResult    Initialize();
	virtual bool				SupportsTail () { return true; }
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*220000;} //roughly five seconds tail
    virtual Float64				GetLatency() {return 0.0;}	// edit these because tail time isn't 1000 samples and latency isn't 1
	
	/*! @method Version */
	virtual ComponentResult		Version() { return kNonlinearSpaceVersion; }
	
private: 
	Float64 avgInputL;
	Float64 avgOutputL;
	Float64 avg2InputL;
	Float64 avg2OutputL;
	Float64 avgInputR;
	Float64 avgOutputR;
	Float64 avg2InputR;
	Float64 avg2OutputR;
	Float64 a2vgInputL;
	Float64 a2vgOutputL;
	Float64 a2vg2InputL;
	Float64 a2vg2OutputL;
	Float64 a2vgInputR;
	Float64 a2vgOutputR;
	Float64 a2vg2InputR;
	Float64 a2vg2OutputR;
	
	Float64 verboutL;
	Float64 verboutR;
	Float64 iirCCSampleL;
	Float64 iirCCSampleR;
	Float64 iirSampleL;
	Float64 iirSampleR;
	
	Float64 dMid[2348];
	Float64 dSide[1334];
	Float64 dLeft[5924];
	Float64 dRight[5926];
	
	Float64 dpreR[7575];
	Float64 dpreL[7575];
	
	Float64 dA[7575];
	Float64 dB[7309];
	Float64 dC[7179];
	Float64 dD[6909];		
	Float64 dE[6781];
	Float64 dF[6523];
	Float64 dG[5983];
	Float64 dH[5565];
	Float64 dI[5299];
	Float64 dJ[4905];
	Float64 dK[4761];
	Float64 dL[4491];
	Float64 dM[4393];
	Float64 dN[4231];
	Float64 dO[4155];
	Float64 dP[3991];
	Float64 dQ[3661];
	Float64 dR[3409];
	Float64 dS[3253];
	Float64 dT[3001];
	Float64 dU[2919];
	Float64 dV[2751];
	Float64 dW[2505];
	Float64 dX[2425];
	Float64 dY[2148];
	Float64 dZ[2090];
	
	Float64 interpolA, pitchshiftA; //7575
	Float64 interpolB, pitchshiftB; //7309
	Float64 interpolC, pitchshiftC; //7179
	Float64 interpolD, pitchshiftD; //6909		
	Float64 interpolE, pitchshiftE; //6781
	Float64 interpolF, pitchshiftF; //6523
	Float64 interpolG, pitchshiftG; //5983
	Float64 interpolH, pitchshiftH; //5565
	Float64 interpolI, pitchshiftI; //5299
	Float64 interpolJ, pitchshiftJ; //4905
	Float64 interpolK, pitchshiftK; //4761
	Float64 interpolL, pitchshiftL; //4491
	Float64 interpolM, pitchshiftM; //4393
	Float64 interpolN, pitchshiftN; //4231
	Float64 interpolO, pitchshiftO; //4155
	Float64 interpolP, pitchshiftP; //3991
	Float64 interpolQ, pitchshiftQ; //3661
	Float64 interpolR, pitchshiftR; //3409
	Float64 interpolS, pitchshiftS; //3253
	Float64 interpolT, pitchshiftT; //3001
	Float64 interpolU, pitchshiftU; //2919
	Float64 interpolV, pitchshiftV; //2751
	Float64 interpolW, pitchshiftW; //2505
	Float64 interpolX, pitchshiftX; //2425
	Float64 interpolY, pitchshiftY; //2148
	Float64 interpolZ, pitchshiftZ; //2090
	
	int oneMid, delayMid, maxdelayMid;
	int oneSide, delaySide, maxdelaySide;
	int oneLeft, delayLeft, maxdelayLeft;
	int oneRight, delayRight, maxdelayRight;
	
	int onepre, delaypre, maxdelaypre;
	
	int oneA, twoA, treA, delayA, maxdelayA;
	int oneB, twoB, treB, delayB, maxdelayB;
	int oneC, twoC, treC, delayC, maxdelayC;
	int oneD, twoD, treD, delayD, maxdelayD;
	int oneE, twoE, treE, delayE, maxdelayE;
	int oneF, twoF, treF, delayF, maxdelayF;
	int oneG, twoG, treG, delayG, maxdelayG;
	int oneH, twoH, treH, delayH, maxdelayH;
	int oneI, twoI, treI, delayI, maxdelayI;
	int oneJ, twoJ, treJ, delayJ, maxdelayJ;
	int oneK, twoK, treK, delayK, maxdelayK;
	int oneL, twoL, treL, delayL, maxdelayL;
	int oneM, twoM, treM, delayM, maxdelayM;
	int oneN, twoN, treN, delayN, maxdelayN;
	int oneO, twoO, treO, delayO, maxdelayO;
	int oneP, twoP, treP, delayP, maxdelayP;
	int oneQ, twoQ, treQ, delayQ, maxdelayQ;
	int oneR, twoR, treR, delayR, maxdelayR;
	int oneS, twoS, treS, delayS, maxdelayS;
	int oneT, twoT, treT, delayT, maxdelayT;
	int oneU, twoU, treU, delayU, maxdelayU;
	int oneV, twoV, treV, delayV, maxdelayV;
	int oneW, twoW, treW, delayW, maxdelayW;
	int oneX, twoX, treX, delayX, maxdelayX;
	int oneY, twoY, treY, delayY, maxdelayY;
	int oneZ, twoZ, treZ, delayZ, maxdelayZ;
	Float64 savedPredelay;
	Float64 savedRoomsize;
	int countdown;
	
	Float64 lowpassSampleAA;
	Float64 lowpassSampleAB;
	Float64 lowpassSampleBA;
	Float64 lowpassSampleBB;
	Float64 lowpassSampleCA;
	Float64 lowpassSampleCB;
	Float64 lowpassSampleDA;
	Float64 lowpassSampleDB;
	Float64 lowpassSampleE;
	Float64 lowpassSampleF;
	Float64 lowpassSampleG;
	
	Float64 rowpassSampleAA;
	Float64 rowpassSampleAB;
	Float64 rowpassSampleBA;
	Float64 rowpassSampleBB;
	Float64 rowpassSampleCA;
	Float64 rowpassSampleCB;
	Float64 rowpassSampleDA;
	Float64 rowpassSampleDB;
	Float64 rowpassSampleE;
	Float64 rowpassSampleF;
	Float64 rowpassSampleG;
	
	bool flip;
	
	Float64 nonlin;
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif