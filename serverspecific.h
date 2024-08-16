#ifndef SERVERSPECIFIC_H__
#define SERVERSPECIFIC_H__

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <Cfg.h>

struct EntityStateBase
{
	float xpos;
	float ypos;
	sf::IntRect texRect;
	Cfg::Textures textureId;
	int entityId;
};

#endif