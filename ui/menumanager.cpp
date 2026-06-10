#include "precomp.h"
#include "ui/menumanager.h"
#include "sprite.h"

MenuManager::MenuManager(bool* restart_in)
	:restart(restart_in)
	,mainMenu(new Surface("assets/ui/titles/main_menu.png"), 1)
	,levelPausedMenu(new Surface("assets/ui/titles/paused_menu.png"), 1)
	,levelFailedMenu(new Surface("assets/ui/titles/level_failed_menu.png"), 1)
	,levelCompletedMenu(new Surface("assets/ui/titles/level_completed_menu.png"), 1)
	,continueButtonSprite(new Surface("assets/ui/buttons/continue.png"), 3)
	,continueButton(continueButtonSprite)
	,pauseButtonSprite(new Surface("assets/ui/buttons/pause.png"), 3)
	,pauseButton(pauseButtonSprite, { SCRWIDTH - pauseButtonSprite.GetWidth() - 25 , 25 })
	,playButtonSprite(new Surface("assets/ui/buttons/play.png"), 3)
	,playButton(playButtonSprite, { SCRWIDTH / 2 - 25 - playButtonSprite.GetWidth(), 275 })
	,quitButtonSprite(new Surface("assets/ui/buttons/quit.png"), 3)
	,quitButton(quitButtonSprite)
	,restartButtonSprite(new Surface("assets/ui/buttons/restart.png"), 3)
	,restartButton(restartButtonSprite)
{}

void MenuManager::SetMousePos(const float2& pos) { mousePos = pos; }

void MenuManager::SetMouseDown(const bool down) { mouseDown = down; }

void MenuManager::SetMenuState(const MenuType state) { menuState = state; }

MenuManager::MenuType MenuManager::GetMenuState() const { return menuState; }

bool MenuManager::GetQuit() const { return quit; }

void MenuManager::Tick()
{
	switch (menuState)
	{
	case MenuManager::MenuType::MAIN:
		if (playButton.IsPressed(mousePos, mouseDown)) { SetMenuState(MenuType::PLAYING); }
		if (quitButton.IsPressed(mousePos, mouseDown)) { quit = true; }
		break;
	case MenuManager::MenuType::PLAYING:
		if (pauseButton.IsPressed(mousePos, mouseDown)) { SetMenuState(MenuType::LEVELPAUSED); }
		break;
	case MenuManager::MenuType::LEVELPAUSED:
		if (continueButton.IsPressed(mousePos, mouseDown)) { SetMenuState(MenuType::PLAYING); }
		if (quitButton.IsPressed(mousePos, mouseDown)) { quit = true; }
		if (restartButton.IsPressed(mousePos, mouseDown)) { *restart = true; }
		break;
	case MenuManager::MenuType::LEVELFAILED:
		if (restartButton.IsPressed(mousePos, mouseDown)) { *restart = true; }
		if (quitButton.IsPressed(mousePos, mouseDown)) { quit = true; }
		break;
	case MenuManager::MenuType::LEVELCOMPLETE:
		if (restartButton.IsPressed(mousePos, mouseDown)) { *restart = true; }
		if (quitButton.IsPressed(mousePos, mouseDown)) { quit = true; }
		break;
	}
}

void MenuManager::Draw(Surface* screen)
{
	switch (menuState)
	{
	case MenuManager::MenuType::MAIN:
		quitButton.SetLocation({ SCRWIDTH / 2 + 25, 275 });

		mainMenu.Draw(screen, SCRWIDTH / 2 - mainMenu.GetWidth() / 2, 50);
		playButton.Draw(screen, mousePos);
		quitButton.Draw(screen, mousePos);
		break;
	case MenuManager::MenuType::PLAYING:
		pauseButton.Draw(screen, mousePos);
		break;
	case MenuManager::MenuType::LEVELPAUSED:
		continueButton.SetLocation({ SCRWIDTH / 2 - 10 - continueButtonSprite.GetWidth(), 250 });
		quitButton.SetLocation({ SCRWIDTH - 10 - quitButtonSprite.GetWidth(), SCRHEIGHT - 10 - quitButtonSprite.GetHeight() });
		restartButton.SetLocation({ SCRWIDTH / 2 + 10, 250 });

		levelPausedMenu.Draw(screen, SCRWIDTH / 2 - levelPausedMenu.GetWidth() / 2, 75);
		continueButton.Draw(screen, mousePos);
		quitButton.Draw(screen, mousePos);
		restartButton.Draw(screen, mousePos);
		break;
	case MenuManager::MenuType::LEVELFAILED:
		quitButton.SetLocation({ SCRWIDTH - 10 - quitButtonSprite.GetWidth(), SCRHEIGHT - 10 - quitButtonSprite.GetHeight() });
		restartButton.SetLocation({ SCRWIDTH / 2 - restartButtonSprite.GetWidth() / 2, 275 });

		levelFailedMenu.Draw(screen, SCRWIDTH / 2 - levelFailedMenu.GetWidth() / 2, 150);
		quitButton.Draw(screen, mousePos);
		restartButton.Draw(screen, mousePos);
		break;
	case MenuManager::MenuType::LEVELCOMPLETE:
		quitButton.SetLocation({ SCRWIDTH - 10 - quitButtonSprite.GetWidth(), SCRHEIGHT - 10 - quitButtonSprite.GetHeight() });
		restartButton.SetLocation({ SCRWIDTH / 2 - restartButtonSprite.GetWidth() / 2, 275 });

		levelCompletedMenu.Draw(screen, SCRWIDTH / 2 - levelCompletedMenu.GetWidth() / 2, 150);
		quitButton.Draw(screen, mousePos);
		restartButton.Draw(screen, mousePos);
		break;
	default:
		break;
	}
}