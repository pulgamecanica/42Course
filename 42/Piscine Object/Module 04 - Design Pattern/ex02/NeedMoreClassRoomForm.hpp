//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __NEEDMORECLASSROOMFORM_HPP__
# define __NEEDMORECLASSROOMFORM_HPP__

#include <iostream>

#include "Form.hpp"
#include "ClassRoom.hpp"

class NeedMoreClassRoomForm: public Form {
    public:
        NeedMoreClassRoomForm();
        ~NeedMoreClassRoomForm();
        void execute();
        ClassRoom *getClassRoom();
    private:
        ClassRoom * class_room_;
};
#endif
