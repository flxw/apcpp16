
#include <stdlib.h>
#include <string.h>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <map>
#include <iostream>
#include <iterator>

//boost::interprocess::file_mapping m_file("/tmp/scorefile.txt", boost::interprocess::read_write);
//boost::interprocess::mapped_region m_region(m_file, read_write);
static boost::interprocess::managed_mapped_file m_file(boost::interprocess::open_or_create, "scorefile", 65536);

std::uint32_t ipc_readScore(std::uint32_t playerID) {
  std::string playerIdString = std::to_string(playerID);
  return *(m_file.find<std::uint32_t>(playerIdString.c_str()).first);
}

void ipc_updateScore(std::uint32_t playerID, std::uint32_t newScore)
{
  std::string playerIdString = std::to_string(playerID);
  m_file.destroy<int>(playerIdString.c_str());
  m_file.construct<std::uint32_t>(playerIdString.c_str())(newScore);
  m_file.flush();
}

std::map<std::uint32_t, std::uint32_t> ipc_readHighScore(bool block) {
  std::map<std::uint32_t, std::uint32_t> ret;

  for (auto it = m_file.named_begin(); it != m_file.named_end(); ++it) {
    ret[atoi((const char*)it->name())] = *(int*) it->value();
  }

  return ret;
}
