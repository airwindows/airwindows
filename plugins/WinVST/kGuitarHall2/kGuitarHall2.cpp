/* ========================================
 *  kGuitarHall2 - kGuitarHall2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kGuitarHall2_H
#include "kGuitarHall2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new kGuitarHall2(audioMaster);}

kGuitarHall2::kGuitarHall2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 1.0;
	C = 0.5;
	D = 0.5;
	E = 0.0;
	F = 0.5;
	
	for(int x = 0; x < d3A+2; x++) {a3AL[x] = 0.0; a3AR[x] = 0.0;}
	for(int x = 0; x < d3B+2; x++) {a3BL[x] = 0.0; a3BR[x] = 0.0;}
	for(int x = 0; x < d3C+2; x++) {a3CL[x] = 0.0; a3CR[x] = 0.0;}
	for(int x = 0; x < d3D+2; x++) {a3DL[x] = 0.0; a3DR[x] = 0.0;}
	for(int x = 0; x < d3E+2; x++) {a3EL[x] = 0.0; a3ER[x] = 0.0;}
	for(int x = 0; x < d3F+2; x++) {a3FL[x] = 0.0; a3FR[x] = 0.0;}
	for(int x = 0; x < d3G+2; x++) {a3GL[x] = 0.0; a3GR[x] = 0.0;}
	for(int x = 0; x < d3H+2; x++) {a3HL[x] = 0.0; a3HR[x] = 0.0;}
	for(int x = 0; x < d3I+2; x++) {a3IL[x] = 0.0; a3IR[x] = 0.0;}
	c3AL = c3BL = c3CL = c3DL = c3EL = c3FL = c3GL = c3HL = c3IL = 1;
	c3AR = c3BR = c3CR = c3DR = c3ER = c3FR = c3GR = c3HR = c3IR = 1;
	
	for(int x = 0; x < d6A+2; x++) {a6AL[x] = 0.0; a6AR[x] = 0.0;}
	for(int x = 0; x < d6B+2; x++) {a6BL[x] = 0.0; a6BR[x] = 0.0;}
	for(int x = 0; x < d6C+2; x++) {a6CL[x] = 0.0; a6CR[x] = 0.0;}
	for(int x = 0; x < d6D+2; x++) {a6DL[x] = 0.0; a6DR[x] = 0.0;}
	for(int x = 0; x < d6E+2; x++) {a6EL[x] = 0.0; a6ER[x] = 0.0;}
	for(int x = 0; x < d6F+2; x++) {a6FL[x] = 0.0; a6FR[x] = 0.0;}
	for(int x = 0; x < d6G+2; x++) {a6GL[x] = 0.0; a6GR[x] = 0.0;}
	for(int x = 0; x < d6H+2; x++) {a6HL[x] = 0.0; a6HR[x] = 0.0;}
	for(int x = 0; x < d6I+2; x++) {a6IL[x] = 0.0; a6IR[x] = 0.0;}
	for(int x = 0; x < d6J+2; x++) {a6JL[x] = 0.0; a6JR[x] = 0.0;}
	for(int x = 0; x < d6K+2; x++) {a6KL[x] = 0.0; a6KR[x] = 0.0;}
	for(int x = 0; x < d6L+2; x++) {a6LL[x] = 0.0; a6LR[x] = 0.0;}
	for(int x = 0; x < d6M+2; x++) {a6ML[x] = 0.0; a6MR[x] = 0.0;}
	for(int x = 0; x < d6N+2; x++) {a6NL[x] = 0.0; a6NR[x] = 0.0;}
	for(int x = 0; x < d6O+2; x++) {a6OL[x] = 0.0; a6OR[x] = 0.0;}
	for(int x = 0; x < d6P+2; x++) {a6PL[x] = 0.0; a6PR[x] = 0.0;}
	for(int x = 0; x < d6Q+2; x++) {a6QL[x] = 0.0; a6QR[x] = 0.0;}
	for(int x = 0; x < d6R+2; x++) {a6RL[x] = 0.0; a6RR[x] = 0.0;}
	for(int x = 0; x < d6S+2; x++) {a6SL[x] = 0.0; a6SR[x] = 0.0;}
	for(int x = 0; x < d6T+2; x++) {a6TL[x] = 0.0; a6TR[x] = 0.0;}
	for(int x = 0; x < d6U+2; x++) {a6UL[x] = 0.0; a6UR[x] = 0.0;}
	for(int x = 0; x < d6V+2; x++) {a6VL[x] = 0.0; a6VR[x] = 0.0;}
	for(int x = 0; x < d6W+2; x++) {a6WL[x] = 0.0; a6WR[x] = 0.0;}
	for(int x = 0; x < d6X+2; x++) {a6XL[x] = 0.0; a6XR[x] = 0.0;}
	for(int x = 0; x < d6Y+2; x++) {a6YL[x] = 0.0; a6YR[x] = 0.0;}
	for(int x = 0; x < d6ZA+2; x++) {a6ZAL[x] = 0.0; a6ZAR[x] = 0.0;}
	for(int x = 0; x < d6ZB+2; x++) {a6ZBL[x] = 0.0; a6ZBR[x] = 0.0;}
	for(int x = 0; x < d6ZC+2; x++) {a6ZCL[x] = 0.0; a6ZCR[x] = 0.0;}
	for(int x = 0; x < d6ZD+2; x++) {a6ZDL[x] = 0.0; a6ZDR[x] = 0.0;}
	for(int x = 0; x < d6ZE+2; x++) {a6ZEL[x] = 0.0; a6ZER[x] = 0.0;}
	for(int x = 0; x < d6ZF+2; x++) {a6ZFL[x] = 0.0; a6ZFR[x] = 0.0;}
	for(int x = 0; x < d6ZG+2; x++) {a6ZGL[x] = 0.0; a6ZGR[x] = 0.0;}
	for(int x = 0; x < d6ZH+2; x++) {a6ZHL[x] = 0.0; a6ZHR[x] = 0.0;}
	for(int x = 0; x < d6ZI+2; x++) {a6ZIL[x] = 0.0; a6ZIR[x] = 0.0;}
	for(int x = 0; x < d6ZJ+2; x++) {a6ZJL[x] = 0.0; a6ZJR[x] = 0.0;}
	for(int x = 0; x < d6ZK+2; x++) {a6ZKL[x] = 0.0; a6ZKR[x] = 0.0;}
	c6AL = c6BL = c6CL = c6DL = c6EL = c6FL = c6GL = c6HL = c6IL = 1;
	c6JL = c6KL = c6LL = c6ML = c6NL = c6OL = c6PL = c6QL = c6RL = 1;
	c6SL = c6TL = c6UL = c6VL = c6WL = c6XL = c6YL = c6ZAL = c6ZBL = 1;
	c6ZCL = c6ZDL = c6ZEL = c6ZFL = c6ZGL = c6ZHL = c6ZIL = c6ZJL = c6ZKL = 1;
	c6AR = c6BR = c6CR = c6DR = c6ER = c6FR = c6GR = c6HR = c6IR = 1;
	c6JR = c6KR = c6LR = c6MR = c6NR = c6OR = c6PR = c6QR = c6RR = 1;
	c6SR = c6TR = c6UR = c6VR = c6WR = c6XR = c6YR = c6ZAR = c6ZBR = 1;
	c6ZCR = c6ZDR = c6ZER = c6ZFR = c6ZGR = c6ZHR = c6ZIR = c6ZJR = c6ZKR = 1;
	f6AL = f6BL = f6CL = f6DL = f6EL = f6FL = 0.0;
	f6FR = f6LR = f6RR = f6XR = f6ZER = f6ZKR = 0.0;
	avg6L = avg6R = 0.0;	
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	for (int x = 0; x < pear_total; x++) pear[x] = 0.0;
	
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

kGuitarHall2::~kGuitarHall2() {}
VstInt32 kGuitarHall2::getVendorVersion () {return 1000;}
void kGuitarHall2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void kGuitarHall2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 kGuitarHall2::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	chunkData[5] = F;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 kGuitarHall2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	F = pinParameter(chunkData[5]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void kGuitarHall2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        case kParamF: F = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float kGuitarHall2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        case kParamF: return F; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void kGuitarHall2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Regen", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Derez", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Filter", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "EarlyRF", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Positin", kVstMaxParamStrLen); break;
		case kParamF: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void kGuitarHall2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        case kParamF: float2string (F, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void kGuitarHall2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 kGuitarHall2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool kGuitarHall2::getEffectName(char* name) {
    vst_strncpy(name, "kGuitarHall2", kVstMaxProductStrLen); return true;
}

VstPlugCategory kGuitarHall2::getPlugCategory() {return kPlugCategEffect;}

bool kGuitarHall2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows kGuitarHall2", kVstMaxProductStrLen); return true;
}

bool kGuitarHall2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
