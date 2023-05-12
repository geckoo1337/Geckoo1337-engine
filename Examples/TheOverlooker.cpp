// https://ldjam.com/events/ludum-dare/53/the-overlooker
// A game made for the 53rd LudumDare session by Geckoo1337
// This game has been created in pure C++ using a single Template
// without any external dependencie or other stuff like sprites, sounds, etc.
// No one wants to code something this way - but I like its 90's years vibe :)
// https://github.com/geckoo1337/Geckoo1337-engine

#define GECKOO_ENGINE_APP
#include "Template.h"	// using my template for graphical displaying
#include <iomanip>		// for coordinates XY in game (debug mode)

class LD53 : public gck::xGameEngine
{
public:
	LD53()
	{	// how to name it? Until the end it was named LD53 :)
		sAppName = " The Overlooker ";
	}

private:
	std::string myStamp =
		"1111111111111111"
		"1000000000000001"
		"1011000000000001"
		"1010000000000001"
		"1000000000000001"
		"1000000000000001"
		"1000000000000001"
		"1000000000000001"
		"1000000000000001"
		"1000000000000001"
		"1000000000000001"
		"1000000000000001"
		"1000000000000001"
		"1000000000000101"
		"1000000000000001"
		"1111111111111111";
// a single structure in order to create each letter - oop paradigm
	struct sLetter
	{
		std::string name;	// name on letter
		std::string street;	// street where this recipient letter
		int house = {};		// house number
		std::string city;	// city where lives this recipient letter
		int zipcode = {};	// city zipcode
		gck::Pixel stamp;	// stamp color
		bool sub = false;	// is the recipient letter lives in a suburbia zone?
		// final state
		bool state;			// approval or rejected?
	};

public:
	std::string next = " *** Press space to continue ***";
	std::string geckoo = " A game by Geckoo1337 for the LD53";
	bool intro = true;
	// timer
	float timer = 60.0f;
	// for lights on panel
	gck::Pixel lightOff = gck::DARK_RED;
	gck::Pixel lightOn = gck::RED;
	gck::Pixel ok = gck::GREEN;
	// I should use a single bit for each electric bulb, but...
	std::vector<gck::Pixel> lights = { lightOff, lightOff, lightOff, lightOff, lightOff };
	// a unique vector in order to store all letters
	std::vector<sLetter> vecLetters;
	// names in game :)
	const std::vector<std::string> names = { 
	"Sacha Gorky", "Elena Sokolov", "Evgeni Zalatoi", "Michka Sakalin",
	"Youri Kaplan", "Igor Schmol", "Boris Netchok", "Nina Karova",
	"Vladimir Neva", "Rachel Mikaevitch", "Xi-Jin Xi-Jin", "Boulat Akoudjava",
	"Fiodor Kniga", "Mila Krasavitsa", "Mikhail Nyebo", "Prapachik Kije",
	"Geckoo1337", "Nicolai Kapousta", "Anatol Khleba", "Strikalo band",
	"Asmir Latich", "Selina Lioubov", "Liouba Pouchkin", "Sergei Tarakan",
	"Wolfy Lyesa", "Tania Gryibi", "Ishran Milka", "Zakhaev Imran",
	"Khaled El-Saba", "Mikoni Kiko", "Salim Equador", "Walter Goodman",
	"Chapoklak", "Krokodil Gena", "Tchebourashka", "Igor Savochkin", 
	"Amadian Kozak", "Petik Goulash", "Kipa Kipa", "Privyet Poka" };
	// streets in game
	const std::vector<std::string> streets = { 
	"Tsaritsa Lily Street", "Main Street", "Stalin Park", "Lenine Avenue",
	"Prince Igor Street", "Prince Sergei Street", "Prince Oleg Street", "Prince Igor Jr Street",
	"Cemetery Street", "Old Water Mill Street", "Gorky Place", "Potemkin Place",
	"Red October Street", "Bolchoi Street", "Old Market Street", "Old Hospital Street",
	"Alexander Street", "Avenue Piotr II", "Avenue Vassili", "Avenue Pavel III",
	"Rail Station Street", "Fidel Castro Street", "Youri Gagarin Street", "Bibliotheka Street",
	"Suburbia A", "Suburbia B", "Suburbia C", "Suburbia D",
	"Suburbia E", "Suburbia F", "Suburbia G", "Suburbia H" };
	// cities in game
	const std::vector<std::string> cities = { "Garadok", "Stolitsa", "Sovietgrad", "Michkina", "Metropolia", "Zaritsa" };
	// stamp colors in game
	const std::vector<gck::Pixel> stamps = { gck::RED, gck::GREEN, gck::BLUE, gck::MAGENTA };
	// your reputation at work
	const std::vector<std::string> reputation = { "Execrable", "Bad", "Disappointing", "Good", "Excellent" };
	// how many letters you have in this level
	int number;
	int shift = 4;	// tiny shift to create a letters stack 
	int seal_shift = 0;
	int size = 16;	// sprites size - only for stamp
	std::string zip = "Z"; // Salav Zakouste!!!

