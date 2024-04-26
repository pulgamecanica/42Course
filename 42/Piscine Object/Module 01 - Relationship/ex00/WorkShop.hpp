//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __WORKSHOP_HPP__
# define __WORKSHOP_HPP__

#include <iostream>
#include <vector>

#include "ex00.inc"
#include <Worker.hpp>

namespace relationship {
    template<class T>
    class WorkShop {
        public:
            WorkShop(const std::string & name): name_(name) {
            }
            virtual ~WorkShop() {
                workers_.clear();
            }
            bool hasValidTool(Worker *w) {
                return (w->getTool<T>() != NULL);
            }

            const std::string getName() const {
                return name_;
            }

            int registeredUsers() const {
                return workers_.size();
            }
            void registerWorker(Worker *worker) {
                if (hasWorker(worker)) {
                    if (DEBUG)
                        std::cout << RED << "FAILED TO REGISTER WORKER\t" << ENDC << *this << std::endl;
                    std::cerr << YELLOW << "Warning: " << *worker << YELLOW << " is already registered" << ENDC << std::endl;
                    return;
                }
                if (!hasValidTool(worker)) {
                    std::cerr << YELLOW << "Warning: " << *worker << YELLOW << " doesn't have the tools to join this workshop!" << ENDC << std::endl;
                    return ;
                }
                if (DEBUG)
                    std::cout << GREEN << "REGISTER WORKER\t" << ENDC << *this << std::endl;
                // worker->addWorkshop(this); // Need to convert using dynamic cast and source should be polymorphic (virtual)
                worker->addWorkshop(dynamic_cast<WorkShop<Tool>* >(this));
                workers_.push_back(worker);
            }
            void releaseWorker(Worker *worker) {
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
                // worker->removeWorkshop(this); // Need to convert using dynamic cast and source should be polymorphic (virtual)
                worker->removeWorkshop(dynamic_cast<WorkShop<Tool>* >(this));

            }
        private: 
            bool hasWorker(Worker * worker) const {
                for (std::vector<Worker *>::const_iterator i = workers_.begin(); i != workers_.end(); ++i) {
                    if (worker == *i)
                        return true;
                }
                return false;
            }
            const std::string name_;
            std::vector<Worker*> workers_;
    };
    template<class T>
    std::ostream& operator<<(std::ostream& s, const WorkShop<T>&ws) {
        s << "WorkShop (" << ws.getName() << ") "<< ws.registeredUsers() << " workers";
        return s;
    }
}
#endif
