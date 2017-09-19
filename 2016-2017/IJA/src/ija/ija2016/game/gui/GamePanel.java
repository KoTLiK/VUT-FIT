/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.gui;

import ija.ija2016.Solitaire;
import ija.ija2016.game.cmd.*;
import ija.ija2016.game.model.Game;
import ija.ija2016.game.model.cards.*;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

/**
 * Třída reprezentující GUI konkrétní hry, její hrací plochy.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class GamePanel extends javax.swing.JPanel {
    private final Solitaire model;
    private final Game game;
    private final int id;
    private final GDeck d0;
    private final GDeck d1;
    private final GDeck t0;
    private final GDeck t1;
    private final GDeck t2;
    private final GDeck t3;
    private final GWorking w0;
    private final GWorking w1;
    private final GWorking w2;
    private final GWorking w3;
    private final GWorking w4;
    private final GWorking w5;
    private final GWorking w6;
    public int sourcePack;
    public int indexCard;
    public final Commander.Invoker invoker = new Commander.Invoker();
    private final List<GCard> marks = new ArrayList<>();

    /**
     * Vytvoření a inicializace celého GUI.
     *
     * @param model Okno programu (JFrame).
     * @param game  Instance hry.
     * @param id    ID hry.
     */
    public GamePanel(Solitaire model, Game game, int id) {
        this.model = model;
        this.game = game;
        this.id = id;
        sourcePack = -1;
        indexCard = -1;
        
        initComponents();
        
        game.setScoreLabel(scoreValue);
        
        d0 = new GDeck(game.getDeck0(), deck0, game);
        d1 = new GDeck(game.getDeck1(), deck1, game);

        t0 = new GDeck(game.getTargets()[0], target0, game);
        t1 = new GDeck(game.getTargets()[1], target1, game);
        t2 = new GDeck(game.getTargets()[2], target2, game);
        t3 = new GDeck(game.getTargets()[3], target3, game);
        
        w0 = new GWorking(game.getWorking()[0], working0, game);
        w1 = new GWorking(game.getWorking()[1], working1, game);
        w2 = new GWorking(game.getWorking()[2], working2, game);
        w3 = new GWorking(game.getWorking()[3], working3, game);
        w4 = new GWorking(game.getWorking()[4], working4, game);
        w5 = new GWorking(game.getWorking()[5], working5, game);
        w6 = new GWorking(game.getWorking()[6], working6, game);
        
        GamePanel x = this;
        for (int i = 0; i < game.getDeck0().size(); i++) {
            GCard g = game.getDeck0().get(i).getGraphics();
            g.addMouseListener(new GamePanel.CardListener(x, g));
        }
        for (int i = 0; i < game.getDeck1().size(); i++) {
            GCard g = game.getDeck1().get(i).getGraphics();
            g.addMouseListener(new GamePanel.CardListener(x, g));
        }
        
        for (int i = 0; i < 7; i++) {
            for (int k = 0; k < game.getWorking()[i].size(); k++) {
                GCard g = game.getWorking()[i].get(k).getGraphics();
                g.addMouseListener(new GamePanel.CardListener(x, g));
            }
        }
        
        paintAllPacks();
    }

    /**
     * Třída reprezentující listener karty.
     */
    class CardListener extends java.awt.event.MouseAdapter {
        private final GCard g;
        private final GamePanel x;

        /**
         * Inicializace listeneru a jeho propojení s GUI.
         *
         * @param x Hrací pole.
         * @param g Grafika karty.
         */
        public CardListener(GamePanel x, GCard g) {
            this.g = g;
            this.x = x;
        }
        
        @Override
        public void mousePressed(java.awt.event.MouseEvent evt) {
            if (evt.getButton() == java.awt.event.MouseEvent.BUTTON1) {
                String source = g.getParent().getName();
                int i, dst = Integer.parseInt(source);
                switch (source) {
                    case "0": // Deck0
                        sourcePack = -1;
                        indexCard = -1;
                        x.unmark();
                        invoker.execute(new nextWaste(game));
                        break;
                    case "1": // Deck1
                        if (sourcePack == 1 && evt.getClickCount() == 2) {
                            if (invoker.add(new wasteToFoundation(game, 0))) {
                                sourcePack = -1;
                                indexCard = -1;
                                x.unmark();
                                checkForWin();
                            } else if (invoker.add(new wasteToFoundation(game, 1))) {
                                sourcePack = -1;
                                indexCard = -1;
                                x.unmark();
                                checkForWin();
                            } else if (invoker.add(new wasteToFoundation(game, 2))) {
                                sourcePack = -1;
                                indexCard = -1;
                                x.unmark();
                                checkForWin();
                            } else if (invoker.add(new wasteToFoundation(game, 3))) {
                                sourcePack = -1;
                                indexCard = -1;
                                x.unmark();
                                checkForWin();
                            } else {
                                x.unmark();
                                sourcePack = 1;
                                indexCard = 0;
                                x.mark(g);
                            }
                        } else {
                            x.unmark();
                            sourcePack = 1;
                            indexCard = 0;
                            x.mark(g);
                        }
                        break;
                    case "2": // Targets
                    case "3":
                    case "4":
                    case "5":
                        if (sourcePack == 1) {
                            invoker.execute(new wasteToFoundation(game, dst-2));
                            sourcePack = -1;
                            indexCard = -1;
                            x.unmark();
                            checkForWin();
                        } else if (sourcePack > 5 && sourcePack < 13) {
                            invoker.execute(new tableauToFoundation(game, indexCard, sourcePack-6, dst-2));
                            sourcePack = -1;
                            indexCard = -1;
                            x.unmark();
                            checkForWin();
                        }
                        break;
                    default: // Working
                        i = game.getWorking()[dst-6].size()-
                                ((javax.swing.JLayeredPane)g.getParent()).getIndexOf(g) -1;
                        if (game.getWorking()[dst-6].get(i).isTurnedFaceUp()) {
                            if (sourcePack == dst) {
                                for (int q = 0; q < 4; q++) {
                                    if (invoker.add(new tableauToFoundation(game, indexCard, sourcePack-6, q))) {
                                        sourcePack = -1;
                                        indexCard = -1;
                                        x.unmark();
                                        checkForWin();
                                        break;
                                    }
                                }
                            } else if (sourcePack == 1) {
                                invoker.execute(new wasteToTableau(game, dst-6));
                                sourcePack = -1;
                                indexCard = -1;
                                x.unmark();
                            } else if (sourcePack > 5 && sourcePack < 13) {
                                invoker.execute(new tableauToTableau(game, indexCard, sourcePack-6, dst-6));
                                sourcePack = -1;
                                indexCard = -1;
                                x.unmark();
                            } else {
                                x.unmark();
                                sourcePack = dst;
                                indexCard = i;
                                x.mark(g);
                            }
                        } else if (game.getWorking()[dst-6].size()-1 == i) {
                            invoker.execute(new tableauTurnOver(game, dst-6));
                            x.unmark();
                        }
                        break;
                }
                x.paintAllPacks();
                x.repaint();
            }
        }
    }

    /**
     * Kontrola, zda hra již neskončila.
     * V případe konce hry (vítěztví), se zobrazí informační dialog.
     */
    public void checkForWin() {
        for (CardDeck deck : game.getTargets()) {
            Card c = deck.get();
            if (c == null) return;
            if (c.value() != 13) return;
        }

        paintAllPacks();
        repaint();
        final javax.swing.ImageIcon icon = new javax.swing.ImageIcon("lib/images/W.png");
        javax.swing.JOptionPane.showMessageDialog(this,"Congratulation!\nYou won the game with score "+game.score+".",
                        "Victory", javax.swing.JOptionPane.INFORMATION_MESSAGE, icon);
        this.model.closeGame(this.id);
    }

    /**
     * Označí grafiku karty.
     *
     * @param g Grafika karty.
     */
    public void mark(GCard g) {
        g.setMark(true);
        marks.add(g);
    }

    /**
     * Odznačí všechny karty.
     */
    public void unmark() {
        for (int i = 0; i < marks.size(); i++)
            marks.remove(0).setMark(false);
    }

    /**
     * Překreslí všechny balíčky.
     */
    public void paintAllPacks() {
        d0.paint();
        d1.paint();
        t0.paint();
        t1.paint();
        t2.paint();
        t3.paint();
        w0.paint();
        w1.paint();
        w2.paint();
        w3.paint();
        w4.paint();
        w5.paint();
        w6.paint();
        game.updateScore();
    }

    /**
     * Překreslí konkrétní balíček.
     *
     * @param name Název balíčku.
     */
    public void paintPack(String name) {
        switch(name) {
            case "0":d0.paint();break;
            case "1":d1.paint();break;
            case "2":t0.paint();break;
            case "3":t1.paint();break;
            case "4":t2.paint();break;
            case "5":t3.paint();break;
            case "6":w0.paint();break;
            case "7":w1.paint();break;
            case "8":w2.paint();break;
            case "9":w3.paint();break;
            case "10":w4.paint();break;
            case "11":w5.paint();break;
            case "12":w6.paint();break;
        }
    }
    
    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        scoreText = new javax.swing.JLabel();
        scoreValue = new javax.swing.JLabel();
        hint = new javax.swing.JButton();
        undo = new javax.swing.JButton();
        closeGame = new javax.swing.JButton();
        saveGame = new javax.swing.JButton();
        board = new javax.swing.JLayeredPane();
        deck0 = new javax.swing.JLayeredPane();
        deck1 = new javax.swing.JLayeredPane();
        empty = new javax.swing.JLabel();
        target0 = new javax.swing.JLayeredPane();
        target1 = new javax.swing.JLayeredPane();
        target2 = new javax.swing.JLayeredPane();
        target3 = new javax.swing.JLayeredPane();
        working0 = new javax.swing.JLayeredPane();
        working1 = new javax.swing.JLayeredPane();
        working2 = new javax.swing.JLayeredPane();
        working3 = new javax.swing.JLayeredPane();
        working4 = new javax.swing.JLayeredPane();
        working5 = new javax.swing.JLayeredPane();
        working6 = new javax.swing.JLayeredPane();

        setBorder(javax.swing.BorderFactory.createLineBorder(new java.awt.Color(100, 100, 100)));
        setMinimumSize(new java.awt.Dimension(700, 520));
        setPreferredSize(new java.awt.Dimension(700, 520));

        scoreText.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        scoreText.setText("Score:");

        scoreValue.setHorizontalAlignment(javax.swing.SwingConstants.RIGHT);
        scoreValue.setText("0");
        scoreValue.setMinimumSize(new java.awt.Dimension(25, 14));
        scoreValue.setPreferredSize(new java.awt.Dimension(25, 14));

        hint.setText("Hint");
        hint.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                hintMousePressed(evt);
            }
        });

        undo.setText("Undo");
        undo.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                undoMousePressed(evt);
            }
        });

        closeGame.setText("Close Game");
        closeGame.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                closeGameMousePressed(evt);
            }
        });

        saveGame.setText("Save Game");
        saveGame.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                saveGameMousePressed(evt);
            }
        });

        board.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                unFocus(evt);
            }
        });

        deck0.setMinimumSize(new java.awt.Dimension(80, 110));
        deck0.setName("0"); // NOI18N

        javax.swing.GroupLayout deck0Layout = new javax.swing.GroupLayout(deck0);
        deck0.setLayout(deck0Layout);
        deck0Layout.setHorizontalGroup(
            deck0Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        deck0Layout.setVerticalGroup(
            deck0Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 110, Short.MAX_VALUE)
        );

        deck1.setMinimumSize(new java.awt.Dimension(80, 110));
        deck1.setName("1"); // NOI18N

        javax.swing.GroupLayout deck1Layout = new javax.swing.GroupLayout(deck1);
        deck1.setLayout(deck1Layout);
        deck1Layout.setHorizontalGroup(
            deck1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        deck1Layout.setVerticalGroup(
            deck1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 110, Short.MAX_VALUE)
        );

        empty.setMaximumSize(new java.awt.Dimension(80, 110));
        empty.setMinimumSize(new java.awt.Dimension(80, 110));
        empty.setPreferredSize(new java.awt.Dimension(80, 110));

        target0.setMinimumSize(new java.awt.Dimension(80, 110));
        target0.setName("2"); // NOI18N

        javax.swing.GroupLayout target0Layout = new javax.swing.GroupLayout(target0);
        target0.setLayout(target0Layout);
        target0Layout.setHorizontalGroup(
            target0Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        target0Layout.setVerticalGroup(
            target0Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 110, Short.MAX_VALUE)
        );

        target1.setMinimumSize(new java.awt.Dimension(80, 110));
        target1.setName("3"); // NOI18N

        javax.swing.GroupLayout target1Layout = new javax.swing.GroupLayout(target1);
        target1.setLayout(target1Layout);
        target1Layout.setHorizontalGroup(
            target1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        target1Layout.setVerticalGroup(
            target1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 110, Short.MAX_VALUE)
        );

        target2.setMinimumSize(new java.awt.Dimension(80, 110));
        target2.setName("4"); // NOI18N

        javax.swing.GroupLayout target2Layout = new javax.swing.GroupLayout(target2);
        target2.setLayout(target2Layout);
        target2Layout.setHorizontalGroup(
            target2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        target2Layout.setVerticalGroup(
            target2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 110, Short.MAX_VALUE)
        );

        target3.setMinimumSize(new java.awt.Dimension(80, 110));
        target3.setName("5"); // NOI18N

        javax.swing.GroupLayout target3Layout = new javax.swing.GroupLayout(target3);
        target3.setLayout(target3Layout);
        target3Layout.setHorizontalGroup(
            target3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        target3Layout.setVerticalGroup(
            target3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 110, Short.MAX_VALUE)
        );

        working0.setMinimumSize(new java.awt.Dimension(80, 110));
        working0.setName("6"); // NOI18N

        javax.swing.GroupLayout working0Layout = new javax.swing.GroupLayout(working0);
        working0.setLayout(working0Layout);
        working0Layout.setHorizontalGroup(
            working0Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        working0Layout.setVerticalGroup(
            working0Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 339, Short.MAX_VALUE)
        );

        working1.setMinimumSize(new java.awt.Dimension(80, 110));
        working1.setName("7"); // NOI18N

        javax.swing.GroupLayout working1Layout = new javax.swing.GroupLayout(working1);
        working1.setLayout(working1Layout);
        working1Layout.setHorizontalGroup(
            working1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        working1Layout.setVerticalGroup(
            working1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        working2.setMinimumSize(new java.awt.Dimension(80, 110));
        working2.setName("8"); // NOI18N

        javax.swing.GroupLayout working2Layout = new javax.swing.GroupLayout(working2);
        working2.setLayout(working2Layout);
        working2Layout.setHorizontalGroup(
            working2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        working2Layout.setVerticalGroup(
            working2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        working3.setMinimumSize(new java.awt.Dimension(80, 110));
        working3.setName("9"); // NOI18N

        javax.swing.GroupLayout working3Layout = new javax.swing.GroupLayout(working3);
        working3.setLayout(working3Layout);
        working3Layout.setHorizontalGroup(
            working3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        working3Layout.setVerticalGroup(
            working3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        working4.setMinimumSize(new java.awt.Dimension(80, 110));
        working4.setName("10"); // NOI18N

        javax.swing.GroupLayout working4Layout = new javax.swing.GroupLayout(working4);
        working4.setLayout(working4Layout);
        working4Layout.setHorizontalGroup(
            working4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        working4Layout.setVerticalGroup(
            working4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        working5.setMinimumSize(new java.awt.Dimension(80, 110));
        working5.setName("11"); // NOI18N

        javax.swing.GroupLayout working5Layout = new javax.swing.GroupLayout(working5);
        working5.setLayout(working5Layout);
        working5Layout.setHorizontalGroup(
            working5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        working5Layout.setVerticalGroup(
            working5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        working6.setMinimumSize(new java.awt.Dimension(80, 110));
        working6.setName("12"); // NOI18N

        javax.swing.GroupLayout working6Layout = new javax.swing.GroupLayout(working6);
        working6.setLayout(working6Layout);
        working6Layout.setHorizontalGroup(
            working6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 80, Short.MAX_VALUE)
        );
        working6Layout.setVerticalGroup(
            working6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        board.setLayer(deck0, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(deck1, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(empty, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(target0, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(target1, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(target2, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(target3, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(working0, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(working1, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(working2, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(working3, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(working4, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(working5, javax.swing.JLayeredPane.DEFAULT_LAYER);
        board.setLayer(working6, javax.swing.JLayeredPane.DEFAULT_LAYER);

        javax.swing.GroupLayout boardLayout = new javax.swing.GroupLayout(board);
        board.setLayout(boardLayout);
        boardLayout.setHorizontalGroup(
            boardLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(boardLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(boardLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(boardLayout.createSequentialGroup()
                        .addComponent(deck0, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(deck1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(empty, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(target0, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(target1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(target2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(target3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(boardLayout.createSequentialGroup()
                        .addComponent(working0, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(working1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(working2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(working3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(working4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(working5, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(working6, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(20, Short.MAX_VALUE))
        );
        boardLayout.setVerticalGroup(
            boardLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(boardLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(boardLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(target3, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(target0, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(empty, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(deck1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(deck0, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(target1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(target2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(boardLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(working0, javax.swing.GroupLayout.DEFAULT_SIZE, 339, Short.MAX_VALUE)
                    .addComponent(working1, javax.swing.GroupLayout.DEFAULT_SIZE, 339, Short.MAX_VALUE)
                    .addComponent(working2, javax.swing.GroupLayout.DEFAULT_SIZE, 339, Short.MAX_VALUE)
                    .addComponent(working3, javax.swing.GroupLayout.DEFAULT_SIZE, 339, Short.MAX_VALUE)
                    .addComponent(working4, javax.swing.GroupLayout.DEFAULT_SIZE, 339, Short.MAX_VALUE)
                    .addComponent(working5, javax.swing.GroupLayout.DEFAULT_SIZE, 339, Short.MAX_VALUE)
                    .addComponent(working6, javax.swing.GroupLayout.DEFAULT_SIZE, 339, Short.MAX_VALUE)))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(board)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(saveGame)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(closeGame)
                .addGap(18, 18, 18)
                .addComponent(undo)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(hint)
                .addGap(18, 18, 18)
                .addComponent(scoreText)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(scoreValue, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(saveGame)
                    .addComponent(closeGame)
                    .addComponent(undo)
                    .addComponent(hint)
                    .addComponent(scoreText)
                    .addComponent(scoreValue, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(board))
        );
    }// </editor-fold>//GEN-END:initComponents

    /**
     * Kliknutí na hrací plochu mimo karet.
     *
     * @param evt Kliknutí.
     */
    private void unFocus(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_unFocus
        if (evt.getButton() == java.awt.event.MouseEvent.BUTTON1) {
            sourcePack = -1;
            indexCard = -1;
            unmark();
            paintAllPacks();
            repaint();
        }
    }//GEN-LAST:event_unFocus

    /**
     * Ukončení hry.
     *
     * @param evt Kliknutí.
     */
    private void closeGameMousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_closeGameMousePressed
        if (evt.getButton() == java.awt.event.MouseEvent.BUTTON1) {
            this.model.closeGame(this.id);
        }
    }//GEN-LAST:event_closeGameMousePressed

    /**
     * Uložení hry.
     *
     * @param evt Kliknutí.
     */
    private void saveGameMousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_saveGameMousePressed
        if (evt.getButton() == java.awt.event.MouseEvent.BUTTON1) {
            evt.getComponent().setEnabled(false);
            savePrompt();
            evt.getComponent().setEnabled(true);
        }
    }//GEN-LAST:event_saveGameMousePressed

    /**
     * Dialogové okno výzvy k uložení hry.
     */
    private void savePrompt(){
        String name = javax.swing.JOptionPane.showInputDialog(this, "Name:",
                "Save", javax.swing.JOptionPane.PLAIN_MESSAGE);
        if (name == null) return;
        if (name.length() == 0) {
            javax.swing.JOptionPane.showMessageDialog(this,
                    "ERROR:\n  Name must contains at least one character!",
                    "Save", javax.swing.JOptionPane.ERROR_MESSAGE);
            return;
        } else {
            int status;
            File dir = new File("examples");
            File[] fileslist = dir.listFiles();
            if (fileslist != null) {
                for (File f : fileslist) {
                    if (f.isFile()) {
                        String x = f.getName();
                        x = x.replaceAll(".save$", "");
                        if (x.equals(name)) {
                            status = javax.swing.JOptionPane.showConfirmDialog(this,
                                    "Would you like to override '"+x+"' save?", "Save",
                                    javax.swing.JOptionPane.YES_NO_OPTION, javax.swing.JOptionPane.QUESTION_MESSAGE);
                            if (status == javax.swing.JOptionPane.YES_OPTION) break;
                            if (status == javax.swing.JOptionPane.NO_OPTION) {
                                savePrompt();
                                break;
                            } else return;
                        }
                    }
                }
            }
        }

        try {
            FileOutputStream saveFile = new FileOutputStream("examples/"+name+".save");
            ObjectOutputStream save;
            save = new ObjectOutputStream(saveFile);
            save.writeObject(game);
            save.close();
            javax.swing.JOptionPane.showMessageDialog(this,"Game was saved.",
                    "Save", javax.swing.JOptionPane.INFORMATION_MESSAGE);
        } catch (IOException e) {
            e.printStackTrace();
            javax.swing.JOptionPane.showMessageDialog(this,"Save game failed!",
                    "Error", javax.swing.JOptionPane.ERROR_MESSAGE);
        }
    }

    /**
     * Nápověda hry.
     *
     * @param evt Kliknutí.
     */
    private void hintMousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_hintMousePressed
        if (evt.getButton() == java.awt.event.MouseEvent.BUTTON1) {
            unmark();

            evt.getComponent().setEnabled(false);
            java.awt.EventQueue.invokeLater(this::printHints);
            evt.getComponent().setEnabled(true);

            paintAllPacks();
            repaint();
        }
    }//GEN-LAST:event_hintMousePressed

    /**
     * Najde všechny možné tahy v hře a zobrazí je v dialogovém okne.
     */
    private void printHints() {
        CardDeck[] target = game.getTargets();
        CardStack[] working = game.getWorking();
        StringBuilder s = new StringBuilder();


        for (int i = 0; i < 7; i++) {
            if (working[i].size() != 0 && !working[i].get().isTurnedFaceUp()) {
                s.append("Turn card over at tableau #").append(i).append("\n");
            }
        }

        for (int i = 0; i < 7; i++) { // tableau to tableau
            for (int j = working[i].size()-1; j >= 0 ; j--) {
                if (working[i].get(j).isTurnedFaceUp()) {
                    for (int k = 0; k < 7; k++) {
                        if (k != i) {
                            Card c;
                            if ((c = game.move.tableauToTableau(j, i, k)) != null) {
                                game.move.tableauToTableauReverse(c, i, k);
                                s.append("From tableau #").append(i).append(" move card ").append(c).append(" to tableau #")
                                        .append(k).append(" at ").append(working[k].get()).append("\n");
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 4 && working[i].size() != 0; j++) {
                if (working[i].get().isTurnedFaceUp()) {
                    if (game.move.tableauToFoundation(working[i].size()-1, i, j)) {
                        game.move.tableauToFoundationReverse(i, j);
                        s.append("From tableau #").append(i).append(" move card ").append(working[i].get())
                                .append(" to target #").append(j).append(" at ").append(target[j].get()).append("\n");
                    }
                }
            }
        }

        for (int i = 0; i < 7; i++) {
            if (game.getDeck1().size() != 0) {
                if (game.move.wasteToTableau(i)) {
                    game.move.wasteToTableauReverse(i);
                    s.append("From waste deck move card ").append(game.getDeck1().get()).append(" to tableau #")
                            .append(i).append(" at ").append(working[i].get()).append("\n");
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            if (game.getDeck1().size() != 0) {
                if (game.move.wasteToFoundation(i)) {
                    game.move.wasteToFoundationReverse(i);
                    s.append("From waste deck move card ").append(game.getDeck1().get()).append(" to target #")
                            .append(i).append(" at ").append(target[i].get()).append("\n");
                }
            }
        }

        if (game.move.nextWaste()) {
            game.move.nextWasteReverse();
            s.append("Waste next card.").append("\n");
        }

        System.out.println(s.capacity());
        javax.swing.JOptionPane.showMessageDialog(this, s.toString(),
                "Hints", javax.swing.JOptionPane.PLAIN_MESSAGE);
    }

    /**
     * Vrácení tahu.
     *
     * @param evt Kliknutí.
     */
    private void undoMousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_undoMousePressed
        if (evt.getButton() == java.awt.event.MouseEvent.BUTTON1) {
            evt.getComponent().setEnabled(false);
            invoker.undo();
            unmark();
            paintAllPacks();
            repaint();
            evt.getComponent().setEnabled(true);
        }
    }//GEN-LAST:event_undoMousePressed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JLayeredPane board;
    private javax.swing.JButton closeGame;
    private javax.swing.JLayeredPane deck0;
    private javax.swing.JLayeredPane deck1;
    private javax.swing.JLabel empty;
    private javax.swing.JButton hint;
    private javax.swing.JButton saveGame;
    private javax.swing.JLabel scoreText;
    private javax.swing.JLabel scoreValue;
    private javax.swing.JLayeredPane target0;
    private javax.swing.JLayeredPane target1;
    private javax.swing.JLayeredPane target2;
    private javax.swing.JLayeredPane target3;
    private javax.swing.JButton undo;
    private javax.swing.JLayeredPane working0;
    private javax.swing.JLayeredPane working1;
    private javax.swing.JLayeredPane working2;
    private javax.swing.JLayeredPane working3;
    private javax.swing.JLayeredPane working4;
    private javax.swing.JLayeredPane working5;
    private javax.swing.JLayeredPane working6;
    // End of variables declaration//GEN-END:variables
}
