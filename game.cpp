// Template, IGAD version 3
// Get the latest version from: https://github.com/jbikker/tmpl8
// IGAD/NHTV/BUAS/UU - Jacco Bikker - 2006-2023

#include "precomp.h"
#include "game.h"
#include "utility/camera/camera.h"
#include "utility/score/score.h"
#include "map/entity/player/player.h"
#include "map/tilemap.h"
#include "map/background.h"
#include "map/object/log/log.h"
#include "map/entity/scorpion/scorpion.h"
#include "map/object/pit/pit.h"
#include "map/object/rope/rope.h"
#include "map/object/hazard/hazard.h"
#include "map/object/collectible/collectible.h"
#include "map/object/finish/finish.h"
#include "ui/menumanager.h"

Game::Game(bool* running_in)
	:running(running_in)
{}

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	// Assign pointers
	camera = new Camera;
	player = new Player;
	tileMap = new Tilemap;
	backGround = new Background;

	menuManager = new MenuManager(&restart);

	log[0] = new Log(true, { 4 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE });
	log[1] = new Log(true, { 7 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE });
	log[2] = new Log(false, { 5 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE });

	hazard[0] = new Hazard({ 25 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE }, Hazard::HazardType::BONFIRE);
	hazard[1] = new Hazard({ 33 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE }, Hazard::HazardType::SNAKE);

	collectible[0] = new Collectible({ 3 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE }, Collectible::CollectibleType::DIAMONDRING);
	collectible[1] = new Collectible({ 8 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE }, Collectible::CollectibleType::GOLD);
	collectible[2] = new Collectible({ 13 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE }, Collectible::CollectibleType::SILVER);
	collectible[3] = new Collectible({ 21 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE }, Collectible::CollectibleType::MONEYBAG);

	scorpion = new Scorpion({ 20 * Tilemap::TILESIZE, 10 * Tilemap::TILESIZE - 33 });
	pit = new Pit({ 21 * Tilemap::TILESIZE, static_cast<int>(6.5 * Tilemap::TILESIZE) }, true , false); // Switch around true & false to get alligators!
	rope = new Rope({ 21 * Tilemap::TILESIZE, 0 }, 5.7 * Tilemap::TILESIZE);
	finish = new Finish({ 39 * Tilemap::TILESIZE, static_cast<int>(5.5 * Tilemap::TILESIZE) });

	score = new Score();

	// Load Data
	tileMap->LoadMap();
	player->SetLoc({ 2.0f * Tilemap::TILESIZE, 6.5f * Tilemap::TILESIZE - player->GetDimensions().y });
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{	
	// Get deltaTime in seconds for calculation purposes, with a max of 0.05f.
	deltaTime *= 0.001f;
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }

	menuManager->Tick();
	if (menuManager->GetQuit()) { *running = false; }

	if (menuManager->GetMenuState() == MenuManager::MenuType::PLAYING) { state = GameState::PLAYING; }
	else { state = GameState::MENU; }

	if (state == GameState::PLAYING)
	{
		// Movement calculations
		float deltaX = 0.0f;
		float deltaY = 0.0f;
		if (left) { deltaX -= 1.0f; }
		if (right) { deltaX += 1.0f; }

		if (up) { deltaY -= 1.0f; }
		if (down) { deltaY += 1.0f; }

		// Collectible update & collision
		for (int i = 0; i < MAXCOLLECTIBLES; i++)
		{
			collectible[i]->UpdateFrame(deltaTime);

			if (collectible[i]->CheckCollision(player->GetLoc(), player->GetDimensions()))
			{
				score->AddScore(collectible[i]->PickUp());
			}
		}

		// Log update & collision
		for (int i = 0; i < MAXLOGS; i++)
		{
			log[i]->Update(deltaTime);

			if (!player->GetIsHanging() && log[i]->CheckCollision(player->GetLoc(), player->GetDimensions()))
			{
				player->Down(!log[i]->GetMoving());

				scoreDeductionTimer += deltaTime;
				while (scoreDeductionTimer >= DEDUCTION_TIME)
				{
					score->RemoveScore(1);
					scoreDeductionTimer -= DEDUCTION_TIME;
				}
			}
		}

		// Jump
		if (jump)
		{
			player->Jump();
			jump = false;
		}

		// Scorpion update & collision
		scorpion->Update(static_cast<float>(player->GetLoc().x + 0.5 * player->GetDimensions().x), deltaTime, tileMap);
		if (!player->GetIsHanging() && scorpion->CheckCollision(player->GetLoc(), player->GetDimensions()))
		{
			menuManager->SetMenuState(MenuManager::MenuType::LEVELFAILED);
			return;
		}

		// Pit collision
		pit->Update(deltaTime);
		if (!player->GetIsHanging() && pit->CheckCollision(player->GetLoc(), player->GetDimensions()))
		{
			player->Reset(true);
			deathByPit = true;
			menuManager->SetMenuState(MenuManager::MenuType::LEVELFAILED);
			return;
		}
		alligatorOffset = pit->GetPlayerOffset();
		if (alligatorOffset.y != 0.0f) { player->IsOnAlligator(); }

		// Hazard update & collision
		for (int i = 0; i < MAXHAZARDS; i++)
		{
			hazard[i]->UpdateFrame(deltaTime);

			if (!player->GetIsHanging() && hazard[i]->CheckCollision(player->GetLoc(), player->GetDimensions()))
			{
				menuManager->SetMenuState(MenuManager::MenuType::LEVELFAILED);
				return;
			}
		}

		// Finish update & collision
		finish->UpdateFrame(deltaTime);
		if (finish->CheckCollision(player->GetLoc(), player->GetDimensions()))
		{
			menuManager->SetMenuState(MenuManager::MenuType::LEVELCOMPLETE);
		}

		// Misc updates
		rope->Update(deltaTime);

		player->Move(tileMap, { deltaX, deltaY }, deltaTime);
		if (alligatorOffset.y != 0.0f) { player->SetLoc(player->GetLoc() + alligatorOffset); }

		player->CheckHanging(rope->GetBottomLoc(), rope->GetTopLoc());
		if (player->FallenInPit()) { score->RemoveScore(100); }
	}
	else if (state == GameState::MENU)
	{
		if (restart) { Reset(); }
	}

	// Update offset
	camera->Tick(screen, player->GetLoc(), deltaTime);

	// Draw Functions
	backGround->Draw(screen, camera->GetOffSet());
	tileMap->Draw(screen, camera->GetOffSet());

	pit->Draw(screen, camera->GetOffSet());
	rope->Draw(screen, camera->GetOffSet());
	scorpion->Draw(screen, camera->GetOffSet());
	finish->Draw(screen, camera->GetOffSet());
	for (int i = 0; i < MAXHAZARDS; i++) { hazard[i]->Draw(screen, camera->GetOffSet()); }
	for (int i = 0; i < MAXCOLLECTIBLES; i++) { collectible[i]->Draw(screen, camera->GetOffSet()); }

	// Draw player
	if (deathByPit && fewerLines < player->GetDimensions().y)
	{
		lineTimer += deltaTime;
		while (lineTimer >= LINE_TIME)
		{
			lineTimer -= LINE_TIME;
			fewerLines++;
		}
		player->Draw(screen, camera->GetOffSet(), fewerLines);
	}
	else if (!deathByPit)
	{
		player->Draw(screen, camera->GetOffSet());
	}

	tileMap->DrawTop(screen, camera->GetOffSet());
	for (int i = 0; i < MAXLOGS; i++) { log[i]->Draw(screen, camera->GetOffSet()); }

	// UI
	if (menuManager->GetMenuState() == MenuManager::MenuType::PLAYING || menuManager->GetMenuState() == MenuManager::MenuType::LEVELPAUSED)
	{
		score->Draw(screen, { 20, 20 });
	}

	menuManager->Draw(screen);
}

