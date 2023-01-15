#pragma once

#include "template.h"
#include "tank.h"

namespace Tmpl8
{
	class Camera
	{
	public:		
		Camera() {};

		// Called by Game. Centers camera on tank.
		void Update(const int tile_size, const Tank& tank);

		// Draws tilemap and tank to visible_layer using offset of camera's position.
		void Draw(Surface* visible_layer, Tilemap& tilemap, Tank& tank, const int tile_size, const int zoom);

	private:
		vec2 m_center{ 0.0f, 0.0f };
		vec2 m_offset{ 0.0f, 0.0f };
	};
};

