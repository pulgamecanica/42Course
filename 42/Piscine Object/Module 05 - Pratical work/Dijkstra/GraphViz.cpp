//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "GraphViz.hpp"

#include <cstring>
#include <algorithm>

#include "Dijkstra.inc"

static void callbackHook(void *this_pointer) {
  static_cast<GraphViz*>(this_pointer)->Update();
}

/******************************/
/*      Connection Viz        */
/******************************/
ConnectionViz::ConnectionViz(Connection & conn, NodeViz * node)
  : conn_(conn), node_(node) {
  ;
}

NodeViz &ConnectionViz::GetNode() {
  return *node_;
}

Connection &ConnectionViz::GetConnection() {
  return conn_;
}

ConnectionViz& ConnectionViz::operator=(const ConnectionViz& other) {
  conn_ = other.conn_;
  node_ = other.node_;
  return *this;
}

/******************************/
/*         Node Viz           */
/******************************/

NodeViz::NodeViz() {
  ;
}

void NodeViz::SetNode(int node_num, std::vector<ConnectionViz>& connections,
                        std::vector<int> paths, int x, int y) {
  node_num_ = node_num;
  x_ = x;
  y_ = y;
  connections_ = connections;
  paths_ = paths;
  color_ = 0x424242;
  std::cout << "Setting Node Viz " << node_num_ << std::endl;
  // If debug
  printf("Vertex Distance from Source (%d)\n", node_num_);
  for (int i = 0; i < (int)paths_.size(); ++i)
    printf("%d \t\t %d\n", i, paths[i]);
}

void NodeViz::DrawNode(mlx_image_t *image, std::set<int>& drew_nodes) {
  for (auto& connection: connections_) {
    // Skip connections for nodes that have been processed
    // We don't want to repeat connections
    NodeViz& node = connection.GetNode();
    if (drew_nodes.contains(node.GetNodeNum()))
      continue;
    draw_line(image, x_, y_, node.x(), node.y(), 0xfff);
  }
  (void) drew_nodes;
  draw_circle(image, x_, y_, 10, color_);
}

int NodeViz::x() const {
  return x_;
}

int NodeViz::y() const {
  return y_;
}

int NodeViz::GetColor() const {
  return color_;
}

int NodeViz::GetNodeNum() const {
  return node_num_;
}

bool operator==(const NodeViz& lhs, int rhs) {
  return lhs.GetNodeNum() == rhs;
}


/******************************/
/*         Graph Viz          */
/******************************/

// Initialize static mutex
std::mutex GraphViz::mutex_{};

// Initialize static pointer instance
GraphViz *GraphViz::instance_{nullptr};

// Open in the window in a new thread, must call JoinThread to avoid orphan!
void GraphViz::Open() {
  if (graph_ == nullptr) {
    std::cerr << "Graph has not been defined" << std::endl;
    return ;
  }
  helper_ = new std::thread(&GraphViz::RunViz, this);
  std::cout << "Opened Vizualizer in a new window" << std::endl;
}

void GraphViz::JoinThread() {
  if (!helper_)
    return ;
  helper_->join();
  delete helper_;
  helper_ = nullptr;
  mlx_terminate(mlx_);
}

GraphViz *GraphViz::GetInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (instance_ == nullptr)
    instance_ = new GraphViz();
  return instance_;
}

void GraphViz::SetWidth(unsigned width) {
  width_ = width;
}

void GraphViz::SetHeight(unsigned height) {
  height_ = height;
}

void GraphViz::SetGraph(Graph *graph) {
  graph_ = graph;
}

void GraphViz::DrawBG() {
  const char bg_color[4] = {(char)66, (char)135, (char)245, (char)255};
  for (unsigned i = 0; i < image_->width * image_->height; ++i)
    std::memcpy(image_->pixels + (i * 4), bg_color, sizeof(char) * 4);
}

void GraphViz::DrawNodes() {
  std::set<int> drew_nodes;
  for (auto& node: nodes_) {
    node.DrawNode(image_, drew_nodes);
    drew_nodes.emplace(node.GetNodeNum());
  }
}

void GraphViz::Update() {
  if (mlx_is_key_down(mlx_, MLX_KEY_ESCAPE))
    mlx_close_window(mlx_);
  // Handle Heys
  // Update Key Events
  DrawBG();
  // React to move
  // Is mouse moving
  // Where the fuck is the mouse
  DrawNodes();
}

GraphViz::GraphViz(): graph_(nullptr), mlx_(nullptr), image_(nullptr),
  width_(512), height_(512), title_("Graph Viz"), resize_(true),
  helper_(nullptr) {
  ;
}

// Very important function
// In charge of setting up all the nodes and it's connections with other nodes
// Also in charge of setting up the paths of the node 
void GraphViz::SetUpNodes() {
  nodes_.resize(graph_->GetSize());
  int rows = 3;
  for (int i = 0; i < graph_->GetSize(); ++i) {
    // Node position in the image
    int x = (i / (float)(graph_->GetSize())) * image_->width * rows;
    int y = ((int)(x / image_->width) / (float)rows) * (image_->height - 20); // rows + 1 to avoid nodes being in the edge
    x %= image_->width - 20;
    x = (x < 20) ? 20 : x;
    y = (y < 20) ? 20 : y;
    // Connections
    std::vector<ConnectionViz> conns;
    std::list<Connection>& connections = (*graph_)[i];
    for (auto& conn: connections)
      conns.emplace_back(conn, &nodes_[conn.Dest()]);
    // Set node
    nodes_[i].SetNode(i, conns, graph_->ShortestPaths(i), x, y);
    if (nodes_[i].GetNodeNum() != i)
      ExitMLX42(); // Proof
  }
}

void GraphViz::SetUp() {
  if (!(mlx_ = mlx_init(width_, height_, title_.c_str(), resize_)))
    ExitMLX42();
  if (!(image_ = mlx_new_image(mlx_, width_, height_)))
    ExitMLX42();
  if (mlx_image_to_window(mlx_, image_, 0, 0) == -1)
    ExitMLX42();
  // Using a static callback to allow a method call to this->Update
  mlx_loop_hook(mlx_, callbackHook, this);
  SetUpNodes();
}

void GraphViz::RunViz() {
  // Must setup in the same thread calling mlx_loop
  SetUp();
  mlx_loop(mlx_);
  return ;
}

void GraphViz::ExitMLX42() {
  if (mlx_)
    mlx_close_window(mlx_);
  std::cerr << (mlx_strerror(mlx_errno)) << std::endl;
  exit(EXIT_FAILURE);
}

