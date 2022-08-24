//
//  ADRS.hpp
//
//  Created by Nigel Redmon on 12/18/12.
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the ADSR envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code for your own purposes, free or commercial.
//
//  1.01  2016-01-02  njr   added calcCoef to SetTargetRatio functions that were in the ADSR widget but missing in this code
//  1.02  2017-01-04  njr   in calcCoef, checked for rate 0, to support non-IEEE compliant compilers
//  1.03  2020-04-08  njr   changed float to double; large target ratio and rate resulted in exp returning 1 in calcCoef
//

#ifndef ADRS_h
#define ADRS_h

#include <math.h>

class ADSR {
public:
	ADSR(void);
	~ADSR(void);
	double process(void);
    double getOutput(void);
    double getLoopVal(void);

    int getState(void);
	void gate(int on);
    void setAttackRate(double rate);
    void setDecayRate(double rate);
    void setLoopRate(double rate);
    void setReleaseRate(double rate);
	void setSustainLevel(double level);
    void setTargetRatioA(double targetRatio);
    void setTargetRatioDR(double targetRatio);
    void setPingPong(int mode);
    void reset(void);

    enum envState {
        env_idle = 0,
        env_attack,
        env_decay,
        env_sustain,
        env_release
    };

protected:
	int state;
	double output;
	double attackRate;
	double decayRate;
	double releaseRate;
	double attackCoef;
	double decayCoef;
	double releaseCoef;
	double sustainLevel;
    double targetRatioA;
    double targetRatioDR;
    double attackBase;
    double decayBase;
    double releaseBase;
    double sus_gain;
    double LoopRate;
    double LoopCoef;
    double LoopBase;
    int ping_pong_flag;
    int loop_direction; //0 = FORWARD 1 = BACKWARD
    double calcCoef(double rate, double targetRatio);
};

inline double ADSR::process() {
	switch (state) {
        case env_idle:
            break;
        case env_attack:
            output = attackBase + output * attackCoef;
            if (output >= 1.0) {
                output = 1.0;
                state = env_decay;
            }
            break;
        case env_decay:
            sus_gain = 0.0;
            loop_direction = 0;//SET TO FORWARD
            output = decayBase + output * decayCoef;
            if (output <= sustainLevel) {
                output = sustainLevel;
                state = env_sustain;
            }
            break;
        case env_sustain:
            if(ping_pong_flag)
            {
                if(loop_direction)//BACKWARD
                {
                sus_gain = sus_gain - LoopBase / LoopCoef;
                if (sus_gain <= 0.0) {
                    loop_direction = 0;
                    sus_gain = 0.0;
                }
                }
                else//FORWARD
                {
                sus_gain = sus_gain + LoopBase * LoopCoef;
                if (sus_gain >= 1.0) {
                    loop_direction = 1;
                    sus_gain = 1.0;
                }
                }
            }
            output = sustainLevel;
            break;
        case env_release:
            output = releaseBase + output * releaseCoef;
            if (output <= 0.0) {
                output = 0.0;
                state = env_idle;
            }
	}
	return output;
}

inline void ADSR::gate(int gate) {
	if (gate)
		state = env_attack;
	else if (state != env_idle)
        state = env_release;
}

inline int ADSR::getState() {
    return state;
}

inline void ADSR::reset() {
    state = env_idle;
    output = 0.0;
    sus_gain = 0.0;
}

inline double ADSR::getOutput() {
	return output;
}

#endif
