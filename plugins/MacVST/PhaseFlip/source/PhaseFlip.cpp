/* ========================================
 *  PhaseFlip - PhaseFlip.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __PhaseFlip_H
#include "PhaseFlip.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new PhaseFlip(audioMaster);}

PhaseFlip::PhaseFlip(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	
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

PhaseFlip::~PhaseFlip() {}
VstInt32 PhaseFlip::getVendorVersion () {return 1000;}
void PhaseFlip::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void PhaseFlip::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 PhaseFlip::getChunk (void** data, bool isPreset)
{
	return kNumParameters * sizeof(float);
}

VstInt32 PhaseFlip::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void PhaseFlip::setParameter(VstInt32 index, float value) {
}

float PhaseFlip::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void PhaseFlip::getParameterName(VstInt32 index, char *text) {
}

void PhaseFlip::getParameterDisplay(VstInt32 index, char *text) {
}

void PhaseFlip::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 PhaseFlip::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool PhaseFlip::getEffectName(char* name) {
    vst_strncpy(name, "PhaseFlip", kVstMaxProductStrLen); return true;
}

VstPlugCategory PhaseFlip::getPlugCategory() {return kPlugCategEffect;}

bool PhaseFlip::getProductString(char* text) {
  	vst_strncpy (text, "airwindows PhaseFlip", kVstMaxProductStrLen); return true;
}

bool PhaseFlip::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
