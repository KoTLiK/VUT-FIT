/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.factory;

import ija.ija2016.game.model.cards.*;

/**
 * Abstraktní třída reprezentující abstraktní továrnu.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public abstract class ASolitaireFactory implements java.io.Serializable {
    public ASolitaireFactory(){}

    /**
     * Vytvoří objekt reprezentující kartu.
     *
     * @param color Barva karty.
     * @param value Hodnota karty v rozsahu 1 až 13.
     * @return Objekt karty. Pokud je některý z parametrů neplatný (objekt nelze vytvořit), vrací null.
     */
    public abstract Card createCard(Card.Color color, int value);

    /**
     * Vytváří objekt reprezentující balíček karet.
     *
     * @return Balíček karet.
     */
    public abstract CardDeck createCardDeck();

    /**
     * Vytváří objekt reprezentující cílový balíček.
     *
     * @return Cílový balíček.
     */
    public abstract CardDeck createTargetPack();

    /**
     * Vytváří objekt reprezentující pracovní pole pro karty.
     *
     * @param index Pozice balíčku a úvodní počet karet, které jsou otočeny FaceDown.
     * @param deck  Zamíchaný balíček karet, zdroj karet pre pracovní balíček.
     * @return Pracovní pole.
     */
    public abstract CardStack createWorkingPack(int index, CardDeck deck);
}
