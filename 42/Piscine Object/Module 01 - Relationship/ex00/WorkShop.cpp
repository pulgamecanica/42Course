//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "WorkShop.hpp"

namespace relationship {

    WorkShop::WorkShop(const std::string & name): name_(name) {
    }

    WorkShop::~WorkShop() {
        workers_.clear();
    }

    bool WorkShop::hasValidTool(Worker *w) {
        return (w->getTool<Tool>() != NULL);
    }

    const std::string WorkShop::getName() const {
        return name_;
    }

    int WorkShop::registeredUsers() const {
        return workers_.size();
    }

    void WorkShop::registerWorker(Worker *worker) {
        if (hasWorker(worker)) {
            if (DEBUG)
                std::cout << RED << "FAILED TO REGISTER WORKER\t" << ENDC << *this << std::endl;
            std::cerr << YELLOW << "Warning: " << *worker << YELLOW << " is already registered" << ENDC << std::endl;
            return;
        }
        if (!hasValidTool(worker)) {
            if (DEBUG)
                std::cout << RED << "FAILED TO REGISTER WORKER\t" << ENDC << *this << std::endl;
            std::cerr << YELLOW << "Warning: " << *worker << YELLOW << " doesn't have the tools to join this workshop!" << ENDC << std::endl;
            return ;
        }
        worker->addWorkshop(this);
        workers_.push_back(worker);
        if (DEBUG)
            std::cout << GREEN << "REGISTER WORKER\t" << ENDC << *this << std::endl;
    }

    void WorkShop::releaseWorker(Worker *worker) {
        std::vector<Worker *>::iterator it = workers_.end();
        for (std::vector<Worker *>::iterator i = workers_.begin(); i != workers_.end(); ++i) {
            if (worker == *i)
                it = i;
        }
        if (it != workers_.end()) {
            if (DEBUG)
                std::cout << GREEN << "RELEASE WORKER\t" << ENDC << *this << std::endl;
            workers_.erase(it);
        } else {
            if (DEBUG)
                std::cout << RED << "RELEASE HAD NO EFFECT, WORKER IS NOT REGISTERED\t" << ENDC << *this << std::endl;
            return;
        }
        worker->removeWorkshop(this); // Need to convert using dynamic cast and source should be polymorphic (virtual)
    }

    void WorkShop::executeWorkDay() {
        for (std::vector<Worker*>::iterator i = workers_.begin(); i != workers_.end(); ++i) {
            (*i)->work(this);
        }
    }

    bool WorkShop::hasWorker(Worker * worker) const {
        for (std::vector<Worker *>::const_iterator i = workers_.begin(); i != workers_.end(); ++i) {
            if (worker == *i)
                return true;
        }
        return false;
    }

    std::ostream& operator<<(std::ostream& s, const WorkShop&ws) {
        s << BLUE << ws.getName() << ENDC << " "<< ws.registeredUsers() << " workers";
        return s;
    }
}
