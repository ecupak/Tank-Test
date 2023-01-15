#include "camera.h"

namespace Tmpl8
{
	void Camera::Update(const int tile_size, const Tank& tank)
	{
		m_center = tank.GetCenter();

		m_offset.x = (m_center.x * tile_size) - (ScreenWidth / 2.0f);
		m_offset.y = (m_center.y * tile_size) - (ScreenHeight / 2.0f);
	}


	void Camera::Draw(Surface* visible_layer, Tilemap& tilemap, Tank& tank, const int tile_size, const int zoom)
	{
		visible_layer->Clear(0x00000000);

		// Draw tilemap underneath.
		tilemap.Draw(visible_layer, tile_size, m_offset);

		// Draw tank in center (no offset).
		tank.Draw(visible_layer, tile_size, zoom, m_offset);
	}
}