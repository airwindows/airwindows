/* ========================================
 *  VerbSixes - VerbSixes.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VerbSixes_H
#include "VerbSixes.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new VerbSixes(audioMaster);}

VerbSixes::VerbSixes(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
#ifdef THREEBYTHREE
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
	f3AL = f3BL = f3CL = 0.0;
	f3CR = f3FR = f3IR = 0.0;
	avg3L = avg3R = 0.0;
#endif
#ifdef FOURBYFOUR
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
	avg4L = avg4R = 0.0;
#endif
#ifdef FIVEBYFIVE
	for(int x = 0; x < d5A+2; x++) {a5AL[x] = 0.0; a5AR[x] = 0.0;}
	for(int x = 0; x < d5B+2; x++) {a5BL[x] = 0.0; a5BR[x] = 0.0;}
	for(int x = 0; x < d5C+2; x++) {a5CL[x] = 0.0; a5CR[x] = 0.0;}
	for(int x = 0; x < d5D+2; x++) {a5DL[x] = 0.0; a5DR[x] = 0.0;}
	for(int x = 0; x < d5E+2; x++) {a5EL[x] = 0.0; a5ER[x] = 0.0;}
	for(int x = 0; x < d5F+2; x++) {a5FL[x] = 0.0; a5FR[x] = 0.0;}
	for(int x = 0; x < d5G+2; x++) {a5GL[x] = 0.0; a5GR[x] = 0.0;}
	for(int x = 0; x < d5H+2; x++) {a5HL[x] = 0.0; a5HR[x] = 0.0;}
	for(int x = 0; x < d5I+2; x++) {a5IL[x] = 0.0; a5IR[x] = 0.0;}
	for(int x = 0; x < d5J+2; x++) {a5JL[x] = 0.0; a5JR[x] = 0.0;}
	for(int x = 0; x < d5K+2; x++) {a5KL[x] = 0.0; a5KR[x] = 0.0;}
	for(int x = 0; x < d5L+2; x++) {a5LL[x] = 0.0; a5LR[x] = 0.0;}
	for(int x = 0; x < d5M+2; x++) {a5ML[x] = 0.0; a5MR[x] = 0.0;}
	for(int x = 0; x < d5N+2; x++) {a5NL[x] = 0.0; a5NR[x] = 0.0;}
	for(int x = 0; x < d5O+2; x++) {a5OL[x] = 0.0; a5OR[x] = 0.0;}
	for(int x = 0; x < d5P+2; x++) {a5PL[x] = 0.0; a5PR[x] = 0.0;}
	for(int x = 0; x < d5Q+2; x++) {a5QL[x] = 0.0; a5QR[x] = 0.0;}
	for(int x = 0; x < d5R+2; x++) {a5RL[x] = 0.0; a5RR[x] = 0.0;}
	for(int x = 0; x < d5S+2; x++) {a5SL[x] = 0.0; a5SR[x] = 0.0;}
	for(int x = 0; x < d5T+2; x++) {a5TL[x] = 0.0; a5TR[x] = 0.0;}
	for(int x = 0; x < d5U+2; x++) {a5UL[x] = 0.0; a5UR[x] = 0.0;}
	for(int x = 0; x < d5V+2; x++) {a5VL[x] = 0.0; a5VR[x] = 0.0;}
	for(int x = 0; x < d5W+2; x++) {a5WL[x] = 0.0; a5WR[x] = 0.0;}
	for(int x = 0; x < d5X+2; x++) {a5XL[x] = 0.0; a5XR[x] = 0.0;}
	for(int x = 0; x < d5Y+2; x++) {a5YL[x] = 0.0; a5YR[x] = 0.0;}
	c5AL = c5BL = c5CL = c5DL = c5EL = c5FL = c5GL = c5HL = 1;
	c5IL = c5JL = c5KL = c5LL = c5ML = c5NL = c5OL = c5PL = 1;
	c5QL = c5RL = c5SL = c5TL = c5UL = c5VL = c5WL = c5XL = c5YL = 1;
	c5AR = c5BR = c5CR = c5DR = c5ER = c5FR = c5GR = c5HR = 1;
	c5IR = c5JR = c5KR = c5LR = c5MR = c5NR = c5OR = c5PR = 1;
	c5QR = c5RR = c5SR = c5TR = c5UR = c5VR = c5WR = c5XR = c5YR = 1;
	f5AL = f5BL = f5CL = f5DL = f5EL = 0.0;
	f5ER = f5JR = f5OR = f5TR = f5YR = 0.0;
	avg5L = avg5R = 0.0;
#endif
#ifdef SIXBYSIX
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
#endif
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
		
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

VerbSixes::~VerbSixes() {}
VstInt32 VerbSixes::getVendorVersion () {return 1000;}
void VerbSixes::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void VerbSixes::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 VerbSixes::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 VerbSixes::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void VerbSixes::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float VerbSixes::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void VerbSixes::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void VerbSixes::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void VerbSixes::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 VerbSixes::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool VerbSixes::getEffectName(char* name) {
    vst_strncpy(name, "VerbSixes", kVstMaxProductStrLen); return true;
}

VstPlugCategory VerbSixes::getPlugCategory() {return kPlugCategEffect;}

bool VerbSixes::getProductString(char* text) {
  	vst_strncpy (text, "airwindows VerbSixes", kVstMaxProductStrLen); return true;
}

bool VerbSixes::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
