#ifndef MSG_DECODER_CPP
#define MSG_DECODER_CPP

#include "hwlib.hpp"
#include "rtos.hpp"

struct msg {
  uint8_t message [4] = {0, 0, 0, 0};
  bool check(){
	  //hwlib::cout << ">>>msg::check: the values to check are " << hwlib::bin << message[0] << ", " << message[1] << " and " << message[2] << ", " << message[3] << '\n';
	  uint8_t messageinverted1 = ~message[1];
	  uint8_t messageinverted3 = ~message[3];
	return(
		message[0] == messageinverted1 && message[2] == messageinverted3 // should be inverted, but isn't due to hardware
    );
  }
  void clear(){
    for(unsigned int i = 0; i < 4; i++){
      message[i]= 0;
    }
  }
};

inline msg& operator <<(msg & lhs, int position){ // using the same counter as
  int diff = position % 8;
  int row =  position / 8;
  //hwlib::cout << ">>msg: adding bit at " << row << ' ' << diff << '\n';
  lhs.message[row] =  lhs.message[row] | ( 1 << diff);
  return lhs;
}

class msg_listener {
  public:
    virtual void msg_received(msg ir_msg) = 0;
};

class pause_listener {
  public:
    virtual void pause_detected(int n) = 0;
};

class msg_decoder : public rtos::task<>, public pause_listener {
private:
  msg_listener & listener;
  bool state = 0; // 0 is idle 1 is message
  unsigned int n =0; // received bit count
  msg m;
  int p; // length pause
  rtos::channel< int, 2048 > pauses;
  bool check(){
    return m.check();
  }
  
public:
  msg_decoder(msg_listener & listener):listener(listener), pauses( this, "pauses" ){}
    void main(){
      while(true){
		  //hwlib::cout << ">msg_decoder: Listnening\n";
          wait(pauses);
		  //hwlib::cout << ">msg_decoder: pause received: ";
          p = pauses.read();
		  //hwlib::cout << p << ' ';
          if(state){
            if(p > 200 && p < 2000){
              n++;
              if( p > 1000){ // positief
				//hwlib::cout << ">it is positive\n";
                m << n-1;
              }//else hwlib::cout << ">it is negative\n";
              if(n == 32){ // TODO: make interface for decoder
				//hwlib::cout << ">>msg_decoder: compleet message received, checking and sending message\n";
                state = 0;
                if(check()){
					//hwlib::cout << ">>msg_decoder: Check returned true\n";
					listener.msg_received(m);
                }//else hwlib::cout << ">>>>msg_decoder: Check returned false!!\n";
              }
            }
            else{
                state = 0;
            }
          }else{
            if(p > 4000 && p < 5000){
			  //hwlib::cout << ">>msg_decoder: it is a startPulse\n";
              state = 1;
              n = 0;
              m.clear();
            }
          }
          }
       }
	   
   void pause_detected(int n)override{
     //hwlib::cout << n;
     pauses.write(n);
   }


};

#endif
