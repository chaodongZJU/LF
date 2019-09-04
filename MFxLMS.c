#include "UserDev.h"
#include "csl_cache.h"
#include "fastmath67x.h"
#define PI 3.14159265358979

float f;
float GoalAmp;
float GoalPhase;
int iFrameCount=0;
float w0 = 0.005;
float w1 = 0;
float StepSize;
float StepIdent;
float alpha = 0.0001;
float beta = 0.0001;  
float xa,xb; 
float outa,outb;
float Ref; 
float error = 0;  
float IdentError = 0;
float uInput,feedback;
int Start;
int StepTime;

void CaluInput()
{
    int i,j,k;
    float temp;	    
    xa = sinsp(2*PI*f*iFrameCount/fixed_samplefreq); 
    xb = cossp(2*PI*f*iFrameCount/fixed_samplefreq);
    Ref = GoalAmp*sinsp(2*PI*f*iFrameCount/fixed_samplefreq+GoalPhase);
    error = Ref - feedback;
    iFrameCount++;    
    if (iFrameCount<Start)
    {				
        uInput = w0*xa+w1*xb;
    }
    else 
    {				
        uInput = w0*xa+w1*xb;	
        outa = w0*alpha+w1*beta;
        outb = -w0*beta+w1*alpha;
        IdentError = feedback-outa*xa-outb*xb;
        temp = w0*w0+w1*w1;
        alpha += StepIdent*IdentError*(w0*xa+w1*xb)/temp;
        beta += StepIdent*IdentError*(w1*xa-w0*xb)/temp;
        if(iFrameCount<StepTime)
        {
            w0=w0+StepSize*(alpha*xa-beta*xb)*error;
            w1=w1+StepSize*(beta*xa+alpha*xb)*error;
        }
        else
        {
            temp = alpha*alpha+beta*beta;   
            w0=w0+StepSize*(alpha*xa-beta*xb)*error/temp;
            w1=w1+StepSize*(beta*xa+alpha*xb)*error/temp;
        }
    }
}	
