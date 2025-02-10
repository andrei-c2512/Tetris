#include "Cell.h"

Cell::Cell(Vec2I pos0)
	:pos(pos0)
{
	MakeEmpty();
}
void Cell::Draw(Graphics& gfx , Camera& ct)
{
	if (pBlock != nullptr)
	{
		if (pBlock->GetPos() != pos)
		{
			DrawCellBackground(gfx, ct);
		}
		pBlock->Draw(gfx, ct);
	}
	else
	{
		DrawCellBackground(gfx, ct);
	}
}


void Cell::MakeBorder()
{
	RNG color_rng = { 0  , 4 };

	Color c = ShadesOfGray[color_rng.GetVal()];

	pBlock->SetInner(c);
	
	int r = int(float(float(c.GetR()) * ((100.0f - DarkeningPercentage) / 100.0f))), 
	    g = int(float(float(c.GetG()) * ((100.0f - DarkeningPercentage) / 100.0f))),
	    b = int(float(float(c.GetB()) * ((100.0f - DarkeningPercentage) / 100.0f)));

	pBlock->SetBorder(Color(std::move(r), std::move(g), std::move(b)));

	status = Status::Border;
}

Vec2I Cell::Block::GetPos() const
{
	return pos;
}
void Cell::DrawCellBackground(Graphics& gfx, Camera& cam)
{
	Color border_c;
	Color inner_c;
	border_c = background_border_color;
	inner_c = background_inner_color;

	for (unsigned int girth = 0; girth < BorderThickness; girth++)
	{
		gfx.DrawRect_Border((RectI(dim.width - girth * 2 , dim.height - girth * 2 , 
			  cam.Transform((Vec2I(pos.x + girth, pos.y + dim.height - girth))))), border_c, Effects::Copy{});
	}
	gfx.DrawRect_Border((RectI(dim, cam.Transform((Vec2I(pos.x, pos.y + dim.height))))), border_c, Effects::Copy{});
	RectI inner_rect = RectI(dim.width - 2 * BorderThickness, dim.height - 2 * BorderThickness,
		cam.Transform(Vec2I(pos.x + BorderThickness, pos.y + dim.height - BorderThickness)));

	gfx.DrawRect(std::move(inner_rect), inner_c, Effects::AlphaBlending{50});
}
const std::unique_ptr<Cell::Block>& Cell::GetBlock() const
{
	return pBlock;
}
std::unique_ptr<Cell::Block> Cell::PassBlock()
{
	status = Status::Empty;
	return std::move(pBlock);
}
void Cell::AssignBlock(std::unique_ptr<Cell::Block> block)
{
	status = Status::InactiveBlock;
	pBlock = std::move(block);
}
void Cell::MakeEmpty()
{
	RNG color_rng = { 0  , 4 };

	Color c = ShadesOfGray[color_rng.GetVal()];

	int iR = int(float(float(c.GetR()) * ((100.0f - 0.5f * DarkeningPercentage) / 100.0f))),
		iG = int(float(float(c.GetG()) * ((100.0f - 0.5f * DarkeningPercentage) / 100.0f))),
		iB = int(float(float(c.GetB()) * ((100.0f - 0.5f * DarkeningPercentage) / 100.0f)));

	int bR = int(float(float(c.GetR()) * ((100.0f - 1.5f * DarkeningPercentage) / 100.0f))),
	    bG = int(float(float(c.GetG()) * ((100.0f - 1.5f * DarkeningPercentage) / 100.0f))),
	    bB = int(float(float(c.GetB()) * ((100.0f - 1.5f * DarkeningPercentage) / 100.0f)));

	background_border_color = Color(std::move(bR), std::move(bG), std::move(bB));
	background_inner_color  = Color(std::move(iR), std::move(iG), std::move(iB));

	pBlock.reset(nullptr);
	status = Status::Empty;
}

bool Cell::UpdateBlock(float time)
{
	if (pBlock != nullptr)
	{
		return pBlock->Fall(time);
	}
	else
	{
		return false;
	}
}
void Cell::SwapCell(Cell& cell)
{
	std::swap(cell.status, status);
	std::swap(cell.pBlock, pBlock);

}
void Cell::Block::MoveTargetBy(Vec2I& offset)
{
	Moving = true;
	TargetPos += offset;
}
void Cell::MakeBlock(Color inner , Color border)
{
	status = Status::ActiveBlock;

	pBlock = std::make_unique<Block>(pos, border, inner);
}

