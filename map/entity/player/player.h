#pragma once

class Tilemap;

// Player class
class Player
{
public:
	Player();
public:
	void Draw(Surface* screen, const float2 offset, const int fewerLines = 0);
	void Move(Tilemap* map, float2 delta_loc, const float& deltaTime);
	void Jump();
	void Reset(const bool animationsOnly = false);
	void SetLoc(const float2& new_loc);
	float2 GetLoc() const;
	int2 GetDimensions() const;
	bool GetIsHanging() const;
	void CheckHanging(float2 bottomLoc, float2 topLoc);
	bool FallenInPit();
	void IsOnAlligator();
	void Down(const bool allowedToMove = true);
private:
	void AnimateCharacter(const float deltaTime = 0.0f);
	void StartClimbing(Tilemap* map, const bool goingDown);
	void Climb(const int delta, Tilemap* map, const float deltaTime);
	void DismountLadder(const int delta, Tilemap* map, const float deltaTime);
	void CheckBelow(Tilemap* map);
	bool CheckSide(Tilemap* map);
private:
	int2 dimensions;
	float2 playerLoc = { 10.0f, 10.0f };
	float2 playerSpeed = { 0.0f, 0.0f };

	float2 dustLoc = { 0.0f, 0.0f };
	float dustTimer = 0.0f;
	int dustFrame = 0;
	static constexpr float DUSTFRAMETIME = 0.05f;
	bool dustActive = false;

	const float GRAVITY = 6.2f;
	const float JUMPHEIGHT = 1.5f;
	const float DETACHSPEED = 1.2f;

	// Ladder timer
	float climbTimer = 0.0f;
	static constexpr float CLIMBSTEPTIME = 0.15f;
	int climbingFrame = 0;

	// Grace period timer after jumping before checks
	float graceTimer = 0.0f;

	// Grace period timer after detaching off rope before checks
	float hangTimer = 0.0f;

	// Animation Variables
	float timeCounter = 0.0f;
	static constexpr float FRAMETIME = 0.1f;
	int currFrame = 0;

	bool canMove = true;

	bool prevIsFalling = false;
	bool prevIsJumping = false;

	bool isWalking = false;
	bool isDown = false;
	bool isClimbing = false;
	bool isFacingRight = true;
	bool isFalling = false;
	bool isJumping = false;

	bool isHanging = false;
	bool isDetaching = false;

	bool isOnAlligator = false;
private:
	Sprite playerSprite;
	Sprite playerVineSprite;
	Sprite dust;
};