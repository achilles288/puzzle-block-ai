#ifndef __ROUTEFINDING_H__
#define __ROUTEFINDING_H__

#include <mutex>
#include <ctime>
#include <queue>
#include <utility>
#include <vector>


class Node;
class Action;
class RouteFinding;

// The structure containing informations of the system which need route finding.
class Node {
  friend RouteFinding;

  public:
    Node();
    virtual ~Node();

  private:
    int cost = 0; // Total cost traveled to get to this node.
    Action *parent = NULL; // The action object which leads to this node.

  protected:
    std::vector<Action*> action; // A list of available actions.

    virtual Node *clone();

    virtual int heuristic(Node *g); // Gets error between this node and goal node.

    // Looks for some available actions. Add them to 'vec'.
    virtual void expandNode(std::vector<Node*>& vec);

    // To remove not to go back to explored node.
    virtual bool isDuplicate(Node *n);
};


// The available step by a node. A linkage between the nodes.
class Action {
  friend RouteFinding;

  public:
    Action(Node *st, Node *en, int c, char p);

  private:
    Node *start; // Start node.
    Node *end; // End node.
    int cost; // Cost to perform this action.
    char parameter = -1; // Action index, the decision for the app.
};

// The route finding system. Algorithms and structures.
class RouteFinding {
  public:
    RouteFinding(Node *g); // Constructs a system with a goal node 'g'.
    void startFinding(Node *start); // Starts route finding from a start node 'st'.
    int getRoute(); //Get action index, the decision for the app.
    void setThinkTime(int ms);

  private:
    Node *nearest;
    Node *goal;

    typedef std::pair<Node*, int> Node_int;
    std::vector<Node*> new_node; // New nodes expanded by a node.
    std::vector<Node_int> frontier; // node-cost pair. The front one is nearest.
    std::vector<Node_int> explored; // node-heuristic pair. Sorted according to h.

    // A list of action indecies, steps to the goal node.
    std::queue<char> answer;
    std::mutex answer_mutex; // To avoid data races in the answer queue.

    clock_t st_clock;
    int search_no = 0;
    int think_time = 5000;
    int timeout = 0;
    int findRoute();
    void clearNodes(); // Clear frontier, explored and new_node.

    typedef std::vector<Node*>::iterator node_iter;
    typedef std::vector<Node_int>::iterator node_int_iter;
    typedef std::vector<Action*>::iterator action_iter;
};

#endif
