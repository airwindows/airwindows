/* ========================================
 *  PurestConsole3Channel - PurestConsole3Channel.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestConsole3Channel_H
#include "PurestConsole3Channel.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PurestConsole3Channel(audioMaster);}

PurestConsole3Channel::PurestConsole3Channel(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
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

PurestConsole3Channel::~PurestConsole3Channel() {}
VstInt32 PurestConsole3Channel::getVendorVersion () {return 1000;}
void PurestConsole3Channel::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PurestConsole3Channel::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!


VstInt32 PurestConsole3Channel::getChunk (void** data, bool isPreset)
{
	return kNumParameters * sizeof(float);
}

VstInt32 PurestConsole3Channel::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void PurestConsole3Channel::setParameter(VstInt32 index, float value) {
}

float PurestConsole3Channel::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PurestConsole3Channel::getParameterName(VstInt32 index, char *text) {
}

void PurestConsole3Channel::getParameterDisplay(VstInt32 index, char *text) {
}

void PurestConsole3Channel::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 PurestConsole3Channel::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PurestConsole3Channel::getEffectName(char* name) {
    vst_strncpy(name, "PurestConsole3Channel", kVstMaxProductStrLen); return true;
}

VstPlugCategory PurestConsole3Channel::getPlugCategory() {return kPlugCategEffect;}

bool PurestConsole3Channel::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PurestConsole3Channel", kVstMaxProductStrLen); return true;
}

bool PurestConsole3Channel::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
