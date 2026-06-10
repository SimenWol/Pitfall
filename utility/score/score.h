#pragma once

class Score
{
public:
	Score();

	void Draw(Surface* screen, const int2 location);
	void Reset();
	void AddScore(const int score);
	void RemoveScore(const int score);
private:
	void UpdateScore();
private:
	Sprite numbers;

	int currentScore = 2000;
	int numbersSplit[5] = { 0, 0, 0, 0, 0 };
};