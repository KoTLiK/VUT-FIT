/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.gui;

import ija.ija2016.game.cmd.recycleWaste;
import ija.ija2016.game.cmd.tableauToFoundation;
import ija.ija2016.game.cmd.wasteToFoundation;
import ija.ija2016.game.model.Game;
import ija.ija2016.game.model.cards.*;
import java.awt.Image;
import java.io.File;

/**
 * Třída reprezentující grafiku hlavního, odhazovacího a cílového balíku.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class GDeck {
    private CardDeck deck;
    private final Game game;
    private final javax.swing.JLayeredPane model;
    private final javax.swing.JLabel empty;

    /**
     * Inicializace prázdního balíku.
     *
     * @param model Model, kde se budou vykreslovat karty.
     * @param game  Instance hry.
     */
    public GDeck(javax.swing.JLayeredPane model, Game game) {
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
                    switch (deck.getID()) {
                        case 0:
                            x.invoker.execute(new recycleWaste(game));
                            x.sourcePack = -1;
                            x.indexCard = -1;
                            x.unmark();
                            break;
                        case 1:
                            x.sourcePack = -1;
                            x.indexCard = -1;
                            x.unmark();
                            break;
                        default:
                            if (x.sourcePack == 1) {
                                x.invoker.execute(new wasteToFoundation(game, deck.getID()-2));
                                x.sourcePack = -1;
                                x.indexCard = -1;
                                x.unmark();
                            } else if (x.sourcePack > 5 && x.sourcePack < 13) {
                                x.invoker.execute(new tableauToFoundation(game, x.indexCard, x.sourcePack-6, deck.getID()-2));
                                x.sourcePack = -1;
                                x.indexCard = -1;
                                x.unmark();
                            }
                            break;
                    }
                    x.paintAllPacks();
                    x.repaint();
                }
            }
        });
    }

    /**
     * Inicializace prázdního balíku a vytvoření grafik pro karty v balíku.
     * @param deck  Balík karet.
     * @param model Model, kde se budou vykreslovat karty.
     * @param game  Instance hry.
     */
    public GDeck(CardDeck deck, javax.swing.JLayeredPane model, Game game) {
        this(model, game);
        this.deck = deck;
        createCards();
    }

    /**
     * Vytvoří grafiky pro karty v balíku.
     */
    private void createCards() {
        for (int i = 0; i < deck.size(); i++) {
            Card c = deck.get(i);
            GCard g = new GCard(c);
            c.setGraphics(g);
            g.setStateOfCard();
        }
    }

    /**
     * Vykreslí karty na svých pozicích.
     */
    public void paint() {
        if (!deck.isEmpty()) {
            for (int i = 0; i < deck.size(); i++) {
                GCard g = deck.get(i).getGraphics();
                g.changePosition(0);
                g.setStateOfCard();
                model.add(g, 1, 0);
            }
        }
    }
}
