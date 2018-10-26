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
 */

#include "puzzleblock_PuzzleBlock_AI.h"
#include "routefinding.h"
#include <algorithm>
#include <vector>
#include <cstdlib>

class RouteFinding_PuzzleBlock;

class BNode: public Node {
  friend class RouteFinding_PuzzleBlock;

  public:
    unsigned short *x;
    unsigned short *y;
    unsigned short arraySize;
    BNode() {};

  private:
    int getBlockIndexByPosition(int x2, int y2) {
      for(int i=0; i<arraySize; i++) {
        if(x[i] == x2 && y[i] == y2)
          return i;
      }
      return -1;
    }

    void copyPositionArray(BNode *n) {
      n->arraySize = arraySize;
      n->x = new unsigned short[arraySize];
      n->y = new unsigned short[arraySize];
      for(int i=0; i<arraySize; i++) {
        n->x[i] = x[i];
        n->y[i] = y[i];
      }
    }
};

class RouteFinding_PuzzleBlock: public RouteFinding {
  public:
    int col;
    int row;
    int blankIndex;

    int heuristic(Node *a, Node *b) {
      BNode *A, *B;
      A = (BNode*)a;
      B = (BNode*)b;
      int h1 = 0;
      int h2 = 0;
      for(int i=0; i < A->arraySize; i++) {
        if(A->x[i] != B->x[i] || A->y[i] != B->y[i])
          h1 += 2;
        h2 = h2 + abs(A->x[i] - B->x[i]) + abs(A->y[i] - B->y[i]);
      };
      return std::max(h1, h2);
    }
    
    RouteFinding_PuzzleBlock(Node *st, Node *g): RouteFinding(st, g) {}

    void expandNode(Node *n) {
      BNode *N = (BNode*)n;
      int bx = N->x[blankIndex];
      int by = N->y[blankIndex];

      int itop = N->getBlockIndexByPosition(bx, by-1);
      if(itop != -1) {
        BNode *e = new BNode();
        N->copyPositionArray(e);
        e->y[itop] = by;
        e->y[blankIndex] = by-1;
        Action *a = new Action(n, e, 1);
        a->parameter = itop;
        n->action.push_back(a);
        newNodes.push_back(e);
      }

      int iright = N->getBlockIndexByPosition(bx+1, by);
      if(iright != -1) {
        BNode *e = new BNode();
        N->copyPositionArray(e);
        e->x[iright] = bx;
        e->x[blankIndex] = bx+1;
        Action *a = new Action(n, e, 1);
        a->parameter = iright;
        n->action.push_back(a);
        newNodes.push_back(e);
      }

      int ibottom = N->getBlockIndexByPosition(bx, by+1);
      if(ibottom != -1) {
        BNode *e = new BNode();
        N->copyPositionArray(e);
        e->y[ibottom] = by;
        e->y[blankIndex] = by+1;
        Action *a = new Action(n, e, 1);
        a->parameter = ibottom;
        n->action.push_back(a);
        newNodes.push_back(e);
      }

      int ileft = N->getBlockIndexByPosition(bx-1, by);
      if(ileft != -1) {
        BNode *e = new BNode();
        N->copyPositionArray(e);
        e->x[ileft] = bx;
        e->x[blankIndex] = bx-1;
        Action *a = new Action(n, e, 1);
        a->parameter = ileft;
        n->action.push_back(a);
        newNodes.push_back(e);
      }
    }

    Node *checkExplored(Node *n) {
      BNode *a = (BNode*) n;
      for(std::vector<Node*>::iterator it = explored.begin();
          it != explored.end(); it++)
      {
        BNode *b = (BNode*)(*it);
        bool identical = true;
        for(int i=0; i < a->arraySize; i++) {
          if(a->x[i] != b->x[i] || a->y[i] != b->y[i]) {
            identical = false;
            break;
          }
        }
        if(identical)
          return *it;
      }
      return NULL;
    }

