#pragma once
#include "Graphics.h"
#include "Timer.h"
#include <forward_list>
#include "Effects.h"
#include "Camera.h"
class Particle {
public:
	Particle(const Rect& rect, const Color& c, float speed0, const Timer& lifespan)
		:HitBox(rect) , color(c) , speed(speed0), LifeSpan(lifespan)
	{
		assert(Graphics::WithinWorld(HitBox));
		Active = true;
	}
	void Draw(Graphics& gfx , Camera& ct)
	{
		Effects::Copy e;
		float tLimit = LifeSpan.GetTimeLimit();
		float time = LifeSpan.GetTime();
		if (tLimit * 0.25f > time)
		{
			gfx.DrawRect(ct.Transform(HitBox), color, e);
		}
		else
		{
			float dt = tLimit - time;
			if (dt > 0)
			{
				int ratio = (float(dt) / float((tLimit * 0.75f))) * 100.0f;

				Effects::AlphaBlending bl(ratio);

				gfx.DrawRect(ct.Transform(HitBox), color, bl);
			}

		}
	}
	void Update(float time)
	{
		LifeSpan.Update(time);
		if (LifeSpan.IsReady())
		{
			Active = false;
		}
		else
		{
			UpdateMovement(time);
		}
	}
	bool InBounds()
	{
		return Graphics::WithinScreen(HitBox);
	}
	bool IsActive() const
	{
		return Active;
	}
protected:
	virtual void UpdateMovement(float time) 
	{

		float AddX = 60.0f * time * speed;
		float AddY = 60.0f * time * speed;
		RectI rect = RectI(HitBox.width, HitBox.height, Vec2I(HitBox.left + int(AddX), HitBox.bottom + int(AddY)));

		if (Graphics::WithinScreen(rect))
		{
			HitBox.left = std::move(rect.left);
			HitBox.bottom = std::move(rect.bottom);
		}
		else
			Active = false;
	}
protected:
	Rect HitBox;
	Color color;

	float speed;

	bool Active;
	Timer LifeSpan;
};

class VanishParticle : public Particle {
public:
	enum class Direction {
		Right,
		Left,
		Up,
		None
	};
public:
	VanishParticle(Rect& hitbox0 , Direction dir)
		:Particle(hitbox0 , ParticleColorPallete[0] , hitbox0.width , 5.0f) , PredominantDir(dir)
	{
		color = ParticleColorPallete[rng.GetVal()];
		assert(HitBox.width == HitBox.height);
		MaxDist = HitBox.width;
		CurrentDir = PredominantDir;
		CompletedDist = 0.0f;
	}
	VanishParticle(Rect& hitbox0, Color color0 , Direction dir)
		:Particle(hitbox0, color0, hitbox0.width, 5.0f), PredominantDir(dir)
	{
		assert(HitBox.width == HitBox.height);
		MaxDist = HitBox.width;
		CurrentDir = PredominantDir;
		CompletedDist = 0.0f;
	}
	static constexpr Color ParticleColorPallete[4] = { Color(223, 178, 0) ,
													   Color(218, 173, 0) ,
													   Color(228, 183, 0) ,
													   Color(213, 168, 0)  };
protected:
	void UpdateMovement(float time) override
	{
		float add = time * 60.0f * speed;
		if (CompletedDist + add >= MaxDist)
		{
			float max_dist = MaxDist - CompletedDist;
			Move(max_dist);
			UpdateDirection();

			add -= max_dist;
			Move(add);
			CompletedDist += add;
		}
		else
		{
			Move(add);
		}
	}
	void UpdateDirection()
	{
		unsigned int val = rng.GetVal();

		switch (val)
		{
		case 0:
			CurrentDir = Direction::Right;
			break;
		case 1:
			CurrentDir = Direction::Left;
			break;
		case 2:
			CurrentDir = Direction::Up;
			break;
		default:
			CurrentDir = PredominantDir;
		}
		CompletedDist = 0.0f;
	}
	void Move(float dist)
	{
		switch (CurrentDir)
		{
		case Direction::Right:
			HitBox.left += dist;
			break;
		case Direction::Left:
			HitBox.left -= dist;
			break;
		case Direction::Up:
			HitBox.bottom += dist;
			break;
		}
	}
private:
	Direction PredominantDir;
	Direction CurrentDir;
	float CompletedDist;
	float MaxDist;
	RNG rng = { 0 , 3 };
};

class ParticleEffect {
public:
	void Update(float time)
	{

		particle_list.remove_if([](const std::unique_ptr<Particle>& particle)
			{
			return (particle->IsActive() == false);
			});

		for (std::unique_ptr<Particle>& particle : particle_list)
		{
			particle->Update(time);
		}
	}
	void Draw(Graphics& gfx , Camera& cam)
	{
		for (std::unique_ptr<Particle>& particle : particle_list)
		{
			particle->Draw(gfx , cam);
		}
	}
	void AddParticle(std::unique_ptr<Particle> particle)
	{
		particle_list.emplace_front(std::move(particle));
	}
private:
	std::forward_list<std::unique_ptr<Particle>> particle_list;
};