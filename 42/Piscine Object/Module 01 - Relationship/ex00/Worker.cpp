//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Worker.hpp"
#include "Tool.hpp"
#include "WorkShop.hpp"

namespace relationship {
    Worker::Worker(const std::string & name, const Position & coord): coord_(coord), name_(name) {
        if (DEBUG)
            std::cout << GREEN << "CREATE\t" << ENDC << *this << std::endl;
    }

    Worker::~Worker() {
        tools_.clear();
    }

    const Position& Worker::getCoord() const {
        return coord_;
    }

    const Statistic& Worker::getStat() const {
        return stat_;
    }

    const std::string& Worker::getName() const {
        return name_;
    }

    void Worker::printTools() const {
        std::cout << "Tools (" << tools_.size() << ")" << std::endl;
        for (std::vector<Tool *>::const_iterator i = tools_.begin(); i != tools_.end(); ++i) {
            std::cout << " - " << **i << std::endl;
        }
    }

    void Worker::printWorkShops() const {
        std::cout << BLUE << getName() << "'s" << ENDC << " WorkShops (" << workshops_.size() << ")" << std::endl;
        for (std::vector<WorkShop<Tool> *>::const_iterator i = workshops_.begin(); i != workshops_.end(); ++i) {
            // std::cout << "- " << *(dynamic_cast<WorkShop<Tool> *>(*i)) << std::endl;
            // std::cout << " - " << **i << std::endl;
            std::cout << " - " << std::endl;
        }
    }

    bool Worker::hasTool(Tool *tool) const {
        for (std::vector<Tool *>::const_iterator i = tools_.begin(); i != tools_.end(); ++i) {
            if (tool == *i)
                return true;
        }
        return false;
    }

    void Worker::addTool(Tool *tool) {
        if (hasTool(tool)) {
            if (DEBUG)
                std::cout << RED << "FAILED TO ADD TOOL\t" << ENDC << *this << std::endl;
            std::cerr << YELLOW << "Warning: " << *tool << YELLOW << " is owned by this worker" << ENDC << std::endl;
            return;
        }
        if (DEBUG)
            std::cout << GREEN << "ADD TOOL\t" << ENDC << *this << std::endl;
        tool->updateWorker(this);
        tools_.push_back(tool);
    }

    void Worker::removeTool(Tool *tool) {
        std::vector<Tool *>::iterator it = tools_.end();
        for (std::vector<Tool *>::iterator i = tools_.begin(); i != tools_.end(); ++i) {
            if (tool == *i)
                it = i;
        }
        if (it != tools_.end()) {
            if (DEBUG)
                std::cout << GREEN << "REMOVE TOOL\t" << ENDC << *this << std::endl;
            tools_.erase(it);
        } else {
            if (DEBUG)
                std::cout << RED << "REMOVE TOOL HAD NO EFFECT TOOL NOT FOUNDED\t" << ENDC << *this << std::endl;
            return;
        }
        tool->updateWorker(NULL);
        // Check Workshops
        std::vector<std::vector<WorkShop<Tool> *>::iterator> invalid_tool_workshops;
        for (std::vector<WorkShop<Tool> *>::iterator i = workshops_.begin(); i != workshops_.end(); ++i) {
            if (!*i)
                continue ;
            if (!(*i)->hasValidTool(this)) {
                std::cout << *this << " Can no longer belong to " << *i << std::endl;
                invalid_tool_workshops.push_back(i);
            }
        }
    }

    void Worker::addWorkshop(WorkShop<Tool> *ws) {
        for (std::vector<WorkShop<Tool> *>::const_iterator i = workshops_.begin(); i != workshops_.end(); ++i) {
            if (*i == ws)
                return ;
        }
        if (DEBUG)
            std::cout << "GREEN" << "WORKER JOIN WORKSHOP\t" << *ws << std::endl;
        workshops_.push_back(ws);
    }

    void Worker::removeWorkshop(WorkShop<Tool> *ws) {
        std::vector<WorkShop<Tool> *>::iterator it = workshops_.end();
        for (std::vector<WorkShop<Tool> *>::iterator i = workshops_.begin(); i != workshops_.end(); ++i) {
            if (*i == ws)
                it = i;
        }
        if (it != workshops_.end()) {
            workshops_.erase(it);
            if (DEBUG)
                std::cout << "GREEN" << "WORKER JOIN WORKSHOP\t" << *ws << std::endl;
        }
    }

    std::ostream& operator<<(std::ostream& s, const Worker& w) {
        s << w.getName() << " (" << w.getCoord() << ") " << w.getStat();
        return (s);
    }
}
