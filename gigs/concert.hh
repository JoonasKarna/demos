/*
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 *
 * */

#ifndef CONCERT_HH
#define CONCERT_HH

#include <string>
#include <vector>
class Concert
{
public:
    Concert();
    Concert(const std::string& artist, const std::string& city,
            const std::string& fest_location, const std::string& date);
    ~Concert();
    std::string get_date() const;
    std::vector<int> get_dates_int();
    std::string get_city() const;
    std::string get_location() const;
    std::string get_artist() const;
    void print_date() const;
private:
    std::string artist_ = "";
    std::string city_ = "";
    std::string fest_location_ = "";
    std::string date_ = "";
    //split funktiota hyödynnetään muuttamalla päivämäärä vektorin kentiksi
    std::vector<std::string> split(std::string& str,
                                   const std::string& delim);
};

#endif // CONCERT_HH
