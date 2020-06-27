// ---------------------------------------------SETTINGS--------------------------------------------------------------------------------------

// Beacon Settings:
#define DEVICE_NAME               "Stapler 1 Beacon"
#define SERVICE_UUID              "7A0247E7-8E88-409B-A959-AB5092DDB03E"
#define BEACON_UUID_REV           "A134D0B2-1DA2-1BA7-C94C-E8E00C9F7A2D"
#define CHARACTERISTIC_UUID_TX    "13258BAA-1372-13E8-13BC-133D7ECD08A5"
#define CHARACTERISTIC_UUID_RX    "12258BAA-1272-12E8-12BC-123D7ECD08A5"

String UUID_Real   = "12345678-1000-2000";//-3000-400000000000
char BEACON_UUID[] = "12345678-1000-2000-0300-040000000000";
                     
int Major = 1;
int Minor = 3;
int ManufacturerId = 0x4C00; // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
int SignalPower = -60; //TX Power bei 1m Abstand -> gemessen vom Handy Locate

String Value = "";


// ---------------------------------------------------------------------------------------------------------------------------------------------