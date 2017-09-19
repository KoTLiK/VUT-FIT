/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ija.ija2016.game.model.cards;

/**
 * Rozhraní reprezentující pracovní balíček karet (zásobník karet).
 * Karty v zásobníku nelze prohazovat. Karty lze vkládat a vybírat z vrcholu hromadně.
 *
 * @author Milan Augustín (xaugus09)
 * @author Vojtěch Kozel (xkozel18)
 */
public interface CardStack extends CardDeck {
    /**
     * Metoda odebere ze zásobníku sekvenci karet od zadané karty až po vrchol zásobníku.
     * Pokud je hledaná karta na vrcholu, bude v sekvenci pouze jedna karta.
     *
     * @param card Hledaná karta.
     * @return Zásobník karet obsahující odebranou sekvenci. Pokud hledaná karta v zásobníku není, vrací null.
     */
    CardStack pop(Card card);

    /**
     * Vloží kartu na vrchol zásobníku, nehledě na pravidlá hry, kvůli implementaci operace undo.
     *
     * @param card Karta, která má být vložena.
     * @return Úspěšnost akce.
     */
    boolean add(Card card);

    /**
     * Vloží karty na vrchol zásobníku ve stejném pořadí, nehledě na pravidlá hry, kvůli implementaci operace undo.
     *
     * @param stack Zásobník karet, který má být vložen.
     * @return Úspěšnost akce.
     */
    boolean add(CardStack stack);

    /**
     * Vloží karty ze zásobníku stack na vrchol zásobníku.
     * Karty vkládá ve stejném pořadí, v jakém jsou uvedeny v zásobníku stack.
     *
     * @param stack Zásobník vkládaných karet.
     * @return Úspěšnost akce.
     */
    boolean put(CardStack stack);
}
