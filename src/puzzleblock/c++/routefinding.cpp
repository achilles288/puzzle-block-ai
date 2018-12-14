#include "routefinding.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <thread>


/*
 * Class:  Node
 */

Node::Node() {}

Node::~Node() {}

Node *Node::clone() {return new Node();}

// Default heuristic function of node. Overridable.
int Node::heuristic(Node *g) {return (this == g) ? 0 : 1;}

// Overridable function.
void Node::expandNode(std::vector<Node*>& vec) {}

// Overridable function.
bool Node::isDuplicate(Node *n) {return false;}


/*
 * Class:  Action
 */

// Constructs action object setting start node, end node and cost.
Action::Action(Node *st, Node *en, int c, char p) {
  start = st;
  end = en;
  cost = c;
  parameter = p;
}


/*
 * Class:  RouteFinding
 */

// Set up the goal node.
RouteFinding::RouteFinding(Node *g) {
  goal = g;
  frontier.reserve(100);
}


/*
 * Run a separate thread that finds answer to the problem. The main thread waits and
 * perform the answer actions.
 * 
 * After a time interval, current best answer is added to the stack and the main
 * thread will do actions. Mean while, another route finding loop is being done.
 */
void RouteFinding::startFinding(Node *st) {
  search_no = 0;
  timeout = 0;
  nearest = st;
  std::thread rf_th([this]() {
    st_clock = clock();
    try {
      while(true) {
        timeout += think_time;
        frontier.push_back(std::make_pair(nearest, 0));
        int res = findRoute();
        clearNodes();
        if(res == 0)
          break;
      }
      float duration = (float)(clock() - st_clock) / CLOCKS_PER_SEC * 1000;
      std::cout << "Computing time: " << duration << "ms" << std::endl;
    }
    catch(std::exception& ex) {
      std::cout << "Error from route finding thread!\n" << ex.what() << std::endl;
      std::exit(1);
    }
  });
  rf_th.detach();
}


using namespace std;

/*
 * A start node is branched into many nodes via available actions. It keeps
 * expanding until a goal node is found. The search algorithm here is A* algorithm.
 * 
 * The function does 3 steps. Finds the node nearest to answer. Check if it is
 * the goal node. If not, seeks for the next nodes branched from it (expand node).
 *
 * For perfect performance, the frontier list and explored list are sorted by a
 * criterion. To get the nearest node, simply pick the first element of frontier.
 * To check duplicity, search the node by binary search which is O(log(n)).
 * When inserting elements, binary search is used to get the position as well.
 */
int RouteFinding::findRoute() {
  if(nearest->heuristic(goal) == 0) {
    std::cout << "Problem is already solved!\n";
    answer_mutex.lock();
    answer.push(-1);
    answer_mutex.unlock();
    return 0;
  }

  while(true) {
    // Expand node of least heuristic and cost.
    nearest = frontier.front().first;
    nearest->expandNode(new_node);
    // Move the expanded frontier to explored. Place in order by heuristic.
    int h;
    h = frontier.front().first->heuristic(goal);
    pair<Node*, int> n = make_pair(frontier.front().first, h);
    node_int_iter pos_near = lower_bound(explored.begin(), explored.end(), n,
      [](Node_int const & p, Node_int const & q) -> bool
        { return p.second < q.second; }
    );
    explored.insert(pos_near, n);
    frontier.erase(frontier.begin());

    // Iterate through the new nodes. Check duplicity, insert to the frontier.
    for(action_iter it = nearest->action.begin(); it != nearest->action.end(); it++)
    {
      Action *a = *it;
      a->end->cost = nearest->cost + a->cost;
      pair<node_int_iter, node_int_iter> range;
      h = a->end->heuristic(goal);
      n = make_pair(a->end, h);

      bool isNewNode = true;

      /*
       * Get a range where the heuristic is equal.
       * These elements are probably duplicate.
       */
      range = equal_range(explored.begin(), explored.end(), n,
        [](Node_int const& p, Node_int const& q)->bool
          { return p.second < q.second; }
      );
      for(node_int_iter p=range.first; p!=range.second; p++) {
        // Check duplicate. If so, reject that node.
        if(a->end->isDuplicate(p->first)) {
          isNewNode = false;
          new_node.erase(remove(new_node.begin(), new_node.end(), a->end));
          delete a->end;
          break;
        }
      }

      if(isNewNode) {
        a->end->parent = a;
        new_node.erase(remove(new_node.begin(), new_node.end(), a->end));
        // Insert the node to frontier list in order by heuristic + cost value.
        n = make_pair(a->end, h + a->end->cost);
        node_int_iter pos = lower_bound(frontier.begin(), frontier.end(), n,
          [](Node_int const & p, Node_int const & q) -> bool
            { return p.second < q.second; }
        );
        if(frontier.size() == frontier.max_size()) {
          delete frontier.back().first;
          frontier.erase(frontier.end()-1);
        }
        frontier.insert(pos, n);
      }
    }

    h = nearest->heuristic(goal);

    // End loop if goal node is found or the think time is up.
    bool isTimeup = (clock() - st_clock) > (timeout * CLOCKS_PER_SEC / 1000);

    // If think time is up, a node with least heuristic value is chosen.
    if(isTimeup)
      nearest = explored.front().first;

    // Back-trace the steps to reach the node. Add these steps to answer list.
    if(h == 0 or isTimeup) {
      Action *action = nearest->parent;
      stack<char> stk;
      while(action != NULL) {
        stk.push(action->parameter);
        action = action->start->parent;
      }

      answer_mutex.lock();
      while(not stk.empty()) {
        answer.push(stk.top());
        stk.pop();
      }

      if(h == 0) {
        answer.push(-1);
        answer_mutex.unlock();
        return 0;
      }
      else {
        answer_mutex.unlock();
        nearest = nearest->clone();
        return 1; // Next loop of route finding.
      }
    }
    std::cout << "searchNo: " << search_no << " heuristic: " << h << std::endl;
    search_no++;
  }
}


// Get the next action index from the list.
int RouteFinding::getRoute() {
  while(true) {
    answer_mutex.lock();
    bool isAvailable = not answer.empty();
    answer_mutex.unlock();
    if(isAvailable) {
      int i = answer.front();
      answer.pop();
      return i;
    }
  }
}


void RouteFinding::setThinkTime(int ms) {
  think_time = ms;
}

// Clear the nodes for the next search loop. Refresh.
void RouteFinding::clearNodes() {
  for(node_iter it=new_node.begin(); it!=new_node.end(); it++)
    delete *it;
  new_node.clear();
  for(node_int_iter it=frontier.begin(); it!=frontier.end(); it++)
    delete it->first;
  frontier.clear();
  for(node_int_iter it=explored.begin(); it!=explored.end(); it++)
    delete it->first;
  explored.clear();
}

