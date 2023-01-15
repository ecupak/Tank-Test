#include "tank.h"

namespace Tmpl8
{
	constexpr int UP{ 0 };
	constexpr int RIGHT{ 4 };
	constexpr int DOWN{ 8 };
	constexpr int LEFT{ 12 };

	Tank::Tank(const int base_tile_size, const KeyboardManager& keyboard_manager)
		: m_keyboard_manager{ keyboard_manager }
		, m_tilemap_collision_box_width{ static_cast<float>(m_collision_box_width) / static_cast<float>(base_tile_size) }
		, m_tilemap_collision_box_height{ static_cast<float>(m_collision_box_height) / static_cast<float>(base_tile_size) }
		, m_backstep_value{ 1.0f / static_cast<float>(base_tile_size) }
		, m_current_heading{ UP }
	{	
		UpdateCenter();
	}


	void Tank::Update(const Tilemap& tilemap, const float& deltaTime)
	{
		if (m_tank_rotation_steps != 0 || m_gun_rotation_steps != 0)
		{
			Rotate(deltaTime);
		}
		else
		{
			ProcessInput(tilemap, deltaTime);
		}
	}


	void Tank::Draw(Surface* visible_layer, const int tile_size, const int zoom, const vec2& camera_offset)
	{
		if (false) // Draw collision box.
		{
			visible_layer->Box(
				(m_position.x * tile_size) - camera_offset.x,
				(m_position.y * tile_size) - camera_offset.y,
				((m_position.x + m_tilemap_collision_box_width - m_backstep_value) * tile_size) - camera_offset.x,
				((m_position.y + m_tilemap_collision_box_height - m_backstep_value) * tile_size) - camera_offset.y,
				0xff0000
			);
		}
				
		m_tank_sprite.DrawScaled(
			(m_position.x * tile_size) - (m_tank_sprite_offset.x * zoom) - camera_offset.x,
			(m_position.y * tile_size) - (m_tank_sprite_offset.y * zoom) - camera_offset.y,
			m_tank_sprite.GetWidth() * zoom,
			m_tank_sprite.GetHeight() * zoom,
			visible_layer
		);

		m_gun_sprite.DrawScaled(
			(m_position.x * tile_size) - (m_gun_sprite_offset.x * zoom) - camera_offset.x,
			(m_position.y * tile_size) - (m_gun_sprite_offset.y * zoom) - camera_offset.y,
			m_gun_sprite.GetWidth() * zoom,
			m_gun_sprite.GetHeight() * zoom,
			visible_layer
		);
	}


	const vec2& Tank::GetCenter() const
	{
		return m_center;
	}


	void Tank::Rotate(const float& deltaTime)
	{
		if (m_tank_rotation_steps != 0)
		{
			RotateTank(deltaTime);
		}

		if (m_gun_rotation_steps != 0)
		{
			RotateGun(deltaTime);
		}
	}


	void Tank::RotateTank(const float& deltaTime)
	{
		m_tank_rotation_elapsed_time += deltaTime;

		if (m_tank_rotation_elapsed_time >= m_rotation_delay)
		{
			m_tank_rotation_steps -= m_rotation_direction;
			m_tank_sprite.SetFrame(Wrap(m_current_heading - m_tank_rotation_steps, 0, 15));
			m_tank_rotation_elapsed_time = 0.0f;
		}
	}


	void Tank::RotateGun(const float& deltaTime)
	{
		m_gun_rotation_elapsed_time += deltaTime;

		if (m_gun_rotation_elapsed_time >= m_rotation_delay)
		{
			m_gun_rotation_steps -= m_rotation_direction;
			m_gun_sprite.SetFrame(Wrap(m_current_heading - m_gun_rotation_steps, 0, 15));
			m_gun_rotation_elapsed_time = 0.0f;
		}
	}


	const int Tank::Wrap(int value, const int begin, const int end) const
	{
		while (value < begin)
		{
			int amount_off{ abs(abs(value) - abs(begin)) };
			value = end - amount_off + 1;
		}

		while (value > end)
		{
			int amount_off{ abs(abs(value) - abs(end)) };
			value = begin + amount_off - 1;
		}

		return value;
	}


	void Tank::ProcessInput(const Tilemap& tilemap, const float& deltaTime)
	{
		// Get direction based on player input.
		const Direction direction{ GetXDirection(),  GetYDirection() };

		// Exit early if no input or diagonal input.
		if (IsDirectionInputIllegal(direction))
		{
			return;
		}

		// Get heading (left/right/up/down) from input.
		int new_heading{ GetNewHeading(direction) };

		// If new heading is different than current heading, begin rotation/turn.
		// Otherwise, move forward.
		if (IsTurning(new_heading))
		{
			FindRotation(new_heading);
		}
		else // Going in same direction already facing
		{
			Move(direction, tilemap, deltaTime);
		}
	}


