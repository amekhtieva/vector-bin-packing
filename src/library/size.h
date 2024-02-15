#pragma once

#include <vector>

using Size = std::vector<uint64_t>;

bool operator<(const Size& lhs, const Size& rhs);
bool operator<=(const Size& lhs, const Size& rhs);

Size operator+(const Size& lhs, const Size& rhs);
Size& operator+=(Size& lhs, const Size& rhs);

Size operator-(const Size& lhs, const Size& rhs);
Size& operator-=(Size& lhs, const Size& rhs);

Size operator*(const Size& lhs, const Size& rhs);
Size& operator*=(Size& lhs, const Size& rhs);

uint64_t SumOfElements(const Size& vec);
uint64_t ProductOfElements(const Size& vec);

uint64_t DotProduct(const Size& lhs, const Size& rhs);
Size Square(const Size& vec);
