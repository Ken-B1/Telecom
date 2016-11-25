#ifndef CLICK_PACKEDDEFINITIONS_HH
#define CLICK_PACKEDDEFINITIONS_HH
#include <click/element.hh>

struct Record{
  uint8_t RecordType;
  uint8_t AuxDataLen;
  uint16_t NumSources;
  IPAddress MulticastAddress;
  //Source is not used here since we only have one source
  IPAddress source;
};


struct MulticastMessage{
  uint8_t Type;
  uint8_t Reserved1;
  uint16_t Checksum;
  uint16_t Reserved2;
  uint16_t NumRecords;
  Record record;
};

#endif
