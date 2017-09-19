/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.cmd;

import ija.ija2016.game.model.Game;

/**
 * Třída reprezentující příkaz {@link ija.ija2016.game.model.Game.Move#tableauToFoundation(int, int, int)}.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 * @see ija.ija2016.game.model.Game.Move
 */
public class tableauToFoundation implements Commander.Command {
    private final Game game;
    private final int c;
    private final int src;
    private final int dest;
    private int old;

    /**
     * Inicializace příkazu.
     *
     * @param game Instance aktuální hry.
     * @param c    Index karty na pracovním balíku.
     * @param src  Zdroj pracovního balíku.
     * @param dest Destinace cílového balíku.
     */
    public tableauToFoundation(Game game, int c, int src, int dest) {
        this.game = game;
        this.c = c;
        this.src = src;
        this.dest = dest;
    }

    /**
     * Vykonání příkazu.
     * Ohodnocení příkazu +10 skóre.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#tableauToFoundation(int, int, int)
     */
    @Override
    public boolean execute() {
        old = game.score;
        if (game.move.tableauToFoundation(c, src, dest)) {
            game.score += 10;
            return true;
        } else return false;
    }

    /**
     * Navrátení se do stavu před spuštením metody {@link tableauToFoundation#execute()}.
     *
     * @return Úspěšnosť akce.
     * @see ija.ija2016.game.model.Game.Move#tableauToFoundationReverse(int, int)
     */
    @Override
    public boolean undo() {
        game.score = old;
        return game.move.tableauToFoundationReverse(src, dest);
    }
}
