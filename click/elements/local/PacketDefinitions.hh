#ifndef CLICK_PACKEDDEFINITIONS_HH
#define CLICK_PACKEDDEFINITIONS_HH
#include <click/element.hh>

//Recordstruct for the client membership reports
struct Record{
  uint8_t RecordType;
  uint8_t AuxDataLen;
  //Numsources will be default 0 since there is only one source 
  uint16_t NumSources;
  IPAddress MulticastAddress;
  //Source is not used here since we only have one source
  IPAddress source;
};

//Messagestruct for the client membership reports
struct MulticastMessage{
  uint8_t Type;
  uint8_t Reserved1;
  uint16_t Checksum;
  uint16_t Reserved2;
  uint16_t NumRecords;
  Record record;
};

//Querystruct for the router membership query
struct MulticastQuery{
  uint8_t Type;
  uint8_t MRC;
  uint16_t Checksum;
  IPAddress GroupAddress;
  int Reserved:4;
  bool Suppress;
  int QRV:3;
  uint8_t QQIC;
  //NumSources is default 1 because there is only 1 source
  uint16_t NumSources;
  IPAddress Source;
};

#endif
