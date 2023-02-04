/* ========================================
 *  Verbity2 - Verbity2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Verbity2_H
#include "Verbity2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Verbity2(audioMaster);}

Verbity2::Verbity2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 1.0;
	
	iirAL = 0.0;
	iirBL = 0.0;
	
	iirAR = 0.0;
	iirBR = 0.0;
	
	//for(int count = 0; count < 5190; count++) {aZL[count] = 0.0; aZR[count] = 0.0;}
	
	for(int count = 0; count < 5004; count++) {aAL[count] = 0.0; aAR[count] = 0.0;} //AFKPU
	for(int count = 0; count < 4952; count++) {aFL[count] = 0.0; aFR[count] = 0.0;}
	for(int count = 0; count < 4920; count++) {aKL[count] = 0.0; aKR[count] = 0.0;}
	for(int count = 0; count < 4800; count++) {aPL[count] = 0.0; aPR[count] = 0.0;}
	for(int count = 0; count < 4752; count++) {aUL[count] = 0.0; aUR[count] = 0.0;}
	
	for(int count = 0; count < 4350; count++) {aBL[count] = 0.0; aBR[count] = 0.0;} //BGLQV
	for(int count = 0; count < 4158; count++) {aGL[count] = 0.0; aGR[count] = 0.0;}
	for(int count = 0; count < 3930; count++) {aLL[count] = 0.0; aLR[count] = 0.0;}
	for(int count = 0; count < 3530; count++) {aQL[count] = 0.0; aQR[count] = 0.0;}
	for(int count = 0; count < 3330; count++) {aVL[count] = 0.0; aVR[count] = 0.0;}
	
	for(int count = 0; count < 3324; count++) {aCL[count] = 0.0; aCR[count] = 0.0;} //CHMRW
	for(int count = 0; count < 2792; count++) {aHL[count] = 0.0; aHR[count] = 0.0;}
	for(int count = 0; count < 2768; count++) {aML[count] = 0.0; aMR[count] = 0.0;}
	for(int count = 0; count < 2390; count++) {aRL[count] = 0.0; aRR[count] = 0.0;}
	for(int count = 0; count < 2348; count++) {aWL[count] = 0.0; aWR[count] = 0.0;}
	
	for(int count = 0; count < 2142; count++) {aDL[count] = 0.0; aDR[count] = 0.0;} //DINSX
	for(int count = 0; count < 1812; count++) {aIL[count] = 0.0; aIR[count] = 0.0;}
	for(int count = 0; count < 1734; count++) {aNL[count] = 0.0; aNR[count] = 0.0;}
	for(int count = 0; count < 1172; count++) {aSL[count] = 0.0; aSR[count] = 0.0;}
	for(int count = 0; count < 788; count++) {aXL[count] = 0.0; aXR[count] = 0.0;}
	
	for(int count = 0; count < 678; count++) {aEL[count] = 0.0; aER[count] = 0.0;} //EJOTY
	for(int count = 0; count < 644; count++) {aJL[count] = 0.0; aJR[count] = 0.0;}
	for(int count = 0; count < 440; count++) {aOL[count] = 0.0; aOR[count] = 0.0;}
	for(int count = 0; count < 350; count++) {aTL[count] = 0.0; aTR[count] = 0.0;}
	for(int count = 0; count < 282; count++) {aYL[count] = 0.0; aYR[count] = 0.0;}
	
	feedbackAL = 0.0;
	feedbackBL = 0.0;
	feedbackCL = 0.0;
	feedbackDL = 0.0;
	feedbackEL = 0.0;
	
	previousAL = 0.0;
	previousBL = 0.0;
	previousCL = 0.0;
	previousDL = 0.0;
	previousEL = 0.0;
	
	feedbackAR = 0.0;
	feedbackBR = 0.0;
	feedbackCR = 0.0;
	feedbackDR = 0.0;
	feedbackER = 0.0;
	
	previousAR = 0.0;
	previousBR = 0.0;
	previousCR = 0.0;
	previousDR = 0.0;
	previousER = 0.0;
	
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0; lastRefR[count] = 0.0;}
	
	countA = 1;
	countB = 1;
	countC = 1;
	countD = 1;	
	countE = 1;
	countF = 1;
	countG = 1;
	countH = 1;
	countI = 1;
	countJ = 1;
	countK = 1;
	countL = 1;
	countM = 1;
	countN = 1;
	countO = 1;
	countP = 1;
	countQ = 1;
	countR = 1;
	countS = 1;
	countT = 1;
	countU = 1;
	countV = 1;
	countW = 1;
	countX = 1;
	countY = 1;
	countZ = 1;
	
	cycle = 0;

	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	//this is reset: values being initialized only once. Startup values, whatever they are.
	
    _canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
    _canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
    _canDo.insert("x2in2out"); 
    setNumInputs(kNumInputs);
    setNumOutputs(kNumOutputs);
    setUniqueID(kUniqueId);
    canProcessReplacing();     // supports output replacing
    canDoubleReplacing();      // supports double precision processing
	programsAreChunks(true);
    vst_strncpy (_programName, "Default", kVstMaxProgNameLen); // default program name
}

Verbity2::~Verbity2() {}
VstInt32 Verbity2::getVendorVersion () {return 1000;}
void Verbity2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Verbity2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Verbity2::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Verbity2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Verbity2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Verbity2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Verbity2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "RmSize", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Sustain", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Mulch", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Wetness", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Verbity2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Verbity2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Verbity2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Verbity2::getEffectName(char* name) {
    vst_strncpy(name, "Verbity2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Verbity2::getPlugCategory() {return kPlugCategEffect;}

bool Verbity2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Verbity2", kVstMaxProductStrLen); return true;
}

bool Verbity2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
