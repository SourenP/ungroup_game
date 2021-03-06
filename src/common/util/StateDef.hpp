#ifndef StateDef_hpp
#define StateDef_hpp

#include <stdio.h>
#include <vector>

#include <SFML/Network.hpp>

#include "../objects/Group.hpp"
#include "../objects/Mine.hpp"
#include "../physics/PhysicsDef.hpp"
#include "../systems/GroupController.hpp"
#include "../systems/ResourceController.hpp"
#include "game_def.hpp"

enum GameStatus {
    not_started,
    playing,
    game_over,
};

struct GameStateObject {
    std::vector<GroupUpdate> group_updates;
    std::vector<MineUpdate> mine_updates;
    std::vector<PlayerUpdate> player_updates;
    GroupControllerUpdate gcu;
    ResourceControllerUpdate rcu;
};

struct GameStateEvent {
    std::vector<Collision> collisions;
};

struct GameStateCore {
    uint32_t tick;
    GameStatus status;
    uint32_t winner_player_id;
};

struct GameState {
    GameStateObject object;
    GameStateEvent event;
    GameStateCore core;
};

sf::Packet& operator<<(sf::Packet& packet, const GameStatus& game_status);
sf::Packet& operator>>(sf::Packet& packet, GameStatus& game_status);

sf::Packet& operator<<(sf::Packet& packet, const GameStateObject& game_state_object);
sf::Packet& operator>>(sf::Packet& packet, GameStateObject& game_state_object);

sf::Packet& operator<<(sf::Packet& packet, const GameStateEvent& game_state_event);
sf::Packet& operator>>(sf::Packet& packet, GameStateEvent& game_state_event);

sf::Packet& operator<<(sf::Packet& packet, const GameStateCore& game_state_core);
sf::Packet& operator>>(sf::Packet& packet, GameStateCore& game_state_core);

sf::Packet& operator<<(sf::Packet& packet, const GameState& game_state);
sf::Packet& operator>>(sf::Packet& packet, GameState& game_state);

#endif /* StateDef_hpp */
