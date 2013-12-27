#include <hge.h>
#include <hgesprite.h>
#include <hgevector.h>
#include <hgefont.h>
#include <hgeresource.h>

#include "player.h"
#include "bot.h"
#include <ctime>
#include <vector>
#include <list>
#include <iostream>
#include <string>
using namespace std;
#define N 4//number of cars
HGE* hge = NULL;
hgeResourceManager* myRes;
c_player*   Player1 = NULL;
hgeFont* font1;

#define BACKGROUND_SCROLLING_SPEED  0.305f//скорость прокрутки кадра
void Save();
void Load();
void StartNew();
bool L_RenderFunc();
bool L_FrameFunc();
bool W_FrameFunc();
bool W_RenderFunc();
bool FrameFunc();
bool RenderFunc();

//Background
HTEXTURE tBackground = 0;
HTEXTURE tLose_Background = 0;
HTEXTURE tWon_Background = 0;

vector<Bot*> bot;

hgeSprite* sBackground = NULL;//анимация
hgeSprite* sLose_Background=NULL;
hgeSprite* sWon_Background=NULL;


hgeVector  vBGPosition = hgeVector(0,0);


int meters=0, a=0, /*буфер замедления прироста метров*/ lvl=1, lives=3;
float dt=0;
float freq=0.9;

bool L_RenderFunc(){

	hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);
	sLose_Background->Render(0,0);
	font1->SetScale(1.2);
	font1->SetColor(ARGB(255,0,191,255));
	font1->printf(5,570, HGETEXT_LEFT,"ESC=EXIT");
	hge->Gfx_EndScene();
	return false;
}

bool L_FrameFunc()
{
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
    return false;
}

bool W_FrameFunc()
{	
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
    return false;
}

bool W_RenderFunc(){

	hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);
	sWon_Background->Render(0,0);
	hge->Gfx_EndScene();
	return false;
}


bool FrameFunc()
{
	
	float delta = hge->Timer_GetDelta();
	//Background
	//меняем функцию кадра
	if(vBGPosition.x > -1782) vBGPosition.x -= BACKGROUND_SCROLLING_SPEED;
	else vBGPosition = hgeVector(0,0);
	
	//Player
	Player1->Update(delta);
	::dt+=hge->Timer_GetDelta();
	if(dt>freq){
		dt=0;

		bot.push_back(new Bot(hgeVector(900,rand()%600+40), hgeVector(5,0),"images/bot.png",hgeVector(124,65)));
	}
	
	for(vector<Bot*>::iterator it=bot.begin();it!=bot.end();)
	{
		if((*it)->GetBoundingBox().Intersect(&Player1->GetBoundingBox()))
		{
			delete (*it);
			it = bot.erase(it);
			Player1->SetPosition(hgeVector(100,268));
			Player1->SetVelocity(hgeVector(0,0));
			//it++;
			lives--;
			if(lives==0)
			{
				hge->System_SetState(HGE_FRAMEFUNC, L_FrameFunc);
				hge->System_SetState(HGE_RENDERFUNC, L_RenderFunc);

			}
			
		}
		else it++;

	}

	for(int i=0;i<bot.size();i++){
		int count=0;
		if(!bot[i]->Update(delta)){
			for(vector<Bot*>::iterator j=bot.begin();j!=bot.end();j++,count++)
				if(count==i){
					bot.erase(j);
					break;
				}
		
		}
	}
	font1->SetScale(1.2); //set text size
	font1->SetColor(ARGB(255,139,9,80));  //set color of text
	a++;
	if(a==50)
	{
		meters++;
		a=0;
		if(meters%1000==0)//lvl up
		{
			freq-=0.2;
			lvl++;
			if(lvl==4)
			{
				hge->System_SetState(HGE_FRAMEFUNC, W_FrameFunc);
				hge->System_SetState(HGE_RENDERFUNC, W_RenderFunc);
			}
		}
	}
 	if (hge->Input_GetKey()==HGEK_SPACE)
	{
		Save();
	}
	if (hge->Input_GetKey()==HGEK_N)
	{
		StartNew();
	}
	//проверяет был ли нажат escape
    if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
    return false;
}
 
