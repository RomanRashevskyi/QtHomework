#include "gamescene.h"
#include <QGraphicsSceneMouseEvent>
#include <cmath>
#include <QRandomGenerator>
#include <QFont>
#include <QPen>
#include <QBrush>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_gameTimer(new QTimer(this))
    , m_scoreText(new QGraphicsTextItem())
    , m_gameOverText(nullptr)
    , m_nextLabel(nullptr)
    , m_nextPreviewItem(nullptr)
    , m_evolutionLabel(nullptr)
    , m_heldFruitItem(nullptr)
    , m_dropLine(nullptr)
{
    setSceneRect(0, 0, 600, 600);
    setupUI();
    
    m_currentTier = getRandomSpawnTier();
    m_nextTier = getRandomSpawnTier();
    
    updateNextPreview();
    updateEvolutionCycle();
    updateHeldFruit(QPointF((m_leftWall + m_rightWall) / 2.0, m_spawnY));
    
    
    QLinearGradient bgGradient(0, 0, 0, 600);
    bgGradient.setColorAt(0, QColor("#e0f7fa"));
    bgGradient.setColorAt(1, QColor("#80deea"));
    setBackgroundBrush(bgGradient);

    connect(m_gameTimer, &QTimer::timeout, this, &GameScene::updateLoop);
    m_gameTimer->start(16);
}

void GameScene::setupUI()
{
    m_scoreText->setPlainText("Score: 0");
    m_scoreText->setDefaultTextColor(QColor("#006064"));
    m_scoreText->setFont(QFont("Segoe UI", 18, QFont::Bold));
    m_scoreText->setPos(10, 10);
    m_scoreText->setZValue(10);
    addItem(m_scoreText);

    
    QLinearGradient wallGradient(m_leftWall - 20, 0, m_rightWall + 20, 0);
    wallGradient.setColorAt(0, QColor("#546e7a"));
    wallGradient.setColorAt(1, QColor("#263238"));

    addRect(m_leftWall - 20, 100, 20, 450, QPen(Qt::NoPen), QBrush(wallGradient));
    addRect(m_rightWall, 100, 20, 450, QPen(Qt::NoPen), QBrush(wallGradient));
    addRect(m_leftWall - 20, m_floor, (m_rightWall - m_leftWall) + 40, 20, QPen(Qt::NoPen), QBrush(wallGradient));

    m_dangerLine = addLine(m_leftWall, RED_LINE_Y, m_rightWall, RED_LINE_Y, QPen(QColor("#ff5252"), 3, Qt::DashLine));
    m_dangerLine->setZValue(5);

    qreal uiX = 480;
    m_nextLabel = new QGraphicsTextItem("Next:");
    m_nextLabel->setFont(QFont("Segoe UI", 14, QFont::Bold));
    m_nextLabel->setDefaultTextColor(QColor("#006064"));
    m_nextLabel->setPos(uiX, 20);
    addItem(m_nextLabel);

    m_nextPreviewItem = new QGraphicsEllipseItem();
    m_nextPreviewItem->setPos(uiX + 30, 80);
    addItem(m_nextPreviewItem);

    m_evolutionLabel = new QGraphicsTextItem("Evolution:");
    m_evolutionLabel->setFont(QFont("Segoe UI", 12, QFont::Bold));
    m_evolutionLabel->setDefaultTextColor(QColor("#006064"));
    m_evolutionLabel->setPos(uiX, 150);
    addItem(m_evolutionLabel);

    const int startY = 180;
    const int spacing = 35;
    for (int i = 0; i < static_cast<int>(FruitItem::Tier::Count); ++i) {
        QGraphicsEllipseItem *icon = new QGraphicsEllipseItem();
        qreal r = 10 + i * 1.5; 
        icon->setRect(-r, -r, r * 2, r * 2);
        icon->setPos(uiX + 40, startY + i * spacing);
        addItem(icon);
        m_evolutionIcons.append(icon);
    }

    m_heldFruitItem = new QGraphicsEllipseItem();
    m_heldFruitItem->setZValue(20);
    addItem(m_heldFruitItem);

    m_dropLine = new QGraphicsLineItem();
    m_dropLine->setPen(QPen(QColor(0, 0, 0, 50), 1, Qt::DashLine));
    m_dropLine->setZValue(15);
    addItem(m_dropLine);
}

