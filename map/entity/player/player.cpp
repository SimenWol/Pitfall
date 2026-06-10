#include "precomp.h"
#include "map/entity/player/player.h"
#include "map/tilemap.h"

Player::Player()
		:playerSprite(new Surface("assets/Player/Character_sheet.png"), 18)
		,playerVineSprite(new Surface("assets/Player/Character_vine_sheet.png"), 2)
		,dust(new Surface("assets/Player/Dust.png"), 8)
	{
		// Initialise playerSprite dimensions
		dimensions.x = playerSprite.GetWidth();
		dimensions.y = playerSprite.GetHeight();
	}

void Player::Draw(Surface* screen, const float2 offset, const int fewerLines)
{
	if (isHanging) { playerVineSprite.Draw(screen, static_cast<int>(playerLoc.x - offset.x), static_cast<int>(playerLoc.y - offset.y) + fewerLines, fewerLines); }
	else { playerSprite.Draw(screen, static_cast<int>(playerLoc.x - offset.x), static_cast<int>(playerLoc.y - offset.y) + fewerLines, fewerLines); }

	// Dust particles
	if (dustActive) { dust.Draw(screen, static_cast<int>(dustLoc.x - offset.x), static_cast<int>(dustLoc.y - offset.y)); }
}

void Player::Move(Tilemap* map, float2 delta_loc, const float& deltaTime)
{
	if (!isFalling && !isJumping)
	{
		playerSpeed.x = 0.0f;
		playerSpeed.y = 0.0f;
	}

	// Check if player can start climbing a ladder
	if (!isClimbing && delta_loc.y > 0.0f) { StartClimbing(map, true); }
	else if (!isClimbing && delta_loc.y < 0.0f) { StartClimbing(map, false); }

	// Update grace timers
	if (isJumping)
	{
		graceTimer -= deltaTime;
		hangTimer -= deltaTime;

		if (graceTimer < 0.0f) { graceTimer = 0.0f; }
		if (hangTimer < 0.0f) { hangTimer = 0.0f; }
	}

	// Check if player can fall
	if (isOnAlligator) { isJumping = false; isFalling = false; canMove = true; }
	else if (!isClimbing) { CheckBelow(map); }

	// Walking
	if (delta_loc.x != 0.0f && !isClimbing && !isFalling && !isJumping && !isHanging && canMove)
	{
		// Set player facing position when walking
		if (delta_loc.x > 0.0f) { isFacingRight = true; }
		else if (delta_loc.x < 0.0f) { isFacingRight = false; }

		// Set walking speed
		if (!CheckSide(map))
		{
			isWalking = true;
			playerSpeed.x = delta_loc.x * 200.0f * deltaTime;
		}
	}
	else if (!isJumping)
		{
			// Reset walking speed
			isWalking = false;
			playerSpeed.x = 0.0f;
		}

	// Check for player wanting to detach from rope
	if (isHanging)
	{
		if (delta_loc.y > 0.0f) { isDetaching = true; }
		return;
	}

	if (isJumping)
	{
		isWalking = false;

		if (!canMove)
		{
			playerSpeed.y += GRAVITY * deltaTime;
			playerLoc.y += playerSpeed.y;
			playerLoc.x += playerSpeed.x;
			canMove = true;
			AnimateCharacter(deltaTime);
			return;
		}

		if (CheckSide(map))
		{
			if (isFacingRight)
			{
				SetLoc({ playerLoc.x - ((static_cast<int>(playerLoc.x) + dimensions.x) % Tilemap::TILESIZE), playerLoc.y });

				isFacingRight = false;
			}
			else
			{
				SetLoc({ playerLoc.x + (static_cast<int>(playerLoc.x) % Tilemap::TILESIZE), playerLoc.y });

				isFacingRight = true;
			}

			playerSpeed.x = -playerSpeed.x;
		}
		playerSpeed.y += GRAVITY * deltaTime;
	}
	else if (isFalling)
	{
		isWalking = false;
		playerSpeed.y = 100.0f * deltaTime;

		if (!canMove) { playerLoc.y += playerSpeed.y; }
	}
	else if (!canMove)
	{
		canMove = true;
		AnimateCharacter(deltaTime);
		return;
	}
	else if (isClimbing)
	{
		isWalking = false;
		playerSpeed.x = 0.0f;
		playerSpeed.y = 0.0f;

		if (delta_loc.y != 0.0f) { Climb(static_cast<int>(delta_loc.y), map, deltaTime); }
		if (delta_loc.x != 0.0f) { DismountLadder(static_cast<int>(delta_loc.x), map, deltaTime); }
	}

	// Update Player Location.
	playerLoc.x += playerSpeed.x;
	playerLoc.y += playerSpeed.y;

	AnimateCharacter(deltaTime);
}

