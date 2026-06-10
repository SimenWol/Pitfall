// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#pragma once

class Background;
class Log;
class Scorpion;
class Player;
class Tilemap;
class Camera;
class Pit;
class Rope;
class Score;
class Hazard;
class Collectible;
class MenuManager;
class Finish;

namespace Tmpl8
{

class Game : public TheApp
{
private:
	enum GameState
	{
		PLAYING,
		MENU
	};
public:
	// Game Flow Methods
	Game(bool* running);
	void Init();
	void Tick(float deltaTime);
	void Reset();
	void Shutdown();

	// Input Handling
	void MouseUp(int button);
	void MouseDown(int button);
	void MouseMove(int x, int y);
	void MouseWheel(float) {}
	void KeyUp(int key);
	void KeyDown(int key);
private:
	float2 alligatorOffset = 0.0f;

	int2 mousePos = 0;
	bool mouseDown = false;

	bool* running;
	bool restart = false;

	Player* player = nullptr;
	Tilemap* tileMap = nullptr;
	Background* backGround = nullptr;

	MenuManager* menuManager = nullptr;

	static constexpr int MAXLOGS = 3;
	Log* log[MAXLOGS] = { nullptr };
	static constexpr int MAXHAZARDS = 2;
	Hazard* hazard[MAXHAZARDS] = { nullptr };
	static constexpr int MAXCOLLECTIBLES = 4;
	Collectible* collectible[MAXCOLLECTIBLES] = { nullptr };

	Scorpion* scorpion = nullptr;
	Pit* pit = nullptr;
	Rope* rope = nullptr;
	Finish* finish = nullptr;

	Camera* camera = nullptr;
	Score* score = nullptr;

	GameState state = GameState::MENU;

	bool left = false, right = false;
	bool up = false, down = false;
	bool jump = false;

	bool deathByPit = false;
	int fewerLines = 0;
	float lineTimer = 0.0f;
	static constexpr float LINE_TIME = 0.015f;

	float scoreDeductionTimer = 0.0f;
	static constexpr float DEDUCTION_TIME = 0.015f;
};
} // namespace Tmpl8