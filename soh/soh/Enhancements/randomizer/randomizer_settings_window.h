#include <libultraship/libultraship.h>

namespace Rando {
class Settings;
}

class RandomizerSettingsWindow : public Ship::GuiWindow {
  public:
    using GuiWindow::GuiWindow;

    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;

  private:
    void DrawTricksGlitchesTab_LogicTable();
    void DrawCollapseAndOpenTricksButtons(std::unordered_map<RandomizerArea, bool>& map, std::string id);
    void DrawDisabledTricksArea(RandomizerArea area, std::vector<RandomizerTrick> trickIds);
    void DrawDisabledTricksColumn();
    void DrawEnabledTricksArea(RandomizerArea area, std::vector<RandomizerTrick> trickIds);
    void DrawEnabledTricksColumn();
    void DrawTricksGlitchesTab();
    bool mNeedsUpdate = false;
    std::shared_ptr<Rando::Settings> mSettings;
};
