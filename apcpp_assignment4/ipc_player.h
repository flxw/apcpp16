
#pragma once

#include <cstdint>


/**
 * @brief
 *		Fetches a score from shared storage.
 *
 * @param[in] playerID
 *		The unique player ID.
 *
 * @return
 *		The score corresponding to the player ID, or 0 if no such player exists.
 *
 * @exceptions
 *		Throws std::runtime_error to indicated failure.
 */
std::uint32_t readScore(std::uint32_t playerID);


/**
 * @brief
 *		Updates a score in the shared storage.
 *
 * @param[in] playerID
 *		The unique player ID.
 * @param[in] newScore
 *		The new score value to be written to shared storage.
 *
 * @exceptions
 *		Throws std::runtime_error to indicated failure.
 */
void updateScore(std::uint32_t playerID, std::uint32_t newScore);
