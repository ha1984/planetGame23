#pragma once
#include <vector>

struct bone {
	std::vector<float> weights;
	std::vector<int> indices;
};