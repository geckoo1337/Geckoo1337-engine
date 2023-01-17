#define GECKOO_ENGINE_APP
#include "LiteGameEngineProto.h"
// structure for stars
struct sStar
{
    gck::vf2d pos;
    gck::vf2d vel;
    gck::vf2d acc;
    gck::Pixel col;

    float fLifespan;
    float fRadius;
    bool explosion;
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

class StarHarvest : public gck::xGameEngine
{
public:
    StarHarvest()
    {
        sAppName = "Star Harvest";
    }

private:
    std::vector<sStar> vecStars;
    short int nbr = 100;            // how many objects
    bool oneShot = false;           // only one shot
    gck::vi2d oob = { -10, -10 };   // out of bounds

    std::vector<sParticles> vecParticles;
    float life = 2.0f; // for decrease life span

    short int level = 1;
    short int harvest = 0;
    int mainScore = 0; // for leaderboard
    short int goal = 0;
    float timer = 0;
    float delay = 4;
    float deathChrono = 0;

    std::string message;
    bool intro = true;
    std::string info = "Press Space to start the game - esc to quit";
    std::string geckoo = "A game by Geckoo1337 for the 52nd LudumDare";

    void _blank()
    {   // first level
        nbr = 100;
        level = 1;
        goal = 0;
        mainScore = 0;
    }

    void reset(bool newGame)
    {
        if (!newGame)
        {
            nbr -= 10;
            ++level;
            deathChrono = 0;
            // end of the game
            if (level > 10)
            {
                sAppName = " Well Done!";
                intro = true;
                _blank();
            }
        }
        else
            _blank();

        harvest = 0;
        timer = 0;
        oneShot = false;

        appCreate();
    }

    void CreateStar(const gck::vf2d& pos, const gck::vf2d& vel, const gck::Pixel& col = gck::WHITE, const bool boom = false)
    {   // randomized color will be at least 30,30,30 and max 255,255,255
        gck::Pixel color = col;

        if (!oneShot)
            color = gck::Pixel(rand() % 225 + 30, rand() % 225 + 30, rand() % 225 + 30);

        sStar p;
        p.pos = pos;
        p.vel = vel;
        p.acc = { 0.0f, 0.0f };
        p.col = color;
        p.fRadius = (rand() % 4 + 1);
        p.fLifespan = 0xFF; // life - full alpha
        p.explosion = boom;

        vecStars.emplace_back(p);
    }

    void DrawParticles(const gck::vf2d& pos, std::vector<sParticles>& vecP, const gck::Pixel& col = gck::WHITE, const int force = 10)
    {   // random velocity
        for (float angle = 0.0f; angle < 2 * gck::PI; angle += 1 / (4 * gck::PI))
        {
            gck::vf2d randVel;

            randVel.x = sin(angle);
            randVel.y = cos(angle);
            randVel *= (rand() % force);

            sParticles p;
            p.pos = pos;
            p.vel = randVel;
            p.col = col;
            p.fLifespan = 0xFF; // full alpha 255

            vecP.emplace_back(p);
        }
    }

public:
    bool appCreate() override
    {   // randomized seed
        srand(static_cast<unsigned int>(time(nullptr)));
        // clean up
        vecStars.clear();
        vecParticles.clear();

        gck::vf2d randStars;
        gck::vf2d randVel;

        for (int i = 0; i < nbr; ++i)
        {
            randStars.x = rand() % ScreenWidth();
            randStars.y = rand() % ScreenHeight();
            randVel.x = (rand() % 20 + (-10)) * 5; // speed
            randVel.y = (rand() % 20 + (-10)) * 5;

            CreateStar(randStars, randVel);
        }
        // set here the level difficulty
        goal = int(nbr * 0.75);
        return true;
    }

