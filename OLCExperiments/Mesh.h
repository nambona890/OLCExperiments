#pragma once
#include <Game.h>
#include <Matrix.h>

class Game::Mesh
{
protected:
	std::vector<Triangle> tris;
	std::vector<Vec3> indVerts;
	std::vector<IndexedTri> indTri;
	bool indexed = false;
public:
	Vec3 rotation;
	Vec3 translation;
public:
	Mesh() = default;
	void Draw(Game& g);
	Triangle TriProject(Game& g, Mat4x4& transfMat, Mat4x4& projection, Triangle& t);
	void FillTriZ(Game& g, Triangle& t);
	void FillTriZBottom(Game& g, Triangle& t);
	void FillTriZTop(Game& g, Triangle& t);
};