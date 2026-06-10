#pragma once

class MenuManager
{
public:
	enum class MenuType
	{
		MAIN,
		PLAYING,
		LEVELPAUSED,
		LEVELFAILED,
		LEVELCOMPLETE
	};
public:
	MenuManager(bool* restart_in);

	void SetMousePos(const float2& pos);
	void SetMouseDown(const bool down);
	void SetMenuState(const MenuType state);
	MenuType GetMenuState() const;
	bool GetQuit() const;

	void Tick();
	void Draw(Surface* screen);
private:
	float2 mousePos = { 0, 0 };
	bool mouseDown = false;
	bool quit = false;
	bool* restart = nullptr;

	MenuType menuState = MenuType::MAIN; // Default to main when game is initially launched.

	// Sprites
	Sprite mainMenu;
	Sprite levelPausedMenu;
	Sprite levelFailedMenu;
	Sprite levelCompletedMenu;

	Sprite continueButtonSprite;
	Sprite pauseButtonSprite;
	Sprite playButtonSprite;
	Sprite quitButtonSprite;
	Sprite restartButtonSprite;

	// Buttons
	Button continueButton;
	Button pauseButton;
	Button playButton;
	Button quitButton;
	Button restartButton;
};