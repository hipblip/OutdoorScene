#pragma once

#include <vector>
#include "GrassGenerator.h"
#include<ctime>

GrassGenerator::GrassGenerator() {
	srand(time(0));
}

GrassGenerator::~GrassGenerator() {}

vector<GrassModel> GrassGenerator::getGrass( int iCount ) {
	vector<GrassModel> grass;
	for( int i = 0; i < iCount; i++ ) {
		GrassModel grs;
		grass.push_back( grs );
	}
	return grass;
}