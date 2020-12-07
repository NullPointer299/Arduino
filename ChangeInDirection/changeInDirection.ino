#include <TinyGPS++.h>
TinyGPSPlus gps;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  changeInDirection();
}

void changeInDirection() {
  const double EIFFEL_TOWER_LAT = 48.85826;
  const double EIFFEL_TOWER_LNG = 2.294516;
  double distanceKm =
    gps.distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      EIFFEL_TOWER_LAT,
      EIFFEL_TOWER_LNG) / 1000.0;
  double courseTo =
    gps.courseTo(
      gps.location.lat(),
      gps.location.lng(),
      EIFFEL_TOWER_LAT,
      EIFFEL_TOWER_LNG);
  Serial.print("Distance (km) to Eiffel Tower: ");
  Serial.println(distanceKm);
  Serial.print("Course to Eiffel Tower: ");
  Serial.println(courseTo);
  Serial.print("Human directions: ");
  Serial.println(gps.cardinal(courseTo));
}
