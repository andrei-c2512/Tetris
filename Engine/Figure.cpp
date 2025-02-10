#include "Figure.h"

Figure::Figure( std::vector<Vec2I> blocks0 , Vec2I pos0)
	:blocks(blocks0) ,pos(pos0)
{
	RNG color_rng = { 0 , 8 };

	inner = Cell::ColorRange[color_rng.GetVal()];

	int bR = int(float(float(inner.GetR()) * ((100.0f - Cell::DarkeningPercentage) / 100.0f))),
		bG = int(float(float(inner.GetG()) * ((100.0f - Cell::DarkeningPercentage) / 100.0f))),
		bB = int(float(float(inner.GetB()) * ((100.0f - Cell::DarkeningPercentage) / 100.0f)));

	border = Color(bR, bG, bB);
}

bool Figure::InBounds(Vec2I offset = { 0 , 0}) const {
	RectI HitBox = RectI::GetRect(blocks);
	
	HitBox.MoveBy(offset + pos);

	return (HitBox.right() < Board::columns) && (HitBox.left >= 0) && (HitBox.top() < Board::rows) && (HitBox.bottom >= 0);
}

void Figure::PutIntoMatrix(Board& brd)
{
	for (Vec2I point : blocks)
	{
		brd.MakeBlock(pos + point, inner, border);
	}
}
void Figure::RemoveFromMatrix(Board& brd)
{
	for (Vec2I point : blocks)
	{
		brd.MakeEmpty(pos + point);
	}
}
bool Figure::Move(Keyboard& kbd, Board& brd)
{
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		Vec2I offset = Vec2I(-1, 0);
		if (InBounds(offset) && CheckCollision(brd, offset) == false)
		{
			MoveInMatrix(pos + offset , brd);
			pos.x -= 1;
			return true;
		}
	}
	else if (kbd.KeyIsPressed(VK_RIGHT))
	{
		Vec2I offset = Vec2I(1, 0);
		if (InBounds(offset) && CheckCollision(brd, offset) == false)
		{
			MoveInMatrix(pos + offset, brd);
			pos.x += 1;
			return true;
		}
	}

	return false;
}

bool Figure::Rotate(Keyboard& kbd, Board& brd)
{
	if (kbd.KeyIsPressed('R'))
	{
		std::vector<Vec2I> rotated_fig = blocks;
		for (Vec2I& block : rotated_fig)
		{
			block.Rotate(PI / 2.0f);
		}
		RectI rect = RectI::GetRect(rotated_fig);
		rect.MoveBy(pos);

		if (Board::InBounds(rect))
		{
			for (Vec2I& block : blocks)
			{
				brd.MakeEmpty(pos + block);
			}

			blocks = rotated_fig;

			PutIntoMatrix(brd);
			return true;
		}
	}
	return false;
}


bool Figure::MoveBy(Vec2I offset , Board& brd)
{
	if (InBounds(offset) && CheckCollision(brd, offset) == false)
	{
		MoveInMatrix(pos + offset, brd);
		pos += offset;
		return true;
	}
	return false;
}

Vec2I Figure::GetPos() const
{
	return pos;
}
void Figure::MakeAllInactive(Board& brd)
{
	for (const Vec2I& point : blocks)
	{
		brd.MakeInactive(point + pos);
	}
}
void Figure::MoveInMatrix(Vec2I new_pos , Board& brd)
{
	for (const Vec2I& point : blocks)
	{
		brd.MakeEmpty(point + pos);
	}
	for (Vec2I point : blocks)
	{
		brd.MakeBlock(point + new_pos , inner , border);
	}
}
void Figure::SetPos(Vec2I& pos0, Board& brd)
{
	pos = pos0;
	PutIntoMatrix(brd);
}
bool Figure::CheckCollision(Board& brd, Vec2I offset = { 0 , 0 })
{
	for (Vec2I point : blocks)
	{
		point += offset + pos;
		if (!Board::InBounds(point))
		{
			return true;
		}
		const Cell& cell = brd.GetCell(point);
		if (cell.GetStatus() == Cell::Status::InactiveBlock)
		{
			return true;
		}
	}
	return false;
}

std::vector<Vec2I> Figure::GetModel() const
{
	return blocks;
}
Color Figure::GetInnerColor() const
{
	return inner;
}
Color Figure::GetBorderColor() const
{
	return border;
}
Player::Player(Board& brd , TetronimoDisplayer& displayer0)
	:displayer(displayer0)
{
	future_tetronimo.resize(4, Vec2I(0, 0));
	pFig  = displayer.PassFigure();
	pFig->SetPos(SpawnLoc, brd);
}
void Player::Spawn(Board& brd)
{
	assert(pFig == nullptr);
	pFig = displayer.PassFigure();
	pFig->SetPos(SpawnLoc, brd);
}

