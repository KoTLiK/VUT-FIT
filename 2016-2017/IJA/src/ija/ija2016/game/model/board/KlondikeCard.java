/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.board;

import ija.ija2016.game.gui.GCard;
import ija.ija2016.game.model.cards.*;

/**
 * Třída, která implementuje rozhrání {@link ija.ija2016.game.model.cards.Card}.
 * Karta obsahuje informaci o své hodnotě (1 až 13) a barvě.
 * Tyto informace jsou nastaveny konstruktorem.
 * Hodnota 1 reprezentuje eso (ace), 11 až 13 postupně kluk (jack), královna (queen) a král (king).
 * Barvu definuje výčtový typ {@link ija.ija2016.game.model.cards.Card.Color}.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class KlondikeCard implements Card {
    private boolean faceUp;
    private final Card.Color col;
    private final int val;
    private transient GCard graphics;
    
    private Position position;

    /**
     * Konstruktor nastavující barvu a hodnotu karty.
     *
     * @param c     Barva karty.
     * @param value Hodnota karty.
     */
    public KlondikeCard(Card.Color c, int value) {
        this.faceUp = false;
        this.col = c;
        this.val = value;
        position = new Position(0, 0);
    }


    /**
     * @return Barva karty.
     */
    @Override
    public Color color() {
        return this.col;
    }

    /**
     * Porovná hodnotu karty se zadanou kartou c.
     * Pokud jsou stejné, vrací 0. Pokud je karta větší než zadaná c, vrací kladný rozdíl hodnot.
     *
     * @param c Karta, s kterou se porovnává.
     * @return Vrací rozdíl hodnot karet.
     */
    @Override
    public int compareValue(Card c) {
        KlondikeCard card = (KlondikeCard) c;
        return this.val - card.val;
    }

    /**
     * Testuje, zda je karta otočená lícem nahoru.
     *
     * @return Výsledek testu: true = karta je otočená lícem nahoru.
     */
    @Override
    public boolean isTurnedFaceUp() {
        return this.faceUp;
    }

    /**
     * @param c Karta pro porovnání.
     * @return Informace o shodě barev karet (true/false).
     * @see Color
     */
    @Override
    public boolean similarColorTo(Card c) {
        KlondikeCard card = (KlondikeCard) c;
        return this.col.similarColorTo(card.col);
    }

    /**
     * Otočí kartu lícem nahoru.
     *
     * @return Výsledek operace.
     */
    @Override
    public boolean turnFaceUp() {
        //return !this.faceUp && (this.faceUp = true);
        return (this.faceUp = true);
    }

    /**
     * Otočí kartu lícem nadol.
     *
     * @return Výsledek operace.
     */
    @Override
    public boolean turnFaceDown() {
        //return this.faceUp && !(this.faceUp = false);
        return !(this.faceUp = false);
    }

    /**
     * @return Hodnota karty.
     */
    @Override
    public int value() {
        return this.val;
    }

    /**
     * Nastaví pozici karty.
     *
     * @param p Nová pozice karty.
     */
    @Override
    public void setPosition(Position p) {
        this.position = p;
    }

    /**
     * @return Vrátí aktuální pozici karty.
     */
    @Override
    public Position getPosition() {
        return this.position;
    }

    /**
     * Nastaví karte grafiku.
     *
     * @param g Grafika karty.
     */
    @Override
    public void setGraphics(GCard g) {
        this.graphics = g;
    }

    /**
     * @return Vrátí grafiku karty.
     */
    @Override
    public GCard getGraphics() {
        return graphics;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) return false;
        if (obj instanceof KlondikeCard) {
            KlondikeCard c = (KlondikeCard) obj;
            if (this.val == c.val && this.col == c.col)
                return true;
        }
        return false;
    }

    @Override
    public int hashCode() {
        return this.val * this.col.hashCode();
    }

    @Override
    public String toString() {
        switch (this.val) {
            case  1: return "A("+ this.col.toString() +")";//+ (this.faceUp?"T":"F");
            case 11: return "J("+ this.col.toString() +")";//+ (this.faceUp?"T":"F");
            case 12: return "Q("+ this.col.toString() +")";//+ (this.faceUp?"T":"F");
            case 13: return "K("+ this.col.toString() +")";//+ (this.faceUp?"T":"F");
            default: return this.val +"("+ this.col.toString() +")";//+ (this.faceUp?"T":"F");
        }
    }
}
