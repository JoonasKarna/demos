/* mainwindow.hh
 *
 * Matopelin kuvaus:
 * pelissä on tarkoitus liikuttaa matoa pelilaudalla syöden laudalle ilmestyviä
 * keltaisia ympyröitä jotka ovat madon ruokaa. Mato kasvaa pituutta syödessä.
 * Jos pelaaja liikuttaa matoa osuen madon osiin poislukien madon niska tai
 * osuen pelilaudan reuniin, niin peli päättyy silloin. Madon syödessä ruoka
 * ilmestyy ruoka jonnekin pelialueelle jossa ei ole madon osia
 *
 * Madon ohjaaminen:
 * Matoa voidaan ohjata käyttäen wasd näppäimiä seuraavasti:
 * w liikuttaa matoa ylöspäin, a liikuttaa matoa vasemmalle, s liikuttaa matoa
 * alaspäin, d liikuttaa matoa oikealle.
 *
 * Matoa voidaan myös ohjata pelilaudalta painamalla niille esitettyjä nappuloi
 * ta samalla logiikalla kuin wasd näppäimiä.
 *
 * Pelin toiminnallisuudet:
 * Start nappula, aloittaa pelin kulun oletussuuntana mato lähtee heti kulkemaan
 * alaspäin. Kun start nappulaa on painettu niin muuttuu sen funktio reset nappu
 * laksi, käyttäjälle tästä ilmoitetaan vaihtamalla teksti reset. Reset
 * palauttaa laudan samaan tilaan missä se oli ennen pelin alkua. Reset nappia
 * ei voida käyttää mikäli peli on pausella
 *
 * Pause nappula, pysäyttää pelin kunnes käyttäjä painaa nappulaa uudestaan.
 * Kun pausea on painettu muuttuu nappulan teksti continue ilmoittaen käyttäjäl
 * le että peliä voi jatkaa. Pausen ajaksi käyttäjän mahdollisten liikkeiden
 * syöttäminen on disabloitu
 *
 * Käyttäjän syötteet, Käyttäjä voi vaikuttaa pelilaudan korkeuteen sekä
 * leveyteen syöttämällä arvoja näille varattuihin kenttiin. Minimi sekä maksimi
 * arvot on valmiiksi annettu eli käyttäjä voi syöttää arvoja näiden väliltä
 *
 * Käyttäjä voi myös syöttää seed arvon mikä arpoo ruoan sijainnin.
 *
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 *
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "gameboard.hh"

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //mahdollistaa näppäinten käytön
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    //start/reset nappulan käyttö
    void onStartButtonClicked();
    //pause nappulan toiminta
    void onPauseButtonClicked();
    void onCloseButtonClicked();

    //käyttöliittymän näppäinten toiminta
    void onLeftButtonClicked();
    void onRightButtonClicked();
    void onDownButtonClicked();
    void onUpButtonClicked();

private:
    //pelilaudan alustaminen
    void initialize();
    //käärmeen piirtämisen toiminta pelilaudalle
    void drawSnake();
    //ruoan piirtämisen toiminta pelilaudalle
    void drawFood();
    //käärmeen liikkumisen mekanismi
    void moveSnakeOnBoard();
    //käärmeen poistaminen pelilaudalta
    void clearSnake();
    //pelin toiminnan loppumisen logiikka
    void gameOver();
    //pelin resetoiminen
    void resetGame();
    //ruoan poistaminen pelilaudalta
    void clearFood();
    //ajastimen pysäytys
    void stopTimer();
    //käärmeen pituuden päivittäminen
    void updateSnakeLength();
    //painonappien käytön estäminen
    void disableButtons();
    //painonappien käyttöön ottaminen
    void activateButtons();
    //pisteiden lisääminen
    void addPoints();
    //nopeuttaa käärmeen liikettä
    void setDurationSpeed();
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;

    //osoitin ruokaan
    QGraphicsEllipseItem* food_;

    //käärmeen osat sisältävä vektori
    std::vector<QGraphicsRectItem*> snakeVec_;

    QTimer* timer_ = nullptr;
    GameBoard* boardPtr_ = nullptr;

    //pelin toimintaan vaikuttavat muuttujat
    bool isGameOn_ = false;
    bool validMove_ = true;
    bool isGamePaused_ =false;
    int gamePoints = 0;
    int duration_;

    //madonsuuntaan vaikuttavat muuttujat
    bool directionLeft_ = false;
    bool directionRight_ = false;
    bool directionUp_ = false;
    bool directionDown_ = true;

    //grafiikkaan vaikuttavat muuttujat
    //step*step on yhden "pelilaudan pisteen" koko
    const int STEP = 20;

    const int BORDER_UP = 0;
    const int borderDown_ = 0;
    const int BORDER_LEFT = 0;
    const int borderRight_ = 0;
};
#endif // MAINWINDOW_HH
