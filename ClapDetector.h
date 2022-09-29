#ifndef _CLAP_DETECTOR_H_
#define _CLAP_DETECTOR_H_


class ClapDetector
{
public:
  ClapDetector(int soundSensorPin);
  ~ClapDetector() = default;
  
  
private:

  int soundSensorPin_ { 0 };
  
  void clapCallback(uint gpio, uint32_t events)
  {
    
  }
  
};


#endif // _CLAP_DETECTOR_H_