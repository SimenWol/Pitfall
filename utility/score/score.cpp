#include "precomp.h"
#include "score.h"
#include "sprite.h"

Score::Score()
	:numbers(new Surface("assets/UI/numbers.png"), 10)
{
	UpdateScore();
}

// Currently only supports numbers up to 9999
void Score::Draw(Surface* screen, const int2 location)
{
	int xOffset = 0;

	if (numbersSplit[0] != 0)
	{
		numbers.SetFrame(numbersSplit[0]);
		numbers.Draw(screen, location.x, location.y);
		xOffset += numbers.GetWidth() + 5;
	}
	if (xOffset != 0 || numbersSplit[1] != 0)
	{
		numbers.SetFrame(numbersSplit[1]);
		numbers.Draw(screen, location.x + xOffset, location.y);
		xOffset += numbers.GetWidth() + 5;
	}
	if (xOffset != 0 || numbersSplit[2] != 0)
	{
		numbers.SetFrame(numbersSplit[2]);
		numbers.Draw(screen, location.x + xOffset, location.y);
		xOffset += numbers.GetWidth() + 5;
	}
	if (xOffset != 0 || numbersSplit[3] != 0)
	{
		numbers.SetFrame(numbersSplit[3]);
		numbers.Draw(screen, location.x + xOffset, location.y);
		xOffset += numbers.GetWidth() + 5;
	}

	numbers.SetFrame(numbersSplit[4]);
	numbers.Draw(screen, location.x + xOffset, location.y);
}

void Score::Reset()
{
	currentScore = 2000;

	UpdateScore();
}

void Score::AddScore(const int score)
{
	currentScore += score;
	
	UpdateScore();
}

void Score::RemoveScore(const int score)
{
	if (currentScore == 0) { return; }

	if (currentScore - score < 0) { currentScore = 0; }
	else { currentScore -= score; }
	
	UpdateScore();
}

void Score::UpdateScore()
{
	numbersSplit[4] = currentScore % 10;
	numbersSplit[3] = ((currentScore - numbersSplit[4]) / 10) % 10;
	numbersSplit[2] = ((currentScore - numbersSplit[4] - numbersSplit[3] * 10) / 100) % 10;
	numbersSplit[1] = ((currentScore - numbersSplit[4] - numbersSplit[3] * 10 - numbersSplit[2] * 100) / 1000) % 10;
	numbersSplit[0] = ((currentScore - numbersSplit[4] - numbersSplit[3] * 10 - numbersSplit[2] * 100 - numbersSplit[1] * 1000) / 10000) % 10;
}
