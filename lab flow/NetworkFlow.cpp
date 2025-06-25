#include <vector>
#include <algorithm>
#include <set>

#include "cs225_graph/graph.h"
#include "cs225_graph/edge.h"

#include "NetworkFlow.h"

int min(int a, int b) {
  if (a<b){
    return a;
  }
  else {
    return b;
  }
}

NetworkFlow::NetworkFlow(Graph & startingGraph, Vertex source, Vertex sink) :
  g_(startingGraph), residual_(Graph(true,true)), flow_(Graph(true,true)), source_(source), sink_(sink) {

  // YOUR CODE HERE
  for (auto v : g_.getVertices()) {
    residual_.insertVertex(v);
    flow_.insertVertex(v);
  }
  
  for (auto e : g_.getEdges()) {
    residual_.insertEdge(e.source, e.dest);
    residual_.setEdgeWeight(e.source, e.dest, e.getWeight());

    flow_.insertEdge(e.source, e.dest);
    flow_.setEdgeWeight(e.source, e.dest, 0);

    residual_.insertEdge(e.dest, e.source);
    residual_.setEdgeWeight(e.dest, e.source, 0);
  }
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the helper function.
   *
   * @param source  The starting (current) vertex
   * @param sink    The destination vertex
   * @param path    The vertices in the path
   * @param visited A set of vertices we have visited
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, 
  std::vector<Vertex> &path, std::set<Vertex> &visited) {
  if (visited.count(source) != 0)
    return false;
  visited.insert(source);

  if (source == sink) {
    return true;
  }

  std::vector<Vertex> adjs = residual_.getAdjacent(source);
  for (auto it = adjs.begin(); it != adjs.end(); it++) {
    if (visited.count(*it) == 0 && residual_.getEdgeWeight(source, *it) > 0) {
      path.push_back(*it);
      if (findAugmentingPath(*it, sink, path, visited))
        return true;
      else {
        path.pop_back();
      }
    }
  }

  return false;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the main function.  It initializes a set to keep track of visited vertices.
   *
   * @param source The starting (current) vertex
   * @param sink   The destination vertex
   * @param path   The vertices in the path
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, std::vector<Vertex> &path) {
   std::set<Vertex> visited;
   path.clear();
   path.push_back(source);
   return findAugmentingPath(source, sink, path, visited);
}

  /**
   * pathCapacity - Determine the capacity of a path in the residual graph.
   *
   * @param path   The vertices in the path
   */

int NetworkFlow::pathCapacity(const std::vector<Vertex> & path) const {
  // YOUR CODE HERE
  if (path.size() < 2){
    return 0;
  } 

  int cap = residual_.getEdgeWeight(path[0], path[1]);
  for (size_t i = 1; i < path.size() - 1; i++) {
    int Ecap = residual_.getEdgeWeight(path[i], path[i + 1]);
    cap = min(cap, Ecap);
  }

  return cap;
}

  /**
   * calculuateFlow - Determine the capacity of a path in the residual graph.
   * Sets the member function `maxFlow_` to be the flow, and updates the
   * residual graph and flow graph according to the algorithm.
   *
   * @return The network flow graph.
   */

const Graph & NetworkFlow::calculateFlow() {
  // YOUR CODE HERE
  std::vector<Vertex> path;
  maxFlow_ = 0;

  while (findAugmentingPath(source_, sink_, path)) {
    int cap = pathCapacity(path);

    for (size_t i = 0; i < path.size() - 1; i++) {
      Vertex x = path[i];
      Vertex y = path[i + 1];

      if (flow_.edgeExists(x, y)) {
        flow_.setEdgeWeight(x, y, flow_.getEdgeWeight(x, y) + cap);
      } else if (flow_.edgeExists(y, x)) {
        flow_.setEdgeWeight(y, x, flow_.getEdgeWeight(y, x) - cap);
      }

      residual_.setEdgeWeight(x, y, residual_.getEdgeWeight(x, y) - cap);
      residual_.setEdgeWeight(y, x, residual_.getEdgeWeight(y, x) + cap);
    }

    maxFlow_ += cap;
  }
  return flow_;
}

int NetworkFlow::getMaxFlow() const {
  return maxFlow_;
}

const Graph & NetworkFlow::getGraph() const {
  return g_;
}

const Graph & NetworkFlow::getFlowGraph() const {
  return flow_;
}

const Graph & NetworkFlow::getResidualGraph() const {
  return residual_;
}

