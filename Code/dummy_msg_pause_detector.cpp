#include "rtos.hpp"
#include "hwlib.hpp"
#include "msg_decoder.cpp"

#ifndef DUMMY_PAUSE_DETECTOR_CPP
#define DUMMY_PAUSE_DETECTOR_CPP

class pause_detector: public rtos::task<>{
  private:
    pause_listener & p_listener;

  public:
    pause_detector(pause_listener & p):p_listener(p){}
    void main ()override{
      while(true){

        hwlib::wait_ms(5000);
        hwlib::cout << ">>pause_detector: Sending bytes\n";

        p_listener.pause_detected(4500); // start bit

        p_listener.pause_detected(600); // 0
        p_listener.pause_detected(700); // 0
        p_listener.pause_detected(1700);// 1
        p_listener.pause_detected(1700);// 1
        p_listener.pause_detected(600); // 0
        p_listener.pause_detected(700); // 0
        p_listener.pause_detected(1700);// 1
        p_listener.pause_detected(1700);// 1

        p_listener.pause_detected(600);
        p_listener.pause_detected(700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(600);
        p_listener.pause_detected(700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(1700);

        p_listener.pause_detected(600);
        p_listener.pause_detected(700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(600);
        p_listener.pause_detected(700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(1700);

        p_listener.pause_detected(600);
        p_listener.pause_detected(700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(600);
        p_listener.pause_detected(700);
        p_listener.pause_detected(1700);
        p_listener.pause_detected(1700);
		hwlib::wait_ms(50000);
      }
    }
};

#endif //
