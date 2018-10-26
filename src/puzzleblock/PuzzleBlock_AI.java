/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package puzzleblock;

import java.io.File;
import javax.swing.JComponent;
import javax.swing.Timer;

/**
 *
 * @author khant
 */
public class PuzzleBlock_AI {
    int totalMoves;
    int computingTime;
    Board board;
    
    static boolean libLoaded = false;
    
    public PuzzleBlock_AI() {
        if(!libLoaded) {
           String libpuzzleblock = getClass().getResource("c++/" + System.mapLibraryName("puzzleblock")).getPath();
           System.load(libpuzzleblock);
        }
    }
    
    private Timer playing;
    int steps = 0;
    
    public void startPlaying(Board b) {
        board = b;
        PuzzleBlock.setInteractionEnabled(false);
        startRouteFinding();
        playing = new Timer(250, e -> {
            int i = getBlockIndex_nextMove();
            if(i >= 0) {
                board.slideBlock(board.blocks[i], board.blocks[board.blankIndex], false);
                steps++;
                PuzzleBlock.lblSteps.setText(String.format("Steps: %d", steps));
            }
            else {
                playing.stop();
                PuzzleBlock.setInteractionEnabled(true);
                clearRouteFinding();
            }
        });
        steps = 0;
        playing.start();
    }
    
    public void setRandomBlocks(Board b) {
        board = b;
        randomBlocks();
        int i = getBlockIndex_nextMove();
        while(true) {
            if(i >= 0)
                board.slideBlock(board.blocks[i], board.blocks[board.blankIndex], true);
            else {
                clearRouteFinding();
                break;
            }
            i = getBlockIndex_nextMove();
        }
    }
    
    private native void startRouteFinding();
    private native void clearRouteFinding();
    private native int getBlockIndex_nextMove();
    private native void randomBlocks();
}
