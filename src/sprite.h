/*
	sprite.h nachocpol@gmail.com
*/

#pragma once

#include "SFML/Graphics.hpp"

class Sprite
{
	friend class Renderer;
	friend class Button;
public:
	Sprite(const char* path);
	~Sprite();

private:
	sf::Sprite mSfSprite;
	sf::Texture* mTexture;
};
