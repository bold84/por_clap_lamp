#ifndef _CLAP_DETECTOR_H_
#define _CLAP_DETECTOR_H_


class ClapDetector
{
public:
  ClapDetector(int soundSensorPin);
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
  

  
  int getClapCount();

  void clapCallback(uint gpio, uint32_t events);
  
  
private:

  int soundSensorPin_ { 0 };
  int clapDebounceTime_ { 100 }; // 100 ms
  int clapSequenceTimeout_ { 1000 }; // 1 second
  uint32_t lastClapTime_ { 0 };
  int clapCount_ { 0 };
  
  
  
  
};


#endif // _CLAP_DETECTOR_H_