#pragma once

#include "template.h"
#include "surface.h"
#include "keyboard_manager.h"

#include "tilemap.h"


namespace Tmpl8
{
	// Stores x and y component of user input. Used to move tank.
	struct Direction
	{
	public:
		Direction(const int x_direction, const int y_direction)
			: x{ x_direction }
			, y{ y_direction }
		{	}

		const int x{ 0 };
		const int y{ 0 };
	};


	class Tank
	{
	public:
		// Neesds base_tile_size to correctly calculate collision information.
		Tank(const int base_tile_size, const KeyboardManager& keyboard_manager);

		// Called by Game. Either rotates the tank or moves to new position on tilemap.
		void Update(const Tilemap& tilemap, const float& deltaTime);

		// Called by Camera. Draws tank to visible_layer scaling by tile_size and zoom.
		void Draw(Surface* visible_layer, const int tile_size, const int zoom, const vec2& camera_offset);

		// Called by Camera.
		const vec2& GetCenter() const;

	private:
		// Rotates tank.
		void Rotate(const float& deltaTime);
		void RotateTank(const float& deltaTime);
		void RotateGun(const float& deltaTime);
		const int Wrap(int value, const int begin, const int end) const;

		// Takes player input and determines movement.
		void ProcessInput(const Tilemap& tilemap, const float& deltaTime);

		// If new direction, set rotation information.
		const bool IsTurning(const int new_facing) const;
		const int GetNewHeading(const Direction& direction) const;
		void FindRotation(const int new_facing);
		const int GetSign(const int value) const;

		// If same direction, find new position and verify can move there, then move.
		void Move(const Direction& direction, const Tilemap& tilemap, const float& deltaTime);
		const int const GetXDirection() const;
		const int const GetYDirection() const;
		const bool IsDirectionInputIllegal(const Direction& direction) const;
		const bool IsNewPositionValid(const vec2& delta_position, const Tilemap& tilemap) const;
		void ApplyNewPosition(const vec2& delta_position);
		void UpdateCenter();
		
		/* ATTRIBUTES */
		Sprite m_tank_sprite{ new Surface{"assets/tank_base.png" }, 16 };
		Sprite m_gun_sprite{ new Surface{"assets/tank_gun.png" }, 16 };
		const KeyboardManager& m_keyboard_manager;
		
		const vec2 m_tank_sprite_offset{ 5.0f, 5.0f };
		const vec2 m_gun_sprite_offset{ 5.0f, 0.0f };
		const int m_collision_box_width{ 42 };
		const int m_collision_box_height{ 40 };
		
		const int m_movement_speed{ 1 }; // 1 tile per second.

		const float m_tilemap_collision_box_width{ 0.0f };
		const float m_tilemap_collision_box_height{ 0.0f };
		const float m_backstep_value{ 0.0f };

		vec2 m_position{ 1.0f, 1.0f };
		vec2 m_delta_position{ 0.0f, 0.0f };
		vec2 m_center{ 0.0f, 0.0f };

		int m_current_heading{ 0 };
		int m_rotation_direction{ 0 };
		int m_tank_rotation_steps{ 0 };
		int m_gun_rotation_steps{ 0 };		
		float m_tank_rotation_elapsed_time{ 0.0f }; // time since last rotation step.
		float m_gun_rotation_elapsed_time{ 0.0f }; // time since last rotation step.
		const float m_rotation_delay{ 0.125f }; // seconds between rotation steps.
		const float m_gun_rotation_offset_delay{ 0.140f }; // additional time the gun waits before starting rotation.		
	};
};


