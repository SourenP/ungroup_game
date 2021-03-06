/**
 * Renders the server's terminal UI.
 */

#ifndef TerminalRenderingController_hpp
#define TerminalRenderingController_hpp

#include <unordered_map>

#include "../../../extern/termbox/src/termbox.h"

#include "../../common/objects/Player.hpp"
#include "../util/TermboxTable.hpp"
#include "../util/TermboxTextOutputArea.hpp"

class TerminalRenderingController {
  public:
    TerminalRenderingController(uint32_t tcp_port);
    ~TerminalRenderingController(){};

    void draw(std::unordered_map<uint32_t, float> player_ids_to_updates_rates,
              std::unordered_map<uint32_t, float> player_ids_to_avg_tick_drifts,
              float broadcast_game_state_rate, std::vector<PlayerUpdate> player_updates,
              float game_step_rate, float game_update_rate, uint32_t tick);

    void preUpdate();
    void update();
    void postUpdate();
    void addWinner(const std::string& winner_player_id);

  private:
    bool m_shutting_down = false;

    TermboxTable m_termboxPlayerTable;
    TermboxTable m_termboxServerStatsTable;
    TermboxTextOutputArea m_termboxTextOutputArea;
};

#endif /* TerminalRenderingController_hpp */
