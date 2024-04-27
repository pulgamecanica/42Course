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
    class WorkShop {
        public:
            WorkShop(const std::string & name);
            virtual ~WorkShop();
            virtual bool hasValidTool(Worker *w);
            const std::string getName() const;
            int registeredUsers() const;
            void registerWorker(Worker *worker);
            void releaseWorker(Worker *worker);
            void executeWorkDay();
        private: 
            bool hasWorker(Worker * worker) const;
            const std::string name_;
            std::vector<Worker*> workers_;
    };
    std::ostream& operator<<(std::ostream& s, const WorkShop&ws);
}
#endif
