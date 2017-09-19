/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.factory;

import ija.ija2016.game.model.board.*;
import static ija.ija2016.game.model.board.KlondikeCardDeck.standardKlondikeCardDeck;
import ija.ija2016.game.model.cards.*;

/**
 * Třída, která rozšiřuje třídu {@link ija.ija2016.game.model.factory.ASolitaireFactory}.
 * Jedná se o Klondike továrnu.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class KlondikeFactory extends ASolitaireFactory {
    public KlondikeFactory(){}

    /**
     * Vytvoří objekt reprezentující kartu.
     *
     * @param color Barva karty.
     * @param value Hodnota karty v rozsahu 1 až 13.
     * @return Objekt karty. Pokud je některý z parametrů neplatný (objekt nelze vytvořit), vrací null.
     */
    @Override
    public Card createCard(Card.Color color, int value) {
        if (value > 0 && value < 14)
            return new KlondikeCard(color, value);
        else return null;
    }

    /**
     * Vytváří objekt reprezentující balíček karet.
     *
     * @return Balíček karet.
     */
    @Override
    public CardDeck createCardDeck() {
        return standardKlondikeCardDeck();
    }

    /**
     * Vytváří objekt reprezentující cílový balíček.
     *
     * @return Cílový balíček.
     */
    @Override
    public CardDeck createTargetPack() {
        return new KlondikeTargetPack();
    }

    /**
     * Vytváří objekt reprezentující pracovní pole pro karty.
     *
     * @param index Pozice balíčku a úvodní počet karet, které jsou otočeny FaceDown.
     * @param deck  Zamíchaný balíček karet, zdroj karet pre pracovní balíček.
     * @return Pracovní pole.
     */
    @Override
    public CardStack createWorkingPack(int index, CardDeck deck) {
        return new KlondikeWorkingPack(index, deck);
    }
}