    bool appUpdate(float fElapsedTime) override
    {
        if (GetKey(gck::Key::ESCAPE).bPressed && intro)
            return false;

        if (GetKey(gck::Key::ESCAPE).bPressed && !intro)
        {
            intro = true;
            reset(true);
        }

        if (GetKey(gck::Key::SPACE).bPressed && intro)
            intro = false;

        if (intro)
        {
            Clear(gck::VERY_DARK_MAGENTA, gck::VERY_DARK_GREEN, 8);
            DrawRect(gck::vi2d(ScreenWidth() / 2, ScreenHeight() / 2 - 8), 2 * sAppName.size() * 10.0f, 40, NULL, gck::DARK_YELLOW, false);

            DrawString(gck::vi2d(ScreenWidth() / 2 - (sAppName.size() * 8.5f), ScreenHeight() / 2 - (3 * 3)), sAppName, gck::VERY_DARK_YELLOW, 3);
            DrawString(gck::vi2d(ScreenWidth() / 2 - (sAppName.size() * 8.5f) - 1, ScreenHeight() / 2 - (3 * 3) - 1), sAppName, gck::DARK_YELLOW, 3);

            DrawString(gck::vi2d(ScreenWidth() / 2 - (info.size() * 2.9f), ScreenHeight() - 90), info, gck::GREEN, 1);
            DrawString(gck::vi2d(ScreenWidth() / 2 - (geckoo.size() * 2.9f), ScreenHeight() - 10), geckoo, gck::CYAN, 1);

            return true;
        }

        Clear(gck::BLACK);
        oob = { GetMouseX(), GetMouseY() }; // cursor position

        if (GetKey(gck::Key::R).bPressed)
            reset(true);

        if (GetMouse(0).bPressed && !oneShot)
        {
            oneShot = true;
            CreateStar(oob, gck::vf2d(0, 0), gck::GREY, true);
            DrawParticles(oob, vecParticles, gck::GREY, 10); // tiny FX
            deathChrono = 0;
        }

        if (oneShot)
            deathChrono += fElapsedTime;

        for (auto it = vecStars.begin(); it != vecStars.end();)
        {
            sStar p = *it;
            // alpha is under 10 - this element is dead and erased
            if (p.fLifespan <= 5.0f)
                it = vecStars.erase(it);
            else
                ++it;
        }
        // move and draw particles
        for (auto& star : vecStars)
        {
            if (star.pos.y < 0)
                star.pos.y += ScreenHeight();
            if (star.pos.y > ScreenHeight())
                star.pos.y = 0;
            if (star.pos.x < 0)
                star.pos.x += ScreenWidth();
            if (star.pos.x > ScreenWidth())
                star.pos.x = 0;

            star.vel += star.acc * fElapsedTime;
            star.pos += star.vel * fElapsedTime;

            if (star.explosion)
            {   // gameplay
                star.fRadius += 9 * fElapsedTime; // increase radius
                star.fLifespan -= 125 * fElapsedTime;  // reduce alpha
                star.col = gck::Pixel(star.col.r, star.col.g, star.col.b, star.fLifespan);
            }

            for (auto& obj : vecStars)
                if (computeDistance(star.pos, obj.pos) < ((star.fRadius + obj.fRadius) * 0.9f) && obj.explosion && !star.explosion)
                {
                    star.explosion = true;
                    DrawParticles(star.pos, vecParticles, star.col, 16); // tiny FX
                    ++harvest;
                    ++mainScore;
                    deathChrono = 0;
                }
       
            DrawCircle(star.pos, star.fRadius, star.col, true); 
            oob = { -10, -10 }; // out of bounds again

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
                {   particle.vel += particle.acc * fElapsedTime * 0.25;
                    particle.pos += particle.vel * fElapsedTime * 0.25;

                    particle.fLifespan -= 5 * fElapsedTime; // reduce alpha
                    particle.col = gck::Pixel(particle.col.r, particle.col.g, particle.col.b, particle.fLifespan);

                    Draw(particle.pos, particle.col);
                }
            }
        }

        message = "Harvest : " + std::to_string(harvest) + '/' + std::to_string(nbr) + '>' + std::to_string(goal);
        gck::Pixel c = gck::DARK_GREEN;

        if (oneShot && harvest >= goal)
        {
            timer += fElapsedTime;
            message = "Level Completed";
            c = gck::DARK_YELLOW;

            if (deathChrono > delay)
                reset(false);
        }
        else if (oneShot) {
            if (deathChrono > delay)
            {
                c = gck::DARK_RED;
                message = "Game Over - R to retry - ESCAPE to quit";
            }              
        }
        // custom cursor :)
        if (!oneShot)
        {
            DrawCircle(gck::vi2d(GetMouseX(), GetMouseY()), 2, gck::GREY);
            Draw(gck::vi2d(GetMouseX(), GetMouseY()), gck::DARK_RED);
        }

        std::string lvl = "Level " + std::to_string(level);
        std::string msc = "Score " + std::to_string(mainScore);

        DrawString(gck::vi2d(10, ScreenHeight() - 10), message, c, 1);
        DrawString(gck::vi2d(ScreenWidth() - 60, 10), lvl, gck::DARK_CYAN, 1);
        DrawString(gck::vi2d(ScreenWidth() - 60, 20), msc, gck::DARK_GREEN, 1);

        return true;
    }
};

int main()
{   
    StarHarvest sh;

    if (sh.Construct(401, 225, 4, 4, true, true, false))
        sh.Start();

    return 0;
}