void Player::ShowFuturePosition(Graphics& gfx, Camera& cam )
{
	if(future_tetronimo[0] != future_tetronimo[1])
	for (Vec2I block : future_tetronimo)
	{
		gfx.DrawRect_Border(RectI(Cell::dim, cam.Transform(Vec2I(block.x ,block.y + Cell::dim.height))), Colors::Green, Effects::Copy{});
	}
}
void Player::DetermineFuturePos(Board& brd)
{
	Vec2I pos = pFig->GetPos();
	
	std::vector<Vec2I> model = pFig->GetModel();

	//if (pos.x != future_pos.x)
	{
		for (unsigned int y = 0; y < Board::rows; )
		{
			if (pFig->CheckCollision(brd, Vec2I(0, -y)))
			{
				future_pos = pos;
				future_pos.y -= (y - 1);
				break;
			}
			else
			{
				y++;
			}
		}
		//right now im getting the matrix pos of the future tetronimo
		for (unsigned int index = 0; index < model.size(); index++)
		{
			future_tetronimo[index] = model[index] + future_pos;
		}

		for (Vec2I& pos : future_tetronimo)
		{
			unsigned int index = pos.y * Board::rows + pos.x;
			pos = brd.GetCell(pos).GetPos();
		}
	}
}

void Player::ShowNextFigure(Graphics& gfx, Camera& ct)
{
	displayer.Display(gfx, ct);
}
void Player::UpdateMovement(Board& brd , Keyboard& kbd , float time)
{
	move_timerY.Update(time);
	move_timerX.Update(time);
	rotation_timer.Update(time);
	drop_timer.Update(time);

	if(move_timerX.IsReady())
	{
		if (pFig->Move(kbd, brd) == false)
		{
			if (kbd.KeyIsPressed(VK_DOWN))
			{
				if (pFig->MoveBy(Vec2I(0, -1), brd) == false)
				{
					pFig->MakeAllInactive(brd);
					pFig.reset(nullptr);
					Spawn(brd);
				}
				else
				{
					move_timerX.ResetTimer();
				}
			}
		}
		else
		{
			move_timerX.ResetTimer();
		}
	}

	if (drop_timer.IsReady())
	{
		if (kbd.KeyIsPressed('Q'))
		{
			pFig->RemoveFromMatrix(brd);
			pFig->SetPos(future_pos, brd);
			pFig->MakeAllInactive(brd);
			pFig.reset(nullptr);
			Spawn(brd);
			drop_timer.ResetTimer();
		}
	}


	if (rotation_timer.IsReady())
	{
		if (pFig->Rotate(kbd , brd))
		{
			rotation_timer.ResetTimer();
		}
	}

	if (move_timerY.IsReady())
	{
		Vec2I dir = Vec2I(0, -1);

		if(pFig->MoveBy(dir , brd) == false)
		{
			pFig->MakeAllInactive(brd);
			pFig.reset(nullptr);
			Spawn(brd);
		}
		else
		{
			move_timerY.ResetTimer();
		}
	}
}

TetronimoDisplayer::TetronimoDisplayer()
{
	rows = 4;
	columns = 4;

	HitBox = RectI(BorderThickness * 2 + columns * Cell::dim.width,
		BorderThickness * 2 + rows * Cell::dim.height + font.GetLetterDim().height, pos);


	int StartY = pos.y - font.GetLetterDim().height - rows * Cell::dim.height - BorderThickness;
	int StartX = pos.x + BorderThickness;
	for (int y = StartY; y < StartY + rows * Cell::dim.height; y += Cell::dim.height)
	{
		for (int x = StartX; x < StartX + columns * Cell::dim.width; x += Cell::dim.width)
		{
			matrix.emplace_back(Vec2I(x, y));
		}
	}

	pNext = std::make_unique<Figure>(spawner.GetRandomTetromino(), MatrixLoc);
}

RectI TetronimoDisplayer::GetRect() const
{
	return HitBox;
}
void TetronimoDisplayer::Display(Graphics& gfx, Camera& cam)
{
	for (auto& cell : matrix)
	{
		cell.Draw(gfx, cam);
	}
	font.DrawWords("Next: ", gfx, cam, pos);

}

void TetronimoDisplayer::Update()
{
	assert(FigChanged == true);

    pNext = std::make_unique<Figure>(spawner.GetRandomTetromino(), MatrixLoc);
	
	auto& model  = pNext->GetModel();
	Color inner  = pNext->GetInnerColor();
	Color border = pNext->GetBorderColor();

	for (auto& block : model)
	{
		unsigned int index = (block.y + MatrixLoc.y) * columns + (block.x + MatrixLoc.x);

		matrix[index].MakeBlock(inner, border);
	}

	FigChanged = false;
}

std::unique_ptr<Figure> TetronimoDisplayer::PassFigure()
{
	FigChanged = true;

	auto& old_model = pNext->GetModel();

	for (auto& block : old_model)
	{
		unsigned int index = (block.y + MatrixLoc.y) * columns + (block.x + MatrixLoc.x);
		matrix[index].MakeEmpty();
	}

	std::unique_ptr<Figure> old = std::move(pNext);

	pNext = std::make_unique<Figure>(spawner.GetRandomTetromino(), MatrixLoc);

	auto& model = pNext->GetModel();
	Color inner = pNext->GetInnerColor();
	Color border = pNext->GetBorderColor();

	for (auto& block : model)
	{
		unsigned int index = (block.y + MatrixLoc.y) * columns + (block.x + MatrixLoc.x);

		matrix[index].MakeBlock(inner, border);
	}

	return std::move(old);
}