/* gigs
 *
 * Kuvaus:
 *  Ohjelma lukee alkutiedostosta neljän kentän kokoisia keikkatietoja
 *  jotka talletetaan ohjelman omaan tietorakenteeseen. Ohjelma tarkastaa ennen
 *  tietojen tallentamista syötetiedoston oikeellisuuden ja ilmoittaa tästä jos
 *  syötetiedosto ei kelpaa.
 *
 *  Talletettuun tietorakenteeseen voidaan syöttää komentoja:
 *
 *  ARTISTS, tulostaa kaikki artistit aakkosjärjestyksessä allekkain
 *  ottaen huomioon ascii järjestyksen.
 *
 *  ARTIST <esiintyjä>, tulostaa annetun esiintyjän keikat aikajärjestyksessä
 *  allekkain. Ohjelma ilmoittaa virheellisestä artistin nimestä tai väärästä
 *  määrästä parametreja.
 *
 *  STAGES, tulostaa aakkosjärjestyksesä kaikki esiintymispaikat allekkain.
 *
 *  STAGE <esiintymispaikka>, tulostaa allekkain ne esiintyjät, joilla on
 *  keikka annetulla esiintymispaikalla. Esiintyjät tulostetaan
 *  aakkosjärjestyksessä ottaen huomioon ascii järjestyksen. Mikäli käyttäjä
 *  syöttää tuntemattoman esiintymispaikan tai väärän määrän parametreja niin
 *  ohjelma ilmoittaa tästä käyttäjälle.
 *
 *  ADD_ARTIST <esiintyjä> lisää tietorakenteeseen annetun esiintyjän. mikäli
 *  esiintyjä on jo tietorakenteessa ilmoittaa ohjelma tästä käyttäjälle.
 *
 *  ADD_GIG <esiintyjä><pvm><paikkakunta><esiintymispaikka> lisää
 *  tietorakenteeseen annetulle artistille annetun keikan mikäli virheitä
 *  tulee annetuilla parametreilla, niin niistä ilmoitetaan käyttäjälle.
 *
 *  CANCEL <esiintyjä><pvm> peruu annetun esiintyjän kaikki keikat annetusta
 *  päivämäärästä lähtien. Mikäli parametreissa esiintyy ongelmia, ilmoitetaan
 *  tästä käyttäjälle.
 *
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 *
 * */
#include <concert.hh>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <set>
#include <cmath>

//alias gigs_map rakenteen tyypille
using gigs = std::map<std::string,std::vector<Concert>>;

// Farthest year for which gigs can be allocated
const std::string FARTHEST_POSSIBLE_YEAR = "2030";

//tiedoston sallittujen parametrien määrä
const int VALID_SIZE = 4;

//väärä input viestin tuloste
const std::string ERROR_COMMAND = "Error: Invalid input.";
//on jo olemassa viestin tuloste
const std::string ALREADY_EXISTS = "Error: Already exists.";
//sallitun komennon rakenne, std::string itse komento ja int param parametrien
//sallittu määrä
struct Command {
    std::string comm;
    int param;
};
//kaikki ohjelman komennot struct muodossa talletettuna vektoriin
const std::vector<Command> COMMANDS ={{"ARTISTS",1},{"ARTIST",2},{"STAGES",1},
                                     {"STAGE",2},{"ADD_ARTIST",2},{"ADD_GIG",5},
                                     {"CANCEL",3}};

