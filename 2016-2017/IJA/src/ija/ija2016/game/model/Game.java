/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model;

import ija.ija2016.game.model.board.KlondikeCardDeck;
import ija.ija2016.game.model.cards.*;
import ija.ija2016.game.model.factory.*;

/**
 * Třída reprezentující logiku hry.
 * Vytvoří instanci celé hry, všechny balíčky a skóre, ovládá celou hru.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class Game implements java.io.Serializable {
    public final Move move = new Move();
    private final CardDeck deck0;
    private final CardDeck deck1;
    private final CardDeck[] targets = new CardDeck[4];
    private final CardStack[] working = new CardStack[7];
    private javax.swing.JLabel scoreValue;
    public int score;

    /**
     * Vytvoří kompletní instanci hry, zamíchá karty a rozdá je.
     */
    public Game() {
        ASolitaireFactory factory = new KlondikeFactory();
        score = 0;
        deck0 = factory.createCardDeck();
        deck0.shuffle();
        deck0.setID(0);
        deck1 = new KlondikeCardDeck(0);
        deck1.setID(1);

        for (int i = 0; i < 4; i++) {
            targets[i] = factory.createTargetPack();
            targets[i].setID(i + 2);
        }

        for (int i = 0; i < 7; i++) {
            working[i] = factory.createWorkingPack(i, deck0);
            working[i].setID(i + 6);
        }
    }

    /**
     * Propojení GUI a skóre.
     *
     * @param label Vstupní textový JLabel.
     */
    public void setScoreLabel(javax.swing.JLabel label) {
        scoreValue = label;
    }

    /**
     * Aktualizuje stav hry.
     */
    public void updateScore() {
        scoreValue.setText(""+score);
    }

    /**
     * @return Vrátí hlavní balíček.
     */
    public CardDeck getDeck0() {
        return deck0;
    }

    /**
     * @return Vrátí odhazovací balíček.
     */
    public CardDeck getDeck1() {
        return deck1;
    }

    /**
     * @return Vrátí pole všech cílových balíčků.
     */
    public CardDeck[] getTargets() {
        return targets;
    }

    /**
     * @return Vrátí pole všech pracovních balíčků.
     */
    public CardStack[] getWorking() {
        return working;
    }

    /**
     * Třída reprezentující všechny tahy hry a jim inverzní tahy.
     */
    public class Move implements java.io.Serializable {
        /**
         * Otočí další kartu z hlavního balíku na odhadzovací balíček.
         *
         * @return Úspěšnost akce.
         */
        public boolean nextWaste() {
            if (deck0.isEmpty())
                return false;
            else {
                Card c = deck0.pop();
                c.turnFaceUp();
                return deck1.put(c);
            }
        }

        /**
         * Inverzní k {@link Move#nextWaste()}.
         *
         * @return Úspěšnost akce.
         */
        public boolean nextWasteReverse() {
            if (deck1.isEmpty())
                return false;
            else {
                Card c = deck1.pop();
                c.turnFaceDown();
                return deck0.put(c);
            }
        }


        /**
         * Všechny karty z odhazovacího balíku sa přesunou do hlavního balíku.
         *
         * @return Úspěšnost akce.
         */
        public boolean recycleWaste() {
            if (deck1.isEmpty()) return true;
            else {
                while (!deck1.isEmpty()) {
                    Card c = deck1.pop();
                    c.turnFaceDown();
                    if (!deck0.put(c))
                        return false;
                }
            }
            return true;
        }

        /**
         * Inverzní k {@link Move#recycleWaste()}.
         *
         * @return Úspěšnost akce.
         */
        public boolean recycleWasteReverse() {
            if (deck0.isEmpty()) return true;
            else {
                while (!deck0.isEmpty()) {
                    Card c = deck0.pop();
                    c.turnFaceUp();
                    if (!deck1.put(c))
                        return false;
                }
            }
            return true;
        }


        /**
         * Přesun karty z odhazovacího balíku na pracovní balíček.
         *
         * @param dest Destinace pracovního balíku.
         * @return Úspěšnost akce.
         */
        public boolean wasteToTableau(int dest) {
            Card c = deck1.pop();
            if (c == null) return false;
            if (working[dest].put(c))
                return true;
            else {
                deck1.put(c);
                return false;
            }
        }

        /**
         * Inverzní k {@link Move#wasteToTableau(int)}.
         *
         * @param dest Destinace (Inverze = zdroj) pracovního balíku.
         * @return Úspěšnost akce.
         */
        public boolean wasteToTableauReverse(int dest) {
            Card c = working[dest].pop();
            if (c == null) return false;
            if (deck1.put(c))
                return true;
            else {
                working[dest].add(c);
                return false;
            }
        }


        /**
         * Přesun karty z odhazovacího balíku na cílový balíček.
         *
         * @param dest Destinace cílového balíku.
         * @return Úspěšnost akce.
         */
        public boolean wasteToFoundation(int dest) {
            Card c = deck1.pop();
            if (c == null) return false;
            if (targets[dest].put(c))
                return true;
            else {
                deck1.put(c);
                return false;
            }
        }

        /**
         * Inverzní k {@link Move#wasteToFoundation(int)}.
         *
         * @param dest Destinace (Inverze = zdroj) cílového balíku.
         * @return Úspěšnost akce.
         */
        public boolean wasteToFoundationReverse(int dest) {
            Card c = targets[dest].pop();
            if (c == null) return false;
            if (deck1.put(c))
                return true;
            else {
                targets[dest].put(c);
                return false;
            }
        }


        /**
         * Otočení vrchní karty pracovního balíku.
         *
         * @param dest Destinace pracovního balíku.
         * @return Úspěšnost akce.
         */
        public boolean tableauTurnOver(int dest) {
            return working[dest].get().turnFaceUp();
        }

        /**
         * Inverzní k {@link Move#tableauTurnOver(int)}.
         *
         * @param dest Destinace (Inverze = zdroj) pracovního balíku.
         * @return Úspěšnost akce.
         */
        public boolean tableauTurnOverReverse(int dest) {
            return working[dest].get().turnFaceDown();
        }


        /**
         * Přesun karty z pracovního balíku na cílový balíček.
         *
         * @param c    Index karty na pracovním balíku.
         * @param src  Zdroj pracovního balíku.
         * @param dest Destinace cílového balíku.
         * @return Úspěšnost akce.
         */
        public boolean tableauToFoundation(int c, int src, int dest) {
            CardStack s = working[src].pop(working[src].get(c));
            if (s == null) return false;
            if (s.size() == 1) {
                Card card = s.get();
                if (targets[dest].put(card))
                    return true;
                else {
                    working[src].add(card);
                    return false;
                }
            } else {
                working[src].add(s);
                return false;
            }
        }

        /**
         * Inverzní k {@link Move#tableauToFoundation(int, int, int)}.
         *
         * @param src  Zdroj (Inverze = destinace) pracovního balíku.
         * @param dest Destinace (Inverze = zdroj) cílového balíku.
         * @return Úspěšnost akce.
         */
        public boolean tableauToFoundationReverse(int src, int dest) {
            Card c = targets[dest].pop();
            if (c == null) return false;
            if (working[src].add(c))
                return true;
            else {
                targets[dest].put(c);
                return false;
            }
        }


        /**
         * Přesun karty mezi dvěma balíčky.
         *
         * @param c    Index karty začátku subzásobníku.
         * @param src  Zdroj pracovního balíku.
         * @param dest Destinace pracovního balíku.
         * @return Úspěšnost akce: TRUE - vrátí kartu, jinak vrátí NULL.
         */
        public Card tableauToTableau(int c, int src, int dest) {
            if (src == dest) return null;
            Card card = working[src].get(c);
            CardStack s = working[src].pop(card);
            if (s == null) return null;
            if (working[dest].put(s))
                return card;
            else {
                working[src].add(s);
                return null;
            }
        }

        /**
         * Inverzní k {@link Move#tableauToTableau(int, int, int)}.
         *
         * @param c    Karta, která byla vybrána původní metodou.
         * @param src  Zdroj (Inverze = destinace) pracovního balíku.
         * @param dest Destinace (Inverze = zdroj) pracovního balíku.
         * @return Úspěšnost akce.
         */
        public boolean tableauToTableauReverse(Card c, int src, int dest) {
            if (src == dest) return false;
            CardStack s = working[dest].pop(c);
            if (s == null) return false;
            if (working[src].add(s))
                return true;
            else {
                working[dest].add(s);
                return false;
            }
        }
    }
}
