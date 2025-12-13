/* ========================================
 *  ConsoleX2Pre - ConsoleX2Pre.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleX2Pre_H
#include "ConsoleX2Pre.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new ConsoleX2Pre(audioMaster);}

ConsoleX2Pre::ConsoleX2Pre(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	TRM = 0.25;
	MOR = 0.0;
	HIG = 0.5;
	HMG = 0.5;
	LMG = 0.5;
	BSG = 0.5;
	HIF = 0.5;
	HMF = 0.5;
	LMF = 0.5;
	BSF = 0.5;
	THR = 1.0;
	ATK = 0.5;
	RLS = 0.5;
	GAT = 0.0;
	LOP = 1.0;
	HIP = 0.0;
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
	
	for (int x = 0; x < bez_total; x++) bezComp[x] = 0.0;
	bezComp[bez_cycle] = 1.0; 
	bezMaxL = 0.0; bezMinL = 0.0; bezGateL = 2.0;
	bezMaxR = 0.0; bezMinR = 0.0; bezGateR = 2.0; //Dual mono version
	//Dynamics3
	
	for(int count = 0; count < 22; count++) {
		iirHPositionL[count] = 0.0;
		iirHAngleL[count] = 0.0;
		iirHPositionR[count] = 0.0;
		iirHAngleR[count] = 0.0;
	}
	hBypass = false;
	
	for(int count = 0; count < 14; count++) {
		iirLPositionL[count] = 0.0;
		iirLAngleL[count] = 0.0;
		iirLPositionR[count] = 0.0;
		iirLAngleR[count] = 0.0;
	}
	lBypass = false;	
	//Cabs2
	
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
		dBaR[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBaPosR = 0.0;
	dBaXL = 1;
	dBaXR = 1;
	//Discontapeity
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; avg32R[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; avg16R[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; avg8R[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; avg4R[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; avg2R[x] = 0.0;}
	avgPos = 0;
	lastSlewL = 0.0; lastSlewR = 0.0;
	lastSlewpleL = 0.0; lastSlewpleR = 0.0;
	//preTapeHack	
	
	lFreqA = 1.0; lFreqB = 1.0;
	hFreqA = 0.0; hFreqB = 0.0;
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

ConsoleX2Pre::~ConsoleX2Pre() {}
VstInt32 ConsoleX2Pre::getVendorVersion () {return 1000;}
void ConsoleX2Pre::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void ConsoleX2Pre::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 ConsoleX2Pre::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = TRM;
	chunkData[1] = MOR;
	chunkData[2] = HIG;
	chunkData[3] = HMG;
	chunkData[4] = LMG;
	chunkData[5] = BSG;
	chunkData[6] = HIF;
	chunkData[7] = HMF;
	chunkData[8] = LMF;
	chunkData[9] = BSF;
	chunkData[10] = THR;
	chunkData[11] = ATK;
	chunkData[12] = RLS;
	chunkData[13] = GAT;
	chunkData[14] = LOP;
	chunkData[15] = HIP;
	chunkData[16] = FAD;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 ConsoleX2Pre::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	TRM = pinParameter(chunkData[0]);
	MOR = pinParameter(chunkData[1]);
	HIG = pinParameter(chunkData[2]);
	HMG = pinParameter(chunkData[3]);
	LMG = pinParameter(chunkData[4]);
	BSG = pinParameter(chunkData[5]);
	HIF = pinParameter(chunkData[6]);
	HMF = pinParameter(chunkData[7]);
	LMF = pinParameter(chunkData[8]);
	BSF = pinParameter(chunkData[9]);
	THR = pinParameter(chunkData[10]);
	ATK = pinParameter(chunkData[11]);
	RLS = pinParameter(chunkData[12]);
	GAT = pinParameter(chunkData[13]);
	LOP = pinParameter(chunkData[14]);
	HIP = pinParameter(chunkData[15]);
	FAD = pinParameter(chunkData[16]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void ConsoleX2Pre::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamTRM: TRM = value; break;
        case kParamMOR: MOR = value; break;
        case kParamHIG: HIG = value; break;
        case kParamHMG: HMG = value; break;
        case kParamLMG: LMG = value; break;
        case kParamBSG: BSG = value; break;
        case kParamHIF: HIF = value; break;
        case kParamHMF: HMF = value; break;
        case kParamLMF: LMF = value; break;
        case kParamBSF: BSF = value; break;
        case kParamTHR: THR = value; break;
        case kParamATK: ATK = value; break;
        case kParamRLS: RLS = value; break;
        case kParamGAT: GAT = value; break;
        case kParamLOP: LOP = value; break;
        case kParamHIP: HIP = value; break;
        case kParamFAD: FAD = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float ConsoleX2Pre::getParameter(VstInt32 index) {
    switch (index) {
        case kParamTRM: return TRM; break;
        case kParamMOR: return MOR; break;
        case kParamHIG: return HIG; break;
        case kParamHMG: return HMG; break;
        case kParamLMG: return LMG; break;
        case kParamBSG: return BSG; break;
        case kParamHIF: return HIF; break;
        case kParamHMF: return HMF; break;
        case kParamLMF: return LMF; break;
        case kParamBSF: return BSF; break;
        case kParamTHR: return THR; break;
        case kParamATK: return ATK; break;
        case kParamRLS: return RLS; break;
        case kParamGAT: return GAT; break;
        case kParamLOP: return LOP; break;
        case kParamHIP: return HIP; break;
        case kParamFAD: return FAD; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void ConsoleX2Pre::getParameterName(VstInt32 index, char *text) {
    switch (index) {
		case kParamTRM: vst_strncpy (text, "Trim", kVstMaxParamStrLen); break;
		case kParamMOR: vst_strncpy (text, "More", kVstMaxParamStrLen); break;
		case kParamHIG: vst_strncpy (text, "High", kVstMaxParamStrLen); break;
		case kParamHMG: vst_strncpy (text, "HMid", kVstMaxParamStrLen); break;
		case kParamLMG: vst_strncpy (text, "LMid", kVstMaxParamStrLen); break;
		case kParamBSG: vst_strncpy (text, "Bass", kVstMaxParamStrLen); break;
		case kParamHIF: vst_strncpy (text, "HighF", kVstMaxParamStrLen); break;
		case kParamHMF: vst_strncpy (text, "HMidF", kVstMaxParamStrLen); break;
		case kParamLMF: vst_strncpy (text, "LMidF", kVstMaxParamStrLen); break;
		case kParamBSF: vst_strncpy (text, "BassF", kVstMaxParamStrLen); break;
		case kParamTHR: vst_strncpy (text, "Thresh", kVstMaxParamStrLen); break;
		case kParamATK: vst_strncpy (text, "Attack", kVstMaxParamStrLen); break;
		case kParamRLS: vst_strncpy (text, "Release", kVstMaxParamStrLen); break;
		case kParamGAT: vst_strncpy (text, "Gate", kVstMaxParamStrLen); break;
		case kParamLOP: vst_strncpy (text, "Lowpass", kVstMaxParamStrLen); break;
		case kParamHIP: vst_strncpy (text, "Hipass", kVstMaxParamStrLen); break;
		case kParamFAD: vst_strncpy (text, "Fader", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void ConsoleX2Pre::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamTRM: int2string ((int)(TRM*4.0), text, kVstMaxParamStrLen); break;
        case kParamMOR: float2string (MOR, text, kVstMaxParamStrLen); break;
        case kParamHIG: float2string (HIG, text, kVstMaxParamStrLen); break;
        case kParamHMG: float2string (HMG, text, kVstMaxParamStrLen); break;
        case kParamLMG: float2string (LMG, text, kVstMaxParamStrLen); break;
        case kParamBSG: float2string (BSG, text, kVstMaxParamStrLen); break;
        case kParamHIF: float2string (HIF, text, kVstMaxParamStrLen); break;
        case kParamHMF: float2string (HMF, text, kVstMaxParamStrLen); break;
        case kParamLMF: float2string (LMF, text, kVstMaxParamStrLen); break;
        case kParamBSF: float2string (BSF, text, kVstMaxParamStrLen); break;
        case kParamTHR: float2string (THR, text, kVstMaxParamStrLen); break;
        case kParamATK: float2string (ATK, text, kVstMaxParamStrLen); break;
        case kParamRLS: float2string (RLS, text, kVstMaxParamStrLen); break;
        case kParamGAT: float2string (GAT, text, kVstMaxParamStrLen); break;
        case kParamLOP: float2string (LOP, text, kVstMaxParamStrLen); break;
        case kParamHIP: float2string (HIP, text, kVstMaxParamStrLen); break;
        case kParamFAD: float2string (FAD, text, kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void ConsoleX2Pre::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamTRM: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamMOR: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamHIG: vst_strncpy (text, "eq", kVstMaxParamStrLen); break;
        case kParamHMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamLMG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamBSG: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamHIF: vst_strncpy (text, "freq", kVstMaxParamStrLen); break;
        case kParamHMF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamLMF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamBSF: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamTHR: vst_strncpy (text, "dyn", kVstMaxParamStrLen); break;
        case kParamATK: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamRLS: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamGAT: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamLOP: vst_strncpy (text, "fltr", kVstMaxParamStrLen); break;
        case kParamHIP: vst_strncpy (text, "", kVstMaxParamStrLen); break;
        case kParamFAD: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 ConsoleX2Pre::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool ConsoleX2Pre::getEffectName(char* name) {
    vst_strncpy(name, "ConsoleX2Pre", kVstMaxProductStrLen); return true;
}

VstPlugCategory ConsoleX2Pre::getPlugCategory() {return kPlugCategEffect;}

bool ConsoleX2Pre::getProductString(char* text) {
  	vst_strncpy (text, "airwindows ConsoleX2Pre", kVstMaxProductStrLen); return true;
}

bool ConsoleX2Pre::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
