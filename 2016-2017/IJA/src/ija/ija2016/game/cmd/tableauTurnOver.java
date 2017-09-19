/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.cmd;

import ija.ija2016.game.model.Game;

/**
 * Třída reprezentující příkaz {@link ija.ija2016.game.model.Game.Move#tableauTurnOver(int)}.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 * @see ija.ija2016.game.model.Game.Move
 */
public class tableauTurnOver implements Commander.Command {
    private final Game game;
    private final int dest;
    private int old;

    /**
     * Inicializace příkazu.
     *
     * @param game Instance aktuální hry.
     * @param dest Destinace pracovního balíku.
     */
    public tableauTurnOver(Game game, int dest) {
        this.game = game;
        this.dest = dest;
    }

    /**
     * Vykonání příkazu.
     * Ohodnocení příkazu +5 skóre.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#tableauTurnOver(int)
     */
    @Override
    public boolean execute() {
        old = game.score;
        if (game.move.tableauTurnOver(dest)) {
            game.score += 5;
            return true;
        } else return false;
    }

    /**
     * Navrátení se do stavu před spuštením metody {@link tableauTurnOver#execute()}.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#tableauTurnOverReverse(int)
     */
    @Override
    public boolean undo() {
        game.score = old;
        return game.move.tableauTurnOverReverse(dest);
    }
}
