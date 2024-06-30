#include <Mouse.h>

void setup() 
{
  Serial.begin(128000);
  Mouse.begin();
}

void loop() 
{
  if (Serial.available() > 0)
  {
    char deltaX = static_cast<char>(Serial.read());
    char deltaY = static_cast<char>(Serial.read());
    Mouse.move(deltaX, deltaY, 0);
  }
}
