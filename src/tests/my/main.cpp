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
    std::ofstream resultOut("my.csv");
    resultOut << "Algorithm,Dimension,Error,Time\n";

    const std::vector<uint64_t> dimensions = {3, 5, 10, 20};
    const uint16_t iters = 10;
    const uint64_t numItems = 30;
    const uint64_t maxCapacity = 100;

    const std::vector<uint64_t> nums = {5, 10, 15};
    std::vector<std::string> algorithms;
    std::vector<SolverPtr> solvers;
    for (uint64_t numOfItemsForRecursion : nums) {
        algorithms.push_back("MY_L1_" + std::to_string(numOfItemsForRecursion));
        solvers.push_back(CreateMySolver(numOfItemsForRecursion, L1));
    }

    for (uint64_t dimension : dimensions) {
        std::vector<std::vector<double>> errors(algorithms.size(), std::vector<double>(iters));
        std::vector<std::vector<double>> times(algorithms.size(), std::vector<double>(iters));

        for (uint16_t i = 0; i < iters; ++i) {
            const Size capacity = GenerateCapacity(dimension, 1, maxCapacity);
            Items items = GenerateItems(numItems, 1, capacity);
            const uint64_t lowerBound = LowerBound(capacity, items);

            for (size_t j = 0; j < solvers.size(); ++j) {
                auto [result, time] = RunSolver(solvers[j], capacity, items);
                errors[j][i] = Error(lowerBound, result);
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
