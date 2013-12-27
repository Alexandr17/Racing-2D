#ifndef C_PLAYER_H
#define C_PLAYER_H
#pragma once
 
#include <hge.h>
#include <hgevector.h>
#include <hgeanim.h>
 
#define PLAYER_FRICTION 0.95

class c_player
{
protected:
    static HGE*     hge;
    hgeVector       Position;
    hgeVector       Velocity;
    HTEXTURE        Texture;
    hgeAnimation*   Sprite;
    hgeRect         BoundingBox; //необходимо  для обнаружения столкновений
    float           Speed;
public:
	c_player(){}
	c_player(const c_player & player){}
	 c_player(hgeVector Position, hgeVector Velocity,char* textureName,hgeVector sizeTexture);//конструктор
    ~c_player();//деструктор
 
    bool Update(float delta);
    void Render();
	
    void        SetVelocity(hgeVector velocity) { Velocity = velocity; };
    void        SetPosition(hgeVector position) { Position = position; };
		
	void SetAnim(HGE* hge, const char* textureName, hgeVector sizeTexture){
		Texture = hge->Texture_Load(textureName);
		Sprite = new hgeAnimation(Texture,1,1,0,0,sizeTexture.x,sizeTexture.y);
		Sprite->SetHotSpot(sizeTexture.x/2,sizeTexture.y/2);
		Sprite->Play();
	}
    hgeRect     GetBoundingBox() { return BoundingBox; };
    hgeVector   GetPosition() { return Position; };
};


#endif
