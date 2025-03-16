/* ========================================
 *  DubPlate - DubPlate.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DubPlate_H
#include "DubPlate.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new DubPlate(audioMaster);}

DubPlate::DubPlate(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	iirA = 0.0;
	iirB = 0.0;
	iirC = 0.0;
	iirD = 0.0;
	fpFlip = true;
	lastSinewAL = 0.0;
	lastSinewAR = 0.0;
	lastSinewBL = 0.0;
	lastSinewBR = 0.0;
	
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

DubPlate::~DubPlate() {}
VstInt32 DubPlate::getVendorVersion () {return 1000;}
void DubPlate::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void DubPlate::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 DubPlate::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 DubPlate::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void DubPlate::setParameter(VstInt32 index, float value) {
}

float DubPlate::getParameter(VstInt32 index) {
	return 0.0; //we only need to update the relevant name, this is simple to manage
}

void DubPlate::getParameterName(VstInt32 index, char *text) {
}

void DubPlate::getParameterDisplay(VstInt32 index, char *text) {
}

void DubPlate::getParameterLabel(VstInt32 index, char *text) {
}

VstInt32 DubPlate::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool DubPlate::getEffectName(char* name) {
    vst_strncpy(name, "DubPlate", kVstMaxProductStrLen); return true;
}

VstPlugCategory DubPlate::getPlugCategory() {return kPlugCategEffect;}

bool DubPlate::getProductString(char* text) {
  	vst_strncpy (text, "airwindows DubPlate", kVstMaxProductStrLen); return true;
}

bool DubPlate::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
