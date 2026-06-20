
#include <libultraship/libultraship.h>
#include "randomizerTypes.h"
#include "soh/Graph/Graph.h"
#include <optional>

class EntranceTrackerGraphWindow final : public Ship::GuiWindow {
    public:
        using GuiWindow::GuiWindow;
        void Draw() override;

        void InitElement() override;
        void DrawElement() override;
        void UpdateElement() override{};
        void UpdateGraphOptions();

    private:
        std::optional<Graph> graph = std::nullopt;
        bool menuOpen = false;
        bool sufficientlyStabilized = false;
};