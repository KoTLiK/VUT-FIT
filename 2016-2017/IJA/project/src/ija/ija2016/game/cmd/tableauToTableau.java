/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.cmd;

import ija.ija2016.game.model.Game;
import ija.ija2016.game.model.cards.Card;

/**
 * Třída reprezentující příkaz {@link ija.ija2016.game.model.Game.Move#tableauToTableau(int, int, int)}.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 * @see ija.ija2016.game.model.Game.Move
 */
public class tableauToTableau implements Commander.Command {
    private final Game game;
    private final int c;
    private final int src;
    private final int dest;
    private Card card;

    /**
     * Inicializace příkazu.
     *
     * @param game Instance aktuální hry.
     * @param c    Index karty začátku subzásobníku.
     * @param src  Zdroj pracovního balíku.
     * @param dest Destinace pracovního balíku.
     */
    public tableauToTableau(Game game, int c, int src, int dest) {
        this.game = game;
        this.c = c;
        this.src = src;
        this.dest = dest;
    }

    /**
     * Vykonání příkazu.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#tableauToTableau(int, int, int)
     */
    @Override
    public boolean execute() {
        card = game.move.tableauToTableau(c, src, dest);
        return (card != null);
    }

    /**
     * Navrátení se do stavu před spuštením metody {@link tableauToTableau#execute()}.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#tableauToTableauReverse(Card, int, int)
     */
    @Override
    public boolean undo() {
        return game.move.tableauToTableauReverse(card, src, dest);
    }
}
