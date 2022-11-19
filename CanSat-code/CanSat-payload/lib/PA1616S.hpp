#include <HardwareSerial.h>
#include <vector>
#include <string>
#include <map>
#define GPSSerial Serial7
using namespace std;
/*
    PA1616S - GPS Module:
        Public functions:
        initialize() - begins GPS serial port
        update() - monitor for GPS coordinate changes
        getLongitude() - gets longitude in GPS coordinates
        getLatitude() - gets latitude in GPS coordinates
*/
class PA1616S
{
    private:
        vector<string> GPGGAFormat = { "code", "time_stamp", "latitude", "latitude_direction", "longitude", "longitude_direction", "quality", "satellite_count", "hdop", "altitude", "altitude_units", "geoid_separation", "geoid_separation_units", "seconds_since_update", "reference_station", "checksum"};
        std::map<string, string> splitData;
        
        std::map<string, string> split(String data)
        {
            size_t position = data.indexOf(",");
            String term;
            int termCount = 0;

            while (position != -1)
            {
                if (position == 0) {
                    term = "null";
                }
                else {
                    term = data.substring(0, position);
                }

                splitData[GPGGAFormat[termCount]] = term.c_str();
                data = data.substring(position+1);		
                position = data.indexOf(",");
                termCount++;
            }
            return splitData;
        }

        double translate_latitude(string rawLatitude, string direction){
            //rawLatitude gives data in degree/minutes, as ddmm.mmmm, needs to be converted to GPS coordinates
            String nmeaLatitude = rawLatitude.c_str();
            if (nmeaLatitude.equals("")) return 0.00;
            int degrees = nmeaLatitude.substring(0, 2).toInt();
            double minutes = nmeaLatitude.substring(2).toFloat();

            double gpsLatitude = degrees + (minutes / 60);
            int directionMultiplier = (direction == "E") ? 1 : -1;
            
            return gpsLatitude*directionMultiplier;
        }

        double translate_longitude(string rawLongitude, string direction){
            String nmeaLongitude = rawLongitude.c_str();
            if (nmeaLongitude.equals("")) return 0.00;
            int degrees = nmeaLongitude.substring(0, 3).toInt();
            double minutes = nmeaLongitude.substring(3).toFloat();

            double gpsLongitude = degrees + (minutes/60);
            int directionMultiplier = (direction == "S") ? 1 : -1;
            
            return gpsLongitude*directionMultiplier;
        }
        
        bool pinged()
        {
            return (GPSSerial.available() > 0);
        };

    public:
        void initialize()
        {
            GPSSerial.begin(9600);
        };

        void update()
        {
            if (pinged()){
                String data = GPSSerial.readStringUntil('\n');
                if (data.substring(0, 6) == "$GPGGA")
                {
                    splitData = split(data);
                    if (splitData.size() < 6) return;
                }
            }
        }

        double getLongitude(){
            return translate_longitude(splitData["longitude"], splitData["longitude_direction"]);
        }

        double getLatitude(){
            return translate_latitude(splitData["latitude"], splitData["latitude_direction"]);
        }
};