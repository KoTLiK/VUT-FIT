/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.cmd;

import java.util.ArrayList;
import java.util.List;

/**
 * Třída reprezentující návrhový vzor Command.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class Commander {
    /**
     * Rozhrání reprezentující příkaz.
     */
    public interface Command {
        /**
         * Vykonání příkazu.
         *
         * @return Úspěšnost akce.
         */
        boolean execute();

        /**
         * Navrátení se do stavu před spuštením metody {@link Command#execute()}.
         * Inverzní k {@link Command#execute()}.
         *
         * @return Úspěšnost akce.
         */
        boolean undo();
    }

    /**
     * Třída, která si uchováva vykonávané příkazy.
     */
    public static class Invoker {
        private List<Command> commands = new ArrayList<>();

        /**
         * Provede vykonání příkazu.
         * V případe úspěchu je příkaz uchován, pro případné undo operace.
         *
         * @param cmd Instance příkazu.
         */
        public void execute(Command cmd) {
            if (cmd.execute())
                commands.add(0, cmd);
        }

        /**
         * Navrátení se do stavu před spuštením metody execute(Command).
         */
        public void undo() {
            if (commands.size() > 0)
                commands.remove(0).undo();
        }

        /**
         * Provede vykonání příkazu.
         * V případe úspěchu je příkaz uchován, pro případné undo operace.
         *
         * @param cmd Instance příkazu.
         * @return Výsledek vykonávaní příkazu.
         */
        public boolean add(Command cmd) {
            if (cmd.execute()) {
                commands.add(0, cmd);
                return true;
            } else return false;
        }
    }
}
