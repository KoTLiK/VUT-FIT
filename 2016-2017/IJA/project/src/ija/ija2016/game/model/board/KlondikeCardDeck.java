/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.board;

import ija.ija2016.game.model.cards.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Třída, která implementuje rozhrání {@link ija.ija2016.game.model.cards.CardDeck}.
 * Jedná se o základní balíček hry. Karty lze vkládat a odebírat pouze z vrcholu balíčku.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class KlondikeCardDeck implements CardDeck {
    protected int id;
    protected int deck_size;
    protected List<Card> deck;

    public KlondikeCardDeck(){}

    /**
     * Vytvoří a inicializuje balíček karet.
     *
     * @param size Velikost balíčku karet.
     */
    public KlondikeCardDeck(int size) {
        this.deck_size = size;
        this.deck = new ArrayList<>(size);
    }

    /**
     * Tovární metoda, která vytvoří balík 52 karet, 13 karet (hodnoty 1 až 13) pro každou barvu.
     *
     * @return Balíček karet.
     */
    public static CardDeck standardKlondikeCardDeck() {
        KlondikeCardDeck cards = new KlondikeCardDeck(52);
        for (int i = 1; i <= 13; i++) {
            cards.deck.add(new KlondikeCard(Card.Color.CLUBS, i));
            cards.deck.add(new KlondikeCard(Card.Color.DIAMONDS, i));
            cards.deck.add(new KlondikeCard(Card.Color.HEARTS, i));
            cards.deck.add(new KlondikeCard(Card.Color.SPADES, i));
        }
        return cards;
    }

    /**
     * Vrátí kartu z vrcholu zásobníku (karta zůstává na zásobníku).
     * Pokud je balíček prázdný, vrací null.
     *
     * @return Karta z vrcholu balíčku.
     */
    @Override
    public Card get() {
        if (this.isEmpty()) return null;
        return this.deck.get(this.size()-1);
    }

    /**
     * Vrátí kartu na uvedenem indexu.
     * Spodní karta je na indexu 0, vrchol je na indexu size()-1.
     * Pokud je balíček prázdný, nebo index mimo rozsah, vrací null.
     *
     * @param index Pozice karty v balíčku.
     * @return Karta z balíčku podle indexu.
     */
    @Override
    public Card get(int index) {
        if (index < this.size() && index >= 0)
            return this.deck.get(index);
        else return null;
    }

    /**
     * Test, zda je balíček karet prázdný.
     *
     * @return Vrací true, pokud je balíček prázdný.
     */
    @Override
    public boolean isEmpty() {
        return this.deck_size == 0;
    }

    /**
     * Odebere kartu z vrcholu balíčku.
     * Pokud je balíček prázdný, vrací null.
     *
     * @return Karta z vrcholu balíčku.
     */
    @Override
    public Card pop() {
        if (this.isEmpty()) return null;
        else return this.deck.remove(--this.deck_size);
    }

    /**
     * Vloží kartu na vrchol balíčku.
     *
     * @param card Vkládaná karta.
     * @return Úspěšnost akce.
     */
    @Override
    public boolean put(Card card) {
        return this.deck.add(card) && (++this.deck_size != 0);
    }

    /**
     * @return Aktuální počet karet v balíčku.
     */
    @Override
    public int size() {
        return this.deck_size;
    }

    /**
     * Zamíchá karty v balíčku.
     */
    @Override
    public void shuffle() {
        Collections.shuffle(this.deck);
    }

    /**
     * Nastaví ID balíčku.
     *
     * @param id ID balíčku.
     */
    @Override
    public void setID(int id) {
        this.id = id;
    }

    /**
     * @return Vrátí ID balíčku.
     */
    @Override
    public int getID() {
        return this.id;
    }
}
