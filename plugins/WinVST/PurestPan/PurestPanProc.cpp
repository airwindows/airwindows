/* ========================================
 *  PurestPan - PurestPan.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __PurestPan_H
#include "PurestPan.h"
#endif

void PurestPan::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    
    
    // left channel attenuation at center is 20*log10(cos(pi/4)^2) or approx -6.02 dB
    // right channel attenuation at center is 20*log10(sin(pi/4)^2) or approx -6.02 dB
    // this pan law is so that panning never affects balances of the mono compatibility mix
    // this is the only correct pan law, anyone who disagrees is wrong and also ugly
    //
    // multiply by 2 at beginning of expression so volume doesn't drop when you turn it on
    double inputgainL = 20.0*log10(2.0*pow(cos(A*M_PI/2.0),2.0));
    double inputgainR = 20.0*log10(2.0*pow(sin(A*M_PI/2.0),2.0));
    
    // PurestGain-style chasing to remove clicks and pops
    if (settingchaseL != inputgainL) {
        chasespeedL *= 2.0;
        settingchaseL = inputgainL;
    }
    
    if (chasespeedL > 2500.0) chasespeedL = 2500.0;
    
    if (gainchaseL < -60.0) gainchaseL = pow(10.0,inputgainL/20.0);
    
    
    if (settingchaseR != inputgainR) {
        chasespeedR *= 2.0;
        settingchaseR = inputgainR;
    }
    
    if (chasespeedR > 2500.0) chasespeedR = 2500.0;
    
    if (gainchaseR < -60.0) gainchaseR = pow(10.0,inputgainR/20.0);
    
    
    double targetgainL;
    double targetgainR;
    
    double outputgainL;
    double outputgainR;
    
    long double inputSampleL;
    long double inputSampleR;
    
    while (--sampleFrames >= 0)
    {
        
        targetgainL = pow(10.0,settingchaseL/20.0);
        targetgainR = pow(10.0,settingchaseR/20.0);
        
        chasespeedL *= 0.9999;
        chasespeedL -= 0.01;
        if (chasespeedL < 350.0) chasespeedL = 350.0;
        
        chasespeedR *= 0.9999;
        chasespeedR -= 0.01;
        if (chasespeedR < 350.0) chasespeedR = 350.0;
        
        gainchaseL = (((gainchaseL*chasespeedL)+targetgainL)/(chasespeedL+1.0));
        gainchaseR = (((gainchaseR*chasespeedR)+targetgainR)/(chasespeedR+1.0));
        
        outputgainL = gainchaseL;
        outputgainR = gainchaseR;
        
        inputSampleL = *in1;
        inputSampleR = *in2;
        if (inputSampleL<1.2e-38 && -inputSampleL<1.2e-38) {
            static int noisesource = 0;
            //this declares a variable before anything else is compiled. It won't keep assigning
            //it to 0 for every sample, it's as if the declaration doesn't exist in this context,
            //but it lets me add this denormalization fix in a single place rather than updating
            //it in three different locations. The variable isn't thread-safe but this is only
            //a random seed and we can share it with whatever.
            noisesource = noisesource % 1700021; noisesource++;
            int residue = noisesource * noisesource;
            residue = residue % 170003; residue *= residue;
            residue = residue % 17011; residue *= residue;
            residue = residue % 1709; residue *= residue;
            residue = residue % 173; residue *= residue;
            residue = residue % 17;
            double applyresidue = residue;
            applyresidue *= 0.00000001;
            applyresidue *= 0.00000001;
            inputSampleL = applyresidue;
        }
        if (inputSampleR<1.2e-38 && -inputSampleR<1.2e-38) {
            static int noisesource = 0;
            noisesource = noisesource % 1700021; noisesource++;
            int residue = noisesource * noisesource;
            residue = residue % 170003; residue *= residue;
            residue = residue % 17011; residue *= residue;
            residue = residue % 1709; residue *= residue;
            residue = residue % 173; residue *= residue;
            residue = residue % 17;
            double applyresidue = residue;
            applyresidue *= 0.00000001;
            applyresidue *= 0.00000001;
            inputSampleR = applyresidue;
            //this denormalization routine produces a white noise at -300 dB which the noise
            //shaping will interact with to produce a bipolar output, but the noise is actually
            //all positive. That should stop any variables from going denormal, and the routine
            //only kicks in if digital black is input. As a final touch, if you save to 24-bit
            //the silence will return to being digital black again.
        }
        
        inputSampleL *= outputgainL;
        inputSampleR *= outputgainR;
        
        //stereo 32 bit dither, made small and tidy.
        int expon; frexpf((float)inputSampleL, &expon);
        long double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
        inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
        frexpf((float)inputSampleR, &expon);
        dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
        inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
        //end 32 bit dither
        
        *out1 = inputSampleL;
        *out2 = inputSampleR;
        
        *in1++;
        *in2++;
        *out1++;
        *out2++;
    }
}

void PurestPan::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
    
    
    // left channel attenuation at center is 20*log10(cos(pi/4)^2) or approx -6.02 dB
    // right channel attenuation at center is 20*log10(sin(pi/4)^2) or approx -6.02 dB
    // this pan law is so that panning never affects balances of the mono compatibility mix
    // this is the only correct pan law, anyone who disagrees is wrong and also ugly
    //
    // multiply by 2 at beginning of expression so volume doesn't drop when you turn it on
    double inputgainL = 20.0*log10(2.0*pow(cos(A*M_PI/2.0),2.0));
    double inputgainR = 20.0*log10(2.0*pow(sin(A*M_PI/2.0),2.0));
    
    // PurestGain-style chasing to remove clicks and pops
    if (settingchaseL != inputgainL) {
        chasespeedL *= 2.0;
        settingchaseL = inputgainL;
    }
    
    if (chasespeedL > 2500.0) chasespeedL = 2500.0;
    
    if (gainchaseL < -60.0) gainchaseL = pow(10.0,inputgainL/20.0);
    
    
    if (settingchaseR != inputgainR) {
        chasespeedR *= 2.0;
        settingchaseR = inputgainR;
    }
    
    if (chasespeedR > 2500.0) chasespeedR = 2500.0;
    
    if (gainchaseR < -60.0) gainchaseR = pow(10.0,inputgainR/20.0);
    
    
    double targetgainL;
    double targetgainR;
    
    double outputgainL;
    double outputgainR;
    
    long double inputSampleL;
    long double inputSampleR;
    
    while (--sampleFrames >= 0)
    {
        
        targetgainL = pow(10.0,settingchaseL/20.0);
        targetgainR = pow(10.0,settingchaseR/20.0);
        
        chasespeedL *= 0.9999;
        chasespeedL -= 0.01;
        if (chasespeedL < 350.0) chasespeedL = 350.0;
        
        chasespeedR *= 0.9999;
        chasespeedR -= 0.01;
        if (chasespeedR < 350.0) chasespeedR = 350.0;
        
        gainchaseL = (((gainchaseL*chasespeedL)+targetgainL)/(chasespeedL+1.0));
        gainchaseR = (((gainchaseR*chasespeedR)+targetgainR)/(chasespeedR+1.0));
        
        outputgainL = gainchaseL;
        outputgainR = gainchaseR;
        
        inputSampleL = *in1;
        inputSampleR = *in2;
        if (inputSampleL<1.2e-38 && -inputSampleL<1.2e-38) {
            static int noisesource = 0;
            //this declares a variable before anything else is compiled. It won't keep assigning
            //it to 0 for every sample, it's as if the declaration doesn't exist in this context,
            //but it lets me add this denormalization fix in a single place rather than updating
            //it in three different locations. The variable isn't thread-safe but this is only
            //a random seed and we can share it with whatever.
            noisesource = noisesource % 1700021; noisesource++;
            int residue = noisesource * noisesource;
            residue = residue % 170003; residue *= residue;
            residue = residue % 17011; residue *= residue;
            residue = residue % 1709; residue *= residue;
            residue = residue % 173; residue *= residue;
            residue = residue % 17;
            double applyresidue = residue;
            applyresidue *= 0.00000001;
            applyresidue *= 0.00000001;
            inputSampleL = applyresidue;
        }
        if (inputSampleR<1.2e-38 && -inputSampleR<1.2e-38) {
            static int noisesource = 0;
            noisesource = noisesource % 1700021; noisesource++;
            int residue = noisesource * noisesource;
            residue = residue % 170003; residue *= residue;
            residue = residue % 17011; residue *= residue;
            residue = residue % 1709; residue *= residue;
            residue = residue % 173; residue *= residue;
            residue = residue % 17;
            double applyresidue = residue;
            applyresidue *= 0.00000001;
            applyresidue *= 0.00000001;
            inputSampleR = applyresidue;
            //this denormalization routine produces a white noise at -300 dB which the noise
            //shaping will interact with to produce a bipolar output, but the noise is actually
            //all positive. That should stop any variables from going denormal, and the routine
            //only kicks in if digital black is input. As a final touch, if you save to 24-bit
            //the silence will return to being digital black again.
        }
        
        inputSampleL *= outputgainL;
        inputSampleR *= outputgainR;
        
        //stereo 64 bit dither, made small and tidy.
        int expon; frexp((double)inputSampleL, &expon);
        long double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
        dither /= 536870912.0; //needs this to scale to 64 bit zone
        inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
        frexp((double)inputSampleR, &expon);
        dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
        dither /= 536870912.0; //needs this to scale to 64 bit zone
        inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
        //end 64 bit dither
        
        *out1 = inputSampleL;
        *out2 = inputSampleR;
        
        *in1++;
        *in2++;
        *out1++;
        *out2++;
    }
}
