#include "game.h"
#include "surface.h"

namespace Tmpl8
{
    Game::Game(Surface* visible_layer)
        : m_visible_layer{ visible_layer }
        , m_tank{ m_base_tile_size, m_keyboard_manager }        
    {   }


    void Game::Tick(float deltaTime)
    {
        deltaTime *= 0.001;

        m_keyboard_manager.Update();
        CheckForZoomChange();

        m_tank.Update(m_tilemap, deltaTime);
        m_camera.Update(m_tile_size, m_tank);

        m_camera.Draw(m_visible_layer, m_tilemap, m_tank, m_tile_size, m_zoom);
    }


    void Game::CheckForZoomChange()
    {
        if (m_keyboard_manager.IsJustPressed(SDL_SCANCODE_KP_PLUS))
        {
            ++m_zoom;
            ApplyZoom();
        }
        else if (m_keyboard_manager.IsJustPressed(SDL_SCANCODE_KP_MINUS))
        {
            --m_zoom;
            ApplyZoom();            
        }
    }


    void Game::ApplyZoom()
    {
        m_zoom = Clamp(m_zoom, m_min_zoom, m_max_zoom);
        m_tile_size = m_zoom * m_base_tile_size;
    }
};