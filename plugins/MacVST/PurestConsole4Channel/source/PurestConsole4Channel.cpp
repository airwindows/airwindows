/* ========================================
 *  PurestConsole4Channel - PurestConsole4Channel.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestConsole4Channel_H
#include "PurestConsole4Channel.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PurestConsole4Channel(audioMaster);}

PurestConsole4Channel::PurestConsole4Channel(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	SMO = 0.5;
	PAN = 0.5;
	FAD = 0.5;
	
	for (int x = 0; x < bezier_total; x++) bezier[x] = 0.0;

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

PurestConsole4Channel::~PurestConsole4Channel() {}
VstInt32 PurestConsole4Channel::getVendorVersion () {return 1000;}
void PurestConsole4Channel::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PurestConsole4Channel::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 PurestConsole4Channel::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = SMO;
	chunkData[1] = PAN;
	chunkData[2] = FAD;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 PurestConsole4Channel::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	SMO = pinParameter(chunkData[0]);
	PAN = pinParameter(chunkData[1]);
	FAD = pinParameter(chunkData[2]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void PurestConsole4Channel::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamSMO: SMO = value; break;
        case kParamPAN: PAN = value; break;
        case kParamFAD: FAD = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float PurestConsole4Channel::getParameter(VstInt32 index) {
    switch (index) {
        case kParamSMO: return SMO; break;
        case kParamPAN: return PAN; break;
        case kParamFAD: return FAD; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PurestConsole4Channel::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamSMO: vst_strncpy (text, "Smooth", kVstMaxParamStrLen); break;
		case kParamPAN: vst_strncpy (text, "Pan", kVstMaxParamStrLen); break;
		case kParamFAD: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void PurestConsole4Channel::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamSMO: float2string (SMO, text, kVstMaxParamStrLen); break;
        case kParamPAN: float2string (PAN, text, kVstMaxParamStrLen); break;
        case kParamFAD: float2string (FAD, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void PurestConsole4Channel::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamSMO: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamPAN: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamFAD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 PurestConsole4Channel::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PurestConsole4Channel::getEffectName(char* name) {
    vst_strncpy(name, "PurestConsole4Channel", kVstMaxProductStrLen); return true;
}

VstPlugCategory PurestConsole4Channel::getPlugCategory() {return kPlugCategEffect;}

bool PurestConsole4Channel::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PurestConsole4Channel", kVstMaxProductStrLen); return true;
}

bool PurestConsole4Channel::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
