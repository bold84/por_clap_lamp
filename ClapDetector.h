#ifndef _CLAP_DETECTOR_H_
#define _CLAP_DETECTOR_H_

/**
 * @brief Class for detecting claps.
 * 
 * This class is used to detect claps. 
 * 
 */
class ClapDetector
{
public:
  /**
   * @brief Constructor.
   * 
   * @param soundSensorPin The GPIO pin number for the sound sensor.
   */
  ClapDetector(int soundSensorPin);
  
  /**
   * @brief Destructor.
   * 
   */
  ~ClapDetector() = default;
  
  /**
   * @brief Get the clap sequence timeout.
   * 
   * This method is used to get the clap sequence timeout.
   * 
   * @return int The clap sequence timeout.
   */
  int getClapSequenceTimeout() const;
  
  /**
   * @brief Set the clap sequence timeout.
   * 
   * This method is used to set the clap sequence timeout.
   * 
   * @param clapSequenceTimeout The clap sequence timeout.
   */
  void setClapSequenceTimeout(int clapSequenceTimeout);
  
  /**
   * @brief Get the clap count.
   * 
   * This method is used to get the clap count.
   * 
   * @return int The clap count.
   */
  int getClapCount();

  /**
   * @brief Clap callback.
   * 
   * This method is used to handle the clap callback.
   * 
   * @param gpio The GPIO pin number.
   * @param events The GPIO events.
   */
  void clapCallback(uint gpio, uint32_t events);
  
  
private:

  int soundSensorPin_ { 0 };                      // The GPIO pin that is connected to the sound sensor.
  int clapDebounceTime_ { 100 };                  // 100 ms
  int clapSequenceTimeout_ { 1000 };              // 1 second
  uint32_t lastClapTime_ { 0 };                   // The time of the last clap.
  int clapCount_ { 0 };                           // The number of claps.

};

#endif // _CLAP_DETECTOR_H_