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
    template<class T>
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
            void addWorkshop(WorkShop<Tool> *ws);
            void removeWorkshop(WorkShop<Tool> *ws);
            // Implementation must be on the hpp file
            template<class T>
            T* getTool() {
                T *res;
                for (std::vector<Tool *>::iterator i = tools_.begin(); i != tools_.end(); ++i) {
                    if ((res = dynamic_cast<T *>(*i)))
                        return res;
                }
                return NULL;
            }
        private:
            bool hasTool(Tool *t) const;
            Position coord_;
            Statistic stat_;
            std::vector<Tool*> tools_;
            std::vector<WorkShop<Tool> *> workshops_;
            const std::string name_;
    };
    std::ostream& operator<<(std::ostream&, const Worker&);
}
#endif
