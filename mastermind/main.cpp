/* Mastermind
 *
 * Kuvaus:
 *   Ohjelma toteuttaa Mastermind-pelin. Pelissä annetaan tai arvotaan
 * ensin salainen neljän värin sarja. Sama väri voi esiintyä sarjassa
 * useita kertoja, mutta mikään neljästä positiosta ei voi olla tyhjä.
 * Käyttäjä yrittää arvata, mitkä värit esiintyvät sarjassa ja missä
 * järjestyksessä. Tätä varten käyttäjä antaa oman neljän värin sarjansa,
 * ja ohjelma ilmoittaa, kuinka monta väriarvausta meni täysin oikein
 * (oikea väri oikeassa positiossa) sekä kuinka monta arvausta meni
 * pelkästään värin osalta oikein (oikea väri väärässä positiossa).
 * Tämän jälkeen käyttäjä voi tehdä uuden arvauksen jne.
 *   Aluksi käyttäjältä kysytään, täytetäänkö peliruudukko satunnaisesti
 * arvottavilla väreillä vai käyttäjän luettelemilla väreillä.
 * (Jälkimmäinen tapa ei ole kovin järkevä, mutta se on hyödyllinen
 * testauksen kannalta.) Ensimmäisessä vaihtoehdossa käyttäjältä kysytään
 * satunnaislukugeneraattorin siemenlukua ja jälkimmäisessä häntä
 * pyydetään syöttämään neljä väriä (värien alkukirjaimet eli neljän
 * merkin mittainen merkkijono).
 *   Joka kierroksella käyttäjältä kysytään uutta arvausta. Peli päättyy
 * pelaajan voittoon, jos arvaus menee jokaisen värin kohdalta täysin
 * oikein. Peli päättyy pelaajan häviöön, jos hän ei ole arvannut oikein
 * maksimimäärällä (10) arvauskertoja.
 *   Ohjelma tarkistaa, että annetut värit kuuluvat sallittujen värien
 * joukkoon. Pelin päättyessä kerrotaan, voittiko vai hävisikö pelaaja.
 *
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 *
 * */
#include "color_series.hh"

#include <random>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

//vakio vektori jossa kaikki sallitut värit
const vector<char> ACCEPTABLE_COLORS = {'B','R','Y','G','O','V'};

void fill_vector(vector<char>& secretColors,
                 const int seed);

//vakio maksimi kierrosten määrä
const unsigned int GUESS_MAX = 10;

//värien määrä
const unsigned int SIZE = 4;

// Length of the suffix part when printing a row.
// The suffix consists of three vertical lines ('|') and two digits, where one
// tells the amount of totally correct colors and the other tells the amount
// of correct colors in incorrect positions.
const unsigned int SUFFIX_LENGTH_IN_PRINT = 5;

// Text printed at the beginning of the program
const string INFO_TEXT = "Colors in use: \
B = Blue, R = Red, Y = Yellow, G = Green, O = Orange, V = Violet";

//parametrina vektori mikä täytetään sekä siemenluku
//funktio täyttää vektorin satunnaisesti sallituista väreistä jotka ovat
//ACCEPTABLE_COLORS vektorissa
//funktio käyttää default_random_engineä
void fill_vector(vector<char>& secretColors,
                 const int seed)
{
    int first_color = 0;
    int last_color = 5;

    default_random_engine gen(seed);
    uniform_int_distribution<int> distr(first_color,last_color);

        for(int i = 0; i<static_cast<int>(SIZE);i++)
        {
            secretColors.push_back(ACCEPTABLE_COLORS.at(distr(gen)));
        }
}

//apufunktio mikä tarkastaa, että vektoriin voidaan tallettaa
//vain sallittuja värejä jotka ovat ACCEPTABLE_COLORS vektorissa
//parametrina merkkijono jossa värit sijaitsevat
//palauttaa false arvon jos löydetään väri mikä ei ole sallittu muuten true
bool is_acceptable_color(string& color_str)
{
    string to_upper_str = "";
    for(char index : color_str)
    {
        to_upper_str += toupper(index);
    }
    for(char color_char : to_upper_str)
    {

        if(!(find(ACCEPTABLE_COLORS.begin(),
                  ACCEPTABLE_COLORS.end(),
                  color_char)
             != ACCEPTABLE_COLORS.end()))
        {
            return false;
            break;
        }
    }
    return true;
}

