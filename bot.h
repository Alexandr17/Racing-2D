#ifndef BOT_H
#define BOT_H
#pragma once

#include <hge.h>
#include <hgevector.h>
#include <hgeanim.h>
#include "player.h"
class Bot:public c_player
{
public:
	Bot(){}
	Bot(hgeVector Position, hgeVector Velocity,char* texturename,hgeVector sizeTexture):c_player(Position,Velocity,texturename,sizeTexture)
	{
	}
	~Bot()
	{
	}
	bool Update(float delta)
	{
	
		if(Position.x < 0)   {
			

			Sprite->Update(delta);
			return false;
		}
	
		Velocity.x *= 0.95;
		Position.x += Velocity.x;
		Velocity.x -= (Speed * M_PI) /2;
		Sprite->Update(delta);
	
		Sprite->GetBoundingBox(Position.x, Position.y, &BoundingBox);
		return true;
	}
private:
	
};

#endif
