/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package puzzleblock;

import javax.swing.Timer;

/**
 *
 * @author khant
 */
public class PuzzleBlock_AI {
    static {
       System.loadLibrary("puzzleblock");
    }
    
    int totalMoves;
    int computingTime;
    Board board;
    
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
    
    private native void startRouteFinding();
    private native void clearRouteFinding();
    private native int getBlockIndex_nextMove();
}
