#define GECKOO_ENGINE_APP
#include "LiteGameEngineProto.h"

class Asteroids : public gck::xGameEngine
{
public:
	Asteroids()
	{
		sAppName = "Asteroids";
	}

private:
	struct sSpaceObject
	{
		int nSize;
		float x;
		float y;
		float dx;
		float dy;
		float angle;
		bool clockwise;
	};
	// structure for particles
	struct sParticles
	{
		gck::vf2d pos;
		gck::vf2d vel;
		gck::vf2d acc;
		gck::Pixel col;
		float fLifespan;
	};
	// fore the menu
	std::string info = " *** Press Enter to start game ***";
	std::string geckoo = "A game by Geckoo1337 in pure C++";
	// full bucket of pixels
	std::vector<std::pair<gck::vi2d, gck::Pixel>> bucket;
	// objects vector
	std::vector<sSpaceObject> vecAsteroids;
	std::vector<sSpaceObject> vecBullets;
	std::vector<sParticles> vecParticles; // particles
	// for particles FX
	float life = 2.0f;
	// our shuttle
	sSpaceObject player;

	int level = 1;
	int score = 0;
	bool bDead = false;
	bool intro = true;

	std::vector<std::pair<float, float>> vecModelShip;
	std::vector<std::pair<float, float>> vecModelAsteroid;

	void DrawParticles(const gck::vf2d& pos, std::vector<sParticles>& vecP, float force = 10)
	{   // random velocity
		for (float angle = 0.0f; angle < 2 * gck::PI; angle += 1 / (4 * gck::PI))
		{
			gck::vf2d randVel;
			gck::Pixel col = gck::Pixel(gck::rndGenerator(33, 255), gck::rndGenerator(33, 255), gck::rndGenerator(33, 255));

			randVel.x = sin(angle);
			randVel.y = cos(angle);
			randVel *= gck::rndGenerator(5.0f, force);

			sParticles p;
			p.pos = pos;
			p.vel = randVel;
			p.col = col;
			p.fLifespan = 0xFF; // full alpha 255

			vecP.push_back(p);
		}
	}

protected:
	virtual bool appCreate()
	{
		vecModelShip =
		{
			{ 0.0f, -5.0f},
			{-2.5f, +2.5f},
			{+2.5f, +2.5f}
		};

		resetGame();
		return true;
	}

	void resetAsteroid()
	{	// generates a random integers for asteroid vertices
		int verts = gck::rndGenerator(12, 20);

		for (int i = 0; i < verts; i++)
		{
			float noise = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
			vecModelAsteroid.push_back(std::make_pair(noise * sinf(((float)i / (float)verts) * 6.28318f), noise * cosf(((float)i / (float)verts) * 6.28318f)));
		}
	}

	void resetPlayer()
	{	// player at the middle of the screen
		player.x = ScreenWidth() / 2.0f;
		player.y = ScreenHeight() / 2.0f;
		player.dx = 0.0f;
		player.dy = 0.0f;
		player.angle = 0.0f;
	}

	void resetValues()
	{
		level = 1;
		score = 0;
	}

	void resetGame()
	{	// clean up arrays
		vecBullets.clear();
		vecAsteroids.clear();
		vecParticles.clear();
		vecModelAsteroid.clear();
		bucket.clear();
		// reset entities
		resetPlayer();
		resetAsteroid(); // this way we got another shape
		// asteroids in game by two
		for (int o = 0; o < level; ++o)
		{
			vecAsteroids.push_back({ (int)16, gck::rndGenerator(30.0f, (float)ScreenWidth() - 30), 30, gck::rndGenerator(-10.0f, 10.0f), gck::rndGenerator(-10.0f, 10.0f), gck::rndGenerator(-180.0f, 180.0f), true });
			vecAsteroids.push_back({ (int)16, 30, gck::rndGenerator(30.0f, (float)ScreenHeight() - 30), gck::rndGenerator(-10.0f, 10.0f), gck::rndGenerator(-10.0f, 10.0f), gck::rndGenerator(-180.0f, 180.0f), false });
		}

		bDead = false;
	}

