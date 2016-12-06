
#include "ipc_player.h"
#include "ipc_file.h"

std::uint32_t readScore(std::uint32_t playerID)
{
  return ipc_readScore(playerID);
}


void updateScore(std::uint32_t playerID, std::uint32_t newScore)
{
  return ipc_updateScore(playerID, newScore);
}