void Player::Jump()
{
	if (isJumping || isFalling || isClimbing || isDown) { return; }
	graceTimer = 0.1f;
	playerSpeed.y -= JUMPHEIGHT;
	isJumping = true;
}

void Player::Reset(const bool animationsOnly)
{
	if (!animationsOnly)
	{
		playerSpeed = 0.0f;
		isFacingRight = true;
		canMove = true;
	}

	dustActive = false;

	isWalking = false;
	isDown = false;
	isClimbing = false;
	isFalling = false;
	isJumping = false;
	isHanging = false;
	isOnAlligator = false;

	prevIsFalling = false;
	prevIsFalling = false;

	graceTimer = 0.0f;
	dustTimer = 0.0f;
	dustFrame = 0;
	climbTimer = 0.0f;
	climbingFrame = 0;
	timeCounter = 0.0f;
	currFrame = 0;

	AnimateCharacter();
}

void Player::SetLoc(const float2& new_loc)
{
	if (isOnAlligator)
	{
		isOnAlligator = false;
		if (graceTimer > 0.0f) { return; }
	}
	playerLoc.x = new_loc.x;
	playerLoc.y = new_loc.y;
}

float2 Player::GetLoc() const { return playerLoc; }

int2 Player::GetDimensions() const { return dimensions; }

bool Player::GetIsHanging() const { return isHanging; }

void Player::CheckHanging(float2 bottomLoc, float2 topLoc)
{
	if (isHanging)
	{
		if (isDetaching)
		{
			isJumping = true;
			isHanging = false;
			isDetaching = false;
			hangTimer = 0.25f;
			if (bottomLoc.x < topLoc.x)
			{
				playerSpeed.x = -DETACHSPEED;
				playerSpeed.y = -DETACHSPEED * 0.5f;
			}
			else
			{
				playerSpeed.x = DETACHSPEED;
				playerSpeed.y = -DETACHSPEED * 0.5f;
			}
		}
		else
		{
			playerLoc.x = bottomLoc.x - 23;
			playerLoc.y = bottomLoc.y - 5;
		}
	}
	else
	{
		if (hangTimer != 0.0f || !isJumping) { return; }
		if (bottomLoc.x < playerLoc.x || bottomLoc.x > playerLoc.x + dimensions.x) { return; }
		if (bottomLoc.y < playerLoc.y || bottomLoc.y > playerLoc.y + dimensions.y) { return; }

		isHanging = true;
		AnimateCharacter();

		playerLoc.x = bottomLoc.x - 23;
		playerLoc.y = bottomLoc.y - 5;
	}
}

bool Player::FallenInPit()
{
	if (isJumping) { prevIsJumping = true; }
	if (!isJumping && prevIsJumping)
	{
		prevIsJumping = false;
		dustLoc.x = playerLoc.x;
		dustLoc.y = playerLoc.y + dimensions.y - dust.GetHeight();
		dustActive = true;
	}

	if (prevIsFalling)
	{
		if (!isFalling) { prevIsFalling = false; }

		return false;
	}
	
	if (isFalling)
	{
		prevIsFalling = true;
		return true;
	}

	return false;
}

void Player::IsOnAlligator() { isOnAlligator = true; }

void Player::Down(const bool allowedToMove)
{
	isDown = true;

	if (!allowedToMove)
	{
		canMove = false;
	}
}

void Player::AnimateCharacter(const float deltaTime)
{
	if (dustActive)
	{
		if (isHanging) { dustFrame = 8; } // Make sure it exits this loop iteration

		dustTimer += deltaTime;
		while (dustTimer >= DUSTFRAMETIME)
		{
			dustTimer -= DUSTFRAMETIME;
			dustFrame++;
		}

		if (dustFrame >= 8)
		{
			dustFrame = 0;
			dustActive = false;
		}

		dust.SetFrame(dustFrame);
	}

	if (isHanging)
	{
		if (isFacingRight) { playerVineSprite.SetFrame(0); }
		else { playerVineSprite.SetFrame(1); }
	}
	else if (isJumping)
	{
		if (isFacingRight) { playerSprite.SetFrame(1); }
		else { playerSprite.SetFrame(9); }
	}
	else if (isDown)
	{
		if (isFacingRight) { playerSprite.SetFrame(7); }
		else { playerSprite.SetFrame(15); }
		isDown = false; // Reset down variable for next iteration
	}
	else if (isClimbing)
	{
		if (climbingFrame > 1) { climbingFrame = 0; }
		playerSprite.SetFrame(climbingFrame + 16);
	}
	else if (isWalking)
	{
		// Update framecounter for walking animation.
		timeCounter += deltaTime;
		if (timeCounter > FRAMETIME)
		{
			timeCounter = 0.0f;
			currFrame++;
			if (currFrame == 5) { currFrame = 0; }
		}

		if (isFacingRight) { playerSprite.SetFrame(currFrame + 2); } // Walking - Facing Right
		else { playerSprite.SetFrame(currFrame + 10); } // Walking - Facing Left
	}
	else
	{
		currFrame = 0;
		climbingFrame = 0;
		timeCounter = 0.0f;

		if (isFacingRight) { playerSprite.SetFrame(0); } // Idle - Facing Right
		else { playerSprite.SetFrame(8); } // Idle - Facing Left
	}
}

