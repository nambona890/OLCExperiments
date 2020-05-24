#pragma once
#include "olcPixelGameEngine.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <execution>
#include <fstream>

struct Vec3
{
	float x = 0, y = 0, z = 0;
};

struct Triangle
{
	Vec3 v[3];
	olc::Pixel c = olc::WHITE;
};

struct IndexedTri
{
	unsigned int v[3];
	olc::Pixel c = olc::WHITE;
};