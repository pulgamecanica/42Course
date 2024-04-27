//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "WoodWorking.hpp"
#include "Hammer.hpp"

namespace relationship {
    WoodWorking::WoodWorking(): WorkShop("Wood Working for dummies") {
    }

    WoodWorking::~WoodWorking() {
    }

    bool WoodWorking::hasValidTool(Worker *w) {
        return (w->getTool<Hammer>() != NULL);
    }
}