	void wrapCoordinates(float ix, float iy, float& ox, float& oy)
	{
		ox = ix;
		oy = iy;
		// manage pixels out of screen
		if (ix < 0.0f)
			ox = ix + (float)ScreenWidth();
		if (ix >= (float)ScreenWidth())
			ox = ix - (float)ScreenWidth();
		if (iy < 0.0f)
			oy = iy + (float)ScreenHeight();
		if (iy >= (float)ScreenHeight())
			oy = iy - (float)ScreenHeight();
	}

	virtual bool _draw(int x, int y, gck::Pixel col = gck::WHITE)
	{
		float fx, fy;
		wrapCoordinates(static_cast<float>(x), static_cast<float>(y), fx, fy);
		Draw(gck::vi2d((int)fx, (int)fy), col);

		return true;
	}
	// split the line into pixels
	void _drawLine(const gck::vi2d& pos1, const gck::vi2d& pos2, gck::Pixel c = gck::WHITE)
	{	// draws a line using a clever stairs/cliff system
		float x1 = static_cast<float>(pos1.x);
		float y1 = static_cast<float>(pos1.y);
		float x2 = static_cast<float>(pos2.x);
		float y2 = static_cast<float>(pos2.y);

		float xx = fabs(x1 - x2);
		float yy = fabs(y1 - y2);

		float xBy = xx / yy;
		float yBx = yy / xx;
		float oo = 0;
		// line is always weird - even horizontal or vertical
		if (xx >= yy)
		{	// stairs
			for (int o = 0; o < xx; o++)
			{
				if (x1 <= x2 && y1 <= y2)
					bucket.push_back(std::make_pair(gck::vi2d(static_cast<int>(x1 + o), static_cast<int>(y1 + round(oo))), c));
				else if (x1 <= x2 && y1 > y2)
					bucket.push_back(std::make_pair(gck::vi2d(static_cast<int>(x1 + o), static_cast<int>(y1 - round(oo))), c));
				else if (x1 > x2 && y1 <= y2)
					bucket.push_back(std::make_pair(gck::vi2d(static_cast<int>(x1 - o), static_cast<int>(y1 + round(oo))), c));
				else if (x1 > x2 && y1 > y2)
					bucket.push_back(std::make_pair(gck::vi2d(static_cast<int>(x1 - o), static_cast<int>(y1 - round(oo))), c));
				else
					return;

				oo += yBx;
			}
		}
		else {
			for (int o = 0; o < yy; o++)
			{	// cliff
				if (x1 <= x2 && y1 <= y2)
					bucket.push_back(std::make_pair(gck::vi2d(static_cast<int>(x1 + round(oo)), static_cast<int>(y1 + o)), c));
				else if (x1 <= x2 && y1 > y2)
					bucket.push_back(std::make_pair(gck::vi2d(static_cast<int>(x1 + round(oo)), static_cast<int>(y1 - o)), c));
				else if (x1 > x2 && y1 <= y2)
					bucket.push_back(std::make_pair(gck::vi2d(static_cast<int>(x1 - round(oo)), static_cast<int>(y1 + o)), c));
				else if (x1 > x2 && y1 > y2)
					bucket.push_back(std::make_pair(gck::vi2d(static_cast<int>(x1 - round(oo)), static_cast<int>(y1 - o)), c));
				else
					return;

				oo += xBy;
			}
		}
	}

	bool IsPointInsideCircle(float cx, float cy, float radius, float x, float y)
	{
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
	}

