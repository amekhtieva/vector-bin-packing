#pragma once

#include "../../algorithms/solver.h"

#include "../../library/bin.h"
#include "../../library/item.h"

#include <string>
#include <vector>
#include <utility>

uint64_t LowerBound(const Size& capacity, const Items& items);
uint64_t Optimal(const Size& capacity, Items items);
uint64_t UpperBound(const Size& capacity, const Items& items);

Size GenerateCapacity(uint64_t dimension, uint64_t min, uint64_t max);
Items GenerateItems(uint64_t num, uint64_t min, const Size& capacity);
Items GenerateTriplets(uint64_t numberOfTriplets, const Size& capacity);

std::pair<uint64_t, double> RunSolver(
    SolverPtr solver,
    const Size& capacity,
    const Items& items
);

double Error(uint64_t lowerBoundOrOptimal, uint64_t algorithmResult);
double Improvement(uint64_t upperBound, uint64_t algorithmResult);
std::vector<double> ToPercentage(std::vector<uint64_t> algorithmResults);

double Average(std::vector<double> elements);

SolverPtr CreateSolverByAlgorithmName(const std::string& algorithmName);
