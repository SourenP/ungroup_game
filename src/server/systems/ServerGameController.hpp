#ifndef ServerGameController_hpp
#define ServerGameController_hpp

#include <memory>
#include <stdio.h>
#include <unordered_map>
#include <vector>

#include "../../common/events/Event.hpp"
#include "../../common/systems/GameController.hpp"
#include "NetworkingServer.hpp"

class ServerGameController : public GameController {
  public:
    explicit ServerGameController(size_t max_player_count, size_t max_mine_count);
    ~ServerGameController();

  private:
    PlayerInputs collectInputs() override;
    void setNetworkState() override;
    void incrementTick() override;
    unsigned int getTick() override;
    void setTick(unsigned int tick) override;
    void step(const PlayerInputs& pi, sf::Int32 delta_ms) override;
    void handleClientConnectedEvent(std::shared_ptr<Event> event);
    void addEventListeners();

    std::unique_ptr<NetworkingServer> m_networkingServer;
};

#endif /* ServerGameController_hpp */