	bool debug = false;		// debug mode
	bool game_over = false;	// is over?
	bool winner = false;	// are you the winner?
	bool perfect = true;	// expert mode
	bool newDay = true;		// a new day starts soon
	int level = 0;			// level/day

	std::string approval = "Approval";
	std::string rejected = "Rejected";
	std::string list = "List Of Rules : day ";
	std::string go = ">> You are fired <<";	// game over :/
	std::string nd = "Ready for the next day?";  // new day
	std::string wn = "! congratulations !";	// winner!
	// final reward if the player break the game
	std::string xxx = "Amazing! You did it without any error. Best score ever > 5/5";
	std::string aaa = "Great! You are the employee of the week. 4/5";
	std::string bbb = "Cool. Not bad at all! Try again. 3/5";
	std::string ccc = "Not the best, but not the least! Try again. 2/5";
	std::string ddd = "What are you doing here? Change your job or try again. 1/5";
	std::string out = "Get out of there! This is not a job for you...";
	// your reputation which can be ++ or --
	int _reputation = 4;	
	// model for a unique pattern
	std::string color_stamp;
	gck::Pixel color;		// first level
	std::string myCity;     // second level
	int myZipCode;			// third level
	// the five rules
	const std::vector<std::string> rules = { "1. Reject letters with a ", 
											 "2. Reject letters for city ", 
											 "3. Allow only zipcodes under ",
											 "4. Nothing for the suburbia zones",
											 "5. Allow only odd house numbers" };

	gck::Sprite* spriteStamp = nullptr;

// This function allows to create a sprite according to a single string
	void createSprite(gck::Sprite* s, std::string mySprite, int size, gck::Pixel c)
	{
		for (int x = 0; x < size; ++x)
			for (int y = 0; y < size; ++y)
			{
				if (mySprite[x * size + y] == '0')
					s->SetPixel(gck::vi2d(y, x), gck::BLANK);
				else
					s->SetPixel(gck::vi2d(y, x), c);
			}
	}
// function to create as a new object each letter with its randomized features
	void AddLetter()
	{
		sLetter letter;
		// select a name in the list
		int n = names.size() - 1;
		letter.name = names[gck::rndGenerator(0, n)];
		// select a street
		int s = streets.size() - 1;
		int x = gck::rndGenerator(0, s);
		letter.street = streets[x];
		// is it in the suburbia zones (he eight last items) ?
		if (x > (s - 8))
			letter.sub = true;
		// generate a house number
		letter.house = gck::rndGenerator(1, 999);
		// select a city in the list
		int c = cities.size() - 1;
		letter.city = cities[gck::rndGenerator(0, c)];
		// generate a zipcode
		letter.zipcode = gck::rndGenerator(1000, 9999);
		// select a stamp color in the list
		int o = stamps.size() - 1;
		letter.stamp = stamps[gck::rndGenerator(0, o)];
		// put this new letter in the vector
		vecLetters.emplace_back(letter);
	}
	// at start, code generates letters managing time - and a MODEL
	bool appCreate() override
	{	
		vecLetters.clear();
		number = 10 + (level * 2);
		++level;

		timer = 60.0f;

		for (int i = 0; i < number; ++i)
			AddLetter();
		// for the MODEL
		int o = stamps.size() - 1;
		color = stamps[gck::rndGenerator(0, o)];

		int oo = cities.size() - 1;
		myCity = cities[gck::rndGenerator(0, oo)];

		myZipCode = gck::rndGenerator(5000, 7500);
		// we can display a seal using different positions
		seal_shift = gck::rndGenerator(-10, 10);
		newDay = true;

		return true;
	}
	// draw the stamp with its color
	void createStamp(gck::Pixel c)
	{
		spriteStamp = new gck::Sprite(size, size);
		createSprite(spriteStamp, myStamp, size, c);
	}
	// oups! we did something wrong :/
	void ding_ding(sLetter l, bool noway)
	{
		perfect = false;

		if (noway)
		{	// defines where are the errors in order to enlight bulbs
			if (checkColor(l))	
				lights[0] = lightOn;
			if (checkCity(l))	
				lights[1] = lightOn;
			if (checkZipCode(l))
				lights[2] = lightOn;
			if (checkStreet(l))	
				lights[3] = lightOn;
			if (checkNumber(l)) 
				lights[4] = lightOn;
		}
		else // obviously we rejected a letter which sets with all rules !!!
			for (int i = 0; i < lights.size(); ++i)
				lights[i] = ok;
		// decrease your reputation (increasing value)
		if (_reputation > 0)
			--_reputation;
		else
		{	// game over
			Beep(440, 200);
			Beep(330, 200);
			Beep(220, 200);
			Beep(330, 200);

			game_over = true;
		}
		// error sound
		Beep(220, 500);
	}
/*
* This part was very hard to develop because I did not understand how to manage each condition according to the state of my decisions. 
* A letter which has been rejected cannot be managed like a letter which has been accepted. I struggled hard with this bug...
* The main diference is that there are some inclusing conditions || - and to the other side, some exclusive conditions &&.
*/
	bool checkColor(sLetter l)
	{	// check stamp color
		if (l.stamp == color)
			return true;
		else
			return false;
	}

