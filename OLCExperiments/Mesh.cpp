#include "Mesh.h"

void Game::Mesh::Draw(Game& g)
{
	Mat4x4 rMat = Mat4x4::RotateXYZ(rotation);
	Mat4x4 tMat = Mat4x4::Translate(translation);
	Mat4x4 transfMat = Mat4x4::Multiply(rMat, tMat);
	Mat4x4 projection = Mat4x4::Perspective(g.GetFOV(), g.GetZNear(), g.GetZFar(), g.GetAspect());
	if (!indexed)
	{
		for (auto t : tris)
		{
			TriProject(g, transfMat, projection, t);
		}
	}
	else
	{
		for (auto it : indTri)
		{
			Triangle t = {
				indVerts[it.v[0]],
				indVerts[it.v[1]],
				indVerts[it.v[2]],
				it.c
			};
			TriProject(g, transfMat, projection, t);
		}
	}
}

void Game::Mesh::TriProject(Game& g, Mat4x4& transfMat, Mat4x4& projection, Triangle& t)
{
	Triangle transformTri, projectedTri;
	transformTri = transfMat.Transform(t);

	Vec3 normal = {
		((transformTri.v[1].y - transformTri.v[0].y) * (transformTri.v[2].z - transformTri.v[0].z)) - ((transformTri.v[1].z - transformTri.v[0].z) * (transformTri.v[2].y - transformTri.v[0].y)),
		((transformTri.v[1].z - transformTri.v[0].z) * (transformTri.v[2].x - transformTri.v[0].x)) - ((transformTri.v[1].x - transformTri.v[0].x) * (transformTri.v[2].z - transformTri.v[0].z)),
		((transformTri.v[1].x - transformTri.v[0].x) * (transformTri.v[2].y - transformTri.v[0].y)) - ((transformTri.v[1].y - transformTri.v[0].y) * (transformTri.v[2].x - transformTri.v[0].x))
	};

	float l = sqrtf((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));
	normal.x /= l;
	normal.y /= l;
	normal.z /= l;

	/*if (normal.x * (transformTri.v[0].x - g.camPos.x) +
		normal.y * (transformTri.v[0].y - g.camPos.y) +
		normal.z * (transformTri.v[0].z - g.camPos.z) > 0.0f)*/
	{

		projectedTri = projection.Transform(transformTri);

		projectedTri.v[0].x += 1.0f;
		projectedTri.v[0].y -= 1.0f;
		projectedTri.v[1].x += 1.0f;
		projectedTri.v[1].y -= 1.0f;
		projectedTri.v[2].x += 1.0f;
		projectedTri.v[2].y -= 1.0f;

		projectedTri.v[0].x *= 0.5f * g.ScreenWidth();
		projectedTri.v[0].y *= -0.5f * g.ScreenHeight();
		projectedTri.v[1].x *= 0.5f * g.ScreenWidth();
		projectedTri.v[1].y *= -0.5f * g.ScreenHeight();
		projectedTri.v[2].x *= 0.5f * g.ScreenWidth();
		projectedTri.v[2].y *= -0.5f * g.ScreenHeight();

		projectedTri.v[0].z = transformTri.v[0].z;
		projectedTri.v[1].z = transformTri.v[1].z;
		projectedTri.v[2].z = transformTri.v[2].z;

		projectedTri.c = t.c;

		/*g.DrawTriangle(projectedTri.v[0].x, projectedTri.v[0].y,
			projectedTri.v[1].x, projectedTri.v[1].y,
			projectedTri.v[2].x, projectedTri.v[2].y,
			olc::WHITE);*/
		//FillTriZ(g, projectedTri);

		g.tList.push_back(projectedTri);
	}

}

void Game::Mesh::FillTriZ(Game& g, Triangle& t)
{
	Triangle sortTri = t;
	std::sort(std::begin(sortTri.v), std::end(sortTri.v), [](Vec3 a, Vec3 b) {
		return a.y < b.y;
	});
	if (t.v[0].y == t.v[1].y)
	{
		FillTriZTop(g, sortTri);
	}
	else if (t.v[1].y == t.v[2].y)
	{
		FillTriZBottom(g, sortTri);
	}
	else
	{
		Vec3 v4 = {
			sortTri.v[0].x + ((sortTri.v[1].y - sortTri.v[0].y) / (sortTri.v[2].y - sortTri.v[0].y)) * (sortTri.v[2].x - sortTri.v[0].x),
			sortTri.v[1].y,
			sortTri.v[0].z + ((sortTri.v[1].y - sortTri.v[0].y) / (sortTri.v[2].y - sortTri.v[0].y)) * (sortTri.v[2].z - sortTri.v[0].z)
		};
		Triangle t1 = { sortTri.v[0],sortTri.v[1],v4,t.c };
		Triangle t2 = { sortTri.v[1],v4,sortTri.v[2],t.c };
		FillTriZBottom(g, t1);
		FillTriZTop(g, t2);
	}
}

