/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.gui;

import ija.ija2016.game.cmd.tableauToTableau;
import ija.ija2016.game.cmd.wasteToTableau;
import ija.ija2016.game.model.Game;
import ija.ija2016.game.model.cards.*;
import java.awt.Image;

/**
 * Třída reprezentující grafiku pracovního balíku.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class GWorking {
    private CardStack stack;
    private final Game game;
    private final javax.swing.JLayeredPane model;
    private final javax.swing.JLabel empty;

    /**
     * Inicializace prázdního balíku.
     *
     * @param model Model, kde se budou vykreslovat karty.
     * @param game  Instance hry.
     */
    public GWorking(javax.swing.JLayeredPane model, Game game) {
        this.model = model;
        this.game = game;
        
        this.empty = new javax.swing.JLabel();
        empty.setMinimumSize(new java.awt.Dimension(80, 110));
        empty.setPreferredSize(new java.awt.Dimension(80, 110));
        javax.swing.ImageIcon icon = new javax.swing.ImageIcon("lib/images/empty.png");
        empty.setIcon(new javax.swing.ImageIcon(icon.getImage().getScaledInstance(80, 110, Image.SCALE_SMOOTH)));
        empty.setBounds(0, 0, 80, 110);
        model.add(empty, 1, 0);
        
        empty.addMouseListener(new java.awt.event.MouseAdapter() {
            @Override
            public void mousePressed(java.awt.event.MouseEvent evt) {
                if (evt.getButton() == java.awt.event.MouseEvent.BUTTON1) {
                    GamePanel x = (GamePanel)model.getParent().getParent();
                    if (x.sourcePack == 1) {
                        x.invoker.execute(new wasteToTableau(game, stack.getID()-6));
                        x.sourcePack = -1;
                        x.indexCard = -1;
                        x.unmark();
                    } else if (x.sourcePack != -1) {
                        x.invoker.execute(new tableauToTableau(game, x.indexCard, x.sourcePack-6, stack.getID()-6));
                        x.sourcePack = -1;
                        x.indexCard = -1;
                        x.unmark();
                    }
                    x.paintAllPacks();
                    x.repaint();
                }
            }
        });
    }

    /**
     * Inicializace prázdního balíku a vytvoření grafik pro karty v balíku.
     * @param stack Balík karet.
     * @param model Model, kde se budou vykreslovat karty.
     * @param game  Instance hry.
     */
    public GWorking(CardStack stack, javax.swing.JLayeredPane model, Game game) {
        this(model, game);
        this.stack = stack;
        createCards();
    }

    /**
     * Vytvoří grafiky pro karty v balíku.
     */
    private void createCards() {
        for (int i = 0; i < stack.size(); i++) {
            Card c = stack.get(i);
            GCard g = new GCard(c);
            c.setGraphics(g);
            g.setStateOfCard();
        }
    }

    /**
     * Vykreslí karty na svých pozicích.
     */
    public void paint() {
        if (!stack.isEmpty()) {
            int offset = 15;
            for (int i = 0; i < stack.size(); i++) {
                GCard g = stack.get(i).getGraphics();
                g.changePosition(offset * i);
                g.setStateOfCard();
                model.add(g, 1, 0);
            }
        }
    }
}
