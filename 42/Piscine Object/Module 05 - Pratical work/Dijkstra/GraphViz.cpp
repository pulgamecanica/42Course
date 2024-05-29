//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "GraphViz.hpp"

#include <cstring>
#include <sstream>
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
  color_ = 0x043354ff;
  // std::cout << "Setting Node Viz " << node_num_ << std::endl;
  // If debug
  // printf("Vertex Distance from Source (%d)\n", node_num_);
  // for (int i = 0; i < (int)paths_.size(); ++i)
  //   printf("%d \t\t %d\n", i, paths[i]);
}

void NodeViz::DrawNode(mlx_t *mlx, mlx_image_t *image, std::set<int>& drew_nodes) {
  for (auto& connection: connections_) {
    // Skip connections for nodes that have been processed
    // We don't want to repeat connections
    NodeViz& node = connection.GetNode();
    if (drew_nodes.contains(node.GetNodeNum()))
      continue;
    draw_line(image, x_, y_, node.x(), node.y(), 0x323040ff);
    std::pair<int, int> mid = ft_midpoint(x_, y_, node.x(), node.y());
    ft_mlx_put_string(mlx, image, std::to_string(connection.GetConnection().Weight()).c_str(), mid.first, mid.second, 0xb30000ff);
  }
  draw_circle(image, x_, y_, 5, color_);
}

void NodeViz::SetX(int x) {
  if (x > 0)
    x_ = x;
}

void NodeViz::SetY(int y) {
  if (y > 0)
    y_ = y;
}

void NodeViz::SetColor(int color) {
  color_ = color;
}

std::vector<ConnectionViz>& NodeViz::GetConnections() {
  return connections_;
}

