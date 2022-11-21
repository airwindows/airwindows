/* ========================================
 *  Holt2 - Holt2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Holt2_H
#include "Holt2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Holt2(audioMaster);}

Holt2::Holt2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 1.0;
	B = 0.5;
	C = 1.0;
	D = 1.0;
	E = 1.0;
	
	previousSampleAL = 0.0;
	previousTrendAL = 0.0;
	previousSampleBL = 0.0;
	previousTrendBL = 0.0;
	previousSampleCL = 0.0;
	previousTrendCL = 0.0;
	previousSampleDL = 0.0;
	previousTrendDL = 0.0;
	previousSampleEL = 0.0;
	previousTrendEL = 0.0;
	previousSampleFL = 0.0;
	previousTrendFL = 0.0;
	previousSampleGL = 0.0;
	previousTrendGL = 0.0;
	previousSampleHL = 0.0;
	previousTrendHL = 0.0;
	
	previousSampleAR = 0.0;
	previousTrendAR = 0.0;
	previousSampleBR = 0.0;
	previousTrendBR = 0.0;
	previousSampleCR = 0.0;
	previousTrendCR = 0.0;
	previousSampleDR = 0.0;
	previousTrendDR = 0.0;
	previousSampleER = 0.0;
	previousTrendER = 0.0;
	previousSampleFR = 0.0;
	previousTrendFR = 0.0;
	previousSampleGR = 0.0;
	previousTrendGR = 0.0;
	previousSampleHR = 0.0;
	previousTrendHR = 0.0;
	
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

Holt2::~Holt2() {}
VstInt32 Holt2::getVendorVersion () {return 1000;}
void Holt2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Holt2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Holt2::getChunk (void** data, bool isPreset)
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

VstInt32 Holt2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
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

void Holt2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Holt2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Holt2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Freq", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Reso", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Poles", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Output", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Holt2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C*8.0, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Holt2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Holt2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Holt2::getEffectName(char* name) {
    vst_strncpy(name, "Holt2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Holt2::getPlugCategory() {return kPlugCategEffect;}

bool Holt2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Holt2", kVstMaxProductStrLen); return true;
}

bool Holt2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
