//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "GUIContainer.hpp"

GUIContainer::GUIContainer(scop::Vulkan42 * & vk, scop::Window * & win): vk(vk), win(win) {
  // Setup Dear ImGui Vulkan Window
  wd = vk->getImGui_ImplVulkanH_Window();
  if (!wd)
    throw std::runtime_error("ImGui Error!");	
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForVulkan(win->glfw_window, true);
  
  ImGui_ImplVulkan_InitInfo init_info = vk->getImGui_ImplVulkan_InitInfo();
  
  ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
  
  (void)init_info;

	// TODO (default constructor)
}

GUIContainer::~GUIContainer() {
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  ImGui_ImplVulkanH_DestroyWindow(vk->getInstance(), vk->getDevice(), wd, nullptr);
  free(wd);
	std::cout << "GUIContainer" << " destroyed" << std::endl;
}

bool showDemo = true;

void	GUIContainer::update() {

	ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Render();
  // ImGui::ShowDemoWindow(&showDemo);
}

std::ostream& operator<<(std::ostream& s, const GUIContainer& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

