/* ========================================
 *  ConsoleX2Buss - ConsoleX2Buss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleX2Buss_H
#include "ConsoleX2Buss.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleX2Buss(audioMaster);}

ConsoleX2Buss::ConsoleX2Buss(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	HIG = 0.5;
	HMG = 0.5;
	LMG = 0.5;
	BSG = 0.5;
	HIF = 0.5;
	HMF = 0.5;
	LMF = 0.5;
	BSF = 0.5;
	THR = 1.0;
	PAN = 0.5;
	FAD = 0.5;
	
	for (int x = 0; x < biq_total; x++) {
		highA[x] = 0.0;
		highB[x] = 0.0;
		highC[x] = 0.0;
		midA[x] = 0.0;
		midB[x] = 0.0;
		midC[x] = 0.0;
		lowA[x] = 0.0;
		lowB[x] = 0.0;
		lowC[x] = 0.0;
	}
	highLIIR = 0.0;
	highRIIR = 0.0;
	midLIIR = 0.0;
	midRIIR = 0.0;
	lowLIIR = 0.0;
	lowRIIR = 0.0;
	//SmoothEQ2
	
	for (int x = 0; x < bez_total; x++) {bezCompF[x] = 0.0;bezCompS[x] = 0.0;}
	bezCompF[bez_cycle] = 1.0;
	bezCompS[bez_cycle] = 1.0;
	//Dynamics2
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; avg32R[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; avg16R[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; avg8R[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; avg4R[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; avg2R[x] = 0.0;}
	avgPos = 0;
	lastSlewL = 0.0; lastSlewR = 0.0;
	lastSlewpleL = 0.0; lastSlewpleR = 0.0;
	//preTapeHack
	
	panA = 0.5; panB = 0.5;
	inTrimA = 0.5; inTrimB = 0.5;
	
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

ConsoleX2Buss::~ConsoleX2Buss() {}
VstInt32 ConsoleX2Buss::getVendorVersion () {return 1000;}
void ConsoleX2Buss::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleX2Buss::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 ConsoleX2Buss::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = HIG;
	chunkData[1] = HMG;
	chunkData[2] = LMG;
	chunkData[3] = BSG;
	chunkData[4] = HIF;
	chunkData[5] = HMF;
	chunkData[6] = LMF;
	chunkData[7] = BSF;
	chunkData[8] = THR;
	chunkData[9] = PAN;
	chunkData[10] = FAD;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 ConsoleX2Buss::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	HIG = pinParameter(chunkData[0]);
	HMG = pinParameter(chunkData[1]);
	LMG = pinParameter(chunkData[2]);
	BSG = pinParameter(chunkData[3]);
	HIF = pinParameter(chunkData[4]);
	HMF = pinParameter(chunkData[5]);
	LMF = pinParameter(chunkData[6]);
	BSF = pinParameter(chunkData[7]);
	THR = pinParameter(chunkData[8]);
	PAN = pinParameter(chunkData[9]);
	FAD = pinParameter(chunkData[10]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void ConsoleX2Buss::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamHIG: HIG = value; break;
        case kParamHMG: HMG = value; break;
        case kParamLMG: LMG = value; break;
        case kParamBSG: BSG = value; break;
        case kParamHIF: HIF = value; break;
        case kParamHMF: HMF = value; break;
        case kParamLMF: LMF = value; break;
        case kParamBSF: BSF = value; break;
        case kParamTHR: THR = value; break;
        case kParamPAN: PAN = value; break;
        case kParamFAD: FAD = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float ConsoleX2Buss::getParameter(VstInt32 index) {
    switch (index) {
        case kParamHIG: return HIG; break;
        case kParamHMG: return HMG; break;
        case kParamLMG: return LMG; break;
        case kParamBSG: return BSG; break;
        case kParamHIF: return HIF; break;
        case kParamHMF: return HMF; break;
        case kParamLMF: return LMF; break;
        case kParamBSF: return BSF; break;
        case kParamTHR: return THR; break;
        case kParamPAN: return PAN; break;
        case kParamFAD: return FAD; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleX2Buss::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamHIG: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamHMG: vst_strncpy (text, "HMid", kVstMaxParamStrLen); break;
		case kParamLMG: vst_strncpy (text, "LMid", kVstMaxParamStrLen); break;
		case kParamBSG: vst_strncpy (text, "Bass", kVstMaxParamStrLen); break;
		case kParamHIF: vst_strncpy (text, "HighF", kVstMaxParamStrLen); break;
		case kParamHMF: vst_strncpy (text, "HMidF", kVstMaxParamStrLen); break;
		case kParamLMF: vst_strncpy (text, "LMidF", kVstMaxParamStrLen); break;
		case kParamBSF: vst_strncpy (text, "BassF", kVstMaxParamStrLen); break;
		case kParamTHR: vst_strncpy (text, "Thresh", kVstMaxParamStrLen); break;
		case kParamPAN: vst_strncpy (text, "Pan", kVstMaxParamStrLen); break;
		case kParamFAD: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ConsoleX2Buss::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamHIG: float2string (HIG, text, kVstMaxParamStrLen); break;
        case kParamHMG: float2string (HMG, text, kVstMaxParamStrLen); break;
        case kParamLMG: float2string (LMG, text, kVstMaxParamStrLen); break;
        case kParamBSG: float2string (BSG, text, kVstMaxParamStrLen); break;
        case kParamHIF: float2string (HIF, text, kVstMaxParamStrLen); break;
        case kParamHMF: float2string (HMF, text, kVstMaxParamStrLen); break;
        case kParamLMF: float2string (LMF, text, kVstMaxParamStrLen); break;
        case kParamBSF: float2string (BSF, text, kVstMaxParamStrLen); break;
        case kParamTHR: float2string (THR, text, kVstMaxParamStrLen); break;
        case kParamPAN: float2string (PAN, text, kVstMaxParamStrLen); break;
        case kParamFAD: float2string (FAD, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ConsoleX2Buss::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamHIG: vst_strncpy (text, "eq", kVstMaxParamStrLen); break;
        case kParamHMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamLMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamBSG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamHIF: vst_strncpy (text, "freq", kVstMaxParamStrLen); break;
        case kParamHMF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamLMF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamBSF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamTHR: vst_strncpy (text, "dyn", kVstMaxParamStrLen); break;
        case kParamPAN: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamFAD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ConsoleX2Buss::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleX2Buss::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleX2Buss", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleX2Buss::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleX2Buss::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleX2Buss", kVstMaxProductStrLen); return true;
}

bool ConsoleX2Buss::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
