#ifndef __PUZZLEBLOCK_H__
#define __PUZZLEBLOCK_H__

#include "routefinding.h"

#include <jni.h>


// Contains a set of board informations (positions of blocks).
class BoardNode: public Node {
  public:
    BoardNode();
    ~BoardNode();

  private:
    char *x; // Block positions.
    char *y;

    int getBlockIndexByPosition(int x2, int y2);
    Node *clone(); // Copies the node data.

    int heuristic(Node *g); // Gets error between this node and goal node.

    // Looks for some available actions. Add them to 'vec'.
    void expandNode(std::vector<Node*>& vec);

    // To remove not to go back to explored node.
    bool isDuplicate(Node *n);

  friend void fetchJavaObjects(JNIEnv *env, jobject obj);
};


// Functions to be invoked by the java application.
extern "C" {
/*
 * Class:     puzzleblock_PuzzleBlock_AI
 * Method:    startRouteFinding
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_puzzleblock_PuzzleBlock_1AI_startRouteFinding
  (JNIEnv *, jobject);

/*
 * Class:     puzzleblock_PuzzleBlock_AI
 * Method:    clearRouteFinding
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_puzzleblock_PuzzleBlock_1AI_clearRouteFinding
  (JNIEnv *, jobject);

/*
 * Class:     puzzleblock_PuzzleBlock_AI
 * Method:    getBlockIndex_nextMove
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_puzzleblock_PuzzleBlock_1AI_getBlockIndex_1nextMove
  (JNIEnv *, jobject);

/*
 * Class:     puzzleblock_PuzzleBlock_AI
 * Method:    randomBlocks
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_puzzleblock_PuzzleBlock_1AI_randomMove
  (JNIEnv *, jobject);
}

int col, row, blankIndex, arraySize;

// Assigns class info and objects from java application into above variables.
void fetchJavaObjects(JNIEnv *env, jobject obj);

#endif