void Player::StartClimbing(Tilemap* map, const bool goingDown)
{
	if (isClimbing || isFalling || isJumping || isDown || isHanging) { return; } // If not in a walking state, player is not allowed to start climbing.

	const int leftX = static_cast<int>(playerLoc.x) - Tilemap::TILESIZE;
	const int midX = static_cast<int>(playerLoc.x + 0.5f * dimensions.x);
	const int rightX = static_cast<int>(playerLoc.x + dimensions.x) + Tilemap::TILESIZE;

	if (goingDown) // Check tiles below feet for a ladder
	{
		const int bottomY = static_cast<int>(playerLoc.y) + dimensions.y + Tilemap::TILESIZE - 1; // 1 pixel correction

		if (map->GetContents({ leftX, bottomY }) == Tilemap::TileContents::Ladder)
		{
			// Place sprite on ladder
			playerLoc.x = (leftX - (leftX % Tilemap::TILESIZE)) + Tilemap::TILESIZE / 2 - static_cast<float>(dimensions.x) / 2;
			playerLoc.y = static_cast<float>(bottomY) - bottomY % Tilemap::TILESIZE;

			isClimbing = true;
		}
		else if (map->GetContents({ midX, bottomY }) == Tilemap::TileContents::Ladder)
		{
			// Place sprite on ladder
			playerLoc.x = (midX - (midX % Tilemap::TILESIZE)) + Tilemap::TILESIZE / 2 - static_cast<float>(dimensions.x) / 2;
			playerLoc.y = static_cast<float>(bottomY) - bottomY % Tilemap::TILESIZE;

			isClimbing = true;
		}
		else if (map->GetContents({ rightX, bottomY }) == Tilemap::TileContents::Ladder)
		{
			// Place sprite on ladder
			playerLoc.x = (rightX - (rightX % Tilemap::TILESIZE)) + Tilemap::TILESIZE / 2 - static_cast<float>(dimensions.x) / 2;
			playerLoc.y = static_cast<float>(bottomY) - bottomY % Tilemap::TILESIZE;

			isClimbing = true;
		}
	}
	else // Check tiles at head height for a ladder
	{
		const int topY = static_cast<int>(playerLoc.y);

		if (map->GetContents({ leftX, topY }) == Tilemap::TileContents::Ladder)
		{
			// Place sprite on ladder
			playerLoc.x = (leftX - (leftX % Tilemap::TILESIZE)) + Tilemap::TILESIZE / 2 - static_cast<float>(dimensions.x) / 2;
			playerLoc.y = static_cast<float>(topY) - topY % Tilemap::TILESIZE + Tilemap::TILESIZE - (dimensions.y - Tilemap::TILESIZE); // loc - remainder tile + tile offset - offset due to playersize

			isClimbing = true;
		}
		else if (map->GetContents({ midX, topY }) == Tilemap::TileContents::Ladder)
		{
			// Place sprite on ladder
			playerLoc.x = (midX - (midX % Tilemap::TILESIZE)) + Tilemap::TILESIZE / 2 - static_cast<float>(dimensions.x) / 2;
			playerLoc.y = static_cast<float>(topY) - topY % Tilemap::TILESIZE + Tilemap::TILESIZE - (dimensions.y - Tilemap::TILESIZE);

			isClimbing = true;
		}
		else if (map->GetContents({ rightX, topY }) == Tilemap::TileContents::Ladder)
		{
			// Place sprite on ladder
			playerLoc.x = (rightX - (rightX % Tilemap::TILESIZE)) + Tilemap::TILESIZE / 2 - static_cast<float>(dimensions.x) / 2;
			playerLoc.y = static_cast<float>(topY) - topY % Tilemap::TILESIZE + Tilemap::TILESIZE - (dimensions.y - Tilemap::TILESIZE);

			isClimbing = true;
		}
	}
}