bool RenderFunc()
{
    hge->Gfx_BeginScene();
    hge->Gfx_Clear(0);
	//Background
	sBackground->Render(vBGPosition.x,vBGPosition.y);
	if(vBGPosition.x < -982)
		sBackground->Render(vBGPosition.x + 982 + 800, 0);
    

	//Player
	Player1->Render();
	//font
	font1->SetScale(1.2); //set text size
	font1->SetColor(ARGB(255,0,238,0));  //set color of text
	font1->printf(5, 5, HGETEXT_LEFT, "%dm", meters); //render text using printf-style formatting 
	font1->SetColor(ARGB(255,255,20,147)); 
	font1->printf(790,5, HGETEXT_RIGHT,"Level: %d",lvl);
	font1->SetColor(ARGB(255,0,191,255)); 
	font1->printf(5,35, HGETEXT_LEFT,"Lives: %d",lives);
	font1->SetColor(ARGB(255,255,130,71));
	font1->printf(5,570, HGETEXT_LEFT,"Space=Save");
	font1->printf(5,540, HGETEXT_LEFT,"N=New");


	//bot
	for(int i=0;i<bot.size();i++)
		bot[i]->Render();

 	

    hge->Gfx_EndScene();
    return false;
}


void Save()
{
	FILE* saves;
	if((saves =fopen("saves.txt", "w"))!=NULL);
	{
		fprintf(saves,"%d ", meters);
		fprintf(saves, "%d ", a);
		fprintf(saves, "%d ", lvl);
		fprintf(saves, "%d ", lives);
		fprintf(saves, "%f ", dt);
		fprintf(saves, "%f ", freq);
		fclose(saves);
	}
}



void Load()
{
	FILE* saves;
	if((saves =fopen("saves.txt", "r"))==0)
		return;
	else
	{
		fscanf(saves,"%d", &meters);
		fscanf(saves, "%d", &a);
		fscanf(saves, "%d", &lvl);
		fscanf(saves, "%d", &lives);
		fscanf(saves, "%f", &dt);
		fscanf(saves, "%f", &freq);
		fclose(saves);
	}
}

void StartNew()
{
	meters=0;
	a=0;
	lvl=1;
	lives=3;
	dt=0;
	freq=0.9;
	Player1->SetPosition(hgeVector(100,268));
	Player1->SetVelocity(hgeVector(0,0));
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	
    hge = hgeCreate(HGE_VERSION);
	hge->System_SetState(HGE_FPS, 700);
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_USESOUND, false);
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
    hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
   // hge->System_SetState(HGE_FPS, HGEFPS_VSYNC);
    hge->System_SetState(HGE_TITLE, "My first game");
	srand(hge->Timer_GetTime());
	
	
	if(hge->System_Initiate())
	{
		Load();
		
		myRes = new hgeResourceManager("Resource08.res");
		//Background
		tBackground = hge->Texture_Load("images/asfalt.jpg");

		sBackground = new hgeSprite(tBackground, 0, 0, 1782, 600);

		tLose_Background = hge->Texture_Load("images/carlose.jpg");

		sLose_Background = new hgeSprite(tLose_Background, 0, 0, 800, 622);

		tWon_Background = hge->Texture_Load("images/youwon.jpg");

		sWon_Background = new hgeSprite(tWon_Background, 0, 0, 800, 600);

		font1 = myRes->GetFont("font1");

		vBGPosition = hgeVector(0, 0);//1782 - 800 = 982

		Player1 = new c_player(hgeVector(10, 268), hgeVector(5,0),"images/player.png",hgeVector(124,61));

		bot.push_back(new Bot(hgeVector(900, rand()%600+40), hgeVector(5,0),"images/bot.png",hgeVector(124,65)));//700
		
		

		hge->System_Start();
		
		//Cleanup..
		 hge->Texture_Free(tBackground);
		 hge->Texture_Free(tLose_Background);
		 hge->Texture_Free(tWon_Background);
		 delete sBackground;
		 delete sLose_Background;
		 delete sWon_Background;
		 delete Player1;
		 delete myRes;

		for(vector<Bot*>::iterator k=bot.begin();k!=bot.end();k++)
		{
			delete (*k);
		}
		bot.clear();
	}
	/*else
	{	
    MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}*/
	

	hge->System_Shutdown();
    hge->Release();
 
    return 0;
}
