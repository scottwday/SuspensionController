
void setup() 
{
  setupComms();
  setupHeightSensors();
  setupRelays();
  setupDisplay();
}

void loop() {
  updateComms();

  //Read sensors and send values over UART
  updateHeightSensors();

  updateRelays();
  
  // Update screen
  updateDisplay();
  
  // Slow down
  delay(1);
}




