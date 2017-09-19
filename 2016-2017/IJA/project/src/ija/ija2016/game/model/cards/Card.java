/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.cards;

import ija.ija2016.game.gui.GCard;
import ija.ija2016.game.model.board.Position;

/**
 * Rozhraní reprezentující jednu kartu.
 * Karta obsahuje informaci o své hodnotě (1 až 13) a barvě.
 * Tyto informace jsou nastaveny konstruktorem.
 * Hodnota 1 reprezentuje eso (ace), 11 až 13 postupně kluk (jack), královna (queen) a král (king).
 * Barvu definuje výčtový typ {@link ija.ija2016.game.model.cards.Card.Color}.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public interface Card extends java.io.Serializable {
    /**
     * Výčtový typ reprezentující barvu karty.
     */
    enum Color {
        CLUBS("C"), DIAMONDS("D"), HEARTS("H"), SPADES("S");

        private final String key;

        Color(String s) {
            this.key = s;
        }

        /**
         * Testuje, zda má karta podobnou barvu jako karta zadaná.
         * Podobnou barvou se myslí černá (piky, kříže) a červená (káry a srdce).
         *
         * @param c Karta pro porovnání.
         * @return Informace o shodě barev karet (true/false).
         */
        public boolean similarColorTo(Card.Color c) {
            if (this.equals(CLUBS) || this.equals(SPADES))
                return (c.equals(CLUBS) || c.equals(SPADES));
            else return (c.equals(DIAMONDS) || c.equals(HEARTS));
        }

        @Override
        public String toString() {
            return this.key;
        }
    }

    /**
     * @return Barva karty.
     */
    Card.Color color();

    /**
     * Porovná hodnotu karty se zadanou kartou c.
     * Pokud jsou stejné, vrací 0. Pokud je karta větší než zadaná c, vrací kladný rozdíl hodnot.
     *
     * @param c Karta, s kterou se porovnává.
     * @return Vrací rozdíl hodnot karet.
     */
    int compareValue(Card c);

    /**
     * Testuje, zda je karta otočená lícem nahoru.
     *
     * @return Výsledek testu: true = karta je otočená lícem nahoru.
     */
    boolean isTurnedFaceUp();

    /**
     * @param c Karta pro porovnání.
     * @return Informace o shodě barev karet (true/false).
     * @see ija.ija2016.game.model.cards.Card.Color
     */
    boolean similarColorTo(Card c);

    /**
     * Otočí kartu lícem nahoru.
     *
     * @return Výsledek operace.
     */
    boolean turnFaceUp();

    /**
     * Otočí kartu lícem nadol.
     *
     * @return Výsledek operace.
     */
    boolean turnFaceDown();

    /**
     * @return Hodnota karty.
     */
    int value();
    @Override
    boolean equals(Object obj);
    @Override
    int hashCode();

    /**
     * Nastaví pozici karty.
     *
     * @param p Nová pozice karty.
     */
    void setPosition(Position p);

    /**
     * @return Vrátí aktuální pozici karty.
     */
    Position getPosition();

    /**
     * Nastaví karte grafiku
     *
     * @param g Grafika karty.
     */
    void setGraphics(GCard g);

    /**
     * @return Vrátí grafiku karty.
     */
    GCard getGraphics();
}
