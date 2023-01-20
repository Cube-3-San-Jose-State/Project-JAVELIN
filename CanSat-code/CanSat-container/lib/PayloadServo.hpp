#include <Servo.h>
#include <Arduino.h>
namespace CanSat
{
    class PayloadServo {
        private:
            Servo servo;
            int position = 0;
            bool open = false;
        public:
            PayloadServo(int pin){
                servo.attach(pin);
                servo.write(0);
            }
            
            void ReleasePayload(){
                servo.write(180);
            }

            
    };
} 