    Node *checkDuplicateFrontier(Node *n) {
      BNode *a = (BNode*) n;
      for(std::vector<Node*>::iterator it = frontier.begin();
          it != frontier.end(); it++)
      {
        BNode *b = (BNode*)(*it);
        bool identical = true;
        for(int i=0; i < a->arraySize; i++) {
          if(a->x[i] != b->x[i] || a->y[i] != b->y[i]) {
            identical = false;
            break;
          }
        }
        if(identical)
          return *it;
      }
      return NULL;
    }

    void randomPath(int steps) {
      BNode *n = (BNode*)start;
      for(int i=0; i<steps; i++) {
        expandNode(n);
        int l = n->action.size();
        Action *a = n->action[rand() % l];
        answer.push_back(a->parameter);
        n = (BNode*)a->end;
      }
    }
};

static RouteFinding_PuzzleBlock *problem;

namespace jni {
  static BNode *start;
}

using namespace jni;

void fetchJavaObjects(JNIEnv *env, jobject obj) {
  puzzleAIClass = env->GetObjectClass(obj);
   idBoard = env->GetFieldID(puzzleAIClass, "board", "Lpuzzleblock/Board;");
   board = env->GetObjectField(obj, idBoard);
  boardClass = env->GetObjectClass(board);
   idCol = env->GetFieldID(boardClass, "col", "I");
   idRow = env->GetFieldID(boardClass, "row", "I");
   idBlankIndex = env->GetFieldID(boardClass, "blankIndex", "I");
   idBlockArray = env->GetFieldID(boardClass, "blocks", "[Lpuzzleblock/Block;");
   blockArrayObj = env->GetObjectField(board, idBlockArray);
   jobjectArray *blockArray = reinterpret_cast<jobjectArray*>(&blockArrayObj);
   blockArrayLen = env->GetArrayLength(*blockArray);
   blocks = new jobject[blockArrayLen];
   for(int i=0; i<blockArrayLen; i++)
     blocks[i] = env->GetObjectArrayElement(*blockArray, i);
  jclass blockClass = env->GetObjectClass(blocks[0]);
   jfieldID idX = env->GetFieldID(blockClass, "x", "I");
   jfieldID idY = env->GetFieldID(blockClass, "y", "I");

  col = env->GetIntField(board, idCol);
  row = env->GetIntField(board, idRow);
  start = new BNode();
  start->arraySize = col*row;
  start->x = new unsigned short[start->arraySize];
  start->y = new unsigned short[start->arraySize];
  for(int i=0; i<start->arraySize; i++) {
    start->x[i] = env->GetIntField(blocks[i], idX);
    start->y[i] = env->GetIntField(blocks[i], idY);
  }
}

JNIEXPORT void JNICALL Java_puzzleblock_PuzzleBlock_1AI_startRouteFinding
  (JNIEnv *env, jobject obj)
{
  fetchJavaObjects(env, obj);

  BNode *goal = new BNode();
  goal->arraySize = col*row;
  goal->x = new unsigned short[goal->arraySize];
  goal->y = new unsigned short[goal->arraySize];
  for(int i=0; i<goal->arraySize; i++) {
    goal->x[i] = i % col;
    goal->y[i] = i / col;
  }
  problem = new RouteFinding_PuzzleBlock(start, goal);
  problem->col = col;
  problem->row = row;
  problem->blankIndex = env->GetIntField(board, idBlankIndex);
  problem->startFinding();
}

JNIEXPORT void JNICALL Java_puzzleblock_PuzzleBlock_1AI_clearRouteFinding
  (JNIEnv *, jobject)
{
  delete problem;
}

JNIEXPORT jint JNICALL Java_puzzleblock_PuzzleBlock_1AI_getBlockIndex_1nextMove
  (JNIEnv *, jobject)
{
  return problem->getRoute();
}

JNIEXPORT void JNICALL Java_puzzleblock_PuzzleBlock_1AI_randomBlocks
  (JNIEnv *env, jobject obj)
{
  fetchJavaObjects(env, obj);
  problem = new RouteFinding_PuzzleBlock(start, NULL);
  problem->col = col;
  problem->row = row;
  problem->blankIndex = env->GetIntField(board, idBlankIndex);
  problem->randomPath(100);
}
