#ifndef MSG_PAUSE_DETECTOR_CPP
#define MSG_PAUSE_DETECTOR_CPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include "msg_decoder.cpp"

class pause_detector: public rtos::task<>{
private:
    hwlib::pin_in & IR;
    //rtos::timer sampleClock;
    rtos::clock sampleClock;
//    uint_fast64_t CurryTime;
    enum class STATE {IDLE, SIGNAL};
    pause_listener & p_listener;
    void main(){
        STATE S = STATE::IDLE;
        int n = 0;
        for(;;){
            switch(S){
                case STATE::IDLE:
                // teller van de pauze is 0
                //100 micro seconde
                // if pauze teller ++
                // if !pauze teller op de channel gooien
                    wait(sampleClock);
                    if(IR.get()){
                        n+=100;
                    }else if(!IR.get()){
                        p_listener.pause_detected(n);
                        S = STATE::SIGNAL;
                    }
                    break;


                // if !pauze, wacht 100 micro seconde
                // if pauze state = idle
                case STATE::SIGNAL:
                    if(!IR.get()){
                        wait(sampleClock);
                    }if(IR.get()){
                        n = 0;
                        S = STATE::IDLE;
                    }
                    break;
            }
        }
    }
public:
    pause_detector(
		const unsigned int priority,
        const char * name,
        hwlib::pin_in & IR,
        long long int delay,
        pause_listener & p

    ):
        task(priority, name ),
        IR( IR ),
        sampleClock( this, delay, "pause_detector" ),
        p_listener(p)
    {}
};

#endif
