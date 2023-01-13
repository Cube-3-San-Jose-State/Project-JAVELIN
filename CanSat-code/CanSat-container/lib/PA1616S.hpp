#include <HardwareSerial.h>
#include <vector>
#include <string>
#include <map>
#define GPSSerial Serial7
/*
    PA1616S - GPS Module:
        Public functions:
        initialize() - begins GPS serial port
        update() - monitor for GPS coordinate changes
        getLongitude() - gets longitude in GPS coordinates
        getLatitude() - gets latitude in GPS coordinates
*/

namespace CanSat{
    class PA1616S
    {
        private:
            std::vector<std::string> GPGGAFormat = { "code", "time_stamp", "latitude", "latitude_direction", "longitude", "longitude_direction", "quality", "satellite_count", "hdop", "altitude", "altitude_units", "geoid_separation", "geoid_separation_units", "seconds_since_update", "reference_station", "checksum"};
            std::map<std::string, std::string> splitData;

            const static int BUFFER_SIZE = 250;
            char buffer[BUFFER_SIZE];

            std::map<std::string, std::string> split(std::string data)
            {
                size_t position = data.find(",");
                std::string term;
                int termCount = 0;
                while (position != -1)
                {
                    if (position == 0) term = "null";
                    else term = data.substr(0, position);
                    splitData[GPGGAFormat[termCount]] = term;
                    data = data.substr(position+1);
                    position = data.find(",");
                    termCount++;
                }

                return splitData;
            }

            float translate_latitude(std::string nmeaLatitude, std::string direction){
                //rawLatitude gives data in degree/minutes, as ddmm.mmmm, needs to be converted to GPS coordinates
                if (nmeaLatitude.compare("") == 0) return 0.00;

                std::string stringDegrees = nmeaLatitude.substr(0, 2);
                int degrees = atoi(stringDegrees.c_str());
                const char* stringMinutes = nmeaLatitude.substr(2).c_str();
                float minutes = atof(stringMinutes);
                float gpsLatitude = degrees + (minutes / 60);
                int directionMultiplier = (direction == "N") ? 1 : -1;
                
                return gpsLatitude*directionMultiplier;
            }

            float translate_longitude(std::string nmeaLongitude, std::string direction){
                if (nmeaLongitude.compare("")  == 0) return 0.00;

                std::string stringDegrees = nmeaLongitude.substr(0, 3);
                int degrees = atoi(stringDegrees.c_str());
                const char* stringMinutes = nmeaLongitude.substr(3).c_str();
                float minutes = atof(stringMinutes);
                float gpsLongitude = degrees + (minutes/60);
                int directionMultiplier = (direction == "E") ? 1 : -1;
                
                return gpsLongitude*directionMultiplier;
            }

            std::string readData(){
                std::string data;
                int feed_length = GPSSerial.readBytesUntil('\n', buffer, BUFFER_SIZE);

                for(int i = 0; i < feed_length; i++){
                    data += buffer[i];
                }

                return data;
            }

        public:
            void Initialize()
            {
                GPSSerial.begin(9600);
            };

            void Update()
            {
                std::string data = readData();
                if (data.length() >= 6){
                    if (data.substr(0, 6) == "$GPGGA")
                    {
                        splitData = split(data);
                    }
                    return;
                }
            }

            float GetLongitude(){
                return translate_longitude(splitData["longitude"], splitData["longitude_direction"]);
            }

            float GetLatitude(){
                return translate_latitude(splitData["latitude"], splitData["latitude_direction"]);
            }
    };
}