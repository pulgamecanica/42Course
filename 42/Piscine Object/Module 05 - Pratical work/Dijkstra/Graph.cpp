//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef DEBUG
# define DEBUG 0
#endif

#include "Graph.hpp"

#include <queue>

Connection::Connection(int dest_node, int weight): dest_node_(dest_node), weight_(weight) {
    if (DEBUG)
        std::cout << "Initialize Connection to: " << dest_node << " weight: " << weight << std::endl;
}

int Connection::Weight() const {
    return weight_;
}

int Connection::Dest() const {
    return dest_node_;
}

bool operator>(const Connection& lhs, const Connection& rhs) {
    if (lhs.Dest() != rhs.Dest())
        return lhs.Dest() > rhs.Dest();
    return lhs.Weight() > rhs.Weight();
}

Graph::Graph(int size): size_(size) {
    if (DEBUG)
        std::cout << "Initialize Graph of size " << size_ << std::endl;
    vertices_ = new std::list<Connection> [size_];
}

Graph::~Graph() {
    if (DEBUG)
        std::cout << "Graph of size " << size_ << " destroyed" << std::endl;
    vertices_->clear();
    delete[] vertices_;
}

int Graph::GetSize() const {
    return size_;
}

void Graph::AddEdge(int first, int second, int weight) {
    if (first < 0 || second < 0)
        throw std::runtime_error("Cannot set a node with a negative value");
    if (first >= size_ || second >= size_)
        throw std::runtime_error("Cannot set a node above the limit");
    vertices_[first].push_back(Connection(second, weight));
    vertices_[second].push_back(Connection(first, weight));
}

# define INF 0x3f3f3f3f

std::vector<int> Graph::ShortestPaths(int src)
{
    // Create a priority queue to store vertices that
    // are being preprocessed.
    std::priority_queue< Connection, std::vector <Connection> , std::greater<Connection> > pq;

    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    std::vector<int> dist(size_, INF);

    // Insert source itself in priority queue and initialize
    // its distance as 0.
    pq.push(Connection(src, 0));
    dist[src] = 0;
    
    std::vector<bool> f(size_, false); // Visited verices

    /* Looping till priority queue becomes empty (or all
    distances are not finalized) */
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().Dest();
        pq.pop();
        f[u] = true;

        // 'i' is used to get all adjacent vertices of a vertex
        std::list< Connection >::iterator i;
        for (i = vertices_[u].begin(); i != vertices_[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = (*i).Dest();
            int weight = (*i).Weight();

            // If there is shorted path to v through u.
            if (f[v] == false && dist[v] > dist[u] + weight)
            {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                pq.push(Connection(v, dist[v]));
            }
        }
    }
    return dist;
}

std::list<Connection>& Graph::operator[](int node) {
    return vertices_[node];
}