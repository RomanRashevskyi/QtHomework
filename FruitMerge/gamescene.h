#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include "fruititem.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

signals:
    void gameOver(int finalScore);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void updateLoop();

private:
    void applyPhysics();
    void checkCollisions();
    bool resolveCollision(FruitItem *a, FruitItem *b);
    void mergeFruits(FruitItem *a, FruitItem *b);
    void spawnFruit(const QPointF &pos, FruitItem::Tier tier);
    void updateScore(int points);
    
    
    void checkGameOver();
    void triggerGameOver();
    
    
    void setupUI();
    void updateNextPreview();
    void updateEvolutionCycle();
    void updateHeldFruit(const QPointF &mousePos);
    FruitItem::Tier getRandomSpawnTier();

    QTimer *m_gameTimer;
    QList<FruitItem*> m_fruits;
    
    
    QGraphicsTextItem *m_scoreText;
    QGraphicsTextItem *m_gameOverText;
    QGraphicsLineItem *m_dangerLine;
    
    
    QGraphicsTextItem *m_nextLabel;
    QGraphicsEllipseItem *m_nextPreviewItem;
    FruitItem::Tier m_nextTier;

    
    QGraphicsEllipseItem *m_heldFruitItem;
    QGraphicsLineItem *m_dropLine;
    FruitItem::Tier m_currentTier;
    
    
    QGraphicsTextItem *m_evolutionLabel;
    QList<QGraphicsEllipseItem*> m_evolutionIcons;
    
    int m_score = 0;
    int m_maxUnlockedTier = 0;
    bool m_isGameOver = false;
    
    
    static constexpr double RED_LINE_Y = 150.0;
    static constexpr int LOSS_THRESHOLD_MS = 1500; 
    static constexpr double STATIONARY_VELOCITY_LIMIT = 5.0;
    
    
    const qreal m_leftWall = 30 + 20;
    const qreal m_rightWall = 430;
    const qreal m_floor = 550;
    const qreal m_spawnY = 50;
    
    const qreal m_gravity = 800.0;
    const qreal m_friction = 0.98;
    const qreal m_restitution = 0.3;
};

#endif 
