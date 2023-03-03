#include <Servo.h>
#include <Arduino.h>
namespace CanSat
{
    class ParachuteServo {
        private:
            Servo servo;
            int position = 0;
            bool open = false;
        public:
            ParachuteServo(int pin){
                servo.attach(pin);
                servo.write(0);
            }
            
            void ReleaseParachute(){
                servo.write(180);
            }
    };
} 
