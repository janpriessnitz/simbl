//
// Created by jp on 09.12.20.
//

#ifndef SIMBL_MAPREADER_H
#define SIMBL_MAPREADER_H


#include "TupleReader.h"

class MapReader : public TupleReader {
public:
  explicit MapReader(const std::string &filename);

  std::string GetString(const std::string &key);
  long long int GetInt(const std::string &key);
  double GetFloat(const std::string &key);

  std::map<std::string, std::string> data_map;
};


#endif//SIMBL_MAPREADER_H
