#include "keyboard_manager.h"

#include <stdexcept>	// std::out_of_range
#include <iterator>
#include <algorithm>


namespace Tmpl8
{
	KeyboardManager::KeyboardManager()
	{
		CreateKeyboardPackets();
	}


	const void KeyboardManager::CreateKeyboardPackets()
	{
		// Find the largest registered key's index. We will make a keyboard_packet for each registered index.
		std::map<int, int>::const_iterator last_registered_keypair{
			std::max_element(registered_keys.begin(), registered_keys.end(),
				[](const std::pair<int, int>& keypair_1, const std::pair<int, int>& keypair_2)
				{
					return keypair_1.second < keypair_2.second;
				}
			)
		};

		// Increase largest index found by 1. A dummy keyboard_packet will be created to hold unused input.
		const int keyboard_packets_size{ 1 + last_registered_keypair->second };

		// Index 0 is a dummy packet for unused keys.
		for (int i{ 0 }; i < keyboard_packets_size + 1; ++i)
		{
			keyboard_packets.push_back(KeyboardPacket{});
		}
	}


	void KeyboardManager::Update()
	{
		for (KeyboardPacket& keypack : keyboard_packets)
		{
			if (keypack.m_was_pressed)
			{
				keypack.m_is_just_pressed = !keypack.m_is_pressed;
				keypack.m_is_pressed = true;
				keypack.m_is_just_released = false;
			}			
			else if (keypack.m_was_released)
			{
				keypack.m_is_just_released = keypack.m_is_pressed;
				keypack.m_is_pressed = false;
				keypack.m_is_just_pressed = false;
			}
			else
			{
				keypack.m_is_just_released = false;
				keypack.m_is_just_pressed = false;
			}
			keypack.Reset();
		}
	}


	void KeyboardManager::Pressed(const int SDL_code)
	{
		keyboard_packets[GetSDLMapping(SDL_code)].m_was_pressed = true;
	}


	void KeyboardManager::Released(const int SDL_code)
	{	
		keyboard_packets[GetSDLMapping(SDL_code)].m_was_released = true;
	}


	const bool KeyboardManager::IsPressed(const int SDL_code) const
	{
		return keyboard_packets[GetSDLMapping(SDL_code)].m_is_pressed;
	}


	const bool KeyboardManager::IsJustPressed(const int SDL_code) const
	{
		return keyboard_packets[GetSDLMapping(SDL_code)].m_is_just_pressed;
	}
	
	
	const bool KeyboardManager::IsJustReleased(const int SDL_code) const
	{
		return keyboard_packets[GetSDLMapping(SDL_code)].m_is_just_released;
	}


	const int KeyboardManager::GetSDLMapping(const int SDL_code) const
	{
		try
		{
			return registered_keys.at(SDL_code);
		}
		catch (const std::out_of_range& oor)
		{
			return 0;
		}

		return -1;

		switch (SDL_code)
		{
		case SDL_SCANCODE_A:
		case SDL_SCANCODE_LEFT:
			return 1;
		case SDL_SCANCODE_D:
		case SDL_SCANCODE_RIGHT:
			return 2;
		case SDL_SCANCODE_W:
		case SDL_SCANCODE_UP:
			return 3;
		case SDL_SCANCODE_S:
		case SDL_SCANCODE_DOWN:
			return 4;
		case SDL_SCANCODE_KP_PLUS: // keypad +
		case SDL_SCANCODE_EQUALS: // number row +
			return 5;
		case SDL_SCANCODE_KP_MINUS:
		case SDL_SCANCODE_MINUS:
			return 6;
		default:
			return 0; // All other keypresses are dumped into a dummy KeyboardPacket.
		}
	}

};