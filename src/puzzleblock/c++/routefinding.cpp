/*
 * First step to AI. Route finding algorithm. C++ for performance and Java for UI.
 *
 * Library	: routefinding
 * Author	: Khant Kyaw Khaung
 * Compiler	: gcc version 7.3.0 (Ubuntu 7.3.0-16ubuntu3)
 * Date		: 17 Oct 2018
 */

/*
 * File		: routefinding.cpp
 *
 * Structures of nodes and actions. A* search algorithm is used.
 */

#include "routefinding.h"
#include <algorithm>
#include <cstdio>

using namespace std;

Node::Node() {}

Node::~Node() {
  for(vector<Action*>::iterator a = action.begin();
      a != action.end(); a++)
  {
    delete *a;
  }
}

Action::Action(Node *st, Node *en, int c) {
  start = st;
  end = en;
  cost = c;
}

RouteFinding::RouteFinding(Node *st, Node *g) {
  start = st;
  goal = g;
  frontier.push_back(st);
}

RouteFinding::~RouteFinding() {clear();}

int RouteFinding::heuristic(Node *a, Node *b) {return (a==b) ? 0:1;}
void RouteFinding::expandNode(Node *n) {}
Node *RouteFinding::checkExplored(Node *n) {return NULL;}
Node *RouteFinding::checkDuplicateFrontier(Node *n) {return NULL;}

int RouteFinding::getRoute() {
  if(answer.size() > 0) {
    int i = answer[answer.size()-1];
    answer.pop_back();
    return i;
  }
  return -1;
}

void RouteFinding::startFinding() {
  if(heuristic(start, goal) == 0)
    return;
  clock_t st_time = clock();
  int searchNo = 0;
  while(answer.size() == 0) {
    // Finds the path of lowest cost and error.
    Node *shortest = frontier[0];
    int i = shortest->cost + heuristic(shortest, goal);
    int ii;
    for(vector<Node*>::iterator it = frontier.begin(); it != frontier.end(); it++) {
      Node *f = *it;
      ii = f->cost + heuristic(f, goal);
      if(ii < i) {
        shortest = f;
        i = ii;
      }
    }
    
    // The path to the goal is confirmed the shortest.
    int h = heuristic(shortest, goal);
    if(h == 0) {
      Action *action = shortest->parent;
      while(action != NULL) {
        answer.push_back(action->parameter);
        action = action->start->parent;
      }
      break;
    }

    // Expand the node of lowest cost and error.
    expandNode(shortest);
    for(vector<Action*>::iterator it = shortest->action.begin();
        it != shortest->action.end(); it++)
    {
      Action *a = *it;
      Node *ex = checkExplored(a->end);
      Node *fr = checkDuplicateFrontier(a->end);
      if(ex != NULL) {
        if(shortest->cost + a->cost < a->end->cost) {
          a->end->cost = shortest->cost + a->cost;
          a->end->parent = a;
          newNodes.erase(remove(newNodes.begin(), newNodes.end(), a->end));
          frontier.push_back(a->end);
          explored.erase(remove(explored.begin(), explored.end(), ex));
          delete ex;
        }
        else {
          newNodes.erase(remove(newNodes.begin(), newNodes.end(), a->end));
          delete a->end;
        }
      }
      else if(fr != NULL) {
        if(shortest->cost + a->cost < a->end->cost) {
          a->end->cost = shortest->cost + a->cost;
          a->end->parent = a;
          newNodes.erase(remove(newNodes.begin(), newNodes.end(), a->end));
          frontier.push_back(a->end);
          frontier.erase(remove(frontier.begin(), frontier.end(), fr));
          delete fr;
        }
        else {
          newNodes.erase(remove(newNodes.begin(), newNodes.end(), a->end));
          delete a->end;
        }
      }
      else {
        a->end->cost = shortest->cost + a->cost;
        a->end->parent = a;
        newNodes.erase(remove(newNodes.begin(), newNodes.end(), a->end));
        frontier.push_back(a->end);
      }
    }
    //printf("SearchNo:%5d  Heuristic:%4d  Cost:%4d  NodeSize:%4d\n", searchNo, h, shortest->cost, (int)(newNodes.size()+frontier.size()+explored.size()));
    frontier.erase(remove(frontier.begin(), frontier.end(), shortest));
    explored.push_back(shortest);
    searchNo++;
  }
  computingTime = clock() - st_time;
  printf("Computing time: %.3fms\n", ((float)computingTime) / CLOCKS_PER_SEC * 1000);
}

void RouteFinding::clear() {
  for(vector<Node*>::iterator n = newNodes.begin(); n != newNodes.end(); n++)
    delete *n;
  for(vector<Node*>::iterator n = frontier.begin(); n != frontier.end(); n++)
    delete *n;
  for(vector<Node*>::iterator n = explored.begin(); n != explored.end(); n++)
    delete *n;
}

