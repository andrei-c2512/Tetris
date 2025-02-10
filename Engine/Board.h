#pragma once
#include "Cell.h"

class Board {
public:
	Board(ParticleEffect& effect0);
	void EmptyLine(int row);
	int CheckLines() const;
	void DrawBoard(Graphics& gfx, Camera& cam);
	void DrawBorder(Graphics& gfx, Camera& cam);
	const Cell& GetCell(unsigned int index) const;
	const Cell& GetCell(Vec2I pos) const;
	void SwapCells(Vec2I pos1, Vec2I pos2);

	void MakeInactive(Vec2I pos);
	void MakeBlock(Vec2I pos , Color inner , Color border);
	void MakeEmpty(Vec2I pos);
	
	bool LineHasType(Cell::Status celltype , unsigned int y) const;
	bool LineIsFull(Cell::Status celltype, unsigned int y) const;
	void CheckForFullLines() ;
	void ClearLine(unsigned int y);
	void Reset();
	static bool InBounds(Vec2I pos);
	static bool InBounds(RectI& pos);
	static constexpr int rows = 20;
	static constexpr int columns = 10;
	static constexpr int BorderThickness = 3;
	static constexpr Color BorderColor = Colors::White;
	static constexpr Vec2I pos = Vec2I(-(columns * Cell::dim.width) / 2 , -Graphics::ScreenHeight / 2 + (rows * Cell::dim.height) / 4)  ;

	void SetFallingDistanceForBlocks(float dist, unsigned int y1 , unsigned int y2 );
	void SetFallingDistForLine(float dist, unsigned int y);

	bool UpdateMovement(float time);
	unsigned int GetMatrixDistFromStart(unsigned int index);
	void UpdateBlockOwners();
	void UpdateBlocks(float time);
private:
	// move the values of a line to another line
	void ReplaceLineValues(unsigned int y1, unsigned int y2);
private:
	std::vector<Cell> matrix;
	ParticleEffect& effect;
};