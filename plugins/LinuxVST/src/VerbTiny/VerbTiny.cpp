/* ========================================
 *  VerbTiny - VerbTiny.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VerbTiny_H
#include "VerbTiny.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new VerbTiny(audioMaster);}

VerbTiny::VerbTiny(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	B = 1.0;
	C = 1.0;
	D = 0.0;
	E = 1.0;

	for(int x = 0; x < d4A+2; x++) {a4AL[x] = 0.0; a4AR[x] = 0.0;}
	for(int x = 0; x < d4B+2; x++) {a4BL[x] = 0.0; a4BR[x] = 0.0;}
	for(int x = 0; x < d4C+2; x++) {a4CL[x] = 0.0; a4CR[x] = 0.0;}
	for(int x = 0; x < d4D+2; x++) {a4DL[x] = 0.0; a4DR[x] = 0.0;}
	for(int x = 0; x < d4E+2; x++) {a4EL[x] = 0.0; a4ER[x] = 0.0;}
	for(int x = 0; x < d4F+2; x++) {a4FL[x] = 0.0; a4FR[x] = 0.0;}
	for(int x = 0; x < d4G+2; x++) {a4GL[x] = 0.0; a4GR[x] = 0.0;}
	for(int x = 0; x < d4H+2; x++) {a4HL[x] = 0.0; a4HR[x] = 0.0;}
	for(int x = 0; x < d4I+2; x++) {a4IL[x] = 0.0; a4IR[x] = 0.0;}
	for(int x = 0; x < d4J+2; x++) {a4JL[x] = 0.0; a4JR[x] = 0.0;}
	for(int x = 0; x < d4K+2; x++) {a4KL[x] = 0.0; a4KR[x] = 0.0;}
	for(int x = 0; x < d4L+2; x++) {a4LL[x] = 0.0; a4LR[x] = 0.0;}
	for(int x = 0; x < d4M+2; x++) {a4ML[x] = 0.0; a4MR[x] = 0.0;}
	for(int x = 0; x < d4N+2; x++) {a4NL[x] = 0.0; a4NR[x] = 0.0;}
	for(int x = 0; x < d4O+2; x++) {a4OL[x] = 0.0; a4OR[x] = 0.0;}
	for(int x = 0; x < d4P+2; x++) {a4PL[x] = 0.0; a4PR[x] = 0.0;}
	c4AL = c4BL = c4CL = c4DL = c4EL = c4FL = c4GL = c4HL = 1;
	c4IL = c4JL = c4KL = c4LL = c4ML = c4NL = c4OL = c4PL = 1;
	c4AR = c4BR = c4CR = c4DR = c4ER = c4FR = c4GR = c4HR = 1;
	c4IR = c4JR = c4KR = c4LR = c4MR = c4NR = c4OR = c4PR = 1;
	f4AL = f4BL = f4CL = f4DL = 0.0;
	f4DR = f4HR = f4LR = f4PR = 0.0;
	
	for(int x = 0; x < d4A+2; x++) {b4AL[x] = 0.0; b4AR[x] = 0.0;}
	for(int x = 0; x < d4B+2; x++) {b4BL[x] = 0.0; b4BR[x] = 0.0;}
	for(int x = 0; x < d4C+2; x++) {b4CL[x] = 0.0; b4CR[x] = 0.0;}
	for(int x = 0; x < d4D+2; x++) {b4DL[x] = 0.0; b4DR[x] = 0.0;}
	for(int x = 0; x < d4E+2; x++) {b4EL[x] = 0.0; b4ER[x] = 0.0;}
	for(int x = 0; x < d4F+2; x++) {b4FL[x] = 0.0; b4FR[x] = 0.0;}
	for(int x = 0; x < d4G+2; x++) {b4GL[x] = 0.0; b4GR[x] = 0.0;}
	for(int x = 0; x < d4H+2; x++) {b4HL[x] = 0.0; b4HR[x] = 0.0;}
	for(int x = 0; x < d4I+2; x++) {b4IL[x] = 0.0; b4IR[x] = 0.0;}
	for(int x = 0; x < d4J+2; x++) {b4JL[x] = 0.0; b4JR[x] = 0.0;}
	for(int x = 0; x < d4K+2; x++) {b4KL[x] = 0.0; b4KR[x] = 0.0;}
	for(int x = 0; x < d4L+2; x++) {b4LL[x] = 0.0; b4LR[x] = 0.0;}
	for(int x = 0; x < d4M+2; x++) {b4ML[x] = 0.0; b4MR[x] = 0.0;}
	for(int x = 0; x < d4N+2; x++) {b4NL[x] = 0.0; b4NR[x] = 0.0;}
	for(int x = 0; x < d4O+2; x++) {b4OL[x] = 0.0; b4OR[x] = 0.0;}
	for(int x = 0; x < d4P+2; x++) {b4PL[x] = 0.0; b4PR[x] = 0.0;}
	e4AL = e4BL = e4CL = e4DL = e4EL = e4FL = e4GL = e4HL = 1;
	e4IL = e4JL = e4KL = e4LL = e4ML = e4NL = e4OL = e4PL = 1;
	e4AR = e4BR = e4CR = e4DR = e4ER = e4FR = e4GR = e4HR = 1;
	e4IR = e4JR = e4KR = e4LR = e4MR = e4NR = e4OR = e4PR = 1;
	g4AL = g4BL = g4CL = g4DL = 0.0;
	g4DR = g4HR = g4LR = g4PR = 0.0;

	for (int x = 0; x < bez_total; x++) {
		bez[x] = 0.0;
		bezF[x] = 0.0;
	}
	bez[bez_cycle] = 1.0;
	bezF[bez_cycle] = 1.0;
	
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

VerbTiny::~VerbTiny() {}
VstInt32 VerbTiny::getVendorVersion () {return 1000;}
void VerbTiny::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void VerbTiny::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 VerbTiny::getChunk (void** data, bool isPreset)
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

VstInt32 VerbTiny::setChunk (void* data, VstInt32 byteSize, bool isPreset)
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

void VerbTiny::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        case kParamD: D = value; break;
        case kParamE: E = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float VerbTiny::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        case kParamD: return D; break;
        case kParamE: return E; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void VerbTiny::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Replace", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Derez", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Filter", kVstMaxParamStrLen); break;
		case kParamD: vst_strncpy (text, "Wider", kVstMaxParamStrLen); break;
		case kParamE: vst_strncpy (text, "Dry/Wet", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void VerbTiny::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: float2string (A, text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        case kParamD: float2string (D, text, kVstMaxParamStrLen); break;
        case kParamE: float2string (E, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void VerbTiny::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamE: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 VerbTiny::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool VerbTiny::getEffectName(char* name) {
    vst_strncpy(name, "VerbTiny", kVstMaxProductStrLen); return true;
}

VstPlugCategory VerbTiny::getPlugCategory() {return kPlugCategEffect;}

bool VerbTiny::getProductString(char* text) {
  	vst_strncpy (text, "airwindows VerbTiny", kVstMaxProductStrLen); return true;
}

bool VerbTiny::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
