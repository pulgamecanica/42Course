//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __WORKER_HPP__
# define __WORKER_HPP__

#include <iostream>
#include <vector>

#include "Position.hpp"
#include "Statistic.hpp"
#include "Hammer.hpp"
#include "Shovel.hpp"

namespace relationship {
    class WorkShop;
    class Tool;
    
    class Worker {
        public:
            Worker(const std::string & name, const Position & pos = Position(42, 42, 42));
            ~Worker();
            const Position& getCoord() const;
            const Statistic& getStat() const;
            const std::string& getName() const;
            void printTools() const;
            void printWorkShops() const;
            void addTool(Tool *t);
            void removeTool(Tool *t);
            void addWorkshop(WorkShop *ws);
            void removeWorkshop(WorkShop *ws);
            void work(WorkShop * ws);
            template<class T>
            Tool* getTool() {
                for (std::vector<Tool *>::iterator i = tools_.begin(); i != tools_.end(); ++i) {
                    if (dynamic_cast<T*>(*i))
                        return *i;
                }
                return NULL;
            }
        private:
            bool hasTool(Tool *t) const;
            bool hasWorkShop(WorkShop *t) const;
            Position coord_;
            Statistic stat_;
            std::vector<Tool*> tools_;
            std::vector<WorkShop *> workshops_;
            const std::string name_;
    };
    std::ostream& operator<<(std::ostream&, const Worker&);
}
#endif
