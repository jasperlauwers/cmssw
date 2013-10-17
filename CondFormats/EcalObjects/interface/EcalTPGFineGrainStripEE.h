#ifndef EcalTPGFineGrainStripEE_h
#define EcalTPGFineGrainStripEE_h

#include <map>
#include <boost/cstdint.hpp>

class EcalTPGFineGrainStripEE 
{
 public:
  EcalTPGFineGrainStripEE() ;
  ~EcalTPGFineGrainStripEE() ;

  struct Item 
  {
    uint32_t threshold ;
    uint32_t lut ;
  };

  const std::map<uint32_t, Item> & getMap() const { return map_; }
  void  setValue(const uint32_t & id, const Item & value) ;

 private:
  std::map<uint32_t, Item> map_ ;

};

typedef std::map<uint32_t, EcalTPGFineGrainStripEE::Item>                 EcalTPGFineGrainStripEEMap;
typedef std::map<uint32_t, EcalTPGFineGrainStripEE::Item>::const_iterator EcalTPGFineGrainStripEEMapIterator;

#endif