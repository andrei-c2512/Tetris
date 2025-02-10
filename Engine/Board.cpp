#include "Board.h"

Board::Board(ParticleEffect& effect0)
	:effect(effect0)
{
	for (int y = pos.y; y < pos.y + rows * Cell::dim.height; y += Cell::dim.height)
	{
		for (int x = pos.x; x < pos.x + columns * Cell::dim.width; x += Cell::dim.width)
		{
			matrix.emplace_back(Cell(Vec2I(x, y)));
		}
	}
}
bool Board::LineIsFull(Cell::Status celltype, unsigned int y) const
{
	unsigned int indexY = y * columns;

	for (unsigned int x = 0; x < columns; x++)
	{
		if (matrix[indexY + x].GetStatus() != celltype)
		{
			return false;
		}
	}
	return true;
}

void Board::DrawBoard(Graphics& gfx, Camera& ct)
{
	int height = rows * Cell::dim.height;
	// gfx.DrawRect(ct.Transform(RectI(columns * Cell::dim.width, height, Vec2I(pos.x, pos.y))), Colors::Gray, Effects::AlphaBlending{ 50 });
	DrawBorder(gfx, ct);
	for (Cell& cell : matrix)
	{
		cell.Draw(gfx, ct);
	}
}

void Board::DrawBorder(Graphics& gfx, Camera& cam)
{
	for (unsigned int girth = 0; girth < BorderThickness; girth++)
	{
		gfx.DrawRect_Border((RectI(columns * Cell::dim.width + 2 * girth, rows * Cell::dim.height + 2 * girth,
			cam.Transform((Vec2I(pos.x - girth, pos.y + Cell::dim.height * rows + girth))))), BorderColor, Effects::Copy{});
	}
}
void Board::EmptyLine(int row)
{
	for (int x = 0; x < columns; x++)
	{
		matrix[row * columns + x].MakeEmpty();
	}
}

int Board::CheckLines() const
{
	for (int y = 0; y < rows; y++)
	{
		bool CanBeEmptied = true;
		for (int x = 0; x < columns; x++)
		{
			const Cell* cell = &matrix[unsigned(y * columns + x)];

			if (cell->GetStatus() != Cell::Status::InactiveBlock)
			{
				CanBeEmptied = false;
				break;
			}
		}
		if (CanBeEmptied)
		{
			return y;
		}
	}
	return -1;
}

void Board::ClearLine(unsigned int y)
{
	int yIndex = y * columns;

	for (unsigned int x = 0; x < columns; x++)
	{
		matrix[yIndex + x].VanishEffect(effect);
		matrix[yIndex + x].Empty();
	}
}


void Board::MakeInactive(Vec2I pos)
{
	unsigned int index = pos.y * columns + pos.x;
	matrix[index].MakeInactive();
}
const Cell& Board::GetCell(unsigned int index) const
{
	assert(index >= 0 && index < columns * rows);
	return matrix[index];
}

const Cell& Board::GetCell(Vec2I pos) const
{
	unsigned int index = pos.y * columns + pos.x;
	return matrix[index];
}

void Board::SwapCells(Vec2I pos1, Vec2I pos2)
{
	unsigned int index1 = pos1.y * columns + pos1.x;
	unsigned int index2 = pos2.y * columns + pos2.x;

	matrix[index1].SwapCell(matrix[index2]);
}

void Board::MakeBlock(Vec2I pos, Color inner, Color border)
{
	unsigned int index = pos.y * columns + pos.x;
	matrix[index].MakeBlock(inner, border);
}
void Board::MakeEmpty(Vec2I pos)
{
	unsigned int index = pos.y * columns + pos.x;
	matrix[index].MakeEmpty();
}

bool Board::InBounds(Vec2I pos) 
{
	return pos.x >= 0 && pos.x < columns&& pos.y >= 0 && pos.y < rows;
}

bool Board::LineHasType(Cell::Status celltype , unsigned int y) const
{
	unsigned int yIndex = y * columns;
	for (unsigned int x = 0; x < columns; x++)
	{
		if (matrix[yIndex + x].GetStatus() == celltype)
		{
			return true;
		}
	}
	return false;
}

bool Board::InBounds(RectI& rect) 
{
	return rect.right() < columns && rect.left >= 0 && rect.top() < rows && rect.bottom >= 0;
}

void Board::CheckForFullLines()
{
	bool NoLinesLeft = false;
	int lines = 0;
	while (NoLinesLeft == false)
	{
		int StopY = -1;
		for (unsigned int y = 0; y < rows; y++)
		{
			if (LineIsFull(Cell::Status::InactiveBlock, y))
			{
				StopY = y;
				break;
			}
		}

		if (StopY != -1)
		{
			ClearLine(StopY);

			//for (unsigned int y = StopY; y <= rows - 2; y++)
			//{
			//	ReplaceLineValues(y, y + 1);
			//}
			//for (unsigned int x = 0; x < columns; x++)
			//{
			//	Cell& cell = matrix[(rows - 1) * columns + x];
			//
			//	if (cell.GetStatus() != Cell::Status::ActiveBlock)
			//	{
			//		cell.Empty();
			//	}
			//}

			SetFallingDistanceForBlocks(-Cell::dim.height, StopY, rows - 1);
		}
		else
		{
			NoLinesLeft = true;
		}
		
	}


}
void Board::Reset()
{
	for (unsigned int y = 0; y < rows; y++)
	{
		ClearLine(y);
	}
}
bool Board::UpdateMovement(float time)
{
	bool Movement = false;
	for (auto& cell : matrix)
	{
		Movement |= cell.UpdateBlock(time);
	}
	return Movement;
}

void Board::ReplaceLineValues(unsigned int y1, unsigned int y2)
{
	for (unsigned int x = 0; x < columns; x++)
	{
		matrix[y1 * columns + x].TakeInfo( matrix[y2 * columns + x]);
	}
}


void Board::SetFallingDistanceForBlocks(float dist , unsigned int y1 , unsigned int y2)
{
	for (unsigned int y = y1; y < y2; y++)
	{
		SetFallingDistForLine(dist, y);
	}
}

void Board::SetFallingDistForLine(float dist, unsigned int y)
{
	unsigned int yIndex = y * columns;
	for (unsigned int x = 0; x < columns; x++)
	{
		matrix[yIndex + x].Block_MoveTargetBy(Vec2I(0, dist));
	}
}

unsigned int Board::GetMatrixDistFromStart(unsigned int index)
{
	const Cell& cell = matrix[index];
	const std::unique_ptr<Cell::Block>& pBlock = cell.GetBlock();

	Vec2I block_pos = pBlock->GetPos();
	Vec2I cell_pos = cell.GetPos();

	int deltaY = block_pos.y - cell_pos.y;
	deltaY /= Cell::dim.height;
	return deltaY;
}

void Board::UpdateBlocks(float time)
{
	if (UpdateMovement(time) == true)
	{
		UpdateBlockOwners();
	}
}

void Board::UpdateBlockOwners()
{
	for (unsigned int y = 0; y < rows; y++)
	{
		for (unsigned int x = 0; x < columns; x++)
		{
			unsigned int index = y * columns + x;
			if (matrix[index].GetStatus() == Cell::Status::InactiveBlock)
			{
				int delta = GetMatrixDistFromStart(index);

				if (delta != 0)
				{
					std::unique_ptr<Cell::Block> pBlock = matrix[index].PassBlock();
					assert(pBlock != nullptr);

					matrix[index + delta * columns].AssignBlock(std::move(pBlock));
				}

			}
		}
	}
}