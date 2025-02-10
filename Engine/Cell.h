#pragma once
#include "Camera.h"
#include "Vec2D.h"
#include "Rect.h"
#include "Effects.h"
#include "RNG.h"
#include "Graphics.h"
#include "ParticleEffect.h"

class Cell {
public:
	enum class Status {
		Border,
		Empty,
		ActiveBlock,
		InactiveBlock
	};
public:
	Cell(Vec2I pos);
	void VanishEffect(ParticleEffect& effect);
	void Draw(Graphics& gfx , Camera& cam);
	void Empty();
	void MakeBorder();
	void MakeEmpty();
	void MakeInactive();
	void MakeBlock(Color inner , Color border);
	void SwapCell(Cell& cell);
	void TakeInfo( Cell& cell);
	bool UpdateBlock(float time);
	void Block_SetTarget(Vec2I& pos);
	void Block_MoveTargetBy(Vec2I& offset);
	//getters
	Vec2I GetPos() const;
	Status GetStatus() const;
public:
	
	static constexpr Color ShadesOfGray[5] = { Color(60 , 60 , 60) , 
											   Color(65 , 65 , 65) , 
											   Color(70 , 70 , 70) , 
											   Color(75 , 75 , 75) , 
											   Color(80 , 80 , 80) };

	static constexpr Color InactiveBlockColor = Color(34, 146, 198);

	static constexpr Color ColorRange[9] = { Color(201, 52, 0) ,
											  Color(0, 151, 0),
											  Color(0, 151, 131) ,
											  Color(123, 105, 255),
											  Color(209, 138, 62) ,
											  Color(84, 138, 62) ,
											  Color(84, 101, 215) ,
											  Color(84, 179, 215) ,
											  Color(176, 205, 255) };
	// for empty tiles 
	static constexpr float DarkeningPercentage = 30;
	static constexpr int   BorderThickness = 3;
	static constexpr Dimensions<short> dim = { 20 , 20 };
public:
	class Block {
	public:
		Block(Vec2I pos, Color border, Color inner);
		bool Fall(float time);
		void SetTarget(Vec2I& pos);
		void MoveTargetBy(Vec2I& offset);
		void Draw(Graphics& gfx, Camera& cam);
		void SetInner(const Color& c);
		void SetBorder(const Color& c);
		void SetPos(Vec2I pos);

		Vec2I GetPos() const;
		Color GetBorder() const;
		Color GetInner() const;

		static constexpr float FallingSpeed = 5.0f;
	private:
		Color inner;
		Color border;

		Vec2I pos;
		Vec2I TargetPos;
		bool  Moving = false;
	};
private:
	void DrawCellBackground(Graphics& gfx, Camera& cam);
private:
	Vec2I pos;
	Color background_border_color;
	Color background_inner_color;
public:
	std::unique_ptr<Block> PassBlock();
	const std::unique_ptr<Block>& GetBlock() const;
	void AssignBlock(std::unique_ptr<Block> block);
private:
	std::unique_ptr<Block> pBlock;
	Status status;
};