std::vector<int> NodeViz::GetPaths() {
  return paths_;
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

GraphViz::GraphViz()
  : graph_(nullptr), selected_node_(nullptr),
    mlx_(nullptr), image_(nullptr),
    width_(512), height_(512), side_bar_width_(192), title_("Graph Viz"), resize_(true), padding_(5),
    background_color_(0xc0d9ebff), node_number_color_(0x000000ff),
    edge_color_(0x323040ff), node_color_(0x043354ff), node_selected_color_(0x3323e8ff),
    node_hover_color_(0x9590d4ff), node_hover_border_color_(0x26262bff), weight_color_(0xb30000ff),
    helper_(nullptr), dragging_(false) {
  ;
}

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
  mlx_delete_image(mlx_, image_);
  mlx_delete_image(mlx_, side_bar_image_);
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

void GraphViz::DrawBG(mlx_image_t *img, int color) {
  const char bg_color[4] = {(char)((color >> 16) & 0XFF), (char)((color >> 8) & 0XFF), (char)(color & 0xFF), (char)255};
  for (unsigned i = 0; i < img->width * img->height; ++i)
    std::memcpy(img->pixels + (i * 4), bg_color, sizeof(char) * 4);
}

void GraphViz::DrawNodes() {
  std::set<int> drew_nodes;
  for (auto& node: nodes_) {
    node.DrawNode(mlx_, image_, drew_nodes);
    ft_mlx_put_string(mlx_, image_, std::to_string(node.GetNodeNum()).c_str(), node.x(), node.y() + padding_, node_number_color_);
    drew_nodes.emplace(node.GetNodeNum());
  }
}

void GraphViz::SelectNode() {
  if (dragging_ && hovered_node_ && select_)
    selected_node_ = hovered_node_;
}

void GraphViz::UpdateKeys() {
  if (mlx_is_key_down(mlx_, MLX_KEY_ESCAPE))
    mlx_close_window(mlx_);
}

void GraphViz::UpdateMouse() {
  bool old_dragging_state = dragging_;
  dragging_ = mlx_is_mouse_down(mlx_, MLX_MOUSE_BUTTON_LEFT);
  select_ = (old_dragging_state == false && dragging_ == true); // Is it the first click? (trigger select)
  mlx_get_mouse_pos(mlx_, &x_cur_, &y_cur_);
}


void GraphViz::UpdateSelectedNode() {
  if (!selected_node_)
    return ;
  if (dragging_) {
    selected_node_->SetColor(node_selected_color_);
    if ((unsigned)x_cur_ < width_)
      selected_node_->SetX(x_cur_);
    if ((unsigned)y_cur_ < height_)
      selected_node_->SetY(y_cur_);
  } else {
    selected_node_->SetColor(node_color_);
    selected_node_ = nullptr;
  }
}

void GraphViz::UpdateHoveredNode() {
  if (hovered_node_) {
    hovered_node_->SetColor(node_hover_color_);
    draw_circle(image_, hovered_node_->x(), hovered_node_->y(), padding_ * 1.5, node_hover_border_color_);
  }
}

void GraphViz::ComputeHover() {
  NodeViz *tmp_node = nullptr;
  for (auto& node: nodes_)
    if (is_inside_rect(node.x(), node.y(), padding_ * 2.5, padding_ * 2.5, x_cur_, y_cur_))
      tmp_node = &node;
  if (!tmp_node && hovered_node_) {
    hovered_node_->SetColor(node_color_);
    hovered_node_ = nullptr;
  }
  hovered_node_ = tmp_node;
}

void GraphViz::DrawSideBar() {
  if (hovered_node_ == nullptr) {
    put_img_to_img(side_bar_image_, side_bar_stand_by_bg_, 0, 0);
    return ;
  }
  put_img_to_img(side_bar_image_, side_bar_node_bg_, 0, 0);
  ft_mlx_put_string(mlx_, side_bar_image_, std::to_string(hovered_node_->GetNodeNum()).c_str(), 90, 36, 0xb30000ff);
  int i = 0;
  for (auto& conn: hovered_node_->GetConnections()) {
    std::stringstream ss;
    ss << "(" << conn.GetNode().GetNodeNum() << ")--->" << conn.GetConnection().Weight() << "km";
    ft_mlx_put_string(mlx_, side_bar_image_, ss.str().c_str(), 10, 28 * (i + 5), 0xb30000ff);
    i++;
  }
  i = 0;
  for (auto& path: hovered_node_->GetPaths()) {
    std::stringstream ss;
    ss << "(" << i << ")--->" << path << "km";
    ft_mlx_put_string(mlx_, side_bar_image_, ss.str().c_str(), 10, 28 * (i + 10), 0xb30000ff);
    i++;
  }
}

void GraphViz::Update() {
  // Draw Graph
  select_ = false;
  DrawBG(image_, background_color_);
  UpdateKeys();
  UpdateMouse();
  ComputeHover();
  SelectNode();
  UpdateHoveredNode();
  UpdateSelectedNode();
  DrawNodes();
  // Draw sidebar
  DrawBG(side_bar_image_, 0x00f);
  DrawSideBar();

  
  if (DEBUG) {
    std::cout << "Cursor [" << x_cur_ << "," << y_cur_ << "]" << std::endl;
    std::cout << "\tDragging:" << std::boolalpha << dragging_ << std::endl;
    std::cout << "\tHovering:" << std::boolalpha << (hovered_node_ != nullptr) << std::endl;
    std::cout << "\tSelected:" << std::boolalpha << (selected_node_ != nullptr) << std::endl;
  }
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
    int y = ((int)(x / image_->width) / (float)rows) * (image_->height - padding_); // rows + 1 to avoid nodes being in the edge
    x %= image_->width - padding_;
    x = (x < padding_) ? padding_ : x;
    y = (y < padding_) ? padding_ : y;
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
  if (!(mlx_ = mlx_init(width_ + side_bar_width_, height_, title_.c_str(), resize_)))
    ExitMLX42();
  if (!(image_ = mlx_new_image(mlx_, width_, height_)))
    ExitMLX42();
  if (mlx_image_to_window(mlx_, image_, 0, 0) == -1)
    ExitMLX42();
  if (!(side_bar_image_ = mlx_new_image(mlx_, side_bar_width_, height_)))
    ExitMLX42();
  if (mlx_image_to_window(mlx_, side_bar_image_, width_, 0) == -1)
    ExitMLX42();

  mlx_texture_t *texture1 = mlx_load_png("images/sidebar_node.png");
  mlx_texture_t *texture2 = mlx_load_png("images/side_bar_stand_by.png");
  if (!(side_bar_node_bg_ = mlx_texture_to_image(mlx_, texture1)))
    ExitMLX42();
  if (!(side_bar_stand_by_bg_ = mlx_texture_to_image(mlx_, texture2)))
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

