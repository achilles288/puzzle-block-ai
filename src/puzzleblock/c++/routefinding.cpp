#include "routefinding.h"

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <thread>


/*
 * Class:  Node
 */

Node::Node() {}

// Default heuristic function of node. Overridable.
int Node::heuristic(Node *g) {
  return (this == g) ? 0 : 1;
}

// Overridable function.
void Node::expandNode(std::vector<Node*>& vec) {}

// Overridable function.
Node *Node::getDuplicateNode(
  std::vector<Node *>::iterator st,
  std::vector<Node *>::iterator en
)
{
  return NULL;
}


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
}


/*
 * Run a separate thread that finds answer to the problem. The main thread waits and
 * perform the answer actions.
 * 
 * After a time interval, current best answer is added to the stack and the main
 * thread will do actions. Mean while, another route finding loop is being done.
 */
void RouteFinding::startFinding(Node *st) {
  nearest = st;
  std::thread rf_th([this]() {
    st_clock = clock();
    try {
      while(true) {
        timeout += think_time;
        frontier.push_back(nearest);
        int res = findRoute();
        clearNodes();
        if(res == 0)
          return;
      }
    }
    catch(std::exception ex) {
      std::cout << "Error from route finding thread!\n" << ex.what() << std::endl;
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
 */
int RouteFinding::findRoute() {
  if(nearest->heuristic(goal) == 0) {
    std::cout << "Problem is already solved!\n";
    answer_mutex.lock();
    answer.push_back(-1);
    answer_mutex.unlock();
    return 0;
  }

  while(true) {
    // Finds the node nearest to the goal from frontiers.
    nearest = frontier[0];
    int hc = nearest->cost + nearest->heuristic(goal);
    for(vector<Node *>::iterator it=frontier.begin()+1; it!=frontier.end(); it++)
    {
      Node *f = *it;
      int i = f->cost + f->heuristic(goal);
      if(i < hc) {
        nearest = f;
        hc = i;
      }
    }

    // End loop if goal node is found or the think time is up.
    int h = nearest->heuristic(goal);
    bool isTimeup = (clock() - st_clock) > (timeout * CLOCKS_PER_SEC / 1000);
    if(isTimeup) {
      nearest = frontier[0];
      h = nearest->heuristic(goal);
      for(vector<Node *>::iterator it=frontier.begin()+1; it!=frontier.end(); it++)
      {
        Node *f = *it;
        int i = f->heuristic(goal);
        if(i < h) {
          nearest = f;
          h = i;
        }
      }
    }
    if(h == 0 or isTimeup) {
      Action *action = nearest->parent;
      vector<char> ans;
      while(action != NULL) {
        ans.insert(ans.begin(), action->parameter);
        action = action->start->parent;
      }
      answer_mutex.lock();
      answer.insert(answer.end(), ans.begin(), ans.end());
      if(h == 0) {
        answer.push_back(-1);
        answer_mutex.unlock();
        return 0;
      }
      else {
        answer_mutex.unlock();
        frontier.erase(remove(frontier.begin(), frontier.end(), nearest));
        nearest->parent = NULL;
        return 1; // Next loop of route finding.
      }
    }

    // Expand the selected node.
    nearest->expandNode(new_node);
    for(vector<Action*>::iterator it = nearest->action.begin();
        it != nearest->action.end(); it++)
    {
      Action *a = *it;
      Node *ex = a->end->getDuplicateNode(explored.begin(), explored.end());
      Node *fr = a->end->getDuplicateNode(frontier.begin(), frontier.end());
      if(ex != NULL or fr != NULL) {
        new_node.erase(remove(new_node.begin(), new_node.end(), a->end));
        delete a->end;
      }
      else {
        a->end->cost = nearest->cost + a->cost;
        a->end->parent = a;
        new_node.erase(remove(new_node.begin(), new_node.end(), a->end));
        frontier.push_back(a->end);
      }
    }
    frontier.erase(remove(frontier.begin(), frontier.end(), nearest));
    explored.push_back(nearest);
    std::cout << "searchNo: " << search_no << " heuristic: " << h << std::endl;
    search_no++;
  }
}


// Get the next action index from the list.
int RouteFinding::getRoute() {
  while(true) {
    answer_mutex.lock();
    int len = answer.size();
    answer_mutex.unlock();
    if(len > step) {
      int i = answer[step];
      step++;
      return i;
    }
  }
}


void RouteFinding::setThinkTime(int ms) {
  think_time = ms;
}

// Clear the nodes for the next search loop. Refresh.
void RouteFinding::clearNodes() {
  for(vector<Node*>::iterator it=new_node.begin(); it!=new_node.end(); it++)
    delete *it;
  new_node.clear();
  for(vector<Node*>::iterator it=frontier.begin(); it!=frontier.end(); it++)
    delete *it;
  frontier.clear();
  for(vector<Node*>::iterator it=explored.begin(); it!=explored.end(); it++)
    delete *it;
  explored.clear();
}

void RouteFinding::clearAnswer() {
  answer.clear();
  step = 0;
  search_no = 0;
  timeout = 0;
}
