#pragma once

#include "template.h"
#include "surface.h"


namespace Tmpl8
{	
	constexpr int m_tilemap_height{ 6 };
	constexpr int m_tilemap_width{ 10 };

	class Tilemap
	{
	public:
		Tilemap();

		// Called by Camera. Draws to visible_layer, scaled by tile_size.
		void Draw(Surface* visible_layer, const int tile_size, const vec2& offset);

		// Called by Tank. Determines if requested position is on a valid tile.
		const bool IsPositionValid(const int x_position, const int y_position) const;

	private:
		// Checks tile_id to see if it can be walked on.
		const bool IsTileWalkable(const int tile_id) const;

		/* ATTRIBUTES */
		Sprite m_tile_sprite{ new Surface{"assets/tilemap_row.png"}, 108 };
		
		const int m_tilemap[m_tilemap_height][m_tilemap_width]{
			46, 46, 46, 46, 46, 46, 46, 46, 46, 46,
			46, 23, 23, 23, 46, 46, 46, 46, 46, 46,
			46, 23, 23, 23, 23, 23, 46, 46, 46, 46,
			46, 23, 23, 23, 23, 23, 46, 46, 46, 46,
			46, 47, 47, 23, 23, 23, 46, 46, 46, 46,
			46, 46, 46, 47, 47, 47, 46, 46, 46, 46
		};
	};
};