	virtual bool appUpdate(float fElapsedTime)
	{
		if (GetKey(gck::ESCAPE).bPressed)
		{
			if (intro)
				return false;
			else {
				intro = true;
				resetValues();
				resetGame();
			}
		}

		if (GetKey(gck::ENTER).bPressed)
			if (intro)
				intro = false;

		if (intro)
		{
			Clear(gck::VERY_DARK_GREY);
			// simple intro screen while intro boolean is true
			DrawRect(gck::vi2d(ScreenWidth() / 2, ScreenHeight() / 2 - 8), 2 * static_cast<int>(sAppName.size()) * 10, 40, NULL, gck::YELLOW, false);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(sAppName.size() * 8 + 1), ScreenHeight() / 2 - (3 * 3) + 1), sAppName, gck::DARK_YELLOW, 3);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(sAppName.size() * 8), ScreenHeight() / 2 - (3 * 3)), sAppName, gck::YELLOW, 3);

			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(info.size() * 3), ScreenHeight() - ScreenHeight()/3), info, gck::MAGENTA, 1);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(geckoo.size() * 3), ScreenHeight() - 10), geckoo, gck::CYAN, 1);


			return true;
		}

		Clear(gck::BLACK);
		bucket.clear();

		if (GetKey(gck::Key::LEFT).bHeld)
			player.angle -= 7.0f * fElapsedTime;
		if (GetKey(gck::Key::RIGHT).bHeld)
			player.angle += 7.0f * fElapsedTime;

		if (GetKey(gck::Key::UP).bHeld)
		{
			player.dx += sin(player.angle) * 60.0f * fElapsedTime;
			player.dy += -cos(player.angle) * 60.0f * fElapsedTime;
		}

		player.x += player.dx * fElapsedTime;
		player.y += player.dy * fElapsedTime;
		// wrap player pixels
		wrapCoordinates(player.x, player.y, player.x, player.y);

		for (auto& a : vecAsteroids)
			if (IsPointInsideCircle(static_cast<float>(a.x), static_cast<float>(a.y), static_cast<float>(a.nSize), player.x, player.y))
			{
				if (!bDead) DrawParticles(gck::vi2d((int)player.x, (int)player.y), vecParticles, 30);
				bDead = true;
			}
		// pew pew pew
		if (GetKey(gck::Key::SPACE).bPressed)
			vecBullets.push_back({ 0, player.x, player.y, 100.0f * sinf(player.angle), -100.0f * cosf(player.angle), 200.0f });

		for (auto& a : vecAsteroids)
		{
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;

			a.clockwise ? a.angle += 0.5f * fElapsedTime : a.angle -= 0.5f * fElapsedTime;
			// wrap asteroid pixels
			wrapCoordinates(a.x, a.y, a.x, a.y);
			abstractLine(vecModelAsteroid, a.x, a.y, a.angle, (float)a.nSize, gck::YELLOW);
		}

		std::vector<sSpaceObject> newAsteroids;

		for (auto& b : vecBullets)
		{
			b.x += b.dx * fElapsedTime;
			b.y += b.dy * fElapsedTime;

			for (auto& a : vecAsteroids)
			{
				if (IsPointInsideCircle(a.x, a.y, (float)a.nSize, b.x, b.y))
				{
					b.x = -100;

					if (a.nSize > 4)
					{
						float angle1 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
						float angle2 = ((float)rand() / (float)RAND_MAX) * 6.283185f;
						newAsteroids.push_back({ (int)a.nSize >> 1 ,a.x, a.y, 10.0f * sinf(angle1), 10.0f * cosf(angle1), 0.0f });
						newAsteroids.push_back({ (int)a.nSize >> 1 ,a.x, a.y, 10.0f * sinf(angle2), 10.0f * cosf(angle2), 0.0f });
					}
					// tiny FX according to the asteroid size
					DrawParticles(gck::vi2d((int)a.x, (int)a.y), vecParticles, (float)5 * a.nSize);
					a.x = -100;
					score += 100;
				}
			}
		}

		for (auto a : newAsteroids)
			vecAsteroids.push_back(a);

		if (vecAsteroids.size() > 0)
		{
			auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(), [&](sSpaceObject o) { return (o.x < 0); });
			if (i != vecAsteroids.end())
				vecAsteroids.erase(i);
		}

		if (vecAsteroids.empty() && vecParticles.empty())
		{
			score += 1000; // bonus
			vecAsteroids.clear();
			vecBullets.clear();

			++level;
			resetGame();
		}

		if (vecBullets.size() > 0)
		{	// erase bullet out of bounds
			auto i = remove_if(vecBullets.begin(), vecBullets.end(), [&](sSpaceObject o) { return (o.x < 1 || o.y < 1 || o.x >= ScreenWidth() - 1 || o.y >= ScreenHeight() - 1); });
			if (i != vecBullets.end())
				vecBullets.erase(i);
		}

		for (auto bullet : vecBullets)
			_draw((int)bullet.x, (int)bullet.y);

		if (!bDead)
			abstractLine(vecModelShip, player.x, player.y, player.angle);

		if (!vecParticles.empty())
        {
            for (auto it = vecParticles.begin(); it != vecParticles.end();)
            {
                sParticles p = *it;

                if (p.fLifespan <= 5.0) // alpha is under 5
                    it = vecParticles.erase(it);
                else
                    ++it;
            }
            // draw particles
            for (auto& particle : vecParticles)
            {   
				particle.vel += particle.acc * fElapsedTime;
                particle.pos += particle.vel * fElapsedTime;
                particle.fLifespan -= 5 * fElapsedTime * 30; // reduce alpha
                particle.col = gck::Pixel(particle.col.r, particle.col.g, particle.col.b, (uint8_t)particle.fLifespan);

                _draw((int)particle.pos.x, (int)particle.pos.y, particle.col);
            }
        }
		// display pixels which are in mu bucket
		for (auto& p : bucket)
			_draw(p.first.x, p.first.y, p.second);
		// score info
		std::string _score = "Score : " + std::to_string(score);
		DrawString(gck::vi2d(10, 10), _score, gck::DARK_CYAN, 1);
		// level info
		std::string lvl = "Level > " + std::to_string(level);
		DrawString(gck::vi2d(ScreenWidth() - (static_cast<int>(lvl.size()) * 6), 10), lvl, gck::DARK_GREEN, 1);

		if (bDead)
		{
			std::string gameOver = "*** GameOver ***";
			DrawString(gck::vi2d(ScreenWidth()/2 - (static_cast<int>(gameOver.size()) * 6), ScreenHeight()/2), gameOver, gck::VERY_DARK_RED, 2);
			DrawString(gck::vi2d(ScreenWidth() / 2 - (static_cast<int>(gameOver.size()) * 6) + 1, ScreenHeight() / 2 + 1), gameOver, gck::DARK_RED, 2);
		}

		return true;
	}

	void abstractLine(const std::vector<std::pair<float, float>>& vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, gck::Pixel col = gck::WHITE)
	{
		std::vector<std::pair<float, float>> vecTransformedCoordinates;
		size_t verts = vecModelCoordinates.size();
		vecTransformedCoordinates.resize(verts);

		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
			vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
		}

		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
		}

		for (int i = 0; i < verts; i++)
		{
			vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
			vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
		}

		for (int i = 0; i < verts + 1; i++)
		{
			int j = i + 1;
			_drawLine(gck::vi2d((int)vecTransformedCoordinates[i % verts].first, (int)vecTransformedCoordinates[i % verts].second), 
				gck::vi2d((int)vecTransformedCoordinates[j % verts].first, (int)vecTransformedCoordinates[j % verts].second), col);
		}
	}
};

std::pair<int, int> GetDesktopResolution()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	return std::make_pair(desktop.right, desktop.bottom);
}

int main()
{
	std::pair<int, int> r = GetDesktopResolution();
	int pixel = 6; // pixel filter

	Asteroids game;

	if (game.Construct(r.first/pixel, r.second/pixel, pixel, pixel, true, true, false, false))
		game.Start();

	return 0;
}
