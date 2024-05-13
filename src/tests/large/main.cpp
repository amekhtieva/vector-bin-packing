#include "../../algorithms/best_fit.h"
#include "../../algorithms/bin_centric.h"
#include "../../algorithms/first_fit.h"
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
    std::ofstream resultOut("large.csv");
    resultOut << "Algorithm,Dimension,NumberOfItems,Time,Error,Improvement,Percentage\n";

    const std::vector<uint64_t> dimensions = {3, 5, 10, 20, 50, 100, 200};
    const uint16_t iters = 20;
    const std::vector<uint64_t> nums = {10, 50, 100, 200, 500, 1000};
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

    for (uint64_t dimension : dimensions) {
        for (uint64_t numItems : nums) {
            std::vector<std::vector<double>> times(algorithms.size(), std::vector<double>(iters));
            std::vector<std::vector<double>> errors(algorithms.size(), std::vector<double>(iters));
            std::vector<std::vector<double>> improvements(algorithms.size(), std::vector<double>(iters));
            std::vector<std::vector<double>> percentage(algorithms.size(), std::vector<double>(iters));

            for (uint16_t i = 0; i < iters; ++i) {
                const Size capacity = GenerateCapacity(dimension, 1, maxCapacity);
                Items items = GenerateItems(numItems, 1, capacity);

                const uint64_t lowerBound = LowerBound(capacity, items);
                const uint64_t upperBound = UpperBound(capacity, items);

                std::vector<uint64_t> results(algorithms.size());

                for (size_t j = 0; j < solvers.size(); ++j) {
                    auto [result, time] = RunSolver(solvers[j], capacity, items);
                    times[j][i] = time;
                    errors[j][i] = Error(lowerBound, result);
                    improvements[j][i] = Improvement(upperBound, result);
                    results[j] = result;
                }

                auto percentageIter = ToPercentage(results);
                for (size_t j = 0; j < solvers.size(); ++j) {
                    percentage[j][i] = percentageIter[j];
                }
            }

            for (size_t j = 0; j < solvers.size(); ++j) {
                const double averageTime = Average(times[j]);
                const double averageError = Average(errors[j]);
                const double averageImprovement = Average(improvements[j]);
                const double averagePercentage = Average(percentage[j]);

                resultOut << algorithms[j] << ","
                    << dimension << ","
                    << numItems << ","
                    << averageTime << ","
                    << averageError << ","
                    << averageImprovement << ","
                    << averagePercentage << "\n";
            }
        }
    }

    resultOut.close();
    return 0;
}
