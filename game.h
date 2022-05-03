#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>

class Game: public QWidget
{
    Q_OBJECT

public:
    Game();

protected:
    void timerEvent(QTimerEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *e) override;

private:
    static const int SECTOR_W = 20;
    static const int SECTOR_L = 20;
    static const int FIELD_W = 20;
    static const int FIELD_L = 20;
    static const int DELAY = 100;

    int g_timerId;
    int g_event_id;

    enum Dirs {
        left, right, up, down
    };

    Dirs g_dir;

    bool g_inGame;
    QVector<QPoint> g_sectors;
    QPoint g_apple;
    int g_eaten = 0;

    void initGame();
    void doDrawing();
    void localeApple();
    void move();
    void gameOver();
    void check_apple();
    void check_colissions();
};

#endif // GAME_H
