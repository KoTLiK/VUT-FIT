/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.cmd;

import ija.ija2016.game.model.Game;

/**
 * Třída reprezentující příkaz {@link ija.ija2016.game.model.Game.Move#nextWaste()}.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 * @see ija.ija2016.game.model.Game.Move
 */
public class nextWaste implements Commander.Command {
    private final Game game;

    /**
     * Inicializace příkazu.
     *
     * @param game Instance aktuální hry.
     */
    public nextWaste(Game game) {
        this.game = game;
    }

    /**
     * Vykonání příkazu.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#nextWaste()
     */
    @Override
    public boolean execute() {
        return game.move.nextWaste();
    }

    /**
     * Navrátení se do stavu před spuštením metody {@link nextWaste#execute()}.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#nextWasteReverse()
     */
    @Override
    public boolean undo() {
        return game.move.nextWasteReverse();
    }
}
