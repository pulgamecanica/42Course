//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Position.hpp"
#include "Worker.hpp"
#include "Tool.hpp"
#include "Shovel.hpp"
#include "Hammer.hpp"
#include "WorkShop.hpp"
#include "WoodWorking.hpp"
#include "Shoveling101.hpp"

int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    {
        // Abstract Proof
        // relationship::Tool t("some_tool", 42);
        // t.get_type();
        // std::cout << t << std::endl;
    }
    
    std::cout << YELLOW << "- - - - - - - - - - - WORKER INDEPENDENCE - - - - - - - - - - - -" << ENDC << std::endl;
    
    relationship::Worker raul("Raul");
    {
        relationship::Shovel s;
        raul.addTool(&s);
    }
    std::cout << raul << std::endl;
    
    std::cout << YELLOW << "- - - - - - - - - - - TOOL INDEPENDENCE - - - - - - - - - - - -" << ENDC << std::endl;

    relationship::Shovel shovshov;
    {
        relationship::Worker miguel("Miguel");
        miguel.addTool(&shovshov);
    }
    std::cout << shovshov.getType() << std::endl;

    std::cout << YELLOW << "- - - - - - - - - - - INIT - - - - - - - - - - - -" << ENDC << std::endl;
   
    relationship::Worker w("Digger1");
    relationship::Worker w2("Digger2", relationship::Position(6, 6, 6));
    relationship::Tool * tool;
    relationship::Shovel s;
    tool = &s;

    std::cout << YELLOW << "OBJ: " << *tool << std::endl;
    std::cout << YELLOW << "OBJ: " << BLUE << w << ENDC << std::endl;
    std::cout << YELLOW << "OBJ: " << BLUE << w2 << ENDC << std::endl;

    std::cout << YELLOW << "- - - - - - - - - - - ADD ACTIONS (Shovel) - - - - - - - - - - - -" << ENDC << std::endl;
   
    w.addTool(tool);
    std::cout << *tool << std::endl;
    w.printTools();
    w.addTool(tool); // Add Same Tool 2 times Error check
    w2.addTool(tool);// Add Tool to another worker
    std::cout << *tool << std::endl;
    w.printTools();
    w2.printTools();
    
    std::cout << YELLOW << "- - - - - - - - - - - - REMOVE ACTIONS (Shovel) - - - - - - - - - - -" << ENDC << std::endl;
    
    w2.removeTool(tool); // Remove tool from worker 2
    w2.removeTool(tool); // Remove tool again Error check
    w2.printTools();

    std::cout << YELLOW << "- - - - - - - - - - - RE INIT - - - - - - - - - - - -" << ENDC << std::endl;

    relationship::Worker w3("WoodPicker1", relationship::Position(9, 6, 3));
    relationship::Worker w4("RoofPlacer2", relationship::Position(0, 0, 0));
    relationship::Tool * tool1;
    relationship::Tool * tool2;
    relationship::Hammer h1;
    relationship::Hammer h2;
    tool1 = &h1;
    tool2 = &h2;

    std::cout << YELLOW << "OBJ: " << *tool1 << std::endl;
    std::cout << YELLOW << "OBJ: " << *tool2 << std::endl;
    std::cout << YELLOW << "OBJ: " << BLUE << w3 << ENDC << std::endl;
    std::cout << YELLOW << "OBJ: " << BLUE << w4 << ENDC << std::endl;

    std::cout << YELLOW << "- - - - - - - - - - - ADD ACTIONS (Hammer) - - - - - - - - - - - -" << ENDC << std::endl;
    
    w3.addTool(tool1);
    w3.addTool(tool2);
    w3.addTool(tool2); // Testing error
    w3.printTools();
    std::cout << *tool1 << std::endl;
    std::cout << *tool2 << std::endl;
    w4.addTool(tool1);
    w4.addTool(tool1); // Testing error
    w3.printTools();
    w4.printTools();
    std::cout << *tool1 << std::endl;
    std::cout << *tool2 << std::endl;

    std::cout << YELLOW << "- - - - - - - - - - - - REMOVE ACTIONS (Hammer) - - - - - - - - - - -" << ENDC << std::endl;
   
    w3.removeTool(tool1); // Error, he no longer has tool 1, because w4 took it
    w3.removeTool(tool2);
    w4.removeTool(tool1);
    std::cout << *tool1 << std::endl;
    std::cout << *tool2 << std::endl;

    std::cout << YELLOW << "- - - - - - - - - - - ADD TOOLS (Shovel & Hammer) - - - - - - - - - - - -" << ENDC << std::endl;
   
    w3.addTool(tool1);
    w3.addTool(tool);
    w3.addTool(tool2);
    w3.printTools();

    std::cout << YELLOW << "- - - - - - - - - - - GET TOOLS (Hammer) - - - - - - - - - - - -" << ENDC << std::endl;
    {
        // Create one shovel and one hammer
        relationship::Hammer the_hammer;
        relationship::Shovel the_shovel;
        // Create one Worker
        relationship::Worker worker("Bob The Builder"); // The constructor migth change depending how you decided to initialize the worker
        // give both tools to the Worker
        worker.addTool(&the_hammer);
        worker.addTool(&the_shovel);
        // Get the two tools using the bonus function
        relationship::Hammer * first_hammer = dynamic_cast<relationship::Hammer *>(worker.getTool<relationship::Hammer>());
        relationship::Shovel * first_shovel = dynamic_cast<relationship::Shovel *>(worker.getTool<relationship::Shovel>());
        if (first_hammer)
            std::cout << "There is a hammer" << std::endl;
        if (first_shovel)
            std::cout << "There is a shovel" << std::endl;

        worker.removeTool(&the_hammer);
        first_hammer = dynamic_cast<relationship::Hammer *>(worker.getTool<relationship::Hammer>());
        if (!first_hammer)
            std::cout << "There is a No hammer :)" << std::endl;
        else
            std::cout << *first_hammer << std::endl;

    }
    // // try hammer now
    // relationship::Hammer * first_hammer = NULL;
    // first_hammer = dynamic_cast<relationship::Hammer *>(w3.getTool<relationship::Hammer>());
    // if (first_hammer != NULL)
    //     std::cout << "GetTool<Hammer>: " << *first_hammer << std::endl;
    // else
    //     std::cout << "GetTool<Hammer> Returned NULL, no tool :(" << std::endl;
    // // w doenst have a hammer, should return null
    // first_hammer = dynamic_cast<relationship::Hammer *>(w.getTool<relationship::Hammer>());
    // if (first_hammer != NULL)
    //     std::cout << "GetTool<Hammer>: " << *first_hammer << std::endl;
    // else
    //     std::cout << "GetTool<Hammer> " << w << RED << " has no Hammer :(" << ENDC << std::endl;
    // // try shovel now
    // relationship::Shovel * first_shovel = dynamic_cast<relationship::Shovel *>(w3.getTool<relationship::Shovel>());
    // if (first_shovel)
    //     w.addTool(first_shovel);
    // first_shovel = dynamic_cast<relationship::Shovel *>(w3.getTool<relationship::Shovel>());
    // if (first_shovel != NULL)
    //     std::cout << "GetTool<Shovel>: " << *first_shovel << std::endl;
    // else
    //     std::cout << "GetTool<Shovel> " << w << RED << " has no Shovel :(" << ENDC << std::endl;
    
    std::cout << YELLOW << "- - - - - - - - - - - - - WORKSHOPS INIT - - - - - - - - - - - - - -" << ENDC << std::endl;
    
    relationship::WoodWorking hammering_ws;
    relationship::Shoveling101 shoveling_ws;
    relationship::WorkShop common_ws("Common Core 101");

    // Tools    
    relationship::Tool * hammer1;
    relationship::Tool * hammer2;
    relationship::Tool * shovel1;
    relationship::Tool * shovel2;
    relationship::Hammer hm1;
    relationship::Hammer hm2;
    relationship::Shovel sv1;
    relationship::Shovel sv2;
    hammer1 = &hm1;
    hammer2 = &hm2;
    shovel1 = &sv1;
    shovel2 = &sv2;

    // Workers
    relationship::Worker ws_w1("Billy", relationship::Position(19, 536, -123));
    relationship::Worker ws_w2("Magie", relationship::Position(96, -63, 32));
    relationship::Worker ws_w3("Ada", relationship::Position(-469, 876, 53));
    relationship::Worker ws_w4("Karla", relationship::Position(-42, -6, -24));
    relationship::Worker *billy = &ws_w1;
    relationship::Worker *magie = &ws_w2;
    relationship::Worker *ada = &ws_w3;
    relationship::Worker *karla = &ws_w4;

    (void)billy; // Poor billy, he has no tools :(
    magie->addTool(hammer1);
    magie->addTool(shovel1); // Magie is a crack, she is rich and bought all tools
    ada->addTool(hammer2);   // Ada has a hammer 
    karla->addTool(shovel2); // Karla has a shovel

    billy->printTools();
    magie->printTools();
    ada->printTools();
    karla->printTools();

    std::cout << YELLOW << "- - - - - - - - - - - - - - REGISTER WORKERS - - - - - - - - - - - - -" << ENDC << std::endl;
    
    common_ws.registerWorker(billy); // Nope
    common_ws.registerWorker(magie); // Yes
    common_ws.registerWorker(magie); // Nope, can't register twice
    common_ws.registerWorker(karla); // Yes
    common_ws.registerWorker(ada); // Yes

    hammering_ws.registerWorker(ada); // Yes
    hammering_ws.registerWorker(magie); // Yes
    hammering_ws.registerWorker(karla); // No

    shoveling_ws.registerWorker(magie); // Yes
    shoveling_ws.registerWorker(ada); // No
    shoveling_ws.registerWorker(karla); // Yes

    std::cout << hammering_ws << std::endl;
    std::cout << shoveling_ws << std::endl;
    std::cout << common_ws << std::endl;
 
    billy->printWorkShops();
    magie->printWorkShops();
    ada->printWorkShops();
    karla->printWorkShops();

    std::cout << YELLOW << "- - - - - - - - - - - - - - RELEASE WORKERS - - - - - - - - - - - - -" << ENDC << std::endl;

    common_ws.releaseWorker(billy); // No, Billy was never accepted :(
    common_ws.releaseWorker(magie); // Yes
    common_ws.releaseWorker(ada);   // Yes
    common_ws.releaseWorker(karla); // Yes

    hammering_ws.releaseWorker(ada); // Yes
    hammering_ws.releaseWorker(magie); // Yes
    hammering_ws.releaseWorker(karla); // No, was never accepted :(

    shoveling_ws.releaseWorker(magie); // Yes
    shoveling_ws.releaseWorker(ada); // No, was never accepted :(
    shoveling_ws.releaseWorker(karla); // Yes

    std::cout << hammering_ws << std::endl;
    std::cout << shoveling_ws << std::endl;
    std::cout << common_ws << std::endl;

    std::cout << YELLOW << "- - - - - - - - - - - - - REGISTER AGAIN WORKERS - - - - - - - - - - -" << ENDC << std::endl;
    
    common_ws.registerWorker(magie); // Yes
    common_ws.registerWorker(karla); // Yes
    common_ws.registerWorker(ada); // Yes
    hammering_ws.registerWorker(ada); // Yes
    hammering_ws.registerWorker(magie); // Yes
    shoveling_ws.registerWorker(magie); // Yes
    shoveling_ws.registerWorker(karla); // Yes

    std::cout << hammering_ws << std::endl;
    std::cout << shoveling_ws << std::endl;
    std::cout << common_ws << std::endl;

    std::cout << YELLOW << "- - - - - - - - - - - - - - INDIRECT REMOVE TOOL, RELEASE WORKERS (Hammer) - - - - - - - - - - - - -" << ENDC << std::endl;

    // Indirect Unregister (someone takes your tool)
    billy->addTool(hammer2); // Billy get's Ada's Hammer (ADA SHOULD BE UNREGISTERED)
    
    ada->printTools();
    ada->printWorkShops();

    billy->printTools();
    billy->printWorkShops();

    std::cout << YELLOW << "- - - - - - - - - - - - - - DIRECT REMOVE TOOL, RELEASE WORKERS (Hammer) - - - - - - - - - - - - -" << ENDC << std::endl;

    common_ws.registerWorker(billy); // Yes, now Billy can Hammer

    billy->printWorkShops();

    // Direct Unregister (after loosing the tool)
    billy->removeTool(hammer2);; // Billy should be unregistered

    billy->printTools();
    billy->printWorkShops();

    std::cout << hammering_ws << std::endl;
    std::cout << shoveling_ws << std::endl;
    std::cout << common_ws << std::endl;

    std::cout << YELLOW << "- - - - - - - - - - - - - - REMOVE TOOL, RELEASE WORKERS (Hammer, Shovel & Tool) - - - - - - - - - - - - -" << ENDC << std::endl;

    // Also test with a concrete class instead of the "Tool" implementation

    // Magie looses all her tools, she's Unregistered from everything! She missbehaved, this rich kid!
    magie->removeTool(hammer1); // Magie should be unregistered
    magie->removeTool(shovel1); // Magie should be unregistered from both shoveling and common
    shoveling_ws.registerWorker(magie); // Can't register, she was muggggeeed!

    std::cout << hammering_ws << std::endl;
    std::cout << shoveling_ws << std::endl;
    std::cout << common_ws << std::endl;

    billy->printWorkShops();
    magie->printWorkShops();
    ada->printWorkShops();
    karla->printWorkShops();

    std::cout << YELLOW << "- - - - - - - - - - - - - REGISTER AGAIN WORKERS - - - - - - - - - - -" << ENDC << std::endl;
    
    // Everyone gets their tools back
    ada->addTool(hammer2);
    magie->addTool(shovel1);
    magie->addTool(hammer1);
    common_ws.registerWorker(magie); // Yes
    common_ws.registerWorker(karla); // Yes
    common_ws.registerWorker(ada); // Yes
    hammering_ws.registerWorker(ada); // Yes
    hammering_ws.registerWorker(magie); // Yes
    shoveling_ws.registerWorker(magie); // Yes
    shoveling_ws.registerWorker(karla); // Yes

    std::cout << hammering_ws << std::endl;
    std::cout << shoveling_ws << std::endl;
    std::cout << common_ws << std::endl;

    billy->printWorkShops();
    magie->printWorkShops();
    ada->printWorkShops();
    karla->printWorkShops();

    std::cout << YELLOW << "- - - - - - - - - - - - - WORKERS WORK - - - - - - - - - - -" << ENDC << std::endl;

    billy->work(&common_ws); // No
    magie->work(&common_ws); // Yes
    ada->work(&common_ws);   // Yes
    karla->work(&common_ws); // Yes

    billy->work(&hammering_ws); // No
    magie->work(&hammering_ws); // Yes
    ada->work(&hammering_ws);   // Yes
    karla->work(&hammering_ws); // No

    billy->work(&shoveling_ws); // No
    magie->work(&shoveling_ws); // Yes
    ada->work(&shoveling_ws);   // No
    karla->work(&shoveling_ws); // Yes

    std::cout << YELLOW << "- - - - - - - - - - - - - WORKSHOPS LAUNCH WORK DAY - - - - - - - - - - -" << ENDC << std::endl;

    std::cout << WHITE << "Karla was shoveling when a balrog woke up from the depth of the mines, Karla escaped but lost her shovel on the way..." << ENDC << std::endl;
    karla->removeTool(shovel2); // Karla was shoveling when a balrog woke up from the depth of the mines, Karla escaped but lost her shovel on the way...
    std::cout << WHITE << "Magie lost here hammer, she is so silly! That is common among rich girls." << ENDC << std::endl;
    magie->removeTool(hammer1); // Magie lost here hammer, she is so silly! That is common among rich girls
    common_ws.executeWorkDay();
    hammering_ws.executeWorkDay();
    shoveling_ws.executeWorkDay();

    std::cout << YELLOW << "- - - - - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << ENDC << std::endl;
    return (0);
}
