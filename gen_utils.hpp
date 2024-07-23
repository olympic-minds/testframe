#ifndef GEN_UTILS_H_
#define GEN_UTILS_H_

#include <fstream>

/**
 * @brief Returns 2 streams: for prompt input files and for solution input files.
 *
 * @note The caller is responsible for closing the file streams using its close() method.
 */
std::pair<std::ofstream, std::ofstream> setupTest(uint64_t testNumber);

#endif
