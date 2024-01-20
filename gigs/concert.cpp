/*
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 *
 * */

#include "concert.hh"
#include <iostream>


Concert::Concert()
{

}

Concert::Concert(const std::string &artist,
                 const std::string &city,
                 const std::string &fest_location,
                 const std::string &date):
    artist_(artist), city_(city), fest_location_(fest_location), date_(date)
{

}

Concert::~Concert()
{

}

std::string Concert::get_date() const
{
    return date_;
}
//plauttaa objektin päivämäärät int muotosena vektorina
std::vector<int> Concert::get_dates_int()
{
    std::vector<int> dates_in_int = {};
    std::vector<std::string> dates_as_string = {};
    std::string date = "";
    date = date_;
    dates_as_string = split(date,"-");

    for(std::string& date : dates_as_string)
    {
        int day = 0;
        day = stoi(date);
        dates_in_int.push_back(day);
    }
    return dates_in_int;
}

std::string Concert::get_city() const
{
    return city_;
}

std::string Concert::get_location() const
{
    return fest_location_;
}

std::string Concert::get_artist() const
{
    return artist_;
}

void Concert::print_date() const
{
    std::cout<<" - "<<date_<<" : "<<city_<<", "<<fest_location_<<std::endl;
}

std::vector<std::string> Concert::split(std::string& str,
                                        const std::string& delim)
{
    std::vector<std::string> date_vec = {};
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delim)) != std::string::npos) {
        token = str.substr(0, pos);
        date_vec.push_back(token);
        str.erase(0, pos + delim.length());
    }
    date_vec.push_back(str);
    return date_vec;
}

