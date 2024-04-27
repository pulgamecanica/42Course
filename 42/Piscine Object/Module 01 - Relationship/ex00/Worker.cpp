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
        workshops_.clear();
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
        std::cout << BLUE << getName() << "'s" << ENDC << " Tools (" << tools_.size() << ")" << std::endl;
        for (std::vector<Tool *>::const_iterator i = tools_.begin(); i != tools_.end(); ++i) {
            if (dynamic_cast<Tool *>(*i))
                std::cout << " - " << **i << std::endl;
        }
    }

    void Worker::printWorkShops() const {
        std::cout << BLUE << getName() << "'s" << ENDC << " WorkShops (" << workshops_.size() << ")" << std::endl;
        for (std::vector<WorkShop *>::const_iterator i = workshops_.begin(); i != workshops_.end(); ++i) {
            std::cout << " - " << **i << std::endl;
        }
    }

    bool Worker::hasTool(Tool *tool) const {
        for (std::vector<Tool *>::const_iterator i = tools_.begin(); i != tools_.end(); ++i) {
            if (tool == *i)
                return true;
        }
        return false;
    }

    bool Worker::hasWorkShop(WorkShop *ws) const {
        for (std::vector<WorkShop *>::const_iterator i = workshops_.begin(); i != workshops_.end(); ++i) {
            if (*i == ws)
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
        std::vector<std::vector<WorkShop *>::iterator> invalid_tool_workshops;
        for (std::vector<WorkShop *>::iterator i = workshops_.begin(); i != workshops_.end(); ++i) {
            if (!*i)
                continue ;
            if (!(*i)->hasValidTool(this)) {
                if (DEBUG)
                    std::cout << *this << " Can no longer belong to " << **i << std::endl;
                invalid_tool_workshops.push_back(i);
            }
        }
        // Need an extra iterator, because releaseWorker (through removeWorkshop) will loop through workshops_ (can't be erasing while looping!)
        for (std::vector<std::vector<WorkShop *>::iterator>::iterator i = invalid_tool_workshops.begin(); i != invalid_tool_workshops.end(); ++i) {
            (**i)->releaseWorker(this);
        }
    }

    void Worker::addWorkshop(WorkShop *ws) {
        if (!hasWorkShop(ws)) {
            if (DEBUG)
                std::cout << GREEN << "WORKER JOIN WORKSHOP\t" << ENDC << *ws << std::endl;
            workshops_.push_back(ws);
        }
    }

    void Worker::removeWorkshop(WorkShop *ws) {
        std::vector<WorkShop *>::iterator it = workshops_.end();
        for (std::vector<WorkShop *>::iterator i = workshops_.begin(); i != workshops_.end(); ++i) {
            if (*i == ws)
                it = i;
        }
        if (it != workshops_.end()) {
            workshops_.erase(it);
            if (DEBUG)
                std::cout << GREEN << "WORKER JOIN WORKSHOP\t" << ENDC << *ws << std::endl;
        }
    }

    void Worker::work(WorkShop * ws) {
        if (hasWorkShop(ws)) {
            std::cout << BLUE << this->getName() << YELLOW << "\tWORKING ON\t" << *ws << std::endl;
        } else if (DEBUG) {
            std::cout << BLUE << this->getName() << RED << "\tCANT WORK, NOT REGISTERED\t" << ENDC << *ws << std::endl;
        }
    }


    std::ostream& operator<<(std::ostream& s, const Worker& w) {
        s << w.getName() << " (" << w.getCoord() << ") " << w.getStat();
        return (s);
    }
}
