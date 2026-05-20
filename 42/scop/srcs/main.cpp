#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <getopt.h>
#include <cstdio>
#include <cstring>
#include <string>

#include "Scop.hpp"
#include "colors.hpp"

namespace {

void printUsage(const char* argv0) {
  std::printf(
    "%sscop%s -- 42 OBJ viewer (Vulkan)\n"
    "\n"
    "Usage: %s [options]\n"
    "\n"
    "Options:\n"
    "  -m, --model <path>   Initial .obj file to load\n"
    "                       (default: first file in assets/models/)\n"
    "  -h, --help           Show this help message and exit\n"
    "\n"
    "Controls (while running):\n"
    "  W/A/S/D   translate in X/Y\n"
    "  Q/E       translate in -Z/+Z\n"
    "  T         toggle texture with smooth blend\n"
    "  R         cycle rotation axis (X -> Y -> Z)\n"
    "  O         cycle through models in assets/models/\n"
    "  ESC       quit\n",
    GREEN, ENDC, argv0);
}

} // namespace

int main(int argc, char** argv) {
  std::string modelPath;

  static const struct option longOpts[] = {
    {"help",  no_argument,       nullptr, 'h'},
    {"model", required_argument, nullptr, 'm'},
    {nullptr, 0, nullptr, 0}
  };

  int opt;
  while ((opt = getopt_long(argc, argv, "hm:", longOpts, nullptr)) != -1) {
    switch (opt) {
      case 'h': printUsage(argv[0]); return 0;
      case 'm': modelPath = optarg;  break;
      default:  printUsage(argv[0]); return 1;
    }
  }

  if (!glfwInit())
    return 1;

  try {
    scop::Scop app(modelPath);
    app.runScop();
  } catch (const std::exception& e) {
    std::cerr << RED << "Fatal: " << e.what() << ENDC << std::endl;
    glfwTerminate();
    return 1;
  }
  return 0;
}
