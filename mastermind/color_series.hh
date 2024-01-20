/*
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 */
#ifndef COLOR_SERIES_HH
#define COLOR_SERIES_HH

#include <vector>
#include <iostream>
using namespace std;

class Color_series
{
public:
    Color_series();

    //vektori jossa värit parametrina
    Color_series(const vector<char>& colorseries);

    //string jossa syötetyt värit parametrina
    void set_color_vec(string& color_str);

    //parametrina oikeiden arvausten määrä
    void set_right_guesses(int right_guesses);

    //parametrina oikeiden värien määrä
    void set_almost_right(int almost_right);

    //palauttaa oikeiden arvausten määrän
    int get_right_guesses() const;

    //printataan olion tiedot
    void print() const;

    //parametrina salattu värisarja olio
    void check_right_guesses(Color_series& secret_object);

private:
    //vektori johon olion värit talletetaan
    vector<char> colorseries_ = {};

    //oikeat arvaukset jotka on tehty
    int right_guesses_ = 0;
    int almost_right_ = 0;
};

#endif // COLOR_SERIES_HH