void GameScene::updateHeldFruit(const QPointF &mousePos)
{
    if (m_isGameOver) return;
    qreal x = mousePos.x();
    FruitItem temp(m_currentTier);
    qreal r = temp.radius();
    if (x < m_leftWall + r) x = m_leftWall + r;
    if (x > m_rightWall - r) x = m_rightWall - r;
    m_heldFruitItem->setRect(temp.rect());
    m_heldFruitItem->setBrush(temp.brush());
    m_heldFruitItem->setPen(temp.pen());
    m_heldFruitItem->setPos(x, m_spawnY);
    m_dropLine->setLine(x, m_spawnY, x, m_floor);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    updateHeldFruit(event->scenePos());
    QGraphicsScene::mouseMoveEvent(event);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isGameOver) return;
    if (event->scenePos().x() > m_rightWall + 20) return;
    spawnFruit(m_heldFruitItem->pos(), m_currentTier);
    m_currentTier = m_nextTier;
    m_nextTier = getRandomSpawnTier();
    updateNextPreview();
    updateHeldFruit(event->scenePos());
}

void GameScene::updateLoop()
{
    if (m_isGameOver) return;
    
    applyPhysics();
    checkCollisions();
    
    
    checkGameOver();
}

void GameScene::applyPhysics()
{
    const qreal dt = 0.016;
    for (FruitItem *fruit : m_fruits) {
        QPointF vel = fruit->velocity();
        vel.setY(vel.y() + m_gravity * dt);
        fruit->setVelocity(vel);
        fruit->updatePhysics(dt);
        
        if (fruit->pos().y() + fruit->radius() > m_floor) {
            fruit->setY(m_floor - fruit->radius());
            vel = fruit->velocity();
            vel.setY(-vel.y() * m_restitution);
            vel.setX(vel.x() * m_friction);
            fruit->setVelocity(vel);
        }
        if (fruit->pos().x() - fruit->radius() < m_leftWall) {
            fruit->setX(m_leftWall + fruit->radius());
            vel = fruit->velocity();
            vel.setX(-vel.x() * m_restitution);
            fruit->setVelocity(vel);
        } else if (fruit->pos().x() + fruit->radius() > m_rightWall) {
            fruit->setX(m_rightWall - fruit->radius());
            vel = fruit->velocity();
            vel.setX(-vel.x() * m_restitution);
            fruit->setVelocity(vel);
        }
        
        
        fruit->updateAboveLineState(fruit->topY() < RED_LINE_Y, 16);
    }
}

void GameScene::checkCollisions()
{
    bool mergeHappened = true;
    while (mergeHappened) {
        mergeHappened = false;
        for (int i = 0; i < m_fruits.size(); ++i) {
            for (int j = i + 1; j < m_fruits.size(); ++j) {
                if (resolveCollision(m_fruits[i], m_fruits[j])) {
                    mergeHappened = true;
                    break;
                }
            }
            if (mergeHappened) break;
        }
    }
}

bool GameScene::resolveCollision(FruitItem *a, FruitItem *b)
{
    if (a->isMerging() || b->isMerging()) return false;
    QPointF delta = b->pos() - a->pos();
    qreal distanceSq = delta.x() * delta.x() + delta.y() * delta.y();
    qreal minDistance = a->radius() + b->radius();
    if (distanceSq < minDistance * minDistance) {
        if (a->tier() == b->tier() && a->tier() != FruitItem::Tier::Watermelon) {
            mergeFruits(a, b);
            return true;
        }
        qreal distance = std::sqrt(distanceSq);
        if (distance == 0) return false;
        QPointF normal = delta / distance;
        qreal overlap = minDistance - distance;
        a->setPos(a->pos() - normal * (overlap / 2.0));
        b->setPos(b->pos() + normal * (overlap / 2.0));
        QPointF relativeVel = b->velocity() - a->velocity();
        qreal velAlongNormal = relativeVel.x() * normal.x() + relativeVel.y() * normal.y();
        if (velAlongNormal > 0) return false;
        qreal j = -(1 + m_restitution) * velAlongNormal;
        j /= 2.0;
        QPointF impulse = j * normal;
        a->setVelocity(a->velocity() - impulse);
        b->setVelocity(b->velocity() + impulse);
    }
    return false;
}