void Cell::MakeInactive()
{
	//pBlock->SetInner(InactiveBlockColor);
	//Color c = InactiveBlockColor;
	//
	// int bR = int(float(float(c.GetR()) * ((100.0f - 1.5f * DarkeningPercentage) / 100.0f))),
	//	 bG = int(float(float(c.GetG()) * ((100.0f - 1.5f * DarkeningPercentage) / 100.0f))),
	//	 bB = int(float(float(c.GetB()) * ((100.0f - 1.5f * DarkeningPercentage) / 100.0f)));
	//
	//pBlock->SetBorder(Color(std::move(bR), std::move(bG), std::move(bB)));

	status = Status::InactiveBlock;
}

void Cell::VanishEffect(ParticleEffect& effect)
{
	if (pBlock != nullptr)
	{
		Dimensions<short> particle_dim = dim / 4;

		for (int y = pos.y; y < pos.y + dim.height; y += particle_dim.height)
		{
			for (int x = pos.x; x < pos.x + dim.width; x += particle_dim.width)
			{
				VanishParticle::Direction dir;
				if (x > pos.x + dim.width / 2 - 1)
				{
					dir = VanishParticle::Direction::Right;
				}
				else
				{
					dir = VanishParticle::Direction::Left;
				}

				effect.AddParticle(std::make_unique<VanishParticle>(Rect(particle_dim, Vec2D(x, y)), pBlock->GetInner(), std::move(dir)));
			}
		}
	}
}
Cell::Status Cell::GetStatus() const
{
	return status;
}

Vec2I Cell::GetPos() const
{
	return pos;
}

void Cell::Empty()
{
	status = Status::Empty;
	pBlock.reset(nullptr);
}

void Cell::TakeInfo(Cell& cell)
{
	if (cell.pBlock != nullptr)
	{
		pBlock.reset(nullptr);
	    pBlock = std::move(cell.pBlock);
		pBlock->SetPos(pos);
	}
	else
	{
		pBlock.reset(nullptr);
	}

	status  = cell.status;

}

Cell::Block::Block(Vec2I pos0, Color border0, Color inner0)
	:pos(pos0) , border(border0 ) , inner(inner0)
{
	TargetPos = pos;
	Moving = false;
}

void Cell::Block::SetBorder(const Color& c)
{
	border = c;
}

void Cell::Block::SetInner(const Color& c)
{
	inner = c;
}
bool Cell::Block::Fall(float time)
{
	if (Moving == true)
	{
		float add = -(time * 60.0f * FallingSpeed);
		if (std::abs(pos.y + add) < std::abs(TargetPos.y))
		{
			pos.y += add;
		}
		else
		{
			pos.y = TargetPos.y;
			Moving = false;
		}
		return true;
	}
	return false;

}

Color Cell::Block::GetBorder() const
{
	return border;
}
Color Cell::Block::GetInner() const
{
	return inner;
}
void Cell::Block::SetTarget(Vec2I& pos0)
{
	Moving = true;
	assert(pos0.x == pos.x);

	TargetPos = pos0;
}
void Cell::Block::Draw(Graphics& gfx, Camera& cam)
{
	for (unsigned int girth = 0; girth < BorderThickness; girth++)
	{
		gfx.DrawRect_Border((RectI(dim.width - girth * 2, dim.height - girth * 2,
			cam.Transform((Vec2I(pos.x + girth, pos.y + dim.height - girth))))), border, Effects::Copy{});
	}
	RectI inner_rect = RectI(dim.width - 2 * BorderThickness, dim.height - 2 * BorderThickness,
		cam.Transform(Vec2I(pos.x + BorderThickness, pos.y + dim.height - BorderThickness)));

	gfx.DrawRect(std::move(inner_rect), inner, Effects::Copy{});
}


void Cell::Block::SetPos(Vec2I pos0)
{
	pos = pos0;
}

void Cell::Block_SetTarget(Vec2I& pos)
{
	if(pBlock != nullptr)
	pBlock->SetTarget(pos);
}
void Cell::Block_MoveTargetBy(Vec2I& offset)
{
	if (pBlock != nullptr)
	pBlock->MoveTargetBy(offset);
}