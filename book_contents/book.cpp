/* book.cpp
 *
 * Toteuttaa book.hh otsikkotiedoston funktiot
 *
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 *
 * */

#include "book.hh"

//konstruktori
Book::Book():
    chapters_({})
{

}

//destructor vapauttaa muistin
Book::~Book()
{
    //vapautetaan muisti
    for(auto pair : chapters_)
    {
        delete pair.second;
    }
}
//lisää uuden struct mallisen chapterin chapters_ vektoriin, parametrit ovat
//string id kappaleen id, string fullName kappaleen nimi sekä
//int length kappaleen pituus
void Book::addNewChapter(const std::string &id,
                         const std::string &fullName,
                         int length)
{
    if(chapterExists(id))
    {
        //Jos kappale on olemassa palataan ja ilmoitetaan siitä
        std::cout<<"Error: Already exists."<<std::endl;
        return;
    }

    //luodaan ja lisätään uusi luku
    //error checks?
    Chapter *new_ch = new Chapter;
    new_ch->id_=id;
    new_ch->fullName_ = fullName;
    new_ch->length_ = length;

    //chapters_.insert({new_ch->id_, new_ch});
    chapters_.push_back({new_ch->id_,new_ch});
}
//lisää kappaleiden yhteydet osoitteita hyödyntäen,
//ottaa kappaleet moduulin chapters_ vektorista
//parametrina annetaan alakappale ja tämän kappaleen vanhempi
void Book::addRelation(const std::string &subchapter,
                       const std::string &parentChapter)
{
    if(chapterExists(subchapter) &&
            chapterExists(parentChapter))
    {
        //haetaan pointterit
        Chapter *parent = findChapter(parentChapter),
                *child = findChapter(subchapter);
        //lisätään relaatio
        parent->subchapters_.push_back(child);
        child->parentChapter_ = parent;
    }
}
//tulostetaan kappaleiden id:t aakkosjärjestyksessä
//tehtävä ei vaadi parametreja komentoon, mutta ne ovat olleet jo valmiina
//pohjakoodissa
void Book::printIds(Params /*params*/) const
{
    IdMap ids = {};
    //talletetaan id:t set stl kirjastoon
    ids = unorderedToOrdered();
    std::cout<<"Book has "<<ids.size()<<" chapters:"<<std::endl;
    for(std::pair<std::string,std::string> pair : ids)
    {
        std::cout<<pair.first<<" --- "<<pair.second<<std::endl;
    }
}
//tulostetaan kappaleet siinä järjestyksessä missä ne on tietorakenteeseen
//talletettu
//tehtävä ei vaadi parametreja komentoon, mutta ne ovat olleet jo valmiina
//pohjakoodissa
void Book::printContents(Params /*params*/) const
{
    //mitkä luvut ovat päälukuja
    std::vector<Chapter*> headChapters = {};
    for(std::pair<std::string, Chapter*> pair : chapters_)
    {
        //mikäli pääluku <--> parent = null
        if(pair.second->parentChapter_ == nullptr)
        {
            headChapters.push_back(pair.second);
        }
    }


    //pääluvut tulostetaan ja sen jälkeen kappaleet
    int index = 1;
    for(Chapter *ptr : headChapters)
    {
        printChaptersRecursive(ptr,index," ");
        index++;
    }

}
//suljetaan haluttu luku mikä annetaan parametrina
void Book::close(Params params) const
{
    //tarkistetaan onko lukua tietorakenteessa
    Chapter *chap_ptr = findChapter(params.at(0));
    if(chap_ptr == nullptr)
    {
        std::cout<<NOTFOUND<<params.at(0)<<std::endl;
        return;
    }
    //mikäli luvulla ei ole alalukuja niin suljetaan vain itse luku
    if(chap_ptr->subchapters_.size()<1)
    {
        chap_ptr->isOpen_ = false;
        return;
    }
    //suljetaan kaikki alaluvut mikäli niitä annetulla luvulla on
    chapVec subVec = {};
    subVec = subsToVector(chap_ptr);
    for(auto& iter : subVec)
    {
        iter->isOpen_ = false;
    }
}
//avataan haluttu luku mikä annetaan parametrina
void Book::open(Params params) const
{
    //etsitään luku mikäli se löytyy
    Chapter *chap_ptr = findChapter(params.at(0));
    if(chap_ptr == nullptr)
    {
        std::cout<<NOTFOUND<<params.at(0)<<std::endl;
        return;
    }
    chap_ptr->isOpen_ = true;
    //avataan aliluvut joilla ei ole omia alilukuja
    for(auto& iter : chap_ptr->subchapters_)
    {
        if(iter->subchapters_.size() < 1)
        {
            iter->isOpen_ = true;
        }
    }
}
//avataan kaikki alaluvut
//tehtävä ei vaadi parametreja komentoon, mutta ne ovat olleet jo valmiina
//pohjakoodissa
void Book::openAll(Params /*params*/) const
{
    for(std::pair<std::string,Chapter*> pair : chapters_)
    {
        if(!pair.second->isOpen_)
        {
            pair.second->isOpen_ = true;
        }
    }
}
//tulostaa luvun kaikki vanhemmat niin kauan kun vanhempia on tai niin paljon
//kun käyttäjä tasoja syöttää parametrina
//sekä halutun luvun ID
void Book::printParentsN(Params params) const
{

    std::string id = params.at(0);
    int level = stoi(params.at(1));
    //käyttäjän tulee syöttää tasoja enemmän kuin yksi
    if(level<1)
    {
        std::cout<<ERRORN<<std::endl;
        return;
    }
    //tulosteessa tarvittava merkkijono
    std::string parentStr = "parent chapters";
    //tarkastetaan kappaleen olemassaolo
    Chapter *ptr = findChapter(id);
    if(ptr == nullptr)
    {
        std::cout<<NOTFOUND<<params.at(0)<<std::endl;
        return;
    }
    if(ptr->parentChapter_ == nullptr)
    {
        std::cout<<id<<" has no parent chapters."<<std::endl;
        return;
    }
    std::vector<Chapter*> parentChapterVec = {};
    //talletaan halutut vanhempiluvut vektoriin hyödyntäen apufunktiota
    parentChapterVec = parentsToVectorRecurisve(ptr,level);
    //tulostetaan vanhmepiluvut hyädyntäen apuvektoria, parametreina
    //luku pointer eli mistä aloitetaan, vektori jossa vanhemmat talletettuna
    //sekä tulosteessa tarvittavaa parentStr merkkijonoa
    printParentOrSub(ptr,parentChapterVec,parentStr);
}
//tulostetaan luvun kaikki aliluvut niin kauan kun niitä riittää tai kunnes
//käyttäjän syöttämä taso on saavutettuna, mikä saadaan parametrina
//sekä halutun luvun ID
void Book::printSubchaptersN(Params params) const
{
    std::string id = params.at(0);
    int level = stoi(params.at(1));
    if(level<1)
    {
        std::cout<<ERRORN<<std::endl;
        return;
    }
    //tulosteessa tarvittava merkkijono
    std::string subStr = "subchapters";
    Chapter *ptr = findChapter(id);

    if(ptr == nullptr)
    {
        std::cout<<NOTFOUND<<params.at(0)<<std::endl;
        return;
    }
    std::vector<Chapter*> siblingChapterVec = {};
    //talletetaan aliluvut vektoriin
    siblingChapterVec = subsToVectorRecurisve(ptr,level);
    if(ptr->subchapters_.size() < 1)
    {
        std::cout<<id<<" has no subchapters."<<std::endl;
        return;
    }
    //tulostetaan aliluvut
    printParentOrSub(ptr,siblingChapterVec,subStr);

}
//tulostetaan sisarluvut parametrina annetaan luvun ID
void Book::printSiblingChapters(Params params) const
{
    //Error: Not found: parametri
    Chapter* ch = findChapter(params.at(0));
    if(ch == nullptr)
    {
        std::cout<<NOTFOUND<<params.at(0)<<std::endl;
        return;
    }
    if(ch->parentChapter_ == nullptr)
    {
        std::cout<<ch->id_<<" has no sibling chapters."<<std::endl;
        return;
    }
    IdSet ids = {};
    //talletetaan luvun sisarluvut erilliseen set tietorakenteeseen
    if(ch->parentChapter_ == nullptr)
    {
        for(std::pair<std::string,Chapter*> pair : chapters_)
        {
            if(pair.first != ch->id_ &&
                    pair.second->parentChapter_ == nullptr)
            {
                ids.insert(pair.first);
            }
        }
    }
    else
    {
        IdSet::iterator it;
        ids = vectorToIdSet(ch->parentChapter_->subchapters_);
        it = ids.find(ch->id_);
        ids.erase(it);
    }
    if(ids.empty())
    {
        std::cout<<ch->id_<<"has no siblings"<<std::endl;
        return;
    }
    std::cout<<ch->id_<<" has "<<ids.size()<<" sibling chapters:"<<std::endl;
    printSet(ids);
}
//tulostetaan parametrina annetun luvun ID:n yhteispituus
void Book::printTotalLength(Params params) const
{
    Chapter *ptr = nullptr;
    //etsitään haluttu luku mikäli sellainen löytyy
    ptr = findChapter(params.at(0));
    if(ptr == nullptr)
    {
        std::cout<<NOTFOUND<<params.at(0)<<std::endl;
        return;
    }

    int length = 0;
    //saadaan pituus hyödyntämällä apufunktiota recursiveCount
    length = recursiveCount(ptr);
    std::cout<<"Total length of "<<ptr->id_<<" is "<<length<<"."<<std::endl;
}
//etsii ja laskee paratemrina annetun ID:n perusteella luvun pisimmän
//kappaleen
void Book::printLongestInHierarchy(Params params) const
{
    Chapter *ptr = nullptr;
    Chapter *comparePtr = nullptr;
    //etistään luku mikäli se löytyy
    ptr = findChapter(params.at(0));
    if(ptr == nullptr)
    {
        std::cout<<NOTFOUND<<params.at(0)<<std::endl;
        return;
    }
    //saadaan luvun pisin hierarkkiassa oleva luku johon luotu osoitin osoittaa
    //hyödyntämällä rekursiivista apufunktiota
    comparePtr = recursiveLongestChapter(ptr);
    //tarkastetaan onko annettu luku pisin vai saatu luku pisin
    if(ptr->length_ < comparePtr->length_)
    {
        std::cout<<"With the length of "
                 <<comparePtr->length_
                 <<", "
                 <<comparePtr->id_
                 <<" is the longest chapter in "
                 <<params.at(0)
                 <<"'s hierarchy."
                 <<std::endl;
    }
    else
    {
        std::cout<<"With the length of "<<ptr->length_
                 <<", "<<comparePtr->id_
                 <<" is the longest chapter in their hierarchy."
                 <<std::endl;
    }
}
//etsitään lyhin luku ja tulostetaan se, parametrina annetaan pääluvun ID josta
//lyhintä lukua etsitään
void Book::printShortestInHierarchy(Params params) const
{
    Chapter *ptr = nullptr;
    Chapter *comparePtr = nullptr;
    ptr = findChapter(params.at(0));
    if(ptr == nullptr)
    {
        std::cout<<NOTFOUND<<params.at(0)<<std::endl;
        return;
    }
    //etsitään lyhin luku hydöyntämällä apufunktiota
    comparePtr = recursiveShortestChapter(ptr);
    if(ptr->length_ > comparePtr->length_)
    {
        std::cout<<"With the length of "
                 <<comparePtr->length_
                 <<", "
                 <<comparePtr->id_
                 <<" is the shortest chapter in "
                 <<params.at(0)
                 <<"'s hierarchy."
                 <<std::endl;
    }
    else
    {
        std::cout<<"With the length of "<<ptr->length_
                 <<", "<<comparePtr->id_
                 <<" is the shortest chapter in their hierarchy."
                 <<std::endl;
    }
}

