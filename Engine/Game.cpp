/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"
#include <sstream>
#include <fstream>
#include <assert.h>
Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	ct(gfx),
	cam(ct),
	cs(cam),
	Timer(),
	player(brd , tDisplayer),
	score_brd(brd , tDisplayer),
	brd(particle_effect)
{
	StartText = "Press SPACE to start";
	StartTextPos = Vec2I(-StartText.length() * font.GetLetterDim().width / 2, -Graphics::ScreenHeight / 2 + font.GetLetterDim().height);

	GameOverText = "Game over! Press SPACE to restart";
	GameOverTextPos = Vec2I(-GameOverText.length() * font.GetLetterDim().width / 2, -Graphics::ScreenHeight / 2 + font.GetLetterDim().height);
}

Game::~Game() {
}
void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	float dt = Timer.DeltaTime();



	if (state == State::AboutToStart)
	{
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			state = State::Running;
		}
	}
	else if (state == State::Running)
	{
		player.UpdateMovement(brd, wnd.kbd, dt);
		player.DetermineFuturePos(brd);
		score_brd.UpdateScore();
		brd.CheckForFullLines();
		brd.UpdateBlocks(dt);

		if (brd.LineHasType(Cell::Status::InactiveBlock, Board::rows - 1))
		{
			state = State::Over;
		}
	}
	else if (state == State::Over)
	{
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			brd.Reset();
			score_brd.Reset();
			state = State::AboutToStart;
		}
	}
	particle_effect.Update(dt);
	MouseStats.Update(wnd.mouse);
}

void Game::ComposeFrame()
{
	background.Draw(gfx, cam);
	brd.DrawBoard(gfx, cam);
	player.ShowNextFigure(gfx, cam);
	player.ShowFuturePosition(gfx, cam);
	if (state == State::AboutToStart)
	{
		font.DrawWords(StartText, gfx, ct, StartTextPos);
	}
	else if (state == State::Over)
	{
		font.DrawWords(GameOverText, gfx, ct, GameOverTextPos);
	}
	score_brd.DrawScore(gfx, cam);
	particle_effect.Draw(gfx, cam);
}

