#pragma once

#include "template.h"

#include "tank.h"
#include "tilemap.h"
#include "keyboard_manager.h"
#include "camera.h"


namespace Tmpl8
{
	class Game
	{
	public:
		Game(Surface* visible_layer);
		void Init() {};
		void Shutdown() {};
		void Tick( float deltaTime );
		void MouseUp(int button) {};
		void MouseDown(int button) {};
		void MouseMove(int x, int y) {};
		void KeyUp(int key) { m_keyboard_manager.Released(key); }
		void KeyDown(int key) { m_keyboard_manager.Pressed(key); }

	private:
		// If player pressed +/-, change m_tile_size and zoom level.
		void CheckForZoomChange();
		void ApplyZoom();

		// Zoom settings.
		const int m_base_tile_size{ 32 };
		const int m_min_zoom{ 1 };
		const int m_max_zoom{ 3 };
		int m_zoom{ m_min_zoom };
		int m_tile_size{ m_zoom * m_base_tile_size };	

		// Core classes.
		Surface* m_visible_layer{ nullptr };
		Tank m_tank;
		Camera m_camera;
		Tilemap m_tilemap;
		KeyboardManager m_keyboard_manager;
	};
};