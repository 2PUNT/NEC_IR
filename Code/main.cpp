#include "hwlib.hpp"
#include "rtos.hpp"

#include "msg_pause_detector.cpp"
#include "msg_decoder.cpp"
#include "msg_logger.cpp"

int main( void ){

   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;

   namespace target = hwlib::target;
   
   auto IR = target::pin_in( target::pins::d6 );
   auto gnd = target::pin_out( target::pins::d5 );
   auto vcc = target::pin_out( target::pins::d4 );
   gnd.set(0);
   vcc.set(1);

   

  auto scl     = hwlib::target::pin_oc(  hwlib::target::pins::scl );
         auto sda     = hwlib::target::pin_oc(  hwlib::target::pins::sda );
         auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
         auto oled    = hwlib::glcd_oled( i2c_bus, 0x3c );

         auto w1 = hwlib::window_part(
            oled,
            hwlib::location( 0, 0 ),
            hwlib::location( 128, 32));
        auto w2 = hwlib::window_part(
            oled,
            hwlib::location( 0, 32 ),
            hwlib::location( 128, 32));
        auto f1 = hwlib::font_default_8x8();
        auto d1 = hwlib::window_ostream( w1, f1 );

        auto f2 = hwlib::font_default_8x8();
        auto d2 = hwlib::window_ostream( w2, f2 );
        
        auto msglog = msg_logger(w1, w2, d1, d2);
        auto msgdec = msg_decoder(msglog);
        auto pause_detec = pause_detector(0, "IR-Sensor", IR, 100, msgdec);
        //auto pause = pause_detector(msgdec);
   rtos::run();

}
