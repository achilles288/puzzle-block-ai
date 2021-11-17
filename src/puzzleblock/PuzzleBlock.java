/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package puzzleblock;

import java.awt.Color;
import javax.swing.*;

/**
 *
 * @author khant
 */
public class PuzzleBlock {
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        new PuzzleBlock();
    }

    static Board board;
    static JButton btnAI, btnRandom;
    static JLabel lblSteps;
    static PuzzleBlock_AI puzzleAI;
    
    public PuzzleBlock() {
        JFrame f = new JFrame();
        f.setBounds(200, 60, 830, 660);
        f.setLayout(null);
        f.getContentPane().setBackground(new Color(0x4e668c));
        
        board = new Board(4, 4, 15);
        board.addBlock(new Block(0, new ImageIcon(getClass().getResource("images/cm_0.jpg")).getImage()));
        board.addBlock(new Block(1, new ImageIcon(getClass().getResource("images/cm_1.jpg")).getImage()));
        board.addBlock(new Block(2, new ImageIcon(getClass().getResource("images/cm_2.jpg")).getImage()));
        board.addBlock(new Block(3, new ImageIcon(getClass().getResource("images/cm_3.jpg")).getImage()));
        board.addBlock(new Block(4, new ImageIcon(getClass().getResource("images/cm_4.jpg")).getImage()));
        board.addBlock(new Block(5, new ImageIcon(getClass().getResource("images/cm_5.jpg")).getImage()));
        board.addBlock(new Block(6, new ImageIcon(getClass().getResource("images/cm_6.jpg")).getImage()));
        board.addBlock(new Block(7, new ImageIcon(getClass().getResource("images/cm_7.jpg")).getImage()));
        board.addBlock(new Block(8, new ImageIcon(getClass().getResource("images/cm_8.jpg")).getImage()));
        board.addBlock(new Block(9, new ImageIcon(getClass().getResource("images/cm_9.jpg")).getImage()));
        board.addBlock(new Block(10, new ImageIcon(getClass().getResource("images/cm_10.jpg")).getImage()));
        board.addBlock(new Block(11, new ImageIcon(getClass().getResource("images/cm_11.jpg")).getImage()));
        board.addBlock(new Block(12, new ImageIcon(getClass().getResource("images/cm_12.jpg")).getImage()));
        board.addBlock(new Block(13, new ImageIcon(getClass().getResource("images/cm_13.jpg")).getImage()));
        board.addBlock(new Block(14, new ImageIcon(getClass().getResource("images/cm_14.jpg")).getImage()));
        board.addBlock(new Block(15));
        board.setBounds(0, 0, 630, 630);
        board.spacing = 5;
        board.setBackground(new Color(0x1e3151));
        f.add(board);
        puzzleAI = new PuzzleBlock_AI();
        btnAI = new JButton("Click to solve");
        btnAI.setBounds(650, 120, 160, 40);
        btnAI.addActionListener((e) -> {
            puzzleAI.startPlaying(board);
        });
        f.add(btnAI);
        btnRandom = new JButton("Random steps 50");
        btnRandom.setBounds(650, 180, 160, 40);
        btnRandom.addActionListener((e) -> {
            board.randomBlocks();
        });
        f.add(btnRandom);
        lblSteps = new JLabel("Steps: 0");
        lblSteps.setBounds(650, 60, 160, 40);
        lblSteps.setForeground(Color.white);
        f.add(lblSteps);
        f.setVisible(true);
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
    
    public static void setInteractionEnabled(boolean en) {
        for(int i=0; i<board.col*board.row; i++)
            board.blocks[i].setEnabled(en);
        btnAI.setEnabled(en);
        btnRandom.setEnabled(en);
    }
}