void Game::Reset()
{
	deathByPit = false;
	fewerLines = 0;

	tileMap->LoadMap();

	player->Reset();
	player->SetLoc({ 2.0f * Tilemap::TILESIZE, 6.5f * Tilemap::TILESIZE - player->GetDimensions().y });

	log[0]->SetLoc({ 4 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE });
	log[1]->SetLoc({ 7 * Tilemap::TILESIZE, 6 * Tilemap::TILESIZE });

	for (int i = 0; i < MAXCOLLECTIBLES; i++) { collectible[i]->Reset(); }

	pit->Reset();
	rope->Reset();

	scorpion->SetLoc({ 20 * Tilemap::TILESIZE, 10 * Tilemap::TILESIZE - 33 });

	camera->Reset();
	score->Reset();

	restart = false;
	menuManager->SetMenuState(MenuManager::MenuType::PLAYING);
}

void Game::Shutdown()
{
	// Delete pointers
	delete player;
	delete tileMap;
	delete camera;
	delete backGround;
	delete menuManager;

	for (int i = 0; i < MAXLOGS; i++) { delete log[i]; }
	for (int i = 0; i < MAXHAZARDS; i++) { delete hazard[i]; }
	for (int i = 0; i < MAXCOLLECTIBLES; i++) { delete collectible[i]; }
	delete scorpion;
	delete pit;
	delete rope;
	delete finish;
	delete score;
}

void Game::MouseUp(int button)
{
	if (button == 0) { menuManager->SetMouseDown(false); }
}

void Game::MouseDown(int button)
{
	if (button == 0) { menuManager->SetMouseDown(true); }
}

void Game::MouseMove(int x, int y)
{
	int2 temp = { x, y };
	menuManager->SetMousePos(temp);
}

void Game::KeyUp(int key)
{
	if (key == 'A') { left = false; }
	if (key == 'D') { right = false; }
	if (key == 'W') { up = false; }
	if (key == 'S') { down = false; }

	if (key == ' ') { jump = false; }
}

void Game::KeyDown(int key)
{
	if (key == 'A') { left = true; }
	if (key == 'D') { right = true; }
	if (key == 'W') { up = true; }
	if (key == 'S') { down = true; }

	if (key == ' ') { jump = true; }

#ifdef _DEBUG
	if (key == 'R') { Reset(); }

	if (key == 'U') { score->AddScore(100); }
	if (key == 'J') { score->RemoveScore(10); }
	if (key == 'K') { score->RemoveScore(1); }
#endif // _DEBUG
}