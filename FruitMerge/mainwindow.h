#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QListWidget>
#include "gamescene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void startGame();
    void showHighScores();
    void showMenu();
    void quitGame();
    void onGameOver(int score);

private:
    void setupMenu();
    void setupScoreView();
    void saveScore(const QString &filename, int score);
    void loadScores();

    QStackedWidget *m_stackedWidget;
    QWidget *m_menuWidget;
    QWidget *m_scoreWidget;
    QListWidget *m_scoreList;
    
    QGraphicsView *m_view;
    GameScene *m_scene;

    struct ScoreEntry {
        QString name;
        int score;
    };
    QList<ScoreEntry> m_scores;
};

#endif 
