/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package puzzleblock;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import static java.lang.Math.abs;
import javax.swing.JComponent;

/**
 *
 * @author khant
 */
public class Block extends JComponent implements MouseListener {
    public int index;
    public int x, y;
    BufferedImage image;
    int state;
    
    public Board board;
    private static Block lastSelected = null;
    
    private static int HOVER = 0b001;
    private static int ACTIVE = 0b010;
    private static int SELECTED = 0b100;
    
    public Block(int i, Image img) {
        index = i;
        image = Effect.toBufferedImage(img);
        addMouseListener(this);
    }
    
    public Block(int i) {
        index = i;
        addMouseListener(this);
    }
    
    public boolean isAdjacent(Block b) {
        return (abs(this.x - b.x) == 1 && this.y == b.y) || (this.x == b.x && abs(this.y - b.y) == 1);
    }
    
    public boolean isBlank() {
        return (this.index == board.blankIndex);
    }
    
    public void deselect() {
        state &= 0b011;
        repaint();
    }
    
    @Override
    public void mouseClicked(MouseEvent e) {
        if(!isEnabled())
            return;
        state ^= SELECTED;
        if(board.slideBlock(lastSelected, this, false)) {
            deselect();
            lastSelected.deselect();
            lastSelected = null;
        }
        else {
            if(lastSelected != null)
                lastSelected.deselect();
            if(lastSelected != this)
                lastSelected = this;
            else
                lastSelected = null;
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {
        if(!isEnabled())
            return;
        state |= ACTIVE;
        repaint();
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        if(!isEnabled())
            return;
        state &= 0b101;
        repaint();
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        if(!isEnabled())
            return;
        state |= HOVER;
        repaint();
    }

    @Override
    public void mouseExited(MouseEvent e) {
        if(!isEnabled())
            return;
        state &= 0b100;
        repaint();
    }
    
    @Override
    public void paintComponent(Graphics g) {
        if(image == null) {
            image = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_INT_RGB);
            Graphics2D g2 = image.createGraphics();
            g2.setColor(board.getBackground());
            g2.fillRect(0, 0, getWidth(), getHeight());
        }
        BufferedImage bi = Effect.resize(image, getWidth(), getHeight());
        if((state & SELECTED) != 0) {
            if((state & ACTIVE) != 0)
                bi = Effect.imageHSL_manipulation(bi, 30, 30, -5);
            else if((state & HOVER) != 0)
                bi = Effect.imageHSL_manipulation(bi, 30, 22, 55);
            else
                bi = Effect.imageHSL_manipulation(bi, 30, 26, 25);
        }
        else {
            if((state & ACTIVE) != 0)
                bi = Effect.imageHSL_manipulation(bi, 0, 5, -35);
            else if((state & HOVER) != 0)
                bi = Effect.imageHSL_manipulation(bi, 0, -5, 35);
        }
        g.drawImage(bi, 0, 0, null);
    }
    
    public void allocateSpace() {
        int s = board.spacing;
        setBounds(x*(board.blockWidth+s) + s, y*(board.blockHeight+s) + s,
            board.blockWidth, board.blockHeight);
    }
}
