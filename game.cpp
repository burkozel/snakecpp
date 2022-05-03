#include "game.h"

#include <QPainter>
#include <QRandomGenerator>
#include <QMessageBox>

#define LEN 4
#define BACKGROUND_COLOR "background-color: black"
#define APPLE_COLOR Qt::yellow
#define HEAD_COLOR Qt::red
#define BODY_COLOR Qt::green
#define FONT_COLOR Qt::white
#define FONT "Times", 10, QFont::Bold

Game::Game() {
    this->resize(SECTOR_W * FIELD_W, SECTOR_L * FIELD_L);
    this->setWindowTitle("SNAKE GAME");
    this->setStyleSheet(BACKGROUND_COLOR);
    this->setWindowFlags(Qt::Dialog);

    initGame();
}

void Game::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);

    if(g_inGame) {
        check_apple();
        move();
        check_colissions();
    }

    this->repaint();

    g_event_id = e->type();
}

void Game::keyPressEvent(QKeyEvent *e) {
    int key = e->key();

    if(g_event_id != 6) {
        if(key == Qt::Key_Left  && g_dir != Dirs::right) { g_dir = Dirs::left;  }
        if(key == Qt::Key_Right && g_dir != Dirs::left)  { g_dir = Dirs::right; }
        if(key == Qt::Key_Up    && g_dir != Dirs::down)  { g_dir = Dirs::up;    }
        if(key == Qt::Key_Down  && g_dir != Dirs::up)    { g_dir = Dirs::down;  }
    }

    g_event_id = e->type();
}

void Game::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);

    doDrawing();
}

void Game::initGame() {
    g_inGame = true;
    g_dir = right;

    g_sectors.resize(LEN);

    for(int i = 0; i < g_sectors.size(); i++) {
        g_sectors[i].rx() = g_sectors.size() - i - 1;
        g_sectors[i].ry() = 0;
    }

    localeApple();

    g_timerId = startTimer(DELAY);
}

void Game::doDrawing() {
    QPainter qp(this);

    if(g_inGame) {
        qp.setBrush(APPLE_COLOR);
        qp.drawEllipse(g_apple.x() * SECTOR_W, g_apple.y() * SECTOR_L, SECTOR_W, SECTOR_L);

        for(int i = 0; i < g_sectors.size(); i++) {
            if(i == 0){
                qp.setBrush(HEAD_COLOR);
            } else {
                qp.setBrush(BODY_COLOR);
            }

            qp.drawRect(g_sectors[i].x() * SECTOR_W, g_sectors[i].y() * SECTOR_L, SECTOR_W, SECTOR_L);
        }

        qp.setPen(FONT_COLOR);
        qp.setFont(QFont(FONT));
        qp.drawText(0, 20, "Score: " + QString::number(g_eaten));
    } else {
        gameOver();
    }
}

void Game::localeApple() {
    g_apple.rx() = QRandomGenerator::global()->generate() % SECTOR_W;
    g_apple.ry() = QRandomGenerator::global()->generate() % SECTOR_L;
}

void Game::move() {
    for(int i = g_sectors.size() - 1; i > 0; i--) {
        g_sectors[i] = g_sectors[i-1];
    }

    switch(g_dir) {
        case left:
            g_sectors[0].rx() = (g_sectors[0].x() + FIELD_W - 1) % FIELD_W;
            break;

        case right:
            g_sectors[0].rx() = (g_sectors[0].x() + 1) % FIELD_W;
            break;

        case up:
            g_sectors[0].ry() = (g_sectors[0].y() + FIELD_L - 1) % FIELD_L;
            break;

        case down:
            g_sectors[0].ry() = (g_sectors[0].y() + 1) % FIELD_L;
            break;
    }
}

void Game::gameOver() {
    QMessageBox msgb;
    msgb.setWindowFlags(Qt::FramelessWindowHint);
    msgb.setIcon(QMessageBox::Information);
    msgb.setText("Game over!");
    QPushButton *btn = msgb.addButton("Exit", QMessageBox::RejectRole);
    msgb.addButton("Restart", QMessageBox::RejectRole);
    msgb.exec();

    if (msgb.clickedButton() == (QAbstractButton *)btn) {
        exit(0);
    }

    initGame();
}

void Game::check_apple() {
    if(g_apple == g_sectors[0]) {
        g_eaten++;
        g_sectors.push_back(QPoint(0,0));
        localeApple();
    }
}

void Game::check_colissions() {
    for(int i = 1; i < g_sectors.size(); i++) {
        if(g_sectors[0] == g_sectors[i]) {
            g_inGame = false;
            break;
        }
    }

    if(!g_inGame) {
        killTimer(g_timerId);
    }
}
