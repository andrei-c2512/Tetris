#pragma once
#include "Board.h"
#include "Font.h"
#include "Figure.h"

class ScoreBoard {
public:
	ScoreBoard(Board& board , TetronimoDisplayer& displayer);
	void DrawScore(Graphics& gfx, Camera& ct);
	void UpdateScore();
	void Reset();
	static constexpr int ScoreAmplifier = 4;
private:
	Board& brd;
	int Score;
	int LinesCompleted;
	Vec2I pos;
	Font font = { "Fixedsys16x28.bmp" };
};