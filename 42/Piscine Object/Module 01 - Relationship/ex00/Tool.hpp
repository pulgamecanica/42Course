//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __TOOL_HPP__
# define __TOOL_HPP__

#include <iostream>

namespace relationship {
    class Worker;

    class Tool {
        public:
            virtual ~Tool();
            const std::string getType() const;
            void updateWorker(Worker *worker);
            const Worker *getWorker() const;
            virtual void use() = 0;
        protected:
            Tool(const std::string &type, int uses);
            int uses_;
        private:
            std::string tool_type_;
            Worker *current_worker_;
    };
    std::ostream& operator<<(std::ostream&, const Tool&);
}
#endif
