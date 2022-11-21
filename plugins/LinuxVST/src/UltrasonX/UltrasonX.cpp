/* ========================================
 *  UltrasonX - UltrasonX.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __UltrasonX_H
#include "UltrasonX.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new UltrasonX(audioMaster);}

UltrasonX::UltrasonX(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.5;
	
	for (int x = 0; x < fix_total; x++) {fixA[x] = 0.0;}
	fixA[fix_reso] = 0.7071; //butterworth Q

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

UltrasonX::~UltrasonX() {}
VstInt32 UltrasonX::getVendorVersion () {return 1000;}
void UltrasonX::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void UltrasonX::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 UltrasonX::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 UltrasonX::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void UltrasonX::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float UltrasonX::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void UltrasonX::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Q", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void UltrasonX::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 4.999 )) //0 to almost edge of # of params
		{	case kA: vst_strncpy (text, "Reso A", kVstMaxParamStrLen); break;
			case kB: vst_strncpy (text, "Reso B", kVstMaxParamStrLen); break;
			case kC: vst_strncpy (text, "Reso C", kVstMaxParamStrLen); break;
			case kD: vst_strncpy (text, "Reso D", kVstMaxParamStrLen); break;
			case kE: vst_strncpy (text, "Reso E", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break;			
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void UltrasonX::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 UltrasonX::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool UltrasonX::getEffectName(char* name) {
    vst_strncpy(name, "UltrasonX", kVstMaxProductStrLen); return true;
}

VstPlugCategory UltrasonX::getPlugCategory() {return kPlugCategEffect;}

bool UltrasonX::getProductString(char* text) {
  	vst_strncpy (text, "airwindows UltrasonX", kVstMaxProductStrLen); return true;
}

bool UltrasonX::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
