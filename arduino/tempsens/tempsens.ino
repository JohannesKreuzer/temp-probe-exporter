#include <OneWire.h>
#include <DallasTemperature.h>

// Change these if appropriate
// The pin that sensors are connected to
#define ONE_WIRE_BUS 2
// What precision to set the sensor to
#define TEMPERATURE_PRECISION 11

OneWire one_wire(ONE_WIRE_BUS);
DallasTemperature sensors(&one_wire);

// We will store an array of probes we find on initial startup. If you have
// more than 20 probes, feel free to bump this number and see if it'll actually
// work at all.
DeviceAddress probes[40];
int num_probes = 0;

void setup() {
  Serial.begin(9600);
  sensors.begin();

  // Set all devices to the same resolution
  sensors.setResolution(TEMPERATURE_PRECISION);

  // Find our sensors, use num_probes as a handy counter.
  one_wire.reset_search();
  while (one_wire.search(probes[num_probes])) {
    num_probes++;
  }
  if (num_probes == 0){
    Serial.println("No Sensors found.");
  }
}

void print_address(DeviceAddress device_address) {
  // Device address is 8 bytes, iterate over them.

  char tBuf[30];
  sprintf(tBuf,"%02x-%02x%02x%02x%02x%02x%02x",device_address[0],device_address[6],device_address[5],device_address[4],device_address[3],device_address[2],device_address[1]);
  Serial.print(tBuf);
}

void print_reading(DeviceAddress device_address) {
  // Print out data in the form of ADDRESS:TEMPERATURE
  // This bit's annoying enough to get its own function
  print_address(device_address);
  Serial.print(":");
  Serial.print(sensors.getTempC(device_address));
  Serial.print('\n');
}

void loop() {
  // Make sure we're not just spitting out some constant meaningless numbers
  sensors.requestTemperatures();
   if (num_probes == 0){
    Serial.print("No Sensors found.\n");
  }
  for (int i = 0; i < num_probes; i++) {
    print_reading(probes[i]);
  }
}
