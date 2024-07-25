#include "gen_utils.hpp"
#include "utils.hpp"

/**
 * @brief Returns 2 streams: for prompt input files and for solution input files.
 *
 * @note The caller is responsible for closing the file streams using its close() method.
 */
std::pair<std::ofstream, std::ofstream> setupTest(uint64_t testNumber) {
    std::ostringstream promptStream;
    promptStream << dirs.at("promptInputDirectory") << "/" << testNumber << ".in";
    std::string promptInPath = promptStream.str();

    std::ostringstream solutionStream;
    solutionStream << dirs.at("solutionInputDirectory") << "/" << testNumber << ".in";
    std::string solutionInPath = solutionStream.str();

    std::ofstream promptInFile(promptInPath);
    if (!promptInFile) {
        std::cerr << "Error: Could not open the file " << promptInPath << std::endl;
        exit(1);
    }
    std::ofstream solutionInFile(solutionInPath);
    if (!solutionInFile) {
        std::cerr << "Error: Could not open the file " << solutionInPath << std::endl;
        exit(1);
    }
    return {std::move(promptInFile), std::move(solutionInFile)};
}
