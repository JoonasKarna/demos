/* mainwindow.cpp
 *
 * toteuttaa mainwindow.hh luokan toiminnot
 *
 * Ohjelman kirjoittaja
 * Nimi: Joonas Kärnä
 * Opiskelijanumero: 50218075
 * Käyttäjätunnus: rgjoka
 * E-Mail: joonas.karna@tuni.fi
 *
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QDebug>

const int MIN_VALUE = 15;
const int MAX_VALUE = 30;
const int START_SPEED = 600;
const QString START = "START";
const QString RESET = "RESET";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //pelilaudan leveyden ja korkeuden säätely
    ui->spinBoxHeight->setMaximum(MAX_VALUE);
    ui->spinBoxHeight->setMinimum(MIN_VALUE);

    ui->spinBoxWidth->setMaximum(MAX_VALUE);
    ui->spinBoxWidth->setMinimum(MIN_VALUE);

    connect(ui->startResetButton, &QPushButton::clicked,
            this, &MainWindow::onStartButtonClicked);
    connect(ui->pauseButton, &QPushButton::clicked,
            this, &MainWindow::onPauseButtonClicked);
    connect(ui->closeButton, &QPushButton::clicked,
            this, &MainWindow::onCloseButtonClicked);

    connect(ui->leftButton, &QPushButton::clicked,
            this, &MainWindow::onLeftButtonClicked);
    connect(ui->rightButton, &QPushButton::clicked,
            this, &MainWindow::onRightButtonClicked);
    connect(ui->upButton, &QPushButton::clicked,
            this, &MainWindow::onUpButtonClicked);
    connect(ui->downButton, &QPushButton::clicked,
            this, &MainWindow::onDownButtonClicked);
}

MainWindow::~MainWindow()
{
    delete boardPtr_;
    delete ui;
}

//Funktio hoitaa pelin aloittamisen kun käyttäjä painaa start nappulaa
//Samalla voidaan resetoida peli mikäli peli on jo käynnissä
void MainWindow::onStartButtonClicked()
{
    if(!isGameOn_)
    {
        //aktivoidaan nappulat sekä alustetaan pelilauta uudelleen
        //piirretään käärme sekä ruoka
        activateButtons();
        initialize();
        drawSnake();
        drawFood();

        //vaihdetaan start nappulan teksti siihen mitä käyttäjä voi tehdä
        //eli resetoida kun peli on jo käynnissä
        ui->startResetButton->setText(RESET);
        ui->statusLabel->setText("Game on");

        isGameOn_ = true;
        timer_->start();

        connect(timer_, &QTimer::timeout, this, &MainWindow::moveSnakeOnBoard);
        return;
    }
    //jos peli on jo käynnissä on resetoinnin vuoro
    ui->startResetButton->setText(START);
    ui->statusLabel->setText("Game reset");

    disableButtons();
    resetGame();

    isGameOn_ = false;
}
//Funktio hoitaa pelin pause toiminnallisuuden mikäli peli on alkanut
void MainWindow::onPauseButtonClicked()
{
    //jos peli ei ole alkanut niin ei tehdä mitään
    if(timer_ == nullptr)
    {
        return;
    }
    //mikäli peli on pausella jatketaan sitä
    if(isGamePaused_)
    {
        timer_->start();
        ui->pauseButton->setText("PAUSE");

        isGamePaused_ = false;

        activateButtons();
        return;
    }
    //Mikäöo peli ei ole pausella pausetetaan se
    ui->pauseButton->setText("CONTINUE");
    isGamePaused_ = true;

    stopTimer();
    disableButtons();
}
//pelilaudan sulkeminen
void MainWindow::onCloseButtonClicked()
{
    close();
}
//pelilaudan suuntanappuloiden toiminta
//vaihetaan suuntaa riippuen nappulan suunnasta
void MainWindow::onLeftButtonClicked()
{
    directionUp_ = false;
    directionDown_ = false;
    directionLeft_ = true;
    directionRight_ = false;
}

void MainWindow::onRightButtonClicked()
{
    directionUp_ = false;
    directionDown_ = false;
    directionLeft_ = false;
    directionRight_ = true;
}

void MainWindow::onDownButtonClicked()
{
    directionUp_ = false;
    directionDown_ = true;
    directionLeft_ = false;
    directionRight_ = false;
}

void MainWindow::onUpButtonClicked()
{
    directionUp_ = true;
    directionDown_ = false;
    directionLeft_ = false;
    directionRight_ = false;
}
//Pelilaudan alustaminen
void MainWindow::initialize()
{
    //käyttäjän syöttämä seed
    int seed = ui->seedInput->value();
    int leftMargin = 50;
    int topMargin = 400;
    //pelilaudan koko käyttäjän syöttämillä arvoilla
    int borderRight_ = ui->spinBoxWidth->value();
    int borderDown_ = ui->spinBoxHeight->value();

    duration_ = START_SPEED;
    //ajastin mikä liikuttaa käärmettä
    timer_ = new QTimer(this);
    //käärmeen "nopeus"
    timer_->setInterval(duration_);

    scene_ = new QGraphicsScene(this);

    //Aloitus suuntana alaspäin
    directionUp_ = false;
    directionDown_ = true;
    directionLeft_ = false;;
    directionRight_ = false;

    //pelilaudan sijoitus ja koko
    ui->graphicsView->setGeometry(leftMargin, topMargin,
                                  borderRight_*STEP+2,borderDown_*STEP+2);
    ui->graphicsView->setScene(scene_);

    scene_->setSceneRect(0, 0, borderRight_*STEP - 1, borderDown_*STEP - 1);

    //itse pelilauta

    boardPtr_ = new GameBoard(borderRight_,borderDown_,seed);
}
//funktio hoitaa käärmeen piirtämisen pelilaudalle
//koordinaatit saamme gameboard ja point luokilta
//käärmeen värit menevät sinisen skaalassa
//käärmeen osat ovat vektorissa jota käytetään käärmeen poistamisessa
void MainWindow::drawSnake()
{
    //käärmeen osat point luokan vektorissa
    std::vector<Point> snakeParts = {};

    //pään värit
    QColor headColor(0,0,255);
    QColor tailColor(0,255,255);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);

    //käärmeen pään koordinaatit
    Point snakeHead = boardPtr_->getHeadCoordintes();
    snakeParts = boardPtr_->getSnakeParts();

    QGraphicsRectItem* snake;
    //piirretään kaikki käärmeen osat sinisen skaalalla
    int colorIndex = 255;
    if(snakeParts.size() > 1)
    {
        Point snakeTail = boardPtr_->getTailCoordintes();
        for(auto& part : snakeParts)
        {

            colorIndex = colorIndex - 10;
            if(colorIndex<0)
            {
                colorIndex = 255;
            }
            QColor snakePartColor(0,colorIndex,255);
            int x = part.getX();
            int y = part.getY();
            if(part == snakeHead)
            {
                snake = scene_->addRect(x*20,y*20,STEP,STEP,blackPen,
                                        headColor);
                snakeVec_.push_back(snake);
            }
            else if(part == snakeTail)
            {
                snake = scene_->addRect(x*20,y*20,STEP,STEP,blackPen,
                                        tailColor);
                snakeVec_.push_back(snake);
            }
            else
            {
                snake = scene_->addRect(x*20,y*20,STEP,STEP,blackPen,
                                        snakePartColor);
                snakeVec_.push_back(snake);
            }
        }
        return;
    }
    //mikäli käärmeellä on vain pää niin piirretään vain se
    int x = snakeHead.getX();
    int y = snakeHead.getY();

    snake = scene_->addRect(x*20,y*20,STEP,STEP,blackPen,headColor);
    snakeVec_.push_back(snake);
}
//piirtää ruoan pelilaudalle saaden koordinaatit gamboard luokasta
void MainWindow::drawFood()
{
    QBrush yellowBrush(Qt::yellow);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);
    Point food = boardPtr_->getFoodCoordinates();
    int x = food.getX();
    int y = food.getY();

    food_ = scene_->addEllipse(x*20,y*20,STEP,STEP,blackPen,yellowBrush);

}
//käärmeen liikuttaminen pelilaudalla
void MainWindow::moveSnakeOnBoard()
{
    //kutsutaan pelilaudalta ruoan ja käärmeen pään koordinaatit
    Point snakeHead = boardPtr_->getHeadCoordintes();
    Point food = boardPtr_->getFoodCoordinates();

    //tarkastetaan mihin suuntaan käyttäjä haluaa liikuttaa käärmettä
    //mikäli liikkuminen ei ole mahdollista, niin palautetaan suunnat
    //aikaisempaan, liikutetaan myös päätä oikeaan suuntaan jotta
    //voimme lisätä pisteitä
    if(directionLeft_)
    {
        validMove_ = boardPtr_->moveSnake("a");
        if(!validMove_)
        {
            directionLeft_ = false;
            directionRight_ = true;
        }
        else
        {
            snakeHead.move("a");
        }
    }
    if(directionRight_)
    {
        validMove_ = boardPtr_->moveSnake("d");
        if(!validMove_)
        {
            directionRight_ = false;
            directionLeft_ = true;
        }
        else
        {
            snakeHead.move("d");
        }
    }
    if(directionUp_)
    {
        validMove_ = boardPtr_->moveSnake("w");
        if(!validMove_)
        {
            directionUp_ = false;
            directionDown_ = true;
        }
        else
        {
            snakeHead.move("w");
        }
    }
    if(directionDown_)
    {
        validMove_ = boardPtr_->moveSnake("s");
        if(!validMove_)
        {
            directionDown_ = false;
            directionUp_ = true;
        }
        else
        {
            snakeHead.move("s");
        }
    }
    //tarkastetaan onko peli ohi
    if(boardPtr_->gameOver())
    {
        gameOver();
        return;
    }

    //poistetaan käärme ja piirretään käärme uudelleen
    clearSnake();
    drawSnake();

    //päivitetään käärmeen pituus
    updateSnakeLength();

    //mikäli käärme on ruoan päällä lisätään pisteitä ja
    //poistetaan ruoka ja piirretään se pelilaudalle uusilla koordinaateilla
    if(snakeHead == food)
    {
        clearFood();
        drawFood();
        addPoints();
        setDurationSpeed();
    }
}
//käärmeen poistaminen eli tyhejnnetään käärmeen osat
//sisältävä vektori
void MainWindow::clearSnake()
{
    for(auto& parts : snakeVec_)
    {
       delete parts;
        parts = nullptr;
    }
    snakeVec_.clear();
}
//kun peli päättyy resetoidaan peli ja ilmoitetaan käyttäjälle pelin päättyneen
void MainWindow::gameOver()
{
    disableButtons();
    if(boardPtr_->gameLost())
    {
        ui->statusLabel->setText("Game over snake is dead");
    }
    if(boardPtr_->gameWon())
    {
        ui->statusLabel->setText("Congratulations you won :)");
    }
    resetGame();
    isGameOn_ = false;
}
//resetoidaan peli eli pelin pisteet nollaan
//poistetaan timer osoitin sekä laudan osoitin
//poistetaan käärme sekä ruoka ja asetetaan labeleihin oikeat tekstit
void MainWindow::resetGame()
{
    gamePoints = 0;
    QString pointStr = QString::number(gamePoints);

    ui->labelPoints->setText("Points: "+pointStr);
    ui->startResetButton->setText(START);

    stopTimer();
    if(timer_)
    {
        delete timer_;
        timer_ = nullptr;
    }
    clearSnake();
    clearFood();
    if(scene_)
    {
        delete scene_;
        scene_ = nullptr;
    }
    if(boardPtr_)
    {
        delete boardPtr_;
        boardPtr_ = nullptr;
    }
}
//poistetaan ruoan pointteri
void MainWindow::clearFood()
{
    if(food_)
    {
        delete food_;
        food_ = nullptr;
    }
}
//ajastimen pysäytys
void MainWindow::stopTimer()
{
    if(timer_)
    {
        timer_->stop();
    }
}
//päivitetään käärmeen pituus hyödyntäen gameboard luokkaa
void MainWindow::updateSnakeLength()
{
    int snakeLength = boardPtr_->getSnakelength();
    QString lengthQstr = QString::number(snakeLength);
    ui->labelSnakeLength->setText("Snake length: "+lengthQstr);
}
//painonapit pois käytöstä
//jos peli on pausella, niin käyttäjä ei voi resetoida peliä
void MainWindow::disableButtons()
{
    if(isGamePaused_)
    {
        ui->startResetButton->setDisabled(true);
        ui->pauseButton->setDisabled(false);
    }
    else
    {
        ui->startResetButton->setDisabled(false);
        ui->pauseButton->setDisabled(true);
    }

    ui->rightButton->setDisabled(true);
    ui->leftButton->setDisabled(true);
    ui->downButton->setDisabled(true);
    ui->upButton->setDisabled(true);
}
//painonappien käyttöön otto
void MainWindow::activateButtons()
{
    ui->pauseButton->setDisabled(false);
    ui->rightButton->setDisabled(false);
    ui->leftButton->setDisabled(false);
    ui->downButton->setDisabled(false);
    ui->upButton->setDisabled(false);
    ui->startResetButton->setDisabled(false);
}
//pisteiden lisääminen aina kun mato on syönyt ruoan
//pisteitä tulee yksi piste per ruoan syönti
void MainWindow::addPoints()
{
    gamePoints = gamePoints +1;
    QString pointStr = QString::number(gamePoints);
    ui->labelPoints->setText("Points: "+pointStr);
}
//muutetaan käärmeen nopeutta
void MainWindow::setDurationSpeed()
{
    if(duration_ < 200)
    {
        return;
    }
    duration_ = duration_ - STEP;
    timer_->setInterval(duration_);
}
//wasd painonappien käyttäminen käärmeen ohjaamiseen
void MainWindow::keyPressEvent(QKeyEvent* event) {
    //mikäli peli on pausella ei käyttäjä voi vaikuttaa käärmeen suuntaan
    if(isGamePaused_)
    {
        return;
    }
    //ylös
    if(event->key() == Qt::Key_W)
    {
        directionUp_ = true;
        directionDown_ = false;
        directionLeft_ = false;
        directionRight_ = false;
        return;
    }
    //alas
    if(event->key() == Qt::Key_S)
    {
        directionUp_ = false;
        directionDown_ = true;
        directionLeft_ = false;
        directionRight_ = false;
        return;
    }
    //vasemmalle
    if(event->key() == Qt::Key_A)
    {
        directionUp_ = false;
        directionDown_ = false;
        directionLeft_ = true;
        directionRight_ = false;
        return;
    }
    //oikealle
    if(event->key() == Qt::Key_D)
    {
        directionUp_ = false;
        directionDown_ = false;
        directionLeft_ = false;
        directionRight_ = true;
        return;
    }
}