	bool checkCity(sLetter l)
	{	// check the city
		if (l.city == myCity)
			return true;
		else
			return false;
	}

	bool checkZipCode(sLetter l)
	{	// check zipcode
		if (l.zipcode > myZipCode)
			return true;
		else
			return false;
	}

	bool checkStreet(sLetter l)
	{	// check the street - according to a single boolean
		return l.sub;
	}

	bool checkNumber(sLetter l)
	{	// check house number
		if (l.house % 2 == 0)
			return true;
		else
			return false;
	}
/*
* I had chosen a way which is not the best in order to check if a letter must be rejected or accepted. 
* For each level there is an condition ensemble - a global status. It's like a knot bag - a tricky maze. 
* I guess that this part could be improved, but at this stage, I have to do something which works in a simple way. 
* Time is running...
*/
	bool checkRules(sLetter l)
	{	// switch lights for each bulb
		for (int i = 0; i < lights.size(); ++i)
			lights[i] = lightOff;
		// this letter has been accepted
		if (l.state)
		{
			if (level == 1 && checkColor(l))
			{
				ding_ding(l, true);
				return false;
			}

			if (level == 2 && (checkCity(l) || checkColor(l)))
			{
				ding_ding(l, true);
				return false;
			}

			if (level == 3 && (checkZipCode(l) || checkCity(l) || checkColor(l)))
			{
				ding_ding(l, true);
				return false;
			}

			if (level == 4 && (checkStreet(l) || checkZipCode(l) || checkCity(l) || checkColor(l)))
			{
				ding_ding(l, true);
				return false;
			}

			if (level == 5 && (checkNumber(l) || checkStreet(l) || checkZipCode(l) || checkCity(l) || checkColor(l)))
			{
				ding_ding(l, true);
				return false;
			}
		}
		else { // this letter has been rejected
			if (level == 1 && !checkColor(l))
			{
				ding_ding(l, false);
				return false;
			}

			if (level == 2 && (!checkCity(l) && !checkColor(l)))
			{
				ding_ding(l, false);
				return false;
			}

			if (level == 3 && (!checkZipCode(l) && !checkCity(l) && !checkColor(l)))
			{
				ding_ding(l, false);
				return false;
			}

			if (level == 4 && (!checkStreet(l) && !checkZipCode(l) && !checkCity(l) && !checkColor(l)))
			{
				ding_ding(l, false);
				return false;
			}

			if (level == 5 && (!checkNumber(l) && !checkStreet(l) && !checkZipCode(l) && !checkCity(l) && !checkColor(l)))
			{
				ding_ding(l, false);
				return false;
			}
		}
		// this is a good decision. The letter fits witht the rules :)
		Beep(440, 100);
		seal_shift = gck::rndGenerator(-10, 10); // change shift

		return true;
	}
	// update section
	bool appUpdate(float fElapsedTime) override
	{	// bye bye!
		if (GetKey(gck::ESCAPE).bPressed)
		{
			if (intro || game_over || winner)
				return false;
			else
				intro = true;
		}
		// pause
		if (GetKey(gck::SPACE).bPressed)
		{
			if (intro || newDay)
			{
				intro = false;
				newDay = false;
			}
		}
		// game over panel
		if (game_over)
		{
			Clear(gck::VERY_DARK_GREEN, gck::DARK_GREEN, 16);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(go.size() * 11.5f + 1), ScreenHeight() / 2 - 13 + 1), go, gck::DARK_YELLOW, 4);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(go.size() * 11.5f), ScreenHeight() / 2 - 13), go, gck::YELLOW, 4);
			DrawString(gck::vi2d(10, 10), out, gck::WHITE, 1);

			return true;
		}
		// winner panel
		if (winner)
		{
			Clear(gck::VERY_DARK_GREEN, gck::DARK_GREEN, 16);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(wn.size() * 11.5f + 1), ScreenHeight() / 2 - 13 + 1), wn, gck::DARK_YELLOW, 4);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(wn.size() * 11.5f), ScreenHeight() / 2 - 13), wn, gck::YELLOW, 4);

			if (perfect)
				DrawString(gck::vi2d(10, 10), xxx, gck::WHITE, 1);
			else {
				if (_reputation == 4)
					DrawString(gck::vi2d(10, 10), aaa, gck::WHITE, 1);
				else if (_reputation == 3)
					DrawString(gck::vi2d(10, 10), bbb, gck::WHITE, 1);
				else if (_reputation == 2)
					DrawString(gck::vi2d(10, 10), ccc, gck::WHITE, 1);
				else if (_reputation == 1)
					DrawString(gck::vi2d(10, 10), ddd, gck::WHITE, 1);
				else
					DrawString(gck::vi2d(10, 10), ddd, gck::WHITE, 1);
			}

			return true;
		}
		// intro at start
		if (intro)
		{
			Clear(gck::VERY_DARK_GREEN, gck::DARK_GREEN, 16);
			// simple intro screen while intro boolean is true
			DrawRect(gck::vi2d(ScreenWidth() / 2, ScreenHeight() / 2 - 12), 2 * static_cast<int>(sAppName.size()) * 12, 50, NULL, gck::YELLOW, false);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(sAppName.size() * 11 + 1), ScreenHeight() / 2 - 13 + 1), sAppName, gck::DARK_YELLOW, 4);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(sAppName.size() * 11), ScreenHeight() / 2 - 13), sAppName, gck::YELLOW, 4);

			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(next.size() * 6), ScreenHeight() - 110), next, gck::MAGENTA, 2);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(geckoo.size() * 6), ScreenHeight() - 14), geckoo, gck::CYAN, 2);

			return true;
		}
		// a new day starts
		if (newDay)
		{
			Clear(gck::VERY_DARK_GREEN, gck::DARK_GREEN, 16);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(nd.size() * 11.5f + 1), ScreenHeight() / 2 - 13 + 1), nd, gck::DARK_YELLOW, 4);
			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(nd.size() * 11.5f), ScreenHeight() / 2 - 13), nd, gck::YELLOW, 4);

			DrawString(gck::vi2d(ScreenWidth() / 2 - static_cast<int>(next.size() * 6), ScreenHeight() - 110), next, gck::MAGENTA, 2);

			return true;
		}
		// the full letters stack has been checked
		if (number <= 0)
		{
			Clear(gck::VERY_DARK_GREEN, gck::DARK_GREEN, 16);

			Beep(1000, 100);
			Beep(1200, 100);
			Beep(1400, 100);
			Beep(1000, 500);
			// add a bonus for your repution
			if (_reputation < reputation.size() - 1)
				++_reputation;

			appCreate();
		}
		// clean up the board
		Clear(gck::VERY_DARK_RED, gck::VERY_DARK_CYAN, 32);
		// debug mode allowed me to get some cursor information in order to control the buttons - no more
		if (debug)
		{
			std::stringstream ss; // for debug
			std::pair<int, int> coord = std::make_pair(GetMouseX(), GetMouseY());
			ss << "x:" << std::setw(3) << std::setfill('0') << coord.first << " y:" << std::setw(3) << std::setfill('0') << coord.second;
			std::string s = ss.str();
			DrawString(gck::vi2d(10, 10), s, gck::WHITE, 1);
		}
		// a single line around the main board
		DrawRect(gck::vi2d(ScreenWidth()/2, ScreenHeight() / 2), ScreenWidth() - 8, ScreenHeight() - 8, NULL, gck::DARK_YELLOW, false);
		// draw all letter, but write the address on the last one. This letter will be checked
		for (int i = 0; i < number; ++i)
		{
			DrawRect(gck::vi2d(ScreenWidth() / 3 + (i * shift), ScreenHeight() - 150 + (i * shift)), 250, 140, 3, gck::WHITE, true);
			DrawRect(gck::vi2d(ScreenWidth() / 3 + (i * shift), ScreenHeight() - 150 + (i * shift)), 250, 140, 3, gck::DARK_GREY, false);
			// create the letter which we are checking
			if (i == number - 1)
			{
				createStamp(vecLetters[i].stamp); // define stamp color
				// address informations
				DrawString(gck::vi2d(ScreenWidth() / 4 + (i * 3), ScreenHeight() - 130 + (i * shift)), vecLetters[i].name, gck::VERY_DARK_GREY, 1);
				std::string hh = std::to_string(vecLetters[i].house);
				DrawString(gck::vi2d(ScreenWidth() / 4 + (i * 3), ScreenHeight() - 120 + (i * shift)), hh, gck::VERY_DARK_GREY, 1);
				DrawString(gck::vi2d(ScreenWidth() / 4 + (i * 3 + 25), ScreenHeight() - 120 + (i * shift)), vecLetters[i].street, gck::VERY_DARK_GREY, 1);
				std::string zp = zip;
				zp.append("-");
				zp.append(std::to_string(vecLetters[i].zipcode));
				DrawString(gck::vi2d(ScreenWidth() / 4 + (i * 3), ScreenHeight() - 110 + (i * shift)), zp, gck::VERY_DARK_GREY, 1);
				DrawString(gck::vi2d(ScreenWidth() / 4 + (i * 3 + 45), ScreenHeight() - 110 + (i * shift)), vecLetters[i].city, gck::VERY_DARK_GREY, 1);
				// stamp
				DrawRect(gck::vi2d(ScreenWidth() / 2 + (i * 3), ScreenHeight() - 187 + (i * shift)), 31, 31, NULL, gck::GREY, true);
				DrawSprite(gck::vi2d(ScreenWidth() / 2 + (i * 3), ScreenHeight() - 187 + (i * shift)), spriteStamp, 2);
				DrawCircle(gck::vi2d(ScreenWidth() / 2 + (i * 3), ScreenHeight() - 187 + (i * shift)), 12, vecLetters[i].stamp, true, NULL);
				DrawString(gck::vi2d(ScreenWidth() / 2 + (i * 3), ScreenHeight() - 187 + (i * shift)), zip, gck::VERY_DARK_GREY, 2);
				// seal
				DrawCircle(gck::vi2d(ScreenWidth() / 4 + (i * 3) - (seal_shift * 2), ScreenHeight() - 170 + (i * shift) + seal_shift), 20, gck::GREY, false, NULL);
				DrawCircle(gck::vi2d(ScreenWidth() / 4 + (i * 3) - (seal_shift * 2), ScreenHeight() - 170 + (i * shift) + seal_shift), 26, gck::GREY, false, NULL);
				DrawRect(gck::vi2d(ScreenWidth() / 4 + (i * 3) - (seal_shift * 2), ScreenHeight() - 170 + (i * shift) + seal_shift), 30, 7, seal_shift, gck::GREY, false);
			}
		}
		// accepted button
		DrawRect(gck::vi2d(ScreenWidth() - 100, ScreenHeight() - 80), 110, 30, NULL, gck::DARK_GREEN, true);
		DrawRect(gck::vi2d(ScreenWidth() - 100, ScreenHeight() - 80), 110, 30, NULL, gck::VERY_DARK_GREY, false);
		DrawString(gck::vi2d(ScreenWidth() - 100 - static_cast<int>(approval.size() * 5), ScreenHeight() - 80), approval, gck::VERY_DARK_GREEN, 2);
		// rejected button
		DrawRect(gck::vi2d(ScreenWidth() - 100, ScreenHeight() - 40), 110, 30, NULL, gck::DARK_RED, true);
		DrawRect(gck::vi2d(ScreenWidth() - 100, ScreenHeight() - 40), 110, 30, NULL, gck::VERY_DARK_GREY, false);
		DrawString(gck::vi2d(ScreenWidth() - 100 - static_cast<int>(approval.size() * 5), ScreenHeight() - 40), rejected, gck::VERY_DARK_RED, 2);
		// rules board
		DrawRect(gck::vi2d(ScreenWidth() / 4 + 10, ScreenHeight() - 310), 270, 140, NULL, gck::GREY, true);
		DrawRect(gck::vi2d(ScreenWidth() / 4 + 10, ScreenHeight() - 310), 270, 140, NULL, gck::VERY_DARK_GREY, false);
		DrawString(gck::vi2d(ScreenWidth() / 18, ScreenHeight() - 370), list, gck::VERY_DARK_RED, 1);
		// reputation string
		std::string r = "Your reputation is ";
		r.append(reputation[_reputation]);
		DrawString(gck::vi2d(ScreenWidth() - 215, 20), r, gck::YELLOW, 1);
		// reputation bar
		DrawRect(gck::vi2d(ScreenWidth() - 184, 40), 66, 10, NULL, gck::DARK_GREY, false);
		DrawRect(gck::vi2d(ScreenWidth() - 184, 40), 66, 10, NULL, gck::GREY, true);

		for (int j = 0; j <= _reputation; ++j)
		{
			DrawRect(gck::vi2d(ScreenWidth() - 210 + (j * 13), 40), 10, 9, NULL, gck::DARK_CYAN, true);
			DrawRect(gck::vi2d(ScreenWidth() - 210 + (j * 13), 40), 10, 9, NULL, gck::VERY_DARK_CYAN, false);
			Draw(gck::vi2d(ScreenWidth() - 213 + (j * 13), 42), gck::CYAN);
		}
		// write the first rule
		if (level > 0)
		{
			std::stringstream x;
			x << level;
			std::string xx = x.str();

			std::string sentence1 = rules[0];
			DrawString(gck::vi2d(ScreenWidth() / 18, ScreenHeight() - 350), sentence1, gck::VERY_DARK_RED, 1);
			DrawString(gck::vi2d(ScreenWidth() / 3 - 45, ScreenHeight() - 370), xx, gck::VERY_DARK_YELLOW, 1);

			if (color == gck::RED)
			{
				color_stamp = "red stamp";
				DrawString(gck::vi2d(ScreenWidth() / 3 - 17, ScreenHeight() - 350), color_stamp, color, 1);
			}
			else if (color == gck::GREEN) {
				color_stamp = "green stamp";
				DrawString(gck::vi2d(ScreenWidth() / 3 - 17, ScreenHeight() - 350), color_stamp, color, 1);
			}
			else if (color == gck::BLUE) {
				color_stamp = "blue stamp";
				DrawString(gck::vi2d(ScreenWidth() / 3 - 17, ScreenHeight() - 350), color_stamp, color, 1);
			}
			else {
				color_stamp = "magenta stamp";
				DrawString(gck::vi2d(ScreenWidth() / 3 - 17, ScreenHeight() - 350), color_stamp, color, 1);
			}
			// its bulb
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 350), 8, gck::DARK_GREY, true);
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 350), 7, lights[0], true);
		}
		// write the second rule
		if (level > 1)
		{
			std::string sentence2 = rules[1];
			DrawString(gck::vi2d(ScreenWidth() / 18, ScreenHeight() - 330), sentence2, gck::VERY_DARK_RED, 1);
			DrawString(gck::vi2d(ScreenWidth() / 3 - 5, ScreenHeight() - 330), myCity, gck::VERY_DARK_GREY, 1);
			// its bulb
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 330), 8, gck::DARK_GREY, true);
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 330), 7, lights[1], true);
		}
		// write the third rule
		if (level > 2)
		{
			std::stringstream z;
			z << myZipCode;
			std::string zz = z.str();

			std::string sentence3 = rules[2];
			DrawString(gck::vi2d(ScreenWidth() / 18, ScreenHeight() - 310), sentence3, gck::VERY_DARK_RED, 1);
			DrawString(gck::vi2d(ScreenWidth() / 3 + 6, ScreenHeight() - 310), zz, gck::VERY_DARK_GREY, 1);
			// its bulb
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 310), 8, gck::DARK_GREY, true);
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 310), 7, lights[2], true);
		}
		// write the fourth rule
		if (level > 3)
		{
			std::string sentence4 = rules[3];
			DrawString(gck::vi2d(ScreenWidth() / 18, ScreenHeight() - 290), sentence4, gck::VERY_DARK_RED, 1);
			// its bulb
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 290), 8, gck::DARK_GREY, true);
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 290), 7, lights[3], true);
		}
		// write the last rule
		if (level > 4)
		{
			std::string sentence5 = rules[4];
			DrawString(gck::vi2d(ScreenWidth() / 18, ScreenHeight() - 270), sentence5, gck::VERY_DARK_RED, 1);
			// its bulb
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 270), 8, gck::DARK_GREY, true);
			DrawCircle(gck::vi2d(ScreenWidth() / 2 - 20, ScreenHeight() - 270), 7, lights[4], true);
		}
		// no more level - you win
		if (level > 5)
			winner = true;
		// check mouse cursor position to know if there is a button here (getting info in debug mode)
		if (GetMouse(0).bPressed && (GetMouseX() > ScreenWidth() - 155 && GetMouseX() < ScreenWidth() - 45)
			&& (GetMouseY() > ScreenHeight() - 95 && GetMouseY() < ScreenHeight() - 65))
		{
			vecLetters[number - 1].state = true; // letter status - approval
			// check this letter
			if (checkRules(vecLetters[number - 1]))
				--number;
		}


		if (GetMouse(0).bPressed && (GetMouseX() > ScreenWidth() - 155 && GetMouseX() < ScreenWidth() - 45)
			&& (GetMouseY() > ScreenHeight() - 55 && GetMouseY() < ScreenHeight() - 25))
		{
			vecLetters[number - 1].state = false; // letter status - rejected
			// check this letter
			if (checkRules(vecLetters[number - 1]))
				--number;
		}

		gck::vi2d center = gck::vi2d(gck::vi2d(ScreenWidth() - 130, 130));
		// draw the frame timer
		DrawRect(center, 150, 150, NULL, gck::VERY_DARK_GREY, true);

		for (int i = 0; i < 4; ++i)
			DrawRect(center, 150 - (i * 3), 150 - (i * 3), NULL, gck::DARK_YELLOW, false);
		// draw the timer
		DrawCircle(center, 50, gck::BLACK, true);
		DrawCircle(center, 50 + 15, gck::VERY_DARK_YELLOW, true, 15);
		DrawCircle(center, 50, gck::MAGENTA, false);
		DrawCircle(center, 50 + 15, gck::MAGENTA, false);
		// draw its subdivisions
		for (int i = 1; i <= 8; ++i)
		{
			std::string si = std::to_string(i);
			gck::vi2d vPointB = GetVector(i, 50 + 8, -8);
			DrawString(vPointB + center, si, gck::YELLOW, 1);
		}
		// draw the timer needle
		DrawLine(center, GetVector(timer, 50 - 5, 60) + center, gck::RED);
		DrawCircle(center, 7, gck::RED, true);
		// draw a simple cursor
		DrawCircle(gck::vi2d(GetMouseX(), GetMouseY()), 3, gck::YELLOW);
		Draw(gck::vi2d(GetMouseX(), GetMouseY()), gck::DARK_BLUE);
		// running time! In debug mode we have no time :)
		if (!debug)
			timer -= fElapsedTime;
		// after 60 seconds, your reputation is depreciated
		if (timer <= 0.0f)
		{
			perfect = false;
			timer = 60.0f;

			if (_reputation > 0)
				--_reputation;
		}

		return true;
	}
};

int main()
{   // hide console
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	int pixel = 2;	// pixel filter
	LD53 game;		// game instantiation

	if (game.Construct(601, 400, pixel, pixel, false, true, false, false))
		game.Start();

	return 0;
}
