/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.cmd;

import ija.ija2016.game.model.Game;

/**
 * Třída reprezentující příkaz {@link ija.ija2016.game.model.Game.Move#recycleWaste()}.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 * @see ija.ija2016.game.model.Game.Move
 */
public class recycleWaste implements Commander.Command {
    private final Game game;
    private int old;

    /**
     * Inicializace příkazu.
     *
     * @param game Instance aktuální hry.
     */
    public recycleWaste(Game game) {
        this.game = game;
    }

    /**
     * Vykonání příkazu.
     * Ohodnocení příkazu -100 skóre.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#recycleWaste()
     */
    @Override
    public boolean execute() {
        old = game.score;
        if (game.move.recycleWaste()) {
            if ((game.score -= 100) < 0)
                game.score = 0;
            return true;
        } else return false;
    }

    /**
     * Navrátení se do stavu před spuštením metody {@link recycleWaste#execute()}.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#recycleWasteReverse()
     */
    @Override
    public boolean undo() {
        game.score = old;
        return game.move.recycleWasteReverse();
    }
}
