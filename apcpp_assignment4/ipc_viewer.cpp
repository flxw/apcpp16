
#include "ipc_viewer.h"
#include "ipc_file.h"


std::map<std::uint32_t, std::uint32_t> readHighScore(bool block)
{
	return ipc_readHighScore(block);
}
