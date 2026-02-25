#include <iostream>
#include <memory>

#include "colony.hh"
#include "nurturer.hh"
#include "provider.hh"
#include "queen.hh"

int main()
{
    auto colony = std::make_shared<Colony>();

    auto q = Queen{ colony, Stage::ADULT };

    auto p1 = Provider{ colony, Stage::ADULT };
    auto p2 = Provider{ colony, Stage::ADULT };

    auto n1 = Nurturer{ colony, Stage::ADULT };
    auto n2 = Nurturer{ colony, Stage::ADULT };

    colony->addAnt(q);
    colony->addAnt(p1);
    colony->addAnt(p2);
    colony->addAnt(n1);
    colony->addAnt(n2);

    // Run several simulation steps and show clear output separators.
    const int steps = 10;
    for (int step = 1; step <= steps; ++step)
    {
        colony->display();
        std::cout << "======== STEP " << step << " ========" << std::endl;
        colony->step();
        std::cout << std::endl;
    }

    std::cout << "Simulation finished." << std::endl;
    return 0;
}
