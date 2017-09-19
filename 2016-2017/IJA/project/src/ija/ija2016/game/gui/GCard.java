/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.gui;

import ija.ija2016.game.model.board.Position;
import ija.ija2016.game.model.cards.Card;
import java.awt.*;
import javax.swing.BorderFactory;

/**
 * Třída reprezentující grafiku karty.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class GCard extends javax.swing.JLabel {
    private final Card card;
    private final javax.swing.ImageIcon front;
    private final javax.swing.ImageIcon back;
    private boolean marked;

    /**
     * Inicializace grafiky, načítaní obrázků.
     *
     * @param card Karta, podle které se vygeneruje grafika.
     */
    public GCard(Card card) {
        this.card = card;
        javax.swing.ImageIcon local = new javax.swing.ImageIcon("lib/images/"+card.color().toString()+"/"+card.value()+".png");
        this.front = new javax.swing.ImageIcon(local.getImage().getScaledInstance(80, 110, Image.SCALE_SMOOTH));
        local = new javax.swing.ImageIcon("lib/images/B.png");
        this.back = new javax.swing.ImageIcon(local.getImage().getScaledInstance(80, 110, Image.SCALE_SMOOTH));
        marked = false;

        this.setMinimumSize(new java.awt.Dimension(80, 110));
        this.setPreferredSize(new java.awt.Dimension(80, 110));
    }

    /**
     * Nastaví grafický stav karty, podle hodnot karty.
     */
    public void setStateOfCard() {
        if (card.isTurnedFaceUp()) {
            this.setIcon(front);
            if (marked)
                this.setBorder(BorderFactory.createLineBorder(Color.blue, 1, true));
            else this.setBorder(null);
        }
        else this.setIcon(back);
    }

    /**
     * Nastaví zda je karta označená nebo není.
     *
     * @param flag Nabíva hodnot true/false (označená/neoznačená).
     */
    public void setMark(boolean flag) {
        marked = flag;
    }

    /**
     * Změna pozece karty.
     *
     * @param point Bod nové pozice.
     */
    public void changePosition(Point point) {
        Position position = new Position((int)point.getX(), (int)point.getY());
        card.setPosition(position);
        Position p = card.getPosition();
        this.setBounds(p.getX(), p.getY(), 80, 110);
    }

    /**
     * Změna pozice karty podle osy Y.
     *
     * @param y Nová pozice na ose Y.
     */
    public void changePosition(int y) {
        Position position = new Position(0, y);
        card.setPosition(position);
        this.setBounds(position.getX(), position.getY(), 80, 110);
    }

    /**
     * Vrátí aktuální pozici karty.
     *
     * @return Bod pozice.
     */
    public Point getPosition() {
        return new Point(card.getPosition().getX(), card.getPosition().getY());
    }

    /**
     * @return Vrátí kartu.
     */
    public Card getCard() {
        return card;
    }
}
