/* ========================================
 *  DeCrackle - DeCrackle.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeCrackle_H
#include "DeCrackle.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new DeCrackle(audioMaster);}

DeCrackle::DeCrackle(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 0.5;
	C = 0.5;
	D = 0.5;
	E = 1.0;

	for(int x = 0; x < kshort+2; x++) {aAL[x] = 0.0; aAR[x] = 0.0;}
	for(int x = 0; x < kshort+2; x++) {aBL[x] = 0.0; aBR[x] = 0.0;}
	for(int x = 0; x < kshort+2; x++) {aCL[x] = 0.0;}
	count = 1;
	for(int x = 0; x < 17; x++) {
		iirLSample[x] = 0.0;
		iirRSample[x] = 0.0;
		iirCSample[x] = 0.0;
		iirLAngle[x] = 0.0;
		iirRAngle[x] = 0.0;
	}
	iirTargetL = 0.0;
	iirTargetR = 0.0;
	iirClickL = 0.0;
	iirClickR = 0.0;
	prevSampleL = 0.0;
	prevSampleR = 0.0;
	prevSurfaceL = 0.0;
	prevSurfaceR = 0.0;
	prevOutL = 0.0;
	prevOutR = 0.0;
	
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

DeCrackle::~DeCrackle() {}
VstInt32 DeCrackle::getVendorVersion () {return 1000;}
void DeCrackle::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void DeCrackle::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 DeCrackle::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	chunkData[3] = D;
	chunkData[4] = E;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 DeCrackle::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	D = pinParameter(chunkData[3]);
	E = pinParameter(chunkData[4]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void DeCrackle::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float DeCrackle::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void DeCrackle::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Filter", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Window", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Thresld", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Surface", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void DeCrackle::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void DeCrackle::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 DeCrackle::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool DeCrackle::getEffectName(char* name) {
    vst_strncpy(name, "DeCrackle", kVstMaxProductStrLen); return true;
}

VstPlugCategory DeCrackle::getPlugCategory() {return kPlugCategEffect;}

bool DeCrackle::getProductString(char* text) {
  	vst_strncpy (text, "airwindows DeCrackle", kVstMaxProductStrLen); return true;
}

bool DeCrackle::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
