#ifndef __ROUTEFINDING_H__
#define __ROUTEFINDING_H__

#include <mutex>
#include <ctime>
#include <vector>


class Node;
class Action;
class RouteFinding;

// The structure containing informations of the system which need route finding.
class Node {
  friend RouteFinding;

  public:
    Node();

  private:
    unsigned short cost = 0; // Total cost traveled to get to this node.
    Action *parent = NULL; // The action object which leads to this node.

  protected:
    std::vector<Action*> action; // A list of available actions.
    virtual int heuristic(Node *g); // Gets error between this node and goal node.

    // Looks for some available actions. Add them to 'vec'.
    virtual void expandNode(std::vector<Node*>& vec);

    // To remove not to go back to explored node.
    virtual Node *getDuplicateNode(
      std::vector<Node*>::iterator st,
      std::vector<Node*>::iterator en
    );
};


// The available step by a node. A linkage between the nodes.
class Action {
  friend RouteFinding;

  public:
    Action(Node *st, Node *en, int c, char p);

  private:
    Node *start; // Start node.
    Node *end; // End node.
    unsigned short cost; // Cost to perform this action.
    char parameter = -1; // Action index, the decision for the app.
};

// The route finding system. Algorithms and structures.
class RouteFinding {
  public:
    RouteFinding(Node *g); // Constructs a system with a goal node 'g'.
    void startFinding(Node *start); // Starts route finding from a start node 'st'.
    int getRoute(); //Get action index, the decision for the app.
    void setThinkTime(int ms);
    void clearAnswer();

  private:
    Node *nearest;
    Node *goal;
    std::vector<Node*> new_node; // New nodes expanded by a node.
    std::vector<Node*> frontier; // A list of nodes leading to answer. 1 must expand.
    std::vector<Node*> explored; // After node expansion, move the parent to it.
    std::vector<char> answer; // A list of action indecies, steps to the goal node.
    std::mutex answer_mutex;
    clock_t st_clock;
    int search_no = 0;
    int step = 0;
    int think_time = 5000;
    int timeout = 0;
    int findRoute();
    void clearNodes(); // Clear frontier, explored and new_node.
};

#endif
