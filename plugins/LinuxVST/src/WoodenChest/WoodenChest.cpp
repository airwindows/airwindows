/* ========================================
 *  WoodenChest - WoodenChest.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __WoodenChest_H
#include "WoodenChest.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new WoodenChest(audioMaster);}

WoodenChest::WoodenChest(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	A = 0.0;
	B = 0.5;
	C = 0.5;

	for(int x = 0; x < d4A+2; x++) {lA[x] = 0.0; rA[x] = 0.0;}
	for(int x = 0; x < d4B+2; x++) {lB[x] = 0.0; rB[x] = 0.0;}
	for(int x = 0; x < d4C+2; x++) {lC[x] = 0.0; rC[x] = 0.0;}
	for(int x = 0; x < d4D+2; x++) {lD[x] = 0.0; rD[x] = 0.0;}
	for(int x = 0; x < d4E+2; x++) {lE[x] = 0.0; rE[x] = 0.0;}
	for(int x = 0; x < d4F+2; x++) {lF[x] = 0.0; rF[x] = 0.0;}
	for(int x = 0; x < d4G+2; x++) {lG[x] = 0.0; rG[x] = 0.0;}
	for(int x = 0; x < d4H+2; x++) {lH[x] = 0.0; rH[x] = 0.0;}
	for(int x = 0; x < d4I+2; x++) {lI[x] = 0.0; rI[x] = 0.0;}
	for(int x = 0; x < d4J+2; x++) {lJ[x] = 0.0; rJ[x] = 0.0;}
	for(int x = 0; x < d4K+2; x++) {lK[x] = 0.0; rK[x] = 0.0;}
	for(int x = 0; x < d4L+2; x++) {lL[x] = 0.0; rL[x] = 0.0;}
	for(int x = 0; x < d4M+2; x++) {lM[x] = 0.0; rM[x] = 0.0;}
	for(int x = 0; x < d4N+2; x++) {lN[x] = 0.0; rN[x] = 0.0;}
	for(int x = 0; x < d4O+2; x++) {lO[x] = 0.0; rO[x] = 0.0;}
	for(int x = 0; x < d4P+2; x++) {lP[x] = 0.0; rP[x] = 0.0;}
	
	cLA = cLB = cLC = cLD = cLE = cLF = cLG = cLH = 1;
	cLI = cLJ = cLK = cLL = cLM = cLN = cLO = cLP = 1;
	
	cRA = cRB = cRC = cRD = cRE = cRF = cRG = cRH = 1;
	cRI = cRJ = cRK = cRL = cRM = cRN = cRO = cRP = 1;
	
	fAL = fBL = fCL = fDL = 0.0f;
	hAL = hBL = hCL = hDL = hEL = 0.0f;
	
	fDR = fHR = fLR = fPR = 0.0f;
	hAR = hBR = hCR = hDR = hER = 0.0f;
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	shortA = 350; shortB = 10; shortC = 21; shortD = 76; 
	shortE = 80; shortF = 72; shortG = 40; shortH = 108; 
	shortI = 83; shortJ = 7; shortK = 47; shortL = 33; 
	shortM = 38; shortN = 65; shortO = 29; shortP = 5; //1 to 13 ms, 6 seat room  
	prevclearcoat = -1;
		
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

WoodenChest::~WoodenChest() {}
VstInt32 WoodenChest::getVendorVersion () {return 1000;}
void WoodenChest::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void WoodenChest::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 WoodenChest::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	chunkData[1] = B;
	chunkData[2] = C;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 WoodenChest::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	B = pinParameter(chunkData[1]);
	C = pinParameter(chunkData[2]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void WoodenChest::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        case kParamB: B = value; break;
        case kParamC: C = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float WoodenChest::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        case kParamB: return B; break;
        case kParamC: return C; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void WoodenChest::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Select", kVstMaxParamStrLen); break;
		case kParamB: vst_strncpy (text, "Reso", kVstMaxParamStrLen); break;
		case kParamC: vst_strncpy (text, "Depth", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void WoodenChest::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: int2string ((VstInt32)( A * 16.999 ), text, kVstMaxParamStrLen); break;
        case kParamB: float2string (B, text, kVstMaxParamStrLen); break;
        case kParamC: float2string (C, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void WoodenChest::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamB: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamC: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 WoodenChest::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool WoodenChest::getEffectName(char* name) {
    vst_strncpy(name, "WoodenChest", kVstMaxProductStrLen); return true;
}

VstPlugCategory WoodenChest::getPlugCategory() {return kPlugCategEffect;}

bool WoodenChest::getProductString(char* text) {
  	vst_strncpy (text, "airwindows WoodenChest", kVstMaxProductStrLen); return true;
}

bool WoodenChest::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
