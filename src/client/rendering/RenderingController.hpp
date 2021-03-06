
/**
 * Renders game.
 * Manages animations and ui.
 */

#ifndef RenderingController_hpp
#define RenderingController_hpp

#include <SFML/Graphics.hpp>

#include "../../common/systems/GameObjectController.hpp"
#include "../../common/systems/GameObjectStore.hpp"
#include "../../common/util/game_settings.hpp"
#include "../rendering/GUIController.hpp"
#include "AnimationController.hpp"
#include "BackgroundController.hpp"
#include "GameObjectRenderer.hpp"

class RenderingController {
  public:
    RenderingController(sf::RenderWindow& window, GameObjectController& goc, GameObjectStore& gos,
                        uint32_t player_id, bool headless);
    ~RenderingController(){};

    void draw();

    void preUpdate();
    void update(sf::Int32 delta_ms);
    void postUpdate(sf::Int32 update_time, const UIData& ui_data);

  private:
    void updateCamera(sf::Int32 delta_ms);
    void drawGameOver();
    void drawPlaying();
    void drawNotStarted();

    sf::RenderWindow& m_window;
    sf::View m_playerView; // View specific to player
    sf::View m_windowView; // View of window. Used to draw UI elements independent of player view.
    sf::RenderTexture m_buffer; // Buffer onto which game is drawn offscreen.
    sf::Sprite m_bufferSprite;  // Sprite used to draw m_buffer.

    // Data needed for rendering
    UIData m_uiData;
    uint32_t m_playerId;

    sf::Text m_winnerText;
    sf::Text m_connectingText;

    GameObjectController& m_gameObjectController;
    ResourceStore m_resourceStore;
    AnimationController m_animationController;
    GUIController m_guiController;
    BackgroundController m_backgroundController;
    GameObjectRenderer m_gameObjectRenderer;

    bool m_headless;
};

#endif /* RenderingController_hpp */