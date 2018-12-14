/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package puzzleblock;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.util.ArrayList;
import java.util.Random;
import javax.swing.JPanel;
import javax.swing.Timer;

/**
 *
 * @author khant
 */
public class Board extends JPanel implements ComponentListener {
    Block[] blocks;
    public int col;
    public int row;
    public int blankIndex;
    
    ArrayList<Integer> indecies;
    public int spacing = 3;
    public int blockWidth;
    public int blockHeight;
    
    public Board(int c, int r, int b) {
        col = c;
        row = r;
        blankIndex = b;
        int n = c * r;
        blocks = new Block[n];
        indecies = new ArrayList<>();
        for(int i=0; i<n; i++)
            indecies.add(i);
        setLayout(null);
        addComponentListener(this);
        
        sliding = new Timer(4, e -> {
            slideCount++;
            int x, y;
            float f = slideCount * 0.05f - 1.0f;
            x = (int)(((float)slidingBlock.x+(slideX*f)) * (blockWidth+spacing) + spacing);
            y = (int)(((float)slidingBlock.y+(slideY*f)) * (blockHeight+spacing) + spacing);
            slidingBlock.setLocation(x, y);
            slidingBlock.repaint();
            if(slideCount == 20) {
                sliding.stop();
                slidingBlank.setVisible(true);
                slidingBlank.allocateSpace();
                slidingBlock.allocateSpace();
            }
        });
    }
    
    int n=0;
    
    public void addBlock(Block b) {
        b.board = this;
        int i = n;
        n++;
        //int i = indecies.remove(new Random().nextInt(indecies.size()));
        b.x = i % col;
        b.y = i / col;
        blocks[b.index] = b;
        b.allocateSpace();
        this.add(b);
    }
    
    private Timer sliding;
    private Block slidingBlock;
    private Block slidingBlank;
    private float slideX, slideY;
    private int slideCount;
                    
    public boolean slideBlock(Block A, Block B, boolean instant) {
        if(A == null)
            return false;
        if(A.isAdjacent(B)) {
            Block b, blank;
            if(A.isBlank()) {
                blank = A;
                b = B;
            }
            else if(B.isBlank()) {
                blank = B;
                b = A;
            }
            else
                return false;
            
            if(instant == false) {
                blank.setVisible(false);
                 slidingBlock = b;
                slidingBlank = blank;
                slideX = blank.x - b.x;
                slideY = blank.y - b.y;
                slideCount = 0;
                sliding.start();
            }
            
            int tmp;
            tmp = blank.x;
            blank.x = b.x;
            b.x = tmp;
            tmp = blank.y;
            blank.y = b.y;
            b.y = tmp;
            
            if(instant == true) {
                A.allocateSpace();
                B.allocateSpace();
            }
            
            return true;
        }
        else {
            return false;
        }
    }
    
    @Override
    public void componentResized(ComponentEvent e) {
        blockWidth = ((this.getWidth() - (spacing * (col + 1))) / col);
        blockHeight = ((this.getHeight() - (spacing * (row + 1))) / row);
        for(int i=0; i<col*row; i++) {
            if(blocks[i] != null)
                blocks[i].allocateSpace();
        }
    }

    @Override
    public void componentMoved(ComponentEvent e) {}

    @Override
    public void componentShown(ComponentEvent e) {}

    @Override
    public void componentHidden(ComponentEvent e) {}
     
    private int getIndexByPosition(int x, int y) {
        for(int i=0; i<col*row; i++) {
            if(blocks[i].x == x && blocks[i].y == y)
                return i;
        }
        return 0;
    }
    
    public void randomBlocks() {
        int moves[][] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        Random rand = new Random();
        for(int i=0; i<50; i++) {
            int bx = blocks[blankIndex].x;
            int by = blocks[blankIndex].y;
            int r = rand.nextInt(4);
            Block b = blocks[getIndexByPosition(bx+moves[r][0], by+moves[r][1])];
            slideBlock(b, blocks[blankIndex], true);
        }
    }
}
