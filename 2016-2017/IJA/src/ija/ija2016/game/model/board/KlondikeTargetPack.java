/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.board;

import ija.ija2016.game.model.cards.*;
import java.util.ArrayList;

/**
 * Třída, která rozšiřuje třídu {@link ija.ija2016.game.model.board.KlondikeCardDeck}.
 * Jedná se o cílový balíček hry. Karty lze pouze vkládat podle pravidel hry (od 1. po 13. kartu rovnaké farby).
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class KlondikeTargetPack extends KlondikeCardDeck {
    private Card.Color col;

    /**
     * Vytvoří a inicializuje cílový balíček karet.
     */
    public KlondikeTargetPack() {
        this.deck_size = 0;
        this.deck = new ArrayList<>(0);
    }

    /**
     * Vloží kartu na vrchol cílového balíčku.
     * Podle pravidel hry (od 1. po 13. kartu rovnaké farby).
     *
     * @param card Vkládaná karta.
     * @return Úspěšnost akce.
     */
    @Override
    public boolean put(Card card) {
        if (this.isEmpty()) {
            if (card.value() != 1) return false;
            this.col = card.color();
            return super.put(card);
        } else
            return card.value() - 1 == this.deck_size && card.color() == this.col && super.put(card);
    }
}
