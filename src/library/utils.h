#pragma once

#include "bin.h"
#include "item.h"

#include <optional>

using VolumeFunction = std::function<uint64_t(const Size&)>;
using FitFunction = std::function<uint64_t(const Item& item, const Bin& bin)>;

void SortBy(Items& items, VolumeFunction volume);

// different fit functions can both increase and decrease with increasing suitability of item and bin,
// so it is necessary to have both maximize and minimize functions to find the most suitable item for the bin

std::optional<size_t> MaximizeFitFunction(const Items& items, const Bin& bin, FitFunction fit);
std::optional<size_t> MinimizeFitFunction(const Items& items, const Bin& bin, FitFunction fit);

std::optional<size_t> MaximizeFitFunction(const Item& item, const Bins& bins, FitFunction fit);
std::optional<size_t> MinimizeFitFunction(const Item& item, const Bins& bins, FitFunction fit);
