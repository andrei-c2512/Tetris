#pragma once
#include "Cell.h"
#include "Keyboard.h"
#include "Board.h"
#include <map>
#include "Timer.h"
#include "ParticleEffect.h"
#include "Font.h"

class TetrominoMap {
public:
	enum class Type {
		Square,
		Line,
		LShape,
		Pyramid,
		Oblique
	};
public:
	TetrominoMap()
	{
		std::vector<Vec2I> Square = { Vec2I(0 , 0),
									  Vec2I(1 , 0),
									  Vec2I(0 , -1),
									  Vec2I(1 , -1) };

		std::vector<Vec2I> Line = { Vec2I(-1 , 0),
									Vec2I(0  , 0),
									Vec2I(1  , 0),
									Vec2I(2  , 0) };

		std::vector<Vec2I> LShape = { Vec2I(0 , 1),
									  Vec2I(0 , 0),
									  Vec2I(0 , -1),
									  Vec2I(1 , -1) };

		std::vector<Vec2I> Pyramid = {Vec2I(0 , 0),
									  Vec2I(1 , 0),
									  Vec2I(-1 , 0),
									  Vec2I(0 , 1) };

		std::vector<Vec2I> Oblique = { Vec2I(0 , 0),
									   Vec2I(1 , 0),
									   Vec2I(0 , -1),
									   Vec2I(-1 , -1) };


		map.emplace(Type::Square , Square);
		map.emplace(Type::Line   , Line);
		map.emplace(Type::Oblique, Oblique);
		map.emplace(Type::Pyramid, Pyramid);
		map.emplace(Type::LShape , LShape);
	}
	std::vector<Vec2I> GetRandomTetromino()
	{
		Type type = Type(rng.GetVal());

		return map[type];
	}
private:
	RNG rng = { 0  , 4 };
	std::map<Type, std::vector<Vec2I>> map;
};
class Figure {
public:
	Figure() = default;
	Figure( std::vector<Vec2I> blocks0 , Vec2I pos);
	bool Move(Keyboard& kbd , Board& brd );
	bool MoveBy(Vec2I offset , Board& brd);
	void SetPos(Vec2I& pos , Board& brd);
	bool Rotate(Keyboard& kbd ,Board& brd);
	bool CheckCollision(Board& brd , Vec2I offset);
	bool InBounds(Vec2I offset) const;
	void MakeAllInactive(Board& brd);
	void RemoveFromMatrix(Board& brd);
	void PutIntoMatrix(Board& brd);

	std::vector<Vec2I> GetModel() const;
	Color GetInnerColor() const;
	Color GetBorderColor() const;
	Vec2I GetPos() const;
private:
	void MoveInMatrix(Vec2I new_pos , Board& brd );
private:
	std::vector<Vec2I> blocks;
	Vec2I pos;

	Color inner;
	Color border;

	float angle = 0.0f;
};


class TetronimoDisplayer {
public:
	TetronimoDisplayer();
	void Display(Graphics& gfx, Camera& cam);
	std::unique_ptr<Figure> PassFigure();
	void Update();
	RectI GetRect() const;
	static constexpr int BorderThickness = 4;

	static constexpr Vec2I pos = Vec2I(Board::pos.x + Board::columns * Cell::dim.width, Board::pos.y + Board::rows * Cell::dim.height);
private:
	std::vector<Cell> matrix;
	Vec2I MatrixLoc = { 1 , 2 };
	Font font = { "Fixedsys16x28.bmp" };

	bool FigChanged = true;

	RectI HitBox;
	int rows = 4;
	int columns = 4;

	std::unique_ptr<Figure> pNext;
	TetrominoMap spawner;
};

class Player {
public:
	Player(Board& board , TetronimoDisplayer& displayer0);
	void Spawn(Board& brd);
	void ShowNextFigure(Graphics& gfx, Camera& cam);
	void ShowFuturePosition(Graphics& gfx , Camera& cam );
	void DetermineFuturePos(Board& brd);
	void UpdateMovement(Board& brd , Keyboard& kbd , float time);
private:
	std::unique_ptr<Figure> pFig = nullptr;
	Vec2I SpawnLoc = { Board::columns / 2 ,  Board::rows - 2 };
	Timer move_timerY = { 0.5f };
	Timer move_timerX = { 0.075f };
	Timer rotation_timer = { 0.15f};
	Timer drop_timer = { 0.15f };
	Font font = { "Fixedsys16x28.bmp" };
	
	Vec2I future_pos = { 0, 0};
	std::vector<Vec2I> future_tetronimo;

	TetronimoDisplayer& displayer;
};
