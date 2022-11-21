/* ========================================
 *  Monitoring2 - Monitoring2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Monitoring2_H
#include "Monitoring2.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Monitoring2(audioMaster);}

Monitoring2::Monitoring2(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	for(int count = 0; count < 99; count++) {
		darkSampleL[count] = 0;
		darkSampleR[count] = 0;
	}
	
	for(int count = 0; count < 1502; count++) {
		aL[count] = 0.0; bL[count] = 0.0; cL[count] = 0.0; dL[count] = 0.0;
		aR[count] = 0.0; bR[count] = 0.0; cR[count] = 0.0; dR[count] = 0.0;
	}
	ax = 1; bx = 1; cx = 1; dx = 1;
	//PeaksOnly
	lastSampleL = 0.0; lastSampleR = 0.0;
	//SlewOnly
	iirSampleAL = 0.0; iirSampleBL = 0.0; iirSampleCL = 0.0; iirSampleDL = 0.0; iirSampleEL = 0.0; iirSampleFL = 0.0; iirSampleGL = 0.0;
	iirSampleHL = 0.0; iirSampleIL = 0.0; iirSampleJL = 0.0; iirSampleKL = 0.0; iirSampleLL = 0.0; iirSampleML = 0.0; iirSampleNL = 0.0; iirSampleOL = 0.0; iirSamplePL = 0.0;
	iirSampleQL = 0.0; iirSampleRL = 0.0; iirSampleSL = 0.0;
	iirSampleTL = 0.0; iirSampleUL = 0.0; iirSampleVL = 0.0;
	iirSampleWL = 0.0; iirSampleXL = 0.0; iirSampleYL = 0.0; iirSampleZL = 0.0;
	
	iirSampleAR = 0.0; iirSampleBR = 0.0; iirSampleCR = 0.0; iirSampleDR = 0.0; iirSampleER = 0.0; iirSampleFR = 0.0; iirSampleGR = 0.0;
	iirSampleHR = 0.0; iirSampleIR = 0.0; iirSampleJR = 0.0; iirSampleKR = 0.0; iirSampleLR = 0.0; iirSampleMR = 0.0; iirSampleNR = 0.0; iirSampleOR = 0.0; iirSamplePR = 0.0;
	iirSampleQR = 0.0; iirSampleRR = 0.0; iirSampleSR = 0.0;
	iirSampleTR = 0.0; iirSampleUR = 0.0; iirSampleVR = 0.0;
	iirSampleWR = 0.0; iirSampleXR = 0.0; iirSampleYR = 0.0; iirSampleZR = 0.0; // o/`	
	//SubsOnly
	for (int x = 0; x < fix_total; x++) {biquad[x] = 0.0;}
	//Bandpasses
	A = 0.0;

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

Monitoring2::~Monitoring2() {}
VstInt32 Monitoring2::getVendorVersion () {return 1000;}
void Monitoring2::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Monitoring2::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

static float pinParameter(float data)
{
	if (data < 0.0f) return 0.0f;
	if (data > 1.0f) return 1.0f;
	return data;
}

VstInt32 Monitoring2::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	chunkData[0] = A;
	/* Note: The way this is set up, it will break if you manage to save settings on an Intel
	 machine and load them on a PPC Mac. However, it's fine if you stick to the machine you 
	 started with. */
	
	*data = chunkData;
	return kNumParameters * sizeof(float);
}

VstInt32 Monitoring2::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	float *chunkData = (float *)data;
	A = pinParameter(chunkData[0]);
	/* We're ignoring byteSize as we found it to be a filthy liar */
	
	/* calculate any other fields you need here - you could copy in 
	 code from setParameter() here. */
	return 0;
}

void Monitoring2::setParameter(VstInt32 index, float value) {
    switch (index) {
        case kParamA: A = value; break;
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Monitoring2::getParameter(VstInt32 index) {
    switch (index) {
        case kParamA: return A; break;
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Monitoring2::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "Monitor", kVstMaxParamStrLen); break;
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Monitoring2::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: switch((VstInt32)( A * 16.999 )) //0 to almost edge of # of params
		{	case kDKAD: vst_strncpy (text, "Out24", kVstMaxParamStrLen); break;
			case kDKCD: vst_strncpy (text, "Out16", kVstMaxParamStrLen); break;
			case kPEAK: vst_strncpy (text, "Peaks", kVstMaxParamStrLen); break;
			case kSLEW: vst_strncpy (text, "Slew", kVstMaxParamStrLen); break;
			case kSUBS: vst_strncpy (text, "Subs", kVstMaxParamStrLen); break;
			case kMONO: vst_strncpy (text, "Mono", kVstMaxParamStrLen); break;
			case kSIDE: vst_strncpy (text, "Side", kVstMaxParamStrLen); break;
			case kVINYL: vst_strncpy (text, "Vinyl", kVstMaxParamStrLen); break;
			case kAURAT: vst_strncpy (text, "Aurat", kVstMaxParamStrLen); break;
			case kMONORAT: vst_strncpy (text, "MonoRat", kVstMaxParamStrLen); break;
			case kMONOLAT: vst_strncpy (text, "MonoLat", kVstMaxParamStrLen); break;
			case kPHONE: vst_strncpy (text, "Phone", kVstMaxParamStrLen); break;
			case kCANSA: vst_strncpy (text, "Cans A", kVstMaxParamStrLen); break;
			case kCANSB: vst_strncpy (text, "Cans B", kVstMaxParamStrLen); break;
			case kCANSC: vst_strncpy (text, "Cans C", kVstMaxParamStrLen); break;
			case kCANSD: vst_strncpy (text, "Cans D", kVstMaxParamStrLen); break;
			case kTRICK: vst_strncpy (text, "V Trick", kVstMaxParamStrLen); break;
			default: break; // unknown parameter, shouldn't happen!
		} break;			
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Monitoring2::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
        case kParamA: vst_strncpy (text, "", kVstMaxParamStrLen); break;
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Monitoring2::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Monitoring2::getEffectName(char* name) {
    vst_strncpy(name, "Monitoring2", kVstMaxProductStrLen); return true;
}

VstPlugCategory Monitoring2::getPlugCategory() {return kPlugCategEffect;}

bool Monitoring2::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Monitoring2", kVstMaxProductStrLen); return true;
}

bool Monitoring2::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
