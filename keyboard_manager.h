#pragma once

#include <vector>
#include <map>
#include <SDL_keycode.h>


namespace Tmpl8
{
	struct KeyboardPacket {

		void Reset()
		{
			m_was_pressed = false;
			m_was_released = false;
		}

		bool m_was_pressed{ false };
		bool m_was_released{ false };

		bool m_is_pressed{ false };		
		bool m_is_just_pressed{ false };
		bool m_is_just_released{ false };
	};

	class KeyboardManager
	{
	public:
		KeyboardManager();

		// Called by Game. Checks all keyboard_packets and applies changes to key state.
		void Update();

		// Finds keyboard_packet index associated with SDL_code.
		const int GetSDLMapping(const int SDL_code) const;
		
		// Called by Game. Updates associated key's keyboard_packet info.
		void Pressed(const int SDL_code);
		void Released(const int SDL_code);

		// Returns key information.
		const bool IsPressed(const int SDL_code) const;
		const bool IsJustPressed(const int SDL_code) const;
		const bool IsJustReleased(const int SDL_code) const;

	private:
		// Creates a keyboard_packet for each key index in the registered_keys map.
		const void CreateKeyboardPackets();

		/* ATTRIBUTES */
		std::vector<KeyboardPacket> keyboard_packets;

		// Maps keys used in game to their keyboard_packet index. Use the same index for keys that share the same result.
		// Do not use index 0 - this will be automatically created and used for all key input that is not registered.
		// Index values should be sequential - do not skip numbers.
		const std::map<int, int> registered_keys{
			{
				{SDL_SCANCODE_A, 1}
				, {SDL_SCANCODE_LEFT, 1}

				, {SDL_SCANCODE_D, 2}
				, {SDL_SCANCODE_RIGHT, 2}

				, {SDL_SCANCODE_W, 3}
				, {SDL_SCANCODE_UP, 3}

				, {SDL_SCANCODE_S, 4}
				, {SDL_SCANCODE_DOWN, 4}

				, {SDL_SCANCODE_KP_PLUS, 5}
				, {SDL_SCANCODE_EQUALS, 5}

				, {SDL_SCANCODE_KP_MINUS, 6}
				, {SDL_SCANCODE_MINUS, 6}
			}
		};
	};
};

