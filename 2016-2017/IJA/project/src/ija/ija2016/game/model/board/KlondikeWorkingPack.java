/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.board;

import ija.ija2016.game.model.cards.*;
import java.util.ArrayList;

/**
 * Třída, která implementuje rozhrání {@link ija.ija2016.game.model.cards.CardStack} a rozšiřuje třídu {@link ija.ija2016.game.model.board.KlondikeCardDeck}.
 * Jedná se o pracovní balíček hry (zásobník karet). Karty v zásobníku nelze prohazovat. Karty lze vkládat a vybírat z vrcholu hromadně.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class KlondikeWorkingPack extends KlondikeCardDeck implements CardStack {
    /**
     * Vytvoří a inicializuje prázdní pracovní balíček karet.
     */
    public KlondikeWorkingPack(){
        this.deck_size = 0;
        this.deck = new ArrayList<>(0);
    }

    /**
     * Vytvoří a inicializuje pracovní balíček karet.
     * Vytvořen balíček obsahuje index+1 karet, kde vrchní karta je otočena FaceUp.
     *
     * @param index Pozice balíčku a úvodní počet karet, které jsou otočeny FaceDown.
     * @param deck  Zamíchaný balíček karet, zdroj karet pre pracovní balíček.
     */
    public KlondikeWorkingPack(int index, CardDeck deck){
        this.deck_size = index + 1;
        this.deck = new ArrayList<>(index + 1);

        for (int i = 0; i <= index; i++) {
            this.deck.add(deck.pop());
        }

        this.get().turnFaceUp();
    }

    /**
     * Metoda odebere ze zásobníku sekvenci karet od zadané karty až po vrchol zásobníku.
     * Pokud je hledaná karta na vrcholu, bude v sekvenci pouze jedna karta.
     *
     * @param card Hledaná karta.
     * @return Zásobník karet obsahující odebranou sekvenci. Pokud hledaná karta v zásobníku není, vrací null.
     */
    @Override
    public CardStack pop(Card card) {
        KlondikeWorkingPack pack = new KlondikeWorkingPack();
        int index = this.deck.indexOf(card);
        if (index == -1 || !this.deck.get(index).isTurnedFaceUp()) return null;
        while (!this.isEmpty() && index < this.size()) {
            pack.deck.add(this.deck.remove(index));
            pack.deck_size++;
            this.deck_size--;
        }
        return pack;
    }

    /**
     * Vloží kartu na vrchol zásobníku, nehledě na pravidlá hry, kvůli implementaci operace undo.
     *
     * @param card Karta, která má být vložena.
     * @return Úspěšnost akce.
     */
    @Override
    public boolean add(Card card) {
        return super.put(card);
    }

    /**
     * Vloží kartu na vrchol pracovního balíčku, podle pravidel hry.
     *
     * @param card Vkládaná karta.
     * @return Úspěšnost akce.
     */
    @Override
    public boolean put(Card card) {
        if (this.isEmpty()) return card.value() == 13 && super.put(card);
        else return this.get().isTurnedFaceUp() && this.get().compareValue(card) == 1 && !this.get().similarColorTo(card) && super.put(card);
    }

    /**
     * Vloží karty na vrchol zásobníku ve stejném pořadí, nehledě na pravidlá hry, kvůli implementaci operace undo.
     *
     * @param stack Zásobník karet, který má být vložen.
     * @return Úspěšnost akce.
     */
    @Override
    public boolean add(CardStack stack) {
        KlondikeWorkingPack s = (KlondikeWorkingPack) stack;
        while (!s.isEmpty()) {
            super.put(s.deck.remove(0));
            s.deck_size--;
        }
        return true;
    }

    /**
     * Vloží karty ze zásobníku stack na vrchol zásobníku.
     * Karty vkládá ve stejném pořadí, v jakém jsou uvedeny v zásobníku stack.
     *
     * @param stack Zásobník vkládaných karet.
     * @return Úspěšnost akce.
     */
    @Override
    public boolean put(CardStack stack) {
       KlondikeWorkingPack s = (KlondikeWorkingPack) stack;
        if (this.isEmpty()) {
            if (stack.get(0).value() == 13) {
                while (!stack.isEmpty()) {
                    super.put(s.deck.remove(0));
                    s.deck_size--;
                }
                return true;
            } else return false;
        } else if (this.get().compareValue(stack.get(0)) == 1 && !this.get(this.size()-1).similarColorTo(stack.get(0))) {
            while (!stack.isEmpty()) {
                super.put(s.deck.remove(0));
                s.deck_size--;
            }
            return true;
        } else return false;
    }
}
