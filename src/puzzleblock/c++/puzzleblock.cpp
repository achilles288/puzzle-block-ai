#include "puzzleblock.h"

#include <iostream>

using namespace std;


BoardNode::BoardNode() {
  x = new char[arraySize];
  y = new char[arraySize];
}

BoardNode::~BoardNode() {
  delete x;
  delete y;
}

// Convert position to index.
int BoardNode::getBlockIndexByPosition(int x2, int y2) {
  for(int i=0; i<arraySize; i++) {
    if(x[i] == x2 and y[i] == y2) {
      return i;
    }
  }
  return -1;
}

Node *BoardNode::clone() {
  BoardNode *b = new BoardNode();
  for(int i=0; i<arraySize; i++) {
    b->x[i] = x[i];
    b->y[i] = y[i];
  }
  return b;
}

// h(s) = number of error blocks + total displacement of the blocks
int BoardNode::heuristic(Node *g) {
  BoardNode *goal = (BoardNode*)g;
  int h1 = 0;
  int h2 = 0;
  for(int i=0; i<arraySize; i++) {
    if(x[i] != goal->x[i] or y[i] != goal->y[i])
      h1 += 2;
    h2 = h2 + abs(x[i] - goal->x[i]) + abs(y[i] - goal->y[i]);
  };
  return h1 + h2;
}

/*
 * Pushes the nodes resulted from available actions. Can move the blocks nearby the
 * blank.
 */
void BoardNode::expandNode(vector<Node*>& vec) {
  int bx = x[blankIndex];
  int by = y[blankIndex];

  int itop = getBlockIndexByPosition(bx, by-1);
  if(itop != -1) {
    BoardNode *e = (BoardNode*)clone();
    e->y[itop] = by;
    e->y[blankIndex] = by-1;
    Action *a = new Action(this, e, 1, itop);
    action.push_back(a);
    vec.push_back(e);
  }

  int iright = getBlockIndexByPosition(bx+1, by);
  if(iright != -1) {
    BoardNode *e = (BoardNode*)clone();
    e->x[iright] = bx;
    e->x[blankIndex] = bx+1;
    Action *a = new Action(this, e, 1, iright);
    action.push_back(a);
    vec.push_back(e);
  }

  int ibottom = getBlockIndexByPosition(bx, by+1);
  if(ibottom != -1) {
    BoardNode *e = (BoardNode*)clone();
    e->y[ibottom] = by;
    e->y[blankIndex] = by+1;
    Action *a = new Action(this, e, 1, ibottom);
    action.push_back(a);
    vec.push_back(e);
  }

  int ileft = getBlockIndexByPosition(bx-1, by);
  if(ileft != -1) {
    BoardNode *e = (BoardNode*)clone();
    e->x[ileft] = bx;
    e->x[blankIndex] = bx-1;
    Action *a = new Action(this, e, 1, ileft);
    action.push_back(a);
    vec.push_back(e);
  }
}


// Compares with every node from the list.
bool BoardNode::isDuplicate(Node *n) {
  BoardNode *b = (BoardNode*)(n);
  bool identical = true;
  for(int i=0; i<arraySize; i++) {
    if(x[i] != b->x[i] or y[i] != b->y[i]) {
      identical = false;
      break;
    }
  }
  return identical;
}


static RouteFinding *finding = NULL;
static BoardNode *start;
static BoardNode *goal;


// Access java class info and objects.
void fetchJavaObjects(JNIEnv *env, jobject obj) {
  jclass puzzleAIClass = env->GetObjectClass(obj);
   jfieldID idBoard = env->GetFieldID(puzzleAIClass, "board", "Lpuzzleblock/Board;");
   jobject board = env->GetObjectField(obj, idBoard);

  jclass boardClass = env->GetObjectClass(board);
   jfieldID idCol = env->GetFieldID(boardClass, "col", "I");
   jfieldID idRow = env->GetFieldID(boardClass, "row", "I");
   jfieldID idBlankIndex = env->GetFieldID(boardClass, "blankIndex", "I");
   jfieldID idBlockArray = env->GetFieldID(boardClass, "blocks", "[Lpuzzleblock/Block;");
   jobject blockArrayObj = env->GetObjectField(board, idBlockArray);
   jobjectArray *blockArray = reinterpret_cast<jobjectArray*>(&blockArrayObj);
   arraySize = env->GetArrayLength(*blockArray);
   jobject *blocks = new jobject[arraySize];
   for(int i=0; i<arraySize; i++)
     blocks[i] = env->GetObjectArrayElement(*blockArray, i);

  jclass blockClass = env->GetObjectClass(blocks[0]);
   jfieldID idX = env->GetFieldID(blockClass, "x", "I");
   jfieldID idY = env->GetFieldID(blockClass, "y", "I");

  col = env->GetIntField(board, idCol);
  row = env->GetIntField(board, idRow);
  blankIndex = env->GetIntField(board, idBlankIndex);

  if(finding == NULL) {
    goal = new BoardNode();
    for(int i=0; i<arraySize; i++) {
      goal->x[i] = i % col;
      goal->y[i] = i / col;
    }
  }
  start = new BoardNode();
  for(int i=0; i<arraySize; i++) {
    start->x[i] = env->GetIntField(blocks[i], idX);
    start->y[i] = env->GetIntField(blocks[i], idY);
  }
}


// Finds the answer for the puzzle block problem.
JNIEXPORT void JNICALL Java_puzzleblock_PuzzleBlock_1AI_startRouteFinding
  (JNIEnv *env, jobject obj)
{
  fetchJavaObjects(env, obj);
  if(finding == NULL) {
    finding = new RouteFinding(goal);
    finding->setThinkTime(8000);
  }
  finding->startFinding(start);
}

JNIEXPORT void JNICALL Java_puzzleblock_PuzzleBlock_1AI_clearRouteFinding
  (JNIEnv *env, jobject obj) {}

JNIEXPORT jint JNICALL Java_puzzleblock_PuzzleBlock_1AI_getBlockIndex_1nextMove
  (JNIEnv *env, jobject obj)
{
  return finding->getRoute();
}
