//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Scop.hpp"
#include "Window.hpp"
#include "utils.hpp"

using namespace scop;

Scop::Scop(): win(400, 500) {
	status = ScopStatus::Menu;
	created_at = gettimeofday_ms();
	updated_at = gettimeofday_ms();
}

Scop::~Scop() {
	std::cout << "Scop" << " destroyed" << std::endl;
}

ScopStatus Scop::getStatus() const { return (status); };

bool Scop::checkStatus(ScopStatus status) const { return (this->status == status); }

void Scop::runScop() {
  int fps = 0;
  while (!glfwWindowShouldClose(win.glfw_window)) {
      if (timestamp_in_ms(updated_at) < 1000) {
          fps++;
          continue;
      }
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      /* Swap front and back buffers */
      glfwSwapBuffers(win.glfw_window);

      /* Poll for and process events */
      glfwPollEvents();
      
      updated_at = gettimeofday_ms();
      std::cout << "[" << GREEN << updated_at - created_at << ENDC << "] " << fps << " fps " << YELLOW << (status == ScopStatus::Menu ? "Menu" : status == ScopStatus::Settings ? "Settings" : "Rendering") << ENDC << std::endl;
      fps = 0;
  }
}

// Scop::Scop(const Scop& param) {
// 	// TODO (copy constructor)
// 	(void)param;
// }

// Scop& Scop::operator= (const Scop& param) {
// 	// TODO (Assignment operatior)
// 	// std::swap()
// 	(void)param;
// 	return (*this);
// }

std::ostream& scop::operator<<(std::ostream& s, const Scop& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

