#ifndef FRUITITEM_H
#define FRUITITEM_H

#include <QGraphicsEllipseItem>
#include <QColor>
#include <QPointF>
#include <QElapsedTimer>
#include <QPropertyAnimation>

class FruitItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    enum class Tier {
        Cherry = 0, Strawberry, Grape, Dekopon, Persimmon,
        Apple, Pear, Peach, Pineapple, Melon, Watermelon, Count
    };

    explicit FruitItem(Tier tier, QGraphicsItem *parent = nullptr);

    Tier tier() const { return m_tier; }
    int tierIndex() const { return static_cast<int>(m_tier); }
    
    void setVelocity(const QPointF &velocity) { m_velocity = velocity; }
    QPointF velocity() const { return m_velocity; }
    
    void updatePhysics(qreal deltaTime);
    qreal radius() const { return m_radius; }

    bool isMerging() const { return m_isMerging; }
    void setMerging(bool merging) { m_isMerging = merging; }

    double topY() const { return y() - (m_radius * m_scale); }
    qint64 age() const { return m_ageTimer.elapsed(); }

    void updateAboveLineState(bool above, qint64 deltaTimeMs);
    qint64 timeAboveLine() const { return m_timeAboveLineMs; }

    
    qreal scale() const { return m_scale; }
    void setScale(qreal s);

    void playPopAnimation();

private:
    Tier m_tier;
    qreal m_radius;
    QPointF m_velocity;
    bool m_isMerging = false;
    QElapsedTimer m_ageTimer;
    qreal m_scale = 1.0;
    
    qint64 m_timeAboveLineMs = 0;

    struct TierProperties {
        qreal radius;
        QColor color;
        QColor lightColor;
    };
    static const TierProperties& getProperties(Tier tier);
    void updateAppearance();
};

#endif 
