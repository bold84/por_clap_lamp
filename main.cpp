
#include <iostream>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include "TriacDimmer.h"
#include "ClapDetector.h"

#define ZEROCROSS_PIN 16
#define PSM_PIN 17
#define SOUND_SENSOR_PIN 15

#define CLAP_TIMEOUT_MS 1000 // 1 second
#define CLAP_MAX_DURATION_MS 100 // 100 ms

void clapCallback(uint gpio, uint32_t events)
{

  
  
}

int main(int argc, char **argv)
{
  stdio_init_all();

  TriacDimmer dimmer(ZEROCROSS_PIN, PSM_PIN);
  dimmer.setPowerState(true);
  ClapDetector clapDetector(SOUND_SENSOR_PIN);

  //gpio_set_irq_enabled_with_callback(SOUND_SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true, &clapCallback);

  sleep_ms(5000);
  std::cout << "Starting dimming..." << std::endl;

  auto lastIncrease = get_absolute_time();
  while (1)
  {

    dimmer.dim();
    if (absolute_time_diff_us(delayed_by_ms(lastIncrease, 1000), get_absolute_time()) > 1000000)
    {
      dimmer.setDimPercentage(dimmer.getDimPercentage() + 5);
      lastIncrease = get_absolute_time();
      std::cout << "Dimming up - " << dimmer.getPsmDuration() << std::endl;
    }
    
    // Check if the clap detector has detected a clap.
    // const auto clapCount = clapDetector.getClapCount();
    // if(clapCount > 0)
    // {
    //   std::cout << "Clap detected! Number of claps: " << clapCount << std::endl;
      
    //   // If the clap count is 2, then increase the brightness. If the clap count is 3, then
    //   // decrease the brightness. If the clap count is 4, then turn off the light.
    //   if(clapCount == 2)
    //   {
    //     std::cout << "Increase dimming..." << std::endl;
    //     dimmer.increaseDimPercentage();
    //   }
    //   else if(clapCount == 3)
    //   {
    //     std::cout << "Decrease dimming..." << std::endl;
    //     dimmer.decreaseDimPercentage();
    //   }
    //   else if(clapCount == 4)
    //   {
    //     // Turn off the light.
    //     std::cout << "Turn the light on/off..." << std::endl;
    //     dimmer.setPowerState(!dimmer.getPowerState());
    //   }
    // }

  }

  return 0;
}
