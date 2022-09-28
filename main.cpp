
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "TriacDimmer.h"
#include "date.h"

#define ZEROCROSS_PIN 16
#define PSM_PIN 17
#define SOUND_SENSOR_PIN 15


#define CLAP_TIMEOUT_MS 1000 // 1 second

volatile static bool zero_cross = false;
static char event_str[128];
static int clap_count = 0;

static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}

// void zc_callback(uint gpio, uint32_t events)
// {
//   // gpio_event_string(event_str, events);
//   // printf("GPIO %d %s\n", gpio, event_str);
//   zero_cross = true;  
// }

// void clap_callback(uint gpio, uint32_t events)
// {
//   printf("Clap no.: %d\n", ++clap_count);
// }



int main(int argc, char **argv) 
{
  stdio_init_all();
    // while (true) {
    //     std::cout << "Hello, world!" << std::endl;
    //     sleep_ms(1000);
    // }
    
    TriacDimmer dimmer(ZEROCROSS_PIN, PSM_PIN);
    
    
    
    // std::cout << "Hello GPIO IRQ!" << std::endl;
    
    // gpio_init(PSM_PIN);
    // gpio_set_dir(PSM_PIN, GPIO_OUT);
    
    // gpio_set_irq_enabled_with_callback(ZEROCROSS_PIN, GPIO_IRQ_EDGE_RISE, true, &zc_callback);
    
    // gpio_set_irq_enabled_with_callback(SOUND_SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true, &clap_callback);
    
    
    int dim = 200;
    bool direction = false;
    int step = 5;
    
    sleep_ms(5000);
    std::cout << "Starting dimming..." << std::endl;

    auto lastIncrease = get_absolute_time();
    
    while(1)
    {

      dimmer.dim();
      
      // if(direction)
      // {
      //   dimmer.setPsmDuration(dimmer.getPsmDuration() + step);
      //   if(dimmer.getPsmDuration() >= MAX_PSM_DURATION)
      //   {
      //     direction = false;
      //     std::cout << "Dimming down" << std::endl;
      //   }
      // }
      // else
      // {
      //   dimmer.setPsmDuration(dimmer.getPsmDuration() - step);
      //   if(dimmer.getPsmDuration() <= MIN_PSM_DURATION)
      //   {
      //     direction = true;
      //     std::cout << "Dimming up" << std::endl;
      //   }
      
      // }
      
      
      if(absolute_time_diff_us(delayed_by_ms(lastIncrease, 1000), get_absolute_time()) > 1000000)
      {
        //dimmer.setPsmDuration(dimmer.getPsmDuration() + 200);
        dimmer.setDimPercentage(dimmer.getDimPercentage() + 5);
        lastIncrease = get_absolute_time();
        std::cout << "Dimming up - " << dimmer.getPsmDuration() << std::endl;
      }

      
      
      // if(zero_cross)
      // {
      //   // zero_cross = false;
      //   // printf("Dim: %d", dim);
      //   // sleep_us(dim);
      //   // gpio_put(PSM_PIN, 1);
      //   // sleep_us(8.333);
      //   // gpio_put(PSM_PIN, 0);
        
      //   // if(direction)
      //   // {
      //   //   dim += step;
      //   //   if(dim >= MAX_PSM_DURATION)
      //   //   {
      //   //     direction = false;
      //   //   }
      //   // }
      //   // else
      //   // {
      //   //   dim -= step;
      //   //   if(dim < MIN_PSM_DURATION)
      //   //   {
      //   //     direction = true;
      //   //   }
      //   // }
        
        
        
      // }
    }

  return 0;
}