// Casual split func, if delim char is between "'s, ignores it.
std::vector<std::string> split(const std::string& str, char delim = ';')
{
    std::vector<std::string> result = {""};
    bool inside_quotation = false;
    for ( char current_char : str )
    {
        if ( current_char == '"' )
        {
            inside_quotation = not inside_quotation;
        }
        else if ( current_char == delim and not inside_quotation )
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    if ( result.back() == "" )
    {
        result.pop_back();
    }
    return result;
}

// Checks if the given date_str is a valid date, i.e. if it has the format
// dd-mm-yyyy and if it is also otherwise possible date.
bool is_valid_date(const std::string& date_str)
{
    std::vector<std::string> date_vec = split(date_str, '-');
    if ( date_vec.size() != 3 )
    {
        return false;
    }

    std::string year = date_vec.at(0);
    std::string month = date_vec.at(1);
    std::string day = date_vec.at(2);
    std::vector<std::string> month_sizes
            = { "31", "28", "31", "30", "31", "30",
                "31", "31", "30", "31", "30", "31" };

    if ( month < "01" or month > "12" )
    {
        return false;
    }
    if ( year < "0001" or year > FARTHEST_POSSIBLE_YEAR )
    {
        return false;
    }
    unsigned int year_int = stoi(year);
    bool is_leap_year = (year_int % 400 == 0) or
                        (year_int % 100 != 0 and year_int % 4 == 0);
    if ( day >= "01" and
        (day <= month_sizes.at(stoi(month) - 1) or
        (month == "02" and is_leap_year and day <= "29")) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

//apu funktio read_file funktiota varten. Tarkemmin sort funktion apuväline,
//jolla iteraattori löytää struct vektorista oikean komennon.
struct find_command : std::unary_function<Command, bool> {
    std::string comm;
    find_command(std::string comm):comm(comm) { }
    bool operator()(Command const& c) const {
        return c.comm == comm;
    }
};
//apufunktio esiintymispaikkojen tallentamiseen omaan map rakenteeseen,
//joka voidaan tulostaa. Palauttaa map rakenteen jossa esiintymispaikka on avain
//ja artistit esiintymispaikan avaimen settinä.
std::map<std::string,std::set<std::string>> fill_location(const gigs& gigs_map,
                                                          std::string location)
{
    std::map<std::string,std::set<std::string>> loc_and_art = {};
    for(auto& artist : gigs_map)
    {
        for(auto& object : artist.second)
        {
            std::string artists_location = object.get_location();
            if(artists_location == location)
            {
                if(loc_and_art.find(location) != loc_and_art.end())
                {
                    loc_and_art.at(location).insert(artist.first);
                }
                else
                {
                    loc_and_art.insert({location,{}});
                    loc_and_art.at(location).insert(artist.first);
                }
            }
        }
    }
    return loc_and_art;
}
//tulostaa esiintymispaikan jonka käyttäjä syöttää ja sen jälkeen kaikki
//esiintymispaikan artisti aakkosjärjestyksessä
void print_stage_location(gigs& gigs_map, std::string location)
{
    std::map<std::string,std::set<std::string>> loc_and_art = {};
    loc_and_art = fill_location(gigs_map,location);

    if(loc_and_art.find(location) != loc_and_art.end())
    {
        std::cout<<"Stage "<<location
                <<" has gigs of the following artists:"<<std::endl;
        for(auto& location : loc_and_art)
        {
            for(auto& artist : location.second)
            {
                std::cout<<" - "<<artist<<std::endl;
            }
        }
    }
    else
    {
        std::cout<<"Error: Not found."<<std::endl;
    }
}
//tulostaa kaikki esiintymispaikat aakkosjärjestyksessä
void print_stages(const gigs& gigs_map)
{
    std::map<std::string,std::set<std::string>> stages_in_order = {};

    for(auto& artist : gigs_map)
    {
        for(auto& object : artist.second)
        {
            std::string city = object.get_city();
            std::string location = object.get_location();
            if(stages_in_order.find(city) != stages_in_order.end())
            {
                //löytyy
                stages_in_order.at(city).insert(location);
            }
            else
            {
                stages_in_order.insert({city,{}});
                stages_in_order.at(city).insert(location);
            }
        }
    }
    std::cout<<"All gig places in alphabetical order:"<<std::endl;
    for(auto& city : stages_in_order)
    {
        for(auto& location : city.second)
        {
            std::cout<<city.first;
            std::cout<<", "<<location<<std::endl;
        }
    }

}
//tulostaa kaikki artistit aakkosjärjestyksessä
void print_artists(const gigs& gigs_map)
{
    std::cout<<"All artists in alphabetical order:"<<std::endl;
    for(const auto& artist : gigs_map)
    {
        std::cout<<artist.first<<std::endl;
    }
}
//apufunktio mikä vertaa päivämääriä jotta ne voidaan tulostaa
//suuruusjärjestyksessä
bool comparator(Concert& obj1,Concert& obj2)
{
    std::vector<int> obj1_dates = {};
    obj1_dates = obj1.get_dates_int();
    std::vector<int> obj2_dates = {};
    obj2_dates = obj2.get_dates_int();

    if(!(obj1_dates.at(0) == obj2_dates.at(0)))
    {
        return obj1_dates.at(0) < obj2_dates.at(0);
    }
    else if(!(obj1_dates.at(1) == obj2_dates.at(1)))
    {
        return obj1_dates.at(1) < obj2_dates.at(1);
    }
    else
    {
        return obj1_dates.at(2) < obj2_dates.at(2);
    }
}

//tulostaa artistin ja kaikki tämän esiintymispäivät  sekä esiintymispaikat
//ja kaupungit päivämäärien mukaan nousujärjestyksessä.
void print_artist_date(gigs& gigs_map, std::string& artist)
{
    std::vector<Concert> concert_vec = {};
    if(gigs_map.find(artist) != gigs_map.end())
    {
        std::cout<<"Artist "<<artist
                 <<" has the following gigs in the order"
                  " they are listed:"<<std::endl;
        concert_vec = gigs_map.at(artist);
        sort(concert_vec.begin(),concert_vec.end(), &comparator);
        for(auto& obj : concert_vec)
        {
            obj.print_date();
        }
    }
    else
    {
        std::cout<<"Error: Not found."<<std::endl;
    }
}

//tarkastetaan ettei päivämäärä ole sama, apufunktio map tietorakenteen
//täyttämiseen ettei samoille päiville voi laittaa samalle artistille
//useampaa keikkaa
bool isnot_duplicate_date(std::vector<Concert>& concert_vec,
                      std::string& date)
{
    for(auto& obj : concert_vec)
    {
        std::string obj_date = "";
        obj_date = obj.get_date();
        if(obj_date == date)
        {
            return false;
            break;
        }

    }
    return true;
}
//täyttää map rakenteen tiedostosta saaduilla parametreilla.
//mikäli tiedon tallentaminen ei onnistu jos artistilla on ja samalla päivällä
//keikka palautetaan false
bool fill_map(std::string& artist,
              std::string& city,
              std::string& fest_location,
              std::string& date,
              gigs& gigs_map)
{
    if(gigs_map.find(artist) == gigs_map.end())
    {
        gigs_map.insert({artist,{}});
        Concert* concert = new Concert(artist,city,fest_location,date);
        gigs_map.at(artist).push_back(*concert);
    }
    else
    {
        //tarkastetaan ettei artistia ole talletettu samalle päivälle
        if(isnot_duplicate_date(gigs_map.at(artist),date))
        {
            Concert* concert = new Concert(artist,city,fest_location,date);
            gigs_map.insert({artist,{}});
            gigs_map.at(artist).push_back(*concert);
        }
        else
        {

            return false;
        }

    }
    return true;

}
//mikäli syötetiedostosta löytyy tyhjä kenttä palautetaan false.
bool is_empty_field(std::vector<std::string>& line_vec)
{
    bool is_empty = false;
    for(auto& field : line_vec)
    {
        if(field.empty() || std::all_of(field.begin(),field.end(), [](char c){
                                        return std::isspace(c);}))
        {
            is_empty = true;
            break;
        }
    }
    return is_empty;
}
//apufunktio is_duplicate_location funktioon funktion tehtävänä on täyttää
//map tietorakenne jossa avaimena on päivämäärä ja tällä parina kaikki
//esiintymispaikat kyseiselle päivämäärälle.
std::map<std::string,std::set<std::string>> fill_locations(gigs& gigs_map)
{
    std::map<std::string,std::set<std::string>>dates_map = {};
    for(auto& key : gigs_map)
    {
        for(auto& obj : key.second)
        {
            std::string date = obj.get_date();
            std::string location = obj.get_location();
            if(dates_map.find(date) == dates_map.end())
            {
                dates_map.insert({date,{}});
            }
            dates_map.at(date).insert(location);
        }
    }
    return dates_map;
}
//tarkastetaan ettei samalle päivälle voida tallettaa samaa esiintymispaikkaa
//palautetaan true jos samalle päivälle yritetään tallettaa samaa
//esiintymispaikkaa
bool is_duplicate_location(gigs gigs_map, std::string& location,
                           std::string& date)
{

    std::map<std::string,std::set<std::string>>dates_map = {};
    dates_map = fill_locations(gigs_map);

    if(dates_map.find(date) != dates_map.end())
    {
        for(auto& loc : dates_map.at(date))
        {
            if(loc == location)
            {
                std::cout<<ALREADY_EXISTS<<std::endl;
                return true;
            }
        }
        return false;
    }
    else
    {
        return false;
    }

}
//lukee tiedoston ja tallettaa tämän tiedot tietorakenteeseen, mikäli lukemisen
//aikana esiintyy virheitä niin palautetaan false
bool read_file(gigs& gigs_map)
{
    std::string file_name = "";
    std::cout<<"Give a name for input file: ";
    getline(std::cin,file_name);
    std::ifstream input_file(file_name);

    //mikäli tiedostoa ei löydy ilmoitetaan tästä
    if(!(input_file))
    {
        std::cout<<"Error: File could not be read."<<std::endl;
        return false;
    }
    int row = 0;
    char delim = ';';

    std::string input_line = "";
    std::vector <std::string> line_vec = {};
    //käydään tiedosto läpi rivi kerrallaan
    while(getline(input_file, input_line))
    {
        ++row;
        line_vec = split(input_line,delim);
        int vec_length = static_cast<int>(line_vec.size());
        //jos tiedostosta löytyy tyhjä kenttä tai mikäli kenttiä on liian vähän
        //ilmoitetaan tästä
        if(!(vec_length == VALID_SIZE) || is_empty_field(line_vec))
        {
            std::cout<<"Error: Invalid format in file."<<std::endl;
            return false;
        }

        std::string date = line_vec.at(1);
        //päivämäärän tarkastaminen
        if(!(is_valid_date(date)))
        {
            std::cout<<"Error: Invalid date."<<std::endl;
            return false;
        }

        std::string artist = "";
        std::string city = "";
        std::string fest_location = "";

        artist = line_vec.at(0);
        city = line_vec.at(2);
        fest_location = line_vec.at(3);
        //tarkastetaan ettei esiintymispaikalle ole jo varattu artistia samalle
        //päivälle
        if(is_duplicate_location(gigs_map, fest_location, date))
        {

            return false;
        }

        //mikäli tietorakenteeseen tallettaminen ei onnistu ilmoitetaan tästä
        if(!fill_map(artist,city,fest_location,date,gigs_map))
        {
            std::cout<<ALREADY_EXISTS<<std::endl;
            return false;
        }
    }
    input_file.close();
    return true;
}
//artistin lisääminen tietorakenteeseen
void add_artist(gigs& gigs_map, std::string& artist)
{
    if(gigs_map.find(artist) != gigs_map.end())
    {
        std::cout<<ALREADY_EXISTS<<std::endl;
    }
    else
    {
        std::cout<<"Artist added."<<std::endl;
        gigs_map[artist];
    }
}
//apufunktio add gig funktiota varten, tarkastetaan, että käyttäjän syöttämä
//päivämäärä on oikea ja että artisti on jo tietorakneteessa
bool is_valid_input(gigs& gigs_map,std::string& artist,std::string& date)
{
    if(gigs_map.find(artist) == gigs_map.end())
    {
        std::cout<<"Error: Not found."<<std::endl;
        return false;
    }
    if(!(is_valid_date(date)))
    {
        std::cout<<"Error: Invalid date."<<std::endl;
        return false;
    }
    return true;
}
//apufunktio add gig funktioon tarkastetaan että pävämäärä ei ole sama
bool is_same_date(std::string& obj_date, std::string& date)
{
    if(obj_date == date)
    {
        std::cout<<ALREADY_EXISTS<<std::endl;
        return true;
    }
    return false;
}
//lisätään tapahtuma tietorakenteeseen ja ilmoitetaan mikäli näin on mahdollista
//tehdä viestillä "Gig added."
void add_gig(gigs& gigs_map,std::string& artist, std::string& date,
             std::string& city,std::string& location)
{
    if(!is_valid_input(gigs_map,artist,date))
    {
        return;
    }
    std::vector<Concert> concert_vec = {};
    concert_vec = gigs_map.at(artist);
    for(auto& obj : concert_vec)
    {
        std::string obj_date = obj.get_date();
        if(is_same_date(obj_date,date))
        {
            return;
        }
    }
    if(!is_duplicate_location(gigs_map,location,date))
    {
        Concert* gig = new Concert(artist,city,location,date);
        gigs_map.at(artist).push_back(*gig);
        std::cout<<"Gig added."<<std::endl;
    }


}
//apufunktio cancel_artitst funktiota varten.
//lasketaan objektin päivämäärä ja käyttäjän syöttämän päivämäärän arvot
//ja palautetaan true mikäli syötetty päivämäärä on sama tai suurempi
bool is_earlier_date(std::vector<std::string>& date_vec
                     , Concert& fluid_obj)
{
    std::vector<int> fluid_dates = fluid_obj.get_dates_int();
    int obj_year = fluid_dates.at(0);
    int obj_month = fluid_dates.at(1);
    int obj_day = fluid_dates.at(2);

    int date_year = stoi(date_vec.at(0));
    int date_month = stoi(date_vec.at(1));
    int date_day = stoi(date_vec.at(2));


    int obj_sum = (365*obj_year)+floor(obj_year/4)-floor(obj_year/400)
            +obj_day+floor((153*obj_month)/5);

    int date_sum = (365*date_year)+floor(date_year/4)-floor(date_year/400)
            +date_day+floor((153*date_month)/5);

    return date_sum >= obj_sum;

}
//poistetaan artistin keikat alkaen päviämäärästä mikäli näin on mahdollista
void cancel_artist(gigs& gigs_map, std::string& artist, std::string& date)
{
    std::vector<Concert> concert_vec = {};
    std::vector<Concert> updated_vec = {};
    concert_vec = gigs_map.at(artist);
    sort(concert_vec.begin(),concert_vec.end(), &comparator);
    std::vector<std::string> date_vec = split(date,'-');

    //täytetään uusi vektori sallituilla päivämäärillä
    for(auto& concert : concert_vec)
    {
        if(!is_earlier_date(date_vec, concert))
        {
            break;
        }
        updated_vec.push_back(concert);
    }
    //ilmoitetaan käyttäjälle mikäli artistilla ei ole tapahtumia annetun
    //päivämäärän jälkeen
    if(updated_vec.size() == concert_vec.size())
    {
        std::cout<<"Error: No gigs after the given date."<<std::endl;
    }
    else
    {
        gigs_map.at(artist) = updated_vec;
        std::cout<<"Artist's gigs after the given date cancelled."<<std::endl;
    }
}
//apufunktio jolla käyttäjän pääkomento muutetaan isommiksi kirjaimiksi
//esimerkiksin "artist" -> "ARTIST"
void to_upper(std::string& command)
{
    std::string toupper_str = "";
    for(char& ch : command)
    {
        toupper_str += toupper(ch);
    }
    command = toupper_str;
}
//käyttöliittymä funktio palauttaa false mikäli käyttäjä syöttää kehotteen
//"QUIT" muutoin palauttaa true arvon jolla pääohjelman loop jatkuu
bool interface(gigs& gigs_map)
{
    std::string command;
    std::cout<<"gigs> ";
    getline(std::cin,command);

    std::vector<std::string> command_vec = {};
    //jaetaan käyttäjän syöte
    command_vec = split(command,' ');

    int length = static_cast<int>(command_vec.size());
    //pääkomento esim ARTIST, ARTISTS etc
    std::string main_command = command_vec.at(0);
    //korotetaan pääkomento isoiksi kirjaimiksi
    to_upper(main_command);

    if(main_command == "QUIT")
    {
        return false;
    }
    //iteraattori mikä hyödyntää itse luota find_command funktiota
    //etsii komentovektorista käyttäjän main_command arvoa
    std::vector<Command>::const_iterator iter;
    iter = std::find_if(COMMANDS.begin(),COMMANDS.end(),
                        find_command(main_command));
    //tarkastetaan sallitut minimi parametrit ja että komento löytyy
    if(iter != COMMANDS.end() && (length >= iter->param))
    {
        switch(distance(COMMANDS.begin(), iter))
        {
        case 0:
            //ARTISTS
            print_artists(gigs_map);
            break;
        case 1:
            //ARTIST <esiintyjä>
            print_artist_date(gigs_map,command_vec.at(1));
            break;
        case 2:
            //STAGES
            print_stages(gigs_map);
            break;
        case 3:
            //STAGE <esiintymispaikka>
            print_stage_location(gigs_map,command_vec.at(1));
            break;
        case 4:
            //ADD_ARTIST <esiintyjä>
            add_artist(gigs_map,command_vec.at(1));
            break;
        case 5:
            //ADD_GIG <esiintyjä> <pvm> <paikkakunta> <esiintymispaikka>
            add_gig(gigs_map,command_vec.at(1),command_vec.at(2),
                       command_vec.at(3),command_vec.at(4));
            break;
        case 6:
            //CANCEL <esiintyjä> <pvm>
            if(is_valid_input(gigs_map,command_vec.at(1),command_vec.at(2)))
            {
                cancel_artist(gigs_map,command_vec.at(1),command_vec.at(2));
            }
            break;
        }
    }
    else
    {
        std::cout<<ERROR_COMMAND<<std::endl;
    }
    return true;
}
int main()
{
    gigs gigs_map;
    bool is_file_read = false;
    is_file_read = read_file(gigs_map);
    //mikäli syötettä ei voida lukea niin palautetaan EXIT_FAILURE arvolla
    if(!is_file_read)
    {
        return EXIT_FAILURE;
    }
    else
    {
        //pääohjelma
        while(interface(gigs_map));
    }
    return 0;
}
