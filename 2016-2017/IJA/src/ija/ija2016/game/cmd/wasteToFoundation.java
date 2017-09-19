/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.cmd;

import ija.ija2016.game.model.Game;

/**
 * Třída reprezentující příkaz {@link ija.ija2016.game.model.Game.Move#wasteToFoundation(int)}.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 * @see ija.ija2016.game.model.Game.Move
 */
public class wasteToFoundation implements Commander.Command {
    private final Game game;
    private final int dest;
    private int old;

    /**
     * Inicializace příkazu.
     *
     * @param game Instance aktuální hry.
     * @param dest Destinace cílového balíku.
     */
    public wasteToFoundation(Game game, int dest) {
        this.game = game;
        this.dest = dest;
    }

    /**
     * Vykonání příkazu.
     * Ohodnocení příkazu +10 skóre.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#wasteToFoundation(int)
     */
    @Override
    public boolean execute() {
        old = game.score;
        if (game.move.wasteToFoundation(dest)) {
            game.score += 10;
            return true;
        } else return false;
    }

    /**
     * Navrátení se do stavu před spuštením metody {@link wasteToFoundation#execute()}.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#wasteToFoundationReverse(int)
     */
    @Override
    public boolean undo() {
        game.score = old;
        return game.move.wasteToFoundationReverse(dest);
    }
}
