/* point.hh
 *
 * valmispohja mikä toteuttaa käärmeen ja ruoan muodostavat point luokan
 * Valmispohjaan lisätty seuraavat omat metodit
 *
 * getX(), palauttaa x koordinaatit
 * getY(), palauttaa y koordinaatit
 *
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 *
 * */
#ifndef POINT_HH
#define POINT_HH

#include <string>

// Constants for directions
const char UP = 'w';
const char LEFT = 'a';
const char DOWN = 's';
const char RIGHT = 'd';

class Point
{
public:
    // Default constructor, creates a new point at (0, 0).
    Point();

    // Constructor, creates a new point at the given coordinates.
    Point(int x, int y);

    // Destructor
    ~Point();

    // Comparison operators
    bool operator==(const Point& rhs) const;
    bool operator!=(const Point& rhs) const;

    // Sets a new position for the point
    void setPosition(int x, int y);

    // Moves the point into the given direction, direction can be one of
    // constants above.
    void move(const std::string& dir);

    // Tells if the point is inside the square specified by the given
    // parameters.
    bool isInside(int left_top_x, int left_top_y,
                  int right_bottom_x, int right_bottom_y) const;

    //palauttaa x koordinaatin
    int getX();
    //palauttaa y koordinaatin
    int getY();

private:
    // Coordinates of the point
    int x_;
    int y_;
};

#endif // POINT_HH