void Game::Mesh::FillTriZBottom(Game& g, Triangle& t)
{
	float xslope1 = (t.v[1].x - t.v[0].x) / (t.v[1].y - t.v[0].y);
	float xslope2 = (t.v[2].x - t.v[0].x) / (t.v[2].y - t.v[0].y);

	float zslope1 = (t.v[1].z - t.v[0].z) / (t.v[1].y - t.v[0].y);
	float zslope2 = (t.v[2].z - t.v[0].z) / (t.v[2].y - t.v[0].y);


	float curx1 = t.v[0].x;
	float curx2 = t.v[0].x;

	float curz1 = t.v[0].z;
	float curz2 = t.v[0].z;


	for (int y = t.v[0].y; y <= t.v[1].y; y++)
	{
		float zslope3 = (curz2 - curz1) / (curx2 - curx1);
		float curz3 = curz1;
		if (curx1 < curx2)
		{
			for (int x = (int)curx1; x <= (int)curx2; x++)
			{
				if (x >= 0 && x < g.ScreenWidth())
				{
					if (curz3 < g.zBuf[x + (g.ScreenWidth() * y)])
					{
						g.Draw(x, y, t.c);
						g.zBuf[x + (g.ScreenWidth() * y)] = curz3;
					}
				}
				curz3 += zslope3;
			}
		}
		else
		{
			for (int x = (int)curx1; x >= (int)curx2; x--)
			{
				if (x >= 0 && x < g.ScreenWidth())
				{
					if (curz3 < g.zBuf[x + (g.ScreenWidth() * y)])
					{
						g.Draw(x, y, t.c);
						g.zBuf[x + (g.ScreenWidth() * y)] = curz3;
					}
				}
				curz3 -= zslope3;
			}
		}
		curx1 += xslope1;
		curx2 += xslope2;

		curz1 += zslope1;
		curz2 += zslope2;
	}
}

void Game::Mesh::FillTriZTop(Game& g, Triangle& t)
{
	float xslope1 = (t.v[2].x - t.v[0].x) / (t.v[2].y - t.v[0].y);
	float xslope2 = (t.v[2].x - t.v[1].x) / (t.v[2].y - t.v[1].y);

	float zslope1 = (t.v[2].z - t.v[0].z) / (t.v[2].y - t.v[0].y);
	float zslope2 = (t.v[2].z - t.v[1].z) / (t.v[2].y - t.v[1].y);


	float curx1 = t.v[2].x;
	float curx2 = t.v[2].x;

	float curz1 = t.v[2].z;
	float curz2 = t.v[2].z;


	for (int y = t.v[2].y; y >= t.v[1].y; y--)
	{
		float zslope3 = (curz2 - curz1) / (curx2 - curx1);
		float curz3 = curz1;
		if (curx1 < curx2)
		{
			for (int x = (int)curx1; x <= (int)curx2; x++)
			{
				if (x >= 0 && x < g.ScreenWidth())
				{
					if (curz3 < g.zBuf[x + (g.ScreenWidth() * y)])
					{
						g.Draw(x, y, t.c);
						g.zBuf[x + (g.ScreenWidth() * y)] = curz3;
					}
				}
				curz3 += zslope3;
			}
		}
		else
		{
			for (int x = (int)curx1; x >= (int)curx2; x--)
			{
				if (x >= 0 && x < g.ScreenWidth())
				{
					if (curz3 < g.zBuf[x + (g.ScreenWidth() * y)])
					{
						g.Draw(x, y, t.c);
						g.zBuf[x + (g.ScreenWidth() * y)] = curz3;
					}
				}
				curz3 -= zslope3;
			}
		}
		curx1 -= xslope1;
		curx2 -= xslope2;

		curz1 -= zslope1;
		curz2 -= zslope2;
	}
}
