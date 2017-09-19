/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game;

import ija.ija2016.game.gui.GamePanel;
import ija.ija2016.Solitaire;
import ija.ija2016.game.model.Game;


/**
 * Třída reprezentující konkrétní jednu hru.
 * Vytváří logiku hry a její GUI.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class SolitaireGame implements Runnable {
    private final Solitaire model;
    private final int id;
    private final Game load;

    /**
     * Konstruktor propojující okno programu a hry.
     *
     * @param model Okno programu (JFrame).
     * @param id    ID hry.
     * @param load  Instance hry. Null v případe nové hry, jinak se jedná o instanci uložené hry.
     */
    public SolitaireGame(Solitaire model, int id, Game load) {
        this.model = model;
        this.id = id;
        this.load = load;
    }

    /**
     * Vytvárí logiku hry nebo ji načítá a spustí její GUI.
     */
    @Override
    public void run() {
        Game game = new Game();

        if (load != null)
            game = load;

        model.addGame(new GamePanel(model, game, id), id);
        model.repaint();
    }
    
}
