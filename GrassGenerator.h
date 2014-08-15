#pragma once

#include <vector>
#include "GrassModel.h"

using std::vector;

class GrassGenerator {

public:
	GrassGenerator();
	virtual ~GrassGenerator();

	vector<GrassModel> getGrass( int );
};
