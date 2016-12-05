
#pragma once

#include <cstdint>
#include <map>


/**
 * @brief
 *		Fetches the entire high score list from shared storage.
 *
 * @param[in] block
 *		If true, the call should block until an update is available.
 *
 * @return
 *		An std::map with the player ID as key and score as value.
 *
 * @exceptions
 *		Throws std::runtime_error to indicated failure.
 */
std::map<std::uint32_t, std::uint32_t> readHighScore(bool block = true);
