/*
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 */
#include "color_series.hh"
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

Color_series::Color_series()
{
}

Color_series::Color_series(const vector<char> &colorseries):
    colorseries_(colorseries)
{
}

//parametrina merkkijono jossa käyttäjän värit
//käydään merkkijonon värit merkit läpi ja lisätään
//väri merkit vektoriin
void Color_series::set_color_vec(string& color_str)
{
    for(char color_ch : color_str)
    {
        colorseries_.push_back(toupper(color_ch));
    }
}

void Color_series::set_right_guesses(int right_guesses)
{
    right_guesses_ = right_guesses;
}

void Color_series::set_almost_right(int almost_right)
{
    almost_right_ = almost_right;
}

int Color_series::get_right_guesses() const
{
    return right_guesses_;
}

//printataan olion tiedot joita ovat
//väri merkit ja oikeat paikan arvaukset
//sekä oikeiden värien arvaukset
void Color_series::print() const
{
    cout<<"| ";
    for(char ch_index : colorseries_)
    {
        cout<<ch_index<<" ";
    }
    cout<<"| "<<right_guesses_<<" | "<<almost_right_<<" |"<<endl;
}

//parametrina olio jossa salattu värimerkkisarja
//tarkastaa kuinka monta oikean paikan arvausta ja kuinka monta värien arvausta
void Color_series::check_right_guesses(Color_series &secret_object)
{
    char color = ' ';
    int v_length = static_cast<int>(secret_object.colorseries_.size());

    vector<char> secret_v = secret_object.colorseries_;

    //apuvektori jota käytetään hyväksi luotaessa joukkoa väreistä
    vector<char> secret_duplicate = colorseries_;
    //joukko vektori joka toimii joukkojen tallennuspaikkana
    //ja jota käytetään hyväksi laskettaessa oikeiden värien arvauksia
    vector<char> set = {};

    for(int i = 0; i< v_length;i++)
    {
        //tarkastetaan oikeiden paikkojen oikeat värit
        color = secret_v.at(i);
        if(secret_object.colorseries_.at(i) ==
                colorseries_.at(i))
        {
            right_guesses_ += 1;
        }
        //jos ei samalla paikalla ole samaa väriä niin katsotaan onko annetussa
        //vektorissa oikeita värejä
        else
        {
            for(int i = 0; i<v_length;i++)
            {
                if(color == secret_duplicate.at(i))
                {
                    set.push_back(color);
                    secret_duplicate.at(i) = 'X';
                    break;
                }
            }
        }
    }
    almost_right_ = static_cast<int>(set.size());
}
