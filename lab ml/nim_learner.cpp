/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "nim_learner.h"
#include <ctime>
#include <iostream>


/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) {
    /* Your code goes here! */
    // Create all the vertices for each player and token count
    for (int i = startingTokens; i >= 0; i--) {
      for (int p = 1; p <= 2; p++) {
        g_.insertVertex("p" + to_string(p) + "-" + to_string(i));
      }
    }
    for (int i = startingTokens; i >= 1; i--) {
      for (int p = 1; p <= 2; p++) {
        string src = "p" + to_string(p) + "-" + to_string(i);
        int player = (p == 1) ? 2 : 1;
        for (int take = 1; take <= 2; take++) {
          if (i - take >= 0) {
            g_.insertEdge(src, "p" + to_string(player) + "-" + to_string(i - take));
            g_.setEdgeWeight(src, "p" + to_string(player) + "-" + to_string(i - take), 0);
          }
        }
      }
    }
    startingVertex_ = "p1-" + to_string(startingTokens);
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
  vector<Edge> path;
 /* Your code goes here! */
    Vertex curr = startingVertex_;
    while (true) {
      if (stoi(curr.substr(curr.find("-") + 1)) == 0) {
        break;
      }
      vector<Vertex> adj = g_.getAdjacent(curr);
      int ind = rand() % adj.size();
      path.push_back(g_.getEdge(curr, adj[ind]));
      curr = adj[ind];
    }
    return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
  /* Your code goes here! */
  size_t pos = path.back().dest.find("-");
  int loser = stoi(path.back().dest.substr(1, pos - 1));
  int winner = (loser == 1) ? 2 : 1;
  for (const Edge & e : path) {
    if (stoi(e.source.substr(1, e.source.find("-") - 1)) == winner) {
      g_.setEdgeWeight(e.source, e.dest, g_.getEdgeWeight(e.source, e.dest) + 1);
    } else {
      g_.setEdgeWeight(e.source, e.dest, g_.getEdgeWeight(e.source, e.dest) - 1);
    }
  }
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);
      if (weight > threshold){ 
        g_.setEdgeLabel(v, w, "WIN"); 
        }
      else if (weight < -1 * threshold) {
        g_.setEdgeLabel(v, w, "LOSE");
        }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
