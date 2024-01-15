//
// Created by jp on 09.12.20.
//

#include "TupleReader.h"
#include <cassert>
#include <cstring>

TupleReader::TupleReader(const std::string &filename) : filename_(filename) {
  FILE *fp = fopen(filename.c_str(), "r");
//  printf("Opening %s\n", filename.c_str());
  if (fp == nullptr) {
    fprintf(stderr, "Error when opening config file \"%s\"\n", filename.c_str());
    exit(1);
  }

  size_t bufferLen = 0;
  ssize_t stringLen;
  char* buffer = nullptr;
  while ((stringLen = getline(&buffer, &bufferLen, fp)) != -1) {
    if (buffer[0] == '#') continue;

    std::vector<std::string> row;

    buffer[stringLen - 1] = '\0';  // convert \n to \0
    char* token;
    char* buffer_helper = buffer;
    while ((token = strtok(buffer_helper, " \t\n")) != nullptr) {
      row.emplace_back(token);
      buffer_helper = nullptr;
    }
    data.push_back(row);
  }

  free(buffer);
  fclose(fp);
}
std::map<std::string, std::string> TupleReader::GetPairMap() {
  std::map<std::string, std::string> result;
  for ( auto &r : data) {
    if (r.size() < 2) continue;
    result[r[0]] = r[1];
  }
  return result;
}

long long TupleReader::GetInt(size_t r, size_t c) {
  return strtoll(data[r][c].c_str(), nullptr, 10);
}
double TupleReader::GetDouble(size_t r, size_t c) {
  return strtod(data[r][c].c_str(), nullptr);
}
