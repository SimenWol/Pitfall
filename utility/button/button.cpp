#include "precomp.h"
#include "button.h"
#include "sprite.h"

Button::Button(Sprite& sprite_in, const int2 loc)
	:location(loc)
	,sprite(sprite_in)
{
	dimensions.y = sprite.GetHeight();
	dimensions.x = sprite.GetWidth();
}
void Button::SetLocation(const int2 loc) { location = loc; }

bool Button::IsPressed(const float2 mousePos, const bool mouseDown)
{
	if (CheckMousePos(mousePos) == false) // Check mouse pos
	{
		prevMouseDown = false;
		return false;
	}
	if (CheckMouseDown(mouseDown) == false) { return false; } // Check mouse down

	return true; // Returns true if all of the above checks out
}

void Button::Draw(Surface* screen, const float2 mousePos)
{
	if (sprite.Frames() == 3)
	{
		if (CheckMousePos(mousePos) && prevMouseDown) { sprite.SetFrame(2); }
		else if (CheckMousePos(mousePos)) { sprite.SetFrame(1); }
		else { sprite.SetFrame(0); }
	}

	sprite.Draw(screen, location.x, location.y);
}

bool Button::CheckMousePos(const float2 mousePos) const
{
	if (mousePos.x < location.x || mousePos.x > (location.x + dimensions.x)) { return false; }
	if (mousePos.y < location.y || mousePos.y > (location.y + dimensions.y)) { return false; }

	return true;
}

bool Button::CheckMouseDown(const bool mouseDown)
{
	if (mouseDown)
	{
		prevMouseDown = true;
		return false;
	}
	else
	{
		if (prevMouseDown == true)
		{
			prevMouseDown = false;
			return true;
		}
		else
		{
			prevMouseDown = false;
			return false;
		}
	}
}