//tarkastetaan onko syöte sallittu
//parametrina käyttäjän syöttämä merkkijono
bool is_correct_input(string& input_str)
{
    if(!(input_str.size() == SIZE))
    {
        cout<<"Wrong size"<<endl;
        return false;
    }
    if(!(is_acceptable_color(input_str)))
    {
        cout<<"Unknown color"<<endl;
        return false;
    }
    return true;
}

//parametrina vektori mikä täytetään sallituilla väreillä
//funktiossa voidaan täyttää vektori satunnaisilla väreillä
//tai käyttäjän syöttämillä väreillä
//Käyttäjän syötteet tarkastetaan niin, että vääriä värejä ei voi syöttää
void get_input(vector<char>& secretColors)
{
    cout << "Enter an input way (R = random, L = list): ";
    string input_str = "";
    cin >> input_str;
    if(input_str == "R" or input_str == "r")
    {
        cout << "Enter a seed value: ";
        int seed = 0;
        cin >> seed;

        fill_vector(secretColors,seed);

    }
    else if(input_str == "L" or input_str == "l")
    {
        bool accepted = false;
        while(not accepted)
        {
            cout << "Enter four colors (four letters without spaces): ";
            string colors = "";
            cin >> colors;
            if(!(colors.size() == SIZE))
            {
                cout<<"Wrong size"<<endl;
            }
            else if(!(is_acceptable_color(colors)))
            {
                cout<<"Unknown color"<<endl;
            }
            else
            {
                accepted = true;
                for(char ch_iter : colors)
                {
                    secretColors.push_back(toupper(ch_iter));
                }
            }
        }
    }
    else
    {
        cout << "Bad input" << endl;
        get_input(secretColors);
    }
}

//printtaa tulostusta varten == merkkijonot
void print_line_with_char(char c, unsigned int line_length)
{
    for(unsigned int i = 0; i < line_length; ++i)
    {
        cout << c;
    }
    cout << endl;
}

//parametrina vektori johon värisarja oliot talletettuna
//funktio käy läpi vektorin jokaisen alkion ja kutsuu olion print metodia
void print_all(const vector<Color_series>& color_object_vec)
{
    print_line_with_char('=', 2 * (SIZE + SUFFIX_LENGTH_IN_PRINT) + 1);
    for(auto &&object_index : color_object_vec)
    {
        object_index.print();
    }
    print_line_with_char('=', 2 * (SIZE + SUFFIX_LENGTH_IN_PRINT) + 1);
}

//parametreina pelaajan tekemien oikeiden arvausten määrä sekä pelattujen
//kierrosten määrä
//funktio tarkastaa onko peli ohi ja jos on niin tulostaa viestin käyttäjälle
//ja palauttaa totuusarvon true
bool is_game_over(int correct_guesses, int round_index)
{
    if(correct_guesses == 4)
    {
        cout<<"You won: Congratulations!"<<endl;
        return true;
    }

    if(round_index == GUESS_MAX)
    {
        cout<<"You lost: Maximum number of guesses done"<<endl;
        return true;
    }
    return false;
}

int main()
{
    bool game_over = false;
    unsigned int round_index = 0;
    int correct_guesses = 0;
    cout << INFO_TEXT << endl;
    print_line_with_char('*', INFO_TEXT.size());

    vector<char> secretColorsV = {};
    vector<Color_series> color_object_vec = {};

    get_input(secretColorsV);
    Color_series game_color(secretColorsV);

    //peli alkaa
    while(!(game_over))
    {

        string input_str = "";
        round_index++;
        Color_series player_colors;

        //syötteen tarkistus
        while(true)
        {
            cout<<"ROW> ";
            cin>>input_str;
            if(input_str == "q" or input_str == "Q")
            {
                return EXIT_SUCCESS;
            }
            if(is_correct_input(input_str))
            {
                break;
            }
        }

        //pelaajaolion vektorin luonti merkkijonosta
        player_colors.set_color_vec(input_str);

        //oikeiden arvausten tarkastaminen
        player_colors.check_right_guesses(game_color);

        //oilion syöttäminen vektoriin
        color_object_vec.push_back(player_colors);

        //tulostetaan lauta
        print_all(color_object_vec);

        //tarkastetaan saatiinko oikea määrä oikeita arvauksia
        correct_guesses = player_colors.get_right_guesses();

        //tarkastetaan onko peli ohi
        game_over = is_game_over(correct_guesses, round_index);


    }
    return 0;
}

