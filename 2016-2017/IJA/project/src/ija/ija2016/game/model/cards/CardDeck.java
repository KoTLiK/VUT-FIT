/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.cards;

/**
 * Rozhraní reprezentující balíček karet.
 * Karty lze vkládat a odebírat pouze z vrcholu balíčku.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public interface CardDeck extends java.io.Serializable {
    /**
     * Vrátí kartu z vrcholu zásobníku (karta zůstává na zásobníku).
     * Pokud je balíček prázdný, vrací null.
     *
     * @return Karta z vrcholu balíčku.
     */
    Card get();

    /**
     * Vrátí kartu na uvedenem indexu.
     * Spodní karta je na indexu 0, vrchol je na indexu size()-1.
     * Pokud je balíček prázdný, nebo index mimo rozsah, vrací null.
     *
     * @param index Pozice karty v balíčku.
     * @return Karta z balíčku podle indexu.
     */
    Card get(int index);

    /**
     * Test, zda je balíček karet prázdný.
     *
     * @return Vrací true, pokud je balíček prázdný.
     */
    boolean isEmpty();

    /**
     * Odebere kartu z vrcholu balíčku.
     * Pokud je balíček prázdný, vrací null.
     *
     * @return Karta z vrcholu balíčku.
     */
    Card pop();

    /**
     * Vloží kartu na vrchol balíčku.
     *
     * @param card Vkládaná karta.
     * @return Úspěšnost akce.
     */
    boolean put(Card card);

    /**
     * @return Aktuální počet karet v balíčku.
     */
    int size();

    /**
     * Zamíchá karty v balíčku.
     */
    void shuffle();

    /**
     * Nastaví ID balíčku.
     *
     * @param id ID balíčku.
     */
    void setID(int id);

    /**
     * @return Vrátí ID balíčku.
     */
    int getID();
}
