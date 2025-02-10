#include "ScoreBoard.h"

ScoreBoard::ScoreBoard(Board& brd0, TetronimoDisplayer& displayer)
	:brd(brd0)
{
	pos.x = Board::pos.x + Board::columns * Cell::dim.width;
	pos.y = Board::pos.y + Board::rows * Cell::dim.height - font.GetLetterDim().height;
	pos.y -= displayer.GetRect().height;

	Score = 0;
	LinesCompleted = 0;
}

void ScoreBoard::UpdateScore()
{
	int lines = 0;
	for (unsigned int y = 0; y < Board::rows; y++)
	{
		if (brd.LineIsFull(Cell::Status::InactiveBlock, y))
		{
			lines++;
		}
	}

	if (lines != 0)
	{
		Score += pow(ScoreAmplifier, lines);
		LinesCompleted += lines;
	}
}

void ScoreBoard::DrawScore(Graphics& gfx, Camera& cam)
{
	font.DrawWords("Score: " + std::to_string(Score), gfx, cam, pos);
	font.DrawWords("LinesCompleted: " + std::to_string(LinesCompleted), gfx, cam, Vec2I(pos.x , pos.y - font.GetLetterDim().height));
}

void ScoreBoard::Reset() {
	Score = LinesCompleted = 0;
}