void GameScene::mergeFruits(FruitItem *a, FruitItem *b)
{
    a->setMerging(true);
    b->setMerging(true);
    QPointF midpoint = (a->pos() + b->pos()) / 2.0;
    int nextTierIdx = a->tierIndex() + 1;
    FruitItem::Tier nextTier = static_cast<FruitItem::Tier>(nextTierIdx);
    updateScore((nextTierIdx + 1) * 2);
    if (nextTierIdx > m_maxUnlockedTier) {
        m_maxUnlockedTier = nextTierIdx;
        updateEvolutionCycle();
    }
    m_fruits.removeOne(a);
    m_fruits.removeOne(b);
    removeItem(a);
    removeItem(b);
    delete a;
    delete b;
    spawnFruit(midpoint, nextTier);
}

void GameScene::spawnFruit(const QPointF &pos, FruitItem::Tier tier)
{
    FruitItem *fruit = new FruitItem(tier);
    fruit->setPos(pos);
    m_fruits.append(fruit);
    addItem(fruit);
    fruit->playPopAnimation();
}

void GameScene::updateScore(int points)
{
    m_score += points;
    m_scoreText->setPlainText(QString("Score: %1").arg(m_score));
}

void GameScene::checkGameOver()
{
    for (FruitItem *fruit : m_fruits) {
        
        if (fruit->topY() < RED_LINE_Y) {
            
            bool isStationary = fruit->velocity().manhattanLength() < STATIONARY_VELOCITY_LIMIT;
            bool overstayed = fruit->timeAboveLine() > LOSS_THRESHOLD_MS;
            
            
            if (fruit->age() > 1000 && (isStationary || overstayed)) {
                triggerGameOver();
                return;
            }
        }
    }
}

void GameScene::triggerGameOver()
{
    m_isGameOver = true;
    m_gameTimer->stop();
    m_heldFruitItem->hide();
    m_dropLine->hide();
    m_gameOverText = new QGraphicsTextItem("GAME OVER");
    m_gameOverText->setDefaultTextColor(Qt::red);
    m_gameOverText->setFont(QFont("Arial", 32, QFont::Bold));
    QRectF br = m_gameOverText->boundingRect();
    m_gameOverText->setPos(sceneRect().width()/2 - br.width()/2, sceneRect().height()/2 - br.height()/2);
    m_gameOverText->setZValue(100);
    addItem(m_gameOverText);
    emit gameOver(m_score);
}

FruitItem::Tier GameScene::getRandomSpawnTier()
{
    int maxSpawnable = std::min(m_maxUnlockedTier, 2);
    int tierIdx = QRandomGenerator::global()->bounded(maxSpawnable + 1);
    return static_cast<FruitItem::Tier>(tierIdx);
}

void GameScene::updateNextPreview()
{
    FruitItem temp(m_nextTier);
    m_nextPreviewItem->setRect(temp.rect());
    m_nextPreviewItem->setBrush(temp.brush());
    m_nextPreviewItem->setPen(temp.pen());
}

void GameScene::updateEvolutionCycle()
{
    for (int i = 0; i < m_evolutionIcons.size(); ++i) {
        FruitItem::Tier tier = static_cast<FruitItem::Tier>(i);
        FruitItem temp(tier);
        if (i <= m_maxUnlockedTier) m_evolutionIcons[i]->setBrush(temp.brush());
        else m_evolutionIcons[i]->setBrush(QBrush(Qt::black));
        m_evolutionIcons[i]->setPen(temp.pen());
    }
}
