/*
 * First step to AI. Route finding algorithm. C++ for performance and Java for UI.
 *
 * Library	: routefinding
 * Author	: Khant Kyaw Khaung
 * Compiler	: gcc version 7.3.0 (Ubuntu 7.3.0-16ubuntu3)
 * Date		: 17 Oct 2018
 */

/*
 * File		: routefinding.h
 *
 * Structures of nodes and actions. A* search algorithm is used.
 */

#ifndef __ROUTEFINDING_H__
#define __ROUTEFINDING_H__

#define NEW_NODE 0
#define FRONTIER 1
#define EXPLORED 2
#define GOAL 3

#include <time.h>
#include <vector>

class Node;
class Action;
class RouteFinding;

class Node {
  public:
    unsigned short cost = 0;
    Action* parent = NULL;
    std::vector<Action*> action;
    Node();
    ~Node();
};

class Action {
  public:
    Node *start;
    Node *end;
    unsigned short cost;
    unsigned short parameter = -1;
    Action(Node *st, Node *en, int c);
};

class RouteFinding {
  protected:
    std::vector<Node*> newNodes;
    std::vector<Node*> frontier;
    std::vector<Node*> explored;
    std::vector<int> answer;
    Node *start;
    Node *goal;

  private:
    unsigned short cost;
    clock_t computingTime;

  public:
    RouteFinding(Node *st, Node *g);
    ~RouteFinding();
    void startFinding();
    virtual int heuristic(Node *a, Node *b);
    virtual void expandNode(Node *n);
    virtual Node *checkExplored(Node *n);
    virtual Node *checkDuplicateFrontier(Node *n);
    int getRoute();
    void clear();
    int getCost();
    float getComputingTime_sec();
};

#endif
