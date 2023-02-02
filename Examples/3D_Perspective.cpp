#define GECKOO_ENGINE_APP
#include "LiteGameEngineProto.h"
#include <strstream>
#include <fstream>

using namespace std;

struct vf3d {
	float x, y, z;
};

struct triangle {
	vf3d p[3];
	gck::Pixel col;
};

struct mesh {
	vector<triangle> tris;

	bool LoadFromObjectFile(string sFilename)
	{
		ifstream f(sFilename);

		if (!f.is_open())
			return false;

		vector<vf3d> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				vf3d v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
			}
		}

		return true;
	}
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

class GameEngine3D : public gck::xGameEngine
{
public:
	GameEngine3D()
	{
		sAppName = "3D_Perspective";
	}

private:
	mesh meshObject;
	mat4x4 matProj;
	vf3d vCamera;
	float fTheta;

	void MultiplyMatrixVector(vf3d& i, vf3d& o, mat4x4& m)
	{
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f)
			o.x /= w; o.y /= w; o.z /= w;
	}
	// red light
	gck::Pixel GetColour(float lum)
	{
		gck::Pixel colour = gck::Pixel(222 * lum, 77 * lum, 77 * lum, 255);
		return colour;
	}

public:
	bool appCreate() override
	{
		meshObject.LoadFromObjectFile("ico.obj");

		float fNear = 0.1f;
		float fFar = 100.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * gck::PI);

		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 0.33f; // as distance
		matProj.m[3][3] = 0.0f;

		return true;
	}

	bool appUpdate(float fElapsedTime) override
	{
		if (GetKey(gck::ESCAPE).bPressed)
			return false;

		Clear(gck::BLACK);
		mat4x4 matRotZ, matRotX;
		fTheta += 0.5f * fElapsedTime;
		// rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;
		// rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;
		// store triangles for rastering later
		std::vector<triangle> vecTrianglesToRaster;
		gck::Pixel c;
		// draw triangles
		for (auto tri : meshObject.tris)
		{
			triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;
			// rotate in Z-Axis
			MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matRotZ);
			// rotate in X-Axis
			MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);
			// offset into the screen
			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 5.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 5.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 5.0f;
			// use Cross-Product to get surface normal
			vf3d normal, line1, line2;
			line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
			line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
			line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

			line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
			line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
			line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;
			// normalise the normal
			float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
			normal.x /= l; normal.y /= l; normal.z /= l;

			if (normal.x * (triTranslated.p[0].x - vCamera.x) + normal.y * (triTranslated.p[0].y - vCamera.y) + normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0f)
			{	// illumination
				vf3d light_direction = { 0.0f, 0.0f, -1.0f };
				float l = sqrtf(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
				light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;
				// how similar is normal to light direction
				float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;
				// choose console colours as required (much easier with RGB)				
				triTranslated.col = GetColour(dp);
				// project triangles from 3D --> 2D
				MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matProj);
				MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matProj);
				MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matProj);
				triProjected.col = triTranslated.col;
				// scale into view
				triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
				triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
				triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
				triProjected.p[0].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[0].y *= 0.5f * (float)ScreenHeight();
				triProjected.p[1].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[1].y *= 0.5f * (float)ScreenHeight();
				triProjected.p[2].x *= 0.5f * (float)ScreenWidth();
				triProjected.p[2].y *= 0.5f * (float)ScreenHeight();
				// store triangle for sorting
				vecTrianglesToRaster.push_back(triProjected);
			}
		}
		// sort triangles according to Z
		sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
			{
				float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
				float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
				return z1 > z2;
			});

		for (auto& triProjected : vecTrianglesToRaster)
		{
			DrawTris(gck::vf2d(triProjected.p[0].x, triProjected.p[0].y),
				gck::vf2d(triProjected.p[1].x, triProjected.p[1].y),
				gck::vf2d(triProjected.p[2].x, triProjected.p[2].y),
				NULL, triProjected.col, true);
		}

		std::string nTris = "number of tris  : " + std::to_string(meshObject.tris.size());
		DrawString(gck::vi2d(10, 10), nTris, gck::DARK_CYAN, 2);

		std::string dTris = "displaying tris : " + std::to_string(vecTrianglesToRaster.size());
		DrawString(gck::vi2d(10, 30), dTris, gck::DARK_CYAN, 2);

		return true;
	}
};

int main()
{
	GameEngine3D window;

	if (window.Construct(500, 500, 1, 1, false, true, false, false))
		window.Start();

	return 0;
}