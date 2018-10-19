#ifndef MSG_LOGGER_CPP
#define MSG_LOGGER_CPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "msg_decoder.cpp"

class msg_logger : public rtos::task<>, public msg_listener {

private:
  rtos::timer theTimer;
  rtos::channel< msg, 128 > messages;
  hwlib::window_part & w1;
  hwlib::window_part & w2;
  hwlib::window_ostream & d1;
  hwlib::window_ostream & d2;
  bool state = false; // false is idle, true is show
  msg receivedMessage;

public:
    void msg_received(msg ir_msg)override{

        //hwlib::cout << ">>msg_logger: message received\n";
        messages.write(ir_msg);
    }
    void clear(){
      w1.clear();
      w2.clear();
    }

    void show(msg m){
     //hwlib::cout << ">>msg_logger: the received message is: " << hwlib::bin << m.message[0] << ' ' << m.message[1] << ' ' << m.message[2] << ' ' << m.message[3] <<hwlib::endl;
	 
     d2 << "\f"
     << m.message[2] << "\n" << hwlib::flush;
    }
    msg_logger(hwlib::window_part & w1,
               hwlib::window_part & w2,
               hwlib::window_ostream & d2,
               hwlib::window_ostream & d1 ):
                      theTimer(this, "timer logger"),
                      messages( this, "messages" ),
                      w1(w1),
                      w2(w2),
                      d1(d1),
                      d2(d2)
    {
    }

    void main(){
		clear();
      while(true){
		  if(state){ // active
			//hwlib::cout << ">>msg_logger: reading\n";
			receivedMessage = messages.read();
			//hwlib::cout << ">>msg_logger: message received and printing\n";
			clear();
			show(receivedMessage); // bericht op het scherm zetten
			theTimer.set(5000); // checken of het scherm uit is
			auto evt = wait(theTimer + messages);
			if(evt == theTimer){ // wait 5 seconds if no new message has appeared
				//clear();
				state = false;
			}
			}else{ // idle
				//hwlib::cout << ">>msg_logger: listening\n";
				wait(messages);
				//hwlib::cout << ">>msg_logger: message received\n";
				state = true;
      }
    } // while
  } // main
}; // class msg_logger

#endif //
