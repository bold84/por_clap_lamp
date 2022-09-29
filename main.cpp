
#include <iostream>           // for cout

#include "pico/stdlib.h"      // for sleep_ms, etc.
#include "hardware/gpio.h"    // for gpio_set_irq_enabled_with_callback, etc.

#include "TriacDimmer.h"      // for TriacDimmer class definition
#include "ClapDetector.h"     // for ClapDetector class definition

#define ZEROCROSS_PIN 16                              // GPIO16, connected to the zero cross detector (ZCD) output.
#define PSM_PIN 17                                    // GPIO17, connected to the PSM input of the triac.
#define SOUND_SENSOR_PIN 15                           // GPIO15, connected to the sound sensor output.

static TriacDimmer dimmer(PSM_PIN);                   // Create a TriacDimmer object.

static ClapDetector clapDetector(SOUND_SENSOR_PIN);   // Create a ClapDetector object.

/**
 * @brief IRQ callback function.
 * 
 * This function is called when an interrupt occurs for either
 * the zero cross pin or the sound sensor pin.
 * 
 * @param gpio The GPIO pin number.
 * @param events The GPIO events.
 */
void gpioCallback(uint gpio, uint32_t events)
{
  if(gpio == ZEROCROSS_PIN)
  {
    dimmer.zeroCrossCallback(gpio, events);
  }
  else if(gpio == SOUND_SENSOR_PIN)
  {
    clapDetector.clapCallback(gpio, events);
  }
}

int main(int argc, char **argv)
{
  stdio_init_all();
  
  // Set the GPIO pin for the zero cross detector and the IRQ callback function.
  gpio_set_irq_enabled_with_callback(ZEROCROSS_PIN, GPIO_IRQ_EDGE_RISE, true, gpioCallback);
  
  // Set the GPIO pin for the sound sensor 
  gpio_set_irq_enabled(SOUND_SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true);

  // Loop forever.  
  while (1)
  {    
    // Dim the light.
    dimmer.dim();
    
    // Check if the clap detector has detected a clap.
    const auto clapCount = clapDetector.getClapCount();
    if(clapCount > 0)
    {
      std::cout << "Clap detected! Number of claps: " << clapCount << std::endl;
      
      // If the clap count is 2, then increase the brightness. If the clap count is 3, then
      // decrease the brightness. If the clap count is 4, then turn off the light.
      if(clapCount == 2)
      {
        std::cout << "Increase dimming..." << std::endl;
        dimmer.increaseDimPercentage();
      }
      else if(clapCount == 3)
      {
        std::cout << "Decrease dimming..." << std::endl;
        dimmer.decreaseDimPercentage();
      }
      else if(clapCount == 4)
      {
        // Turn off the light.
        std::cout << "Turn the light on/off..." << std::endl;
        dimmer.setPowerState(!dimmer.getPowerState());
      }
    }
  }
  return 0;
}
