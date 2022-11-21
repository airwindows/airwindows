/* ========================================
 *  Ultrasonic - Ultrasonic.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Ultrasonic_H
#include "Ultrasonic.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Ultrasonic(audioMaster);}

Ultrasonic::Ultrasonic(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for (int x = 0; x < 15; x++) {biquadA[x] = 0.0; biquadB[x] = 0.0; biquadC[x] = 0.0; biquadD[x] = 0.0; biquadE[x] = 0.0;}
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

Ultrasonic::~Ultrasonic() {}
VstInt32 Ultrasonic::getVendorVersion () {return 1000;}
void Ultrasonic::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Ultrasonic::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 Ultrasonic::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Ultrasonic::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void Ultrasonic::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Ultrasonic::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Ultrasonic::getParameterName(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Ultrasonic::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Ultrasonic::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Ultrasonic::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Ultrasonic::getEffectName(char* name) {
    vst_strncpy(name, "Ultrasonic", kVstMaxProductStrLen); return true;
}

VstPlugCategory Ultrasonic::getPlugCategory() {return kPlugCategEffect;}

bool Ultrasonic::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Ultrasonic", kVstMaxProductStrLen); return true;
}

bool Ultrasonic::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
