/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "FrameTimer.h"
#include "Font.h"
#include <string>
#include "Bench.h"
#include "Camera.h"
#include "CoordinateShower.h"
#include "Board.h"
#include "Figure.h"
#include "ScoreBoard.h"
#include "Background.h"
class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	~Game();
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
	enum class State {
		AboutToStart,
		Running,
		Over
	};
private:
	MainWindow& wnd;

	Graphics gfx;
	CoordinateTransformer ct;
	CoordinateShower cs;
	Camera cam;

	Board brd;
	TetronimoDisplayer tDisplayer;
	Player player;
	ScoreBoard score_brd;
	ParticleEffect particle_effect;

	FrameTimer Timer;
	FrameShower FPS;
	Sprite sprite;
	MouseLastFrameStats MouseStats;
	Bench bench;
	Background background;

	Vec2I SpeedButton_pos = Vec2I(Graphics::ScreenWidth - 60 , 60);

	float GameSpeed = 3.0f;
	State state = State::AboutToStart; 

	Font font = { "Fixedsys16x28.bmp" };

	std::string StartText;
	Vec2I StartTextPos;

	std::string GameOverText;
	Vec2I GameOverTextPos;
	//GameSpeedButton GameSpeed_button = { Colors::Gray , Colors::White ,
	//	RectI(30 , 60 , Vec2I(SpeedButton_pos)) };

	/********************************/
	/*  User Variables              */
	/********************************/
};