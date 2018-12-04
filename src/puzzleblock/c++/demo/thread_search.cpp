#include <cstdlib>
#include <iostream>
#include <thread>
#include <time.h>
#include <vector>
#include "threadpool.h"

using namespace std;

class Node {
  public:
    long x;
    Node();
    Node(long ld);
    long func(long n);
};

Node::Node() {}
Node::Node(long ld) {x = ld;}
long Node::func(long n) {
  if(n == 0)
    return 1;
  return n*func(n-1);
}

vector<Node> vec;

void fillRandom() {
  for(int i=0; i<1000; i++) {
    vec.push_back(Node(rand()%10000));
  }
}

void shortest_thread(vector<Node>::iterator st, vector<Node>::iterator en, Node *p) {
  Node shortest = *st;
  vector<Node>::iterator it = st+1;
  while(it != en) {
    Node n = *it;
    n.func(100);
    if(n.x < shortest.x)
      shortest = n;
    it++;
  }
  *p = shortest;
}

int main() {
  fillRandom();
  clock_t st = clock();
  for(int i=0; i<10000; i++) {
  Node n, n1, n2, n3, n4;
  vector<Node>::iterator pivot1 = vec.begin() + vec.size()/4;
  vector<Node>::iterator pivot2 = pivot1 + vec.size()/4;
  vector<Node>::iterator pivot3 = pivot2 + vec.size()/4;
  thread th1(shortest_thread, vec.begin(), pivot1, &n1);
  thread th2(shortest_thread, pivot1, pivot2, &n2);
  thread th3(shortest_thread, pivot2, pivot3, &n3);
  thread th4(shortest_thread, pivot3, vec.end(), &n4);
  th1.join();
  th2.join();
  th3.join();
  th4.join();
  //ld = min(min(ld1, ld2), min(ld3, ld4));
  }
  clock_t en = clock();/*
  clock_t st = clock();
  for(int i=0; i<10000; i++) {
  Node n;
  shortest_thread(vec.begin(), vec.end(), &n);
  }
  clock_t en = clock();*/
  cout << "runtime: " << float(en-st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
}
