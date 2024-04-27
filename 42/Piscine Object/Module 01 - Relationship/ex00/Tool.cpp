//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Tool.hpp"
#include "Worker.hpp"

namespace relationship {
    Tool::Tool(const std::string & type, int uses): uses_(uses), tool_type_(type), current_worker_(NULL) {
    }

    Tool::~Tool() {
    }

    const std::string Tool::getType() const {
        return tool_type_;
    }

    void Tool::updateWorker(Worker * worker) {
        if (worker == NULL) {
            current_worker_ = NULL;
            if (DEBUG)
                std::cout << GREEN << "UPDATE TOOL WORKER\t" << ENDC << *this << std::endl;
            return ;
        }
        if (current_worker_ != NULL && worker != NULL)
            current_worker_->removeTool(this);
        current_worker_ = worker;
        if (DEBUG)
            std::cout << GREEN << "UPDATE TOOL WORKER\t" << ENDC << *this << std::endl;
    }

    const Worker *Tool::getWorker() const {
        return current_worker_;
    }

    std::ostream& operator<<(std::ostream& s, const Tool& tool) {
        const Worker * w = tool.getWorker();
        if (w != NULL)
            s << BLUE << tool.getType() << ENDC << " " << *w;
        else
            s << BLUE << tool.getType() << ENDC << " No Worker has this tool";
        return (s);
    }
}
