/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.board;

/**
 * Třída, reprezentující pozici karty.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public class Position implements java.io.Serializable {
    private int x, y;

    /**
     * Vytvoří a inicializuje pozici.
     *
     * @param p Nová pozice.
     */
    public Position(Position p) {
        this.x = p.x;
        this.y = p.y;
    }

    /**
     * Vytvoří a inicializuje pozici.
     *
     * @param x Souřadnice X.
     * @param y Souřadnice Y.
     */
    public Position(int x, int y) {
        this.x = x;
        this.y = y;
    }

    /**
     * @return Vrátí aktuální souřadnici X.
     */
    public int getX() {
        return x;
    }

    /**
     * @return Vrátí aktuální souřadnici Y.
     */
    public int getY() {
        return y;
    }
}
