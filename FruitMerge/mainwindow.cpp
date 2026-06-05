#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <QFont>
#include <QInputDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(new QStackedWidget(this))
    , m_menuWidget(nullptr)
    , m_scoreWidget(nullptr)
    , m_scoreList(nullptr)
    , m_view(nullptr)
    , m_scene(nullptr)
{
    setCentralWidget(m_stackedWidget);
    
    loadScores();
    setupMenu();
    setupScoreView();
    
    resize(600, 800);
    setWindowTitle(tr("Fruit Merge Game"));
}

void MainWindow::setupMenu()
{
    m_menuWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_menuWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QLabel *titleLabel = new QLabel(tr("Fruit Merge"), m_menuWidget);
    titleLabel->setFont(QFont("Arial", 48, QFont::Bold));
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QPushButton *startBtn = new QPushButton(tr("Start Game"), m_menuWidget);
    startBtn->setFixedSize(200, 60);
    startBtn->setFont(QFont("Arial", 14));
    connect(startBtn, &QPushButton::clicked, this, &MainWindow::startGame);
    layout->addWidget(startBtn, 0, Qt::AlignCenter);

    QPushButton *scoreBtn = new QPushButton(tr("High Scores"), m_menuWidget);
    scoreBtn->setFixedSize(200, 60);
    scoreBtn->setFont(QFont("Arial", 14));
    connect(scoreBtn, &QPushButton::clicked, this, &MainWindow::showHighScores);
    layout->addWidget(scoreBtn, 0, Qt::AlignCenter);

    QPushButton *quitBtn = new QPushButton(tr("Quit"), m_menuWidget);
    quitBtn->setFixedSize(200, 60);
    quitBtn->setFont(QFont("Arial", 14));
    connect(quitBtn, &QPushButton::clicked, this, &MainWindow::quitGame);
    layout->addWidget(quitBtn, 0, Qt::AlignCenter);

    m_stackedWidget->addWidget(m_menuWidget);
}

void MainWindow::setupScoreView()
{
    m_scoreWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_scoreWidget);
    layout->setAlignment(Qt::AlignCenter);

    QLabel *title = new QLabel(tr("High Scores"), m_scoreWidget);
    title->setFont(QFont("Arial", 24, QFont::Bold));
    layout->addWidget(title, 0, Qt::AlignCenter);

    m_scoreList = new QListWidget(m_scoreWidget);
    m_scoreList->setFont(QFont("Arial", 12));
    layout->addWidget(m_scoreList);

    QPushButton *backBtn = new QPushButton(tr("Back to Menu"), m_scoreWidget);
    backBtn->setFixedSize(200, 50);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showMenu);
    layout->addWidget(backBtn, 0, Qt::AlignCenter);

    m_stackedWidget->addWidget(m_scoreWidget);
}

void MainWindow::startGame()
{
    if (m_view) {
        m_stackedWidget->removeWidget(m_view);
        delete m_view;
        m_view = nullptr;
        m_scene = nullptr;
    }

    m_scene = new GameScene(this);
    m_view = new QGraphicsView(m_scene, this);
    
    connect(m_scene, &GameScene::gameOver, this, &MainWindow::onGameOver);

    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setBackgroundBrush(QBrush(QColor("#f0f0f0")));

    m_stackedWidget->addWidget(m_view);
    m_stackedWidget->setCurrentWidget(m_view);
    }
void MainWindow::onGameOver(int score)
{
    bool ok;
    QString filename = QInputDialog::getText(this, tr("Game Over"),
                                         tr("Enter your name for the savefile:"),
                                         QLineEdit::Normal, tr("Player"), &ok);
    if (ok && !filename.isEmpty()) {
        saveScore(filename, score);
    }
    
    showMenu();
}

void MainWindow::showHighScores()
{
    m_scoreList->clear();
    
    std::sort(m_scores.begin(), m_scores.end(), [](const ScoreEntry &a, const ScoreEntry &b) {
        return a.score > b.score;
    });

    for (const auto &entry : m_scores) {
        m_scoreList->addItem(QString("%1: %2").arg(entry.name).arg(entry.score));
    }
    
    m_stackedWidget->setCurrentWidget(m_scoreWidget);
}

void MainWindow::showMenu()
{
    m_stackedWidget->setCurrentWidget(m_menuWidget);
}

void MainWindow::saveScore(const QString &filename, int score)
{
    m_scores.append({filename, score});

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    QFile file(path + "/scores.json");
    
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray array;
        for (const auto &entry : m_scores) {
            QJsonObject obj;
            obj["name"] = entry.name;
            obj["score"] = entry.score;
            array.append(obj);
        }
        QJsonDocument doc(array);
        file.write(doc.toJson());
    }
}

void MainWindow::loadScores()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(path + "/scores.json");
    
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray array = doc.array();
        
        m_scores.clear();
        for (int i = 0; i < array.size(); ++i) {
            QJsonObject obj = array[i].toObject();
            m_scores.append({obj["name"].toString(), obj["score"].toInt()});
        }
    }
}

void MainWindow::quitGame()
{
    QApplication::quit();
}