void Player::Climb(const int delta, Tilemap* map, const float deltaTime)
{
	if (delta == -1 && map->GetContents({ static_cast<int>(playerLoc.x), static_cast<int>(playerLoc.y - 1) }) == Tilemap::TileContents::Ladder)
	{
		// Update climb timer
		climbTimer += deltaTime;
		if (climbTimer > CLIMBSTEPTIME)
		{
			climbTimer -= CLIMBSTEPTIME;
			climbingFrame++;

			// Update player loc
			playerLoc.y -= 0.25 * Tilemap::TILESIZE;
			if (map->GetContents({ static_cast<int>(playerLoc.x), static_cast<int>(playerLoc.y) }) != Tilemap::TileContents::Ladder)
			{
				playerLoc.y = playerLoc.y - (static_cast<int>(playerLoc.y) % Tilemap::TILESIZE) + Tilemap::TILESIZE - 2;
			}
		}
	}
	else if (delta == 1 && map->GetContents({ static_cast<int>(playerLoc.x), static_cast<int>(playerLoc.y + dimensions.y + 1) }) == Tilemap::TileContents::Ladder)
	{
		// Update climb timer
		climbTimer += deltaTime;
		if (climbTimer > CLIMBSTEPTIME)
		{
			climbTimer -= CLIMBSTEPTIME;
			climbingFrame++;

			// Update player loc
			playerLoc.y += 0.25 * Tilemap::TILESIZE;
			if (map->GetContents({ static_cast<int>(playerLoc.x), static_cast<int>(playerLoc.y + dimensions.y) }) != Tilemap::TileContents::Ladder)
			{
				playerLoc.y = playerLoc.y - (static_cast<int>(playerLoc.y + dimensions.y) % Tilemap::TILESIZE) - 1;
			}
		}
	}
}

void Player::DismountLadder(const int delta, Tilemap* map, const float deltaTime)
{
	if (graceTimer != 0.0f) { return; }

	// Top
	if (map->GetContents({ static_cast<int>(playerLoc.x), static_cast<int>(playerLoc.y - 1) }) != Tilemap::TileContents::Ladder)
	{
		// Set player direction for animations
		if (delta > 0.0f) { isFacingRight = true; }
		else { isFacingRight = false; }

		// Set player speed
		isClimbing = false;
		playerSpeed.x = delta * 200.0f * deltaTime;
		playerLoc.y -= 30.0f;							// Add some extra height before jumping off in order to not clip the tiles next to the ladder.

		Jump();
	}

	// Bottom
	else if (map->GetContents({ static_cast<int>(playerLoc.x), static_cast<int>(playerLoc.y + dimensions.y + 1) }) != Tilemap::TileContents::Ladder)
	{
		isClimbing = false;
	}
}

void Player::CheckBelow(Tilemap* map)
{
	if (graceTimer != 0.0f) { return; }

	const int leftX = static_cast<int>(playerLoc.x);
	const int rightX = static_cast<int>(playerLoc.x + dimensions.x);
	const int bottomY = static_cast<int>(playerLoc.y + dimensions.y);
	
	if (isOnAlligator) { isJumping = false; isFalling = false; }
	else if (map->GetContents({ leftX, bottomY }) == Tilemap::TileContents::Obstacle
		|| map->GetContents({ rightX, bottomY }) == Tilemap::TileContents::Obstacle)
	{
		// Let player know that there is a block below.
		isJumping = false;
		isFalling = false;
	}
	else if ((map->GetContents({ leftX, bottomY }) == Tilemap::TileContents::ObstacleBottomHalf
		|| map->GetContents({ rightX, bottomY }) == Tilemap::TileContents::ObstacleBottomHalf)
		&& (static_cast<int>(playerLoc.y + dimensions.y)) % Tilemap::TILESIZE >= 32)
	{
		// Let player know that there is a block below.
		isJumping = false;
		isFalling = false;
	}
	else if (!isJumping && !isHanging) { isFalling = true; } // Set falling to true if player is not jumping
}

bool Player::CheckSide(Tilemap* map)
{
	const int topY = static_cast<int>(playerLoc.y) + 3;
	const int bottomY = static_cast<int>(playerLoc.y + dimensions.y) - 3;

	if (isFacingRight)
	{
		const int rightX = static_cast<int>(playerLoc.x + dimensions.x);
		return rightX > Tilemap::TILESIZE * Tilemap::MAPWIDTH
			|| map->GetContents({ rightX, topY }) == Tilemap::TileContents::Obstacle
			|| map->GetContents({ rightX, bottomY }) == Tilemap::TileContents::Obstacle;
	}
	else
	{
		const int leftX = static_cast<int>(playerLoc.x) - 1;
		return leftX < 0
			|| map->GetContents({ leftX, topY }) == Tilemap::TileContents::Obstacle
			|| map->GetContents({ leftX, bottomY }) == Tilemap::TileContents::Obstacle;
	}
}