	const int Tank::GetXDirection() const
	{
		return -(m_keyboard_manager.IsPressed(SDL_SCANCODE_LEFT)) + m_keyboard_manager.IsPressed(SDL_SCANCODE_RIGHT);
	}


	const int Tank::GetYDirection() const
	{
		return -(m_keyboard_manager.IsPressed(SDL_SCANCODE_UP)) + m_keyboard_manager.IsPressed(SDL_SCANCODE_DOWN);
	}


	const bool Tank::IsDirectionInputIllegal(const Direction& direction) const
	{
		// Don't allow diagonal movement (both directions have a non-zero value).
		if (direction.x != 0 && direction.y != 0)
		{
			return true;
		}

		// Don't continue if no direction is pressed.		
		if (direction.x == 0 && direction.y == 0)
		{
			return true;
		}

		// Input is ok.
		return false;
	}


	const int Tank::GetNewHeading(const Direction& direction) const
	{
		if (direction.x != 0)
		{
			return (direction.x == -1 ? LEFT : RIGHT);
		}
		else // if (direction.y != 0)
		{
			return (direction.y == -1 ? UP : DOWN);
		}
	}


	const bool Tank::IsTurning(const int new_heading) const
	{
		return new_heading != m_current_heading;
	}


	void Tank::FindRotation(const int new_heading)
	{
		m_tank_rotation_steps = new_heading - m_current_heading;

		// We use the sign and value of the difference in current facing and new facing
		// to get rotation direction and steps. This falls apart when we rotate between
		// UP and LEFT, as they are not contiguous like the other directions.
		// If rotation is between UP and LEFT, the value is either 12 or -12:
		// - Set to 4 instead of 12
		// - Flip sign to match correct rotation.
		if (abs(m_tank_rotation_steps) >= 12)
		{
			m_tank_rotation_steps = 4 * -1 * GetSign(m_tank_rotation_steps);
		}

		// Set gun to have same rotation as tank and prime for delay.
		m_gun_rotation_steps = m_tank_rotation_steps;
		m_gun_rotation_elapsed_time = -(m_rotation_delay);

		// Set direction of rotation and final position.
		m_rotation_direction = GetSign(m_tank_rotation_steps);
		m_current_heading = new_heading;
	}


	const int Tank::GetSign(const int value) const
	{
		return (value > 0 ? 1 : (value < 0 ? -1 : 0));
	}


	void Tank::Move(const Direction& direction, const Tilemap& tilemap, const float& deltaTime)
	{
		// Get potential change in position.
		const vec2 delta_position{
			direction.x * m_movement_speed * deltaTime,
			direction.y * m_movement_speed * deltaTime
		};

		// Move to new position if valid spot.
		if (IsNewPositionValid(delta_position, tilemap))
		{
			ApplyNewPosition(delta_position);
			UpdateCenter();			
		}
	}


	const bool Tank::IsNewPositionValid(const vec2& delta_position, const Tilemap& tilemap) const
	{
		// Get new x positions (left and right edge).
		const int tilemap_x_positions[2]{
			static_cast<int>(floor(m_position.x + delta_position.x)),
			static_cast<int>(floor(m_position.x + delta_position.x + (m_tilemap_collision_box_width - m_backstep_value)))
		};

		// Get new y positions (top and bottom edge).
		const int tilemap_y_positions[2]{
			static_cast<int>(floor(m_position.y + delta_position.y)),
			static_cast<int>(floor(m_position.y + delta_position.y + (m_tilemap_collision_box_height - m_backstep_value)))
		};

		// Check that each corner combination is on a tile that can be walked on.
		for (int x_corner{ 0 }; x_corner < 2; ++x_corner)
		{
			for (int y_corner{ 0 }; y_corner < 2; ++y_corner)
			{
				if (!tilemap.IsPositionValid(
					tilemap_x_positions[x_corner],
					tilemap_y_positions[y_corner])
					)
				{
					// Stop checking as soon as a single false is found.
					return false;
				}
			}
		}
		return true;
	}


	void Tank::ApplyNewPosition(const vec2& delta_position)
	{
		m_position.x += delta_position.x;
		m_position.y += delta_position.y;
	}


	void Tank::UpdateCenter()
	{
		m_center.x = m_position.x + (m_tilemap_collision_box_width / 2);
		m_center.y = m_position.y + (m_tilemap_collision_box_height / 2);
	}
}
