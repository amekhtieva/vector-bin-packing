#include "../../algorithms/best_fit.h"
#include "../../algorithms/bin_centric.h"
#include "../../algorithms/first_fit.h"
#include "../../algorithms/my.h"
#include "../../algorithms/next_fit.h"
#include "../../algorithms/worst_fit.h"

#include "../../library/bin.h"
#include "../../library/item.h"
#include "../../library/utils.h"

#include "../utils/utils.h"

#include <fstream>
#include <random>
#include <string>
#include <vector>

int main() {
    std::ofstream resultOut("small.csv");
    resultOut << "Algorithm,Dimension,Error,Time\n";

    const std::vector<uint64_t> dimensions = {3, 5, 10, 20, 50, 100, 200};
    const uint16_t iters = 20;
    const uint64_t minNumItems = 10;
    const uint64_t maxNumItems = 20;
    const uint64_t maxCapacity = 100;
    const std::vector<std::string> algorithms = {
        "BFD_L1",
        "BFD_L2",
        "BFD_LInf",
        "FFD_L1",
        "FFD_L2",
        "FFD_LInf",
        "NFD_L1",
        "NFD_L2",
        "NFD_LInf",
        "WFD_L1",
        "WFD_L2",
        "WFD_LInf",
        "BC_DP",
        "BC_L1",
        "BC_L2",
        "BC_LInf",
        "MY_L1",
        "MY_L2",
        "MY_LInf",
    };

    std::vector<SolverPtr> solvers;
    for (const auto& algorithm : algorithms) {
        solvers.push_back(CreateSolverByAlgorithmName(algorithm));
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(minNumItems, maxNumItems);

    for (uint64_t dimension : dimensions) {
        std::vector<std::vector<double>> errors(algorithms.size(), std::vector<double>(iters));
        std::vector<std::vector<double>> times(algorithms.size(), std::vector<double>(iters));

        for (uint16_t i = 0; i < iters; ++i) {
            const Size capacity = GenerateCapacity(dimension, 1, maxCapacity);
            Items items = GenerateItems(dist(gen), 1, capacity);
            const uint64_t optimal = Optimal(capacity, items);

            for (size_t j = 0; j < solvers.size(); ++j) {
                auto [result, time] = RunSolver(solvers[j], capacity, items);
                errors[j][i] = Error(optimal, result);
                times[j][i] = time;
            }
        }

        for (size_t j = 0; j < solvers.size(); ++j) {
            const double averageError = Average(errors[j]);
            const double averageTime = Average(times[j]);

            resultOut << algorithms[j] << ","
                << dimension << ","
                << averageError << ","
                << averageTime << "\n";
        }
    }

    resultOut.close();
    return 0;
}