void Book::printParent(Params /*params*/) const
{

}

void Book::printSubchapters(Params /*params*/) const
{

}
//apufunktio jolla etsitään id:n perusteella lukua tietorakenteesta johon luvut
//on talletettu
Chapter *Book::findChapter(const std::string &id) const
{
    Chapter *ptr = nullptr;
    //onko ID olemassa
    if(!chapterExists(id))
    {
        return ptr;
    }
    for(std::pair<std::string,Chapter*> pair : chapters_)
    {
        if(pair.first == id)
        {
            ptr = pair.second;
            break;
        }
    }
    return ptr;
}
//apufunktio jolla muutetaan vektori jossa on struct muotoisia luku osoittimia
//ja talletetaan lukujen ID:t omaan set muotoiseen tietorakenteeseen
IdSet Book::vectorToIdSet(const std::vector<Chapter *> &container) const
{
    IdSet ids = {};
    for(Chapter *ptr : container)
    {
        ids.insert(ptr->id_);
    }
    return ids;
}
//apufunktio jolla tarkastetaan onko kappaletta olemassa
bool Book::chapterExists(const std::string &id) const
{
    for(std::pair<std::string,Chapter*> pair : chapters_)
    {
        if(pair.first == id)
        {
            return true;
        }
    }
    return false;
}
//tulostaa kirjan kappaleet rekursiivisesti, mikäli kappale on kiinni, ei
//tulosteta alilukuja ja - merkki kappaleen eteen
//mikäli kappale on auki laitetaan + merkki kappaleen eteen
void Book::printChaptersRecursive(Chapter *ch,
                                  int index,
                                  const std::string &indentation) const
{
    char sign = ch->isOpen_ ? '-' : '+';

    std::cout << sign << indentation << index
              << ". " << ch->fullName_
              << " ( "<< ch->length_ << " )"
              << std::endl;
    //head recursive
    //trivial case : no subchapters
    if(!ch->isOpen_ || ch->subchapters_.empty()){return;}

    //lähdetään iteroimaan alilukuja:
    index = 1;
    for(Chapter *subch : ch->subchapters_)
    {
        printChaptersRecursive(subch, index, indentation + "  ");
        index++;
    }
}
//palauttaa ID:t aakkosjärjestyksessä ja hakee halutut id:t moduulin vektor
//rakenteesta johon luvut on talletettu
Book::IdMap Book::unorderedToOrdered() const
{
    IdMap ids = {};
    for(std::pair<std::string, Chapter*> pair : chapters_)
    {
        Chapter *ptr = findChapter(pair.first);
        ids.insert({ptr->fullName_,ptr->id_});
    }
    return ids;
}
//apufunktio set rakenteen tulostamiseen
void Book::printSet(const std::set<std::string> &idSet) const
{
    for(const std::string &id : idSet)
    {
        std::cout<<id<<std::endl;
    }
}
//apufunktio jolla lasketaan lukujen pituuksia rekursiivisesti parametrina
//saadaan osoitin jonka luvun ja alilukujen pituudet halutaan selvittää
//palauttaa int muodossa pituuden
int Book::recursiveCount(Chapter *ptr) const
{
    int length = ptr->length_;
    if(ptr->subchapters_.size() < 1)
    {
        return length;
    }
    for(auto& iter : ptr->subchapters_)
    {
        length += recursiveCount(iter);
    }
    return length;
}
//Etsitään rekursiivisesti parametrina annetun luvun pisin luku ja palautetaan
//osoitin joka osoittaa pisimpään lukuun
Chapter *Book::recursiveLongestChapter(Chapter *ptr) const
{
    Chapter *longestPtr = ptr;
    Chapter *iterPtr = nullptr;
    if(ptr->subchapters_.size()<1)
    {
        return ptr;
    }
    for(auto& iter : ptr->subchapters_)
    {
        iterPtr = recursiveLongestChapter(iter);
        if(longestPtr->length_ < iterPtr->length_)
        {
            longestPtr = iterPtr;
        }
    }
    return longestPtr;
}
//Etsitään rekursiivisesti parametrina annetun luvun lyhyin luku ja palautetaan
//osoitin joka osoittaa lyhyimpään lukuun
Chapter *Book::recursiveShortestChapter(Chapter *ptr) const
{
    Chapter *shortestPtr = ptr;
    Chapter *iterPtr = nullptr;
    if(ptr->subchapters_.size()<1)
    {
        return ptr;
    }
    for(auto& iter : ptr->subchapters_)
    {
        iterPtr = recursiveShortestChapter(iter);
        if(shortestPtr->length_ > iterPtr->length_)
        {
            shortestPtr = iterPtr;
        }
    }
    return shortestPtr;
}
//etstiään rekursiivisesti halutun luvun vanhemmat talletetaan ne vektoriin
//missä vanhempiin osittavat osoittimet ja palautetaan tämä vektori
//parametreina annettu käyttäjän syöttämä taso mikä voi olla enemmän kuin
//vanhempia, jolloin palautetaan vain vanhempien määrä
std::vector<Chapter *> Book::parentsToVectorRecurisve(Chapter *ptr,
                                                      int level) const
{
    std::vector<Chapter*> chVec = {};
    if(ptr->parentChapter_ == nullptr || level < 1)
    {
        chVec.push_back(ptr);
        return chVec;
    }
    while(level >= 1)
    {
        std::vector<Chapter*> tempVec =
                parentsToVectorRecurisve(ptr->parentChapter_,level -1);
        chVec.insert(chVec.end(),tempVec.begin(), tempVec.end());
        --level;
    }
    return chVec;
}
//
std::vector<Chapter *> Book::subsToVectorRecurisve(Chapter *ptr,
                                                   int level) const
{
    std::vector<Chapter*> siblingsVec = {};
    if(ptr->subchapters_.size() < 1 || level < 1)
    {
        siblingsVec.push_back(ptr);
        return siblingsVec;
    }
    for(auto& iter : ptr->subchapters_)
    {
        siblingsVec.push_back(iter);
        std::vector<Chapter*> tempVec = subsToVectorRecurisve(iter,level-1);
        siblingsVec.insert(siblingsVec.end(),tempVec.begin(),tempVec.end());
    }
    return siblingsVec;
}
//Apufunktio jolla tulostetaan teksti printParentN funktiossa sekä
//printSubsN funktiossa parametreina ptr, josta saadaan haluttu id, vektorin
//pituus sekä merkkijono liittyen halutun funktion tulostukseen
void Book::printParentOrSub(Chapter* ptr, chapVec &chapVec,
                            std::string &parentOrSub) const
{

    std::set<std::string> siblingIdSet = {};
    //laitetaan set muotoiseen containeriin printtausta varten
    siblingIdSet = vectorToIdSet(chapVec);
    std::cout<<ptr->id_<<" has "<<siblingIdSet.size()<<" "<<parentOrSub
            <<":"<<std::endl;
    printSet(siblingIdSet);
}
//palautetaan vektori johon talletettuna parametrina annetun luvun aliluvut
std::vector<Chapter *> Book::subsToVector(Chapter *ptr) const
{
    std::vector<Chapter*> siblingsVec = {};
    if(ptr->subchapters_.size() < 1)
    {
        siblingsVec.push_back(ptr);
        return siblingsVec;
    }
    siblingsVec.push_back(ptr);
    for(auto& iter : ptr->subchapters_)
    {
        //siblingsVec.push_back(iter);
        std::vector<Chapter*> tempVec = subsToVector(iter);
        siblingsVec.insert(siblingsVec.end(),tempVec.begin(),tempVec.end());
    }
    return siblingsVec;
}
