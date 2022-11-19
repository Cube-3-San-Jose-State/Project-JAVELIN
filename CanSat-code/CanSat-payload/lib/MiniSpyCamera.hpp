#pragma once
namespace CanSat
{
    class MiniSpyCamera
    {
        /*Connect the red wire to 3.7V-5VDC and the black wire to ground. Then use the white wire to control the camera. When touched to ground for under about 100ms, the camera will take a photo and stick it in a PHOTO folder on the SD card, numbered from PIC000.jpg up to PIC999.jpg.*/
        private:
           int trig;
            public:
            MiniSpyCamera(int &trig_pin)
            {
               trig = trig_pin
            }
            public:
            MiniSpyCamera(int &trig_pin)
            {
               trig = trig_pin
            }
            

        void TakeImage(){
            digitalWrite(trig, LOW);   
            delay(50);               
            digitalWrite(trig, HIGH);   
            delay(1000); 
        }

        void TakeVideo(int video_length){
            digitalWrite(trig, HIGH);   
            delay(video_length);               
            digitalWrite(trig, LOW); 
            delay(1000);
        }


    };
}
