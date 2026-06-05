#include "fruititem.h"
#include <QPen>
#include <QBrush>
#include <QRadialGradient>

FruitItem::FruitItem(Tier tier, QGraphicsItem *parent)
    : QObject(), QGraphicsEllipseItem(parent)
    , m_tier(tier)
    , m_velocity(0, 0)
{
    const auto &props = getProperties(tier);
    m_radius = props.radius;
    
    setRect(-m_radius, -m_radius, m_radius * 2, m_radius * 2);
    updateAppearance();
    
    m_ageTimer.start();
}

void FruitItem::updateAppearance()
{
    const auto &props = getProperties(m_tier);
    
    
    QRadialGradient gradient(-m_radius * 0.3, -m_radius * 0.3, m_radius * 1.5);
    gradient.setColorAt(0, props.lightColor);
    gradient.setColorAt(0.5, props.color);
    gradient.setColorAt(1, props.color.darker(150));
    
    setBrush(QBrush(gradient));
    setPen(QPen(props.color.darker(200), 2));
}

void FruitItem::setScale(qreal s)
{
    m_scale = s;
    setTransform(QTransform::fromScale(s, s));
}

void FruitItem::playPopAnimation()
{
    QPropertyAnimation *anim = new QPropertyAnimation(this, "scale");
    anim->setDuration(200);
    anim->setStartValue(0.1);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void FruitItem::updatePhysics(qreal deltaTime)
{
    setPos(pos() + m_velocity * deltaTime);
}

void FruitItem::updateAboveLineState(bool above, qint64 deltaTimeMs)
{
    if (above) {
        m_timeAboveLineMs += deltaTimeMs;
    } else {
        m_timeAboveLineMs = 0;
    }
}

const FruitItem::TierProperties& FruitItem::getProperties(Tier tier)
{
    static const TierProperties props[] = {
        {15, QColor("#FF2D2D"), QColor("#FF9B9B")}, 
        {22, QColor("#FF4D4D"), QColor("#FFBABA")}, 
        {30, QColor("#9B51E0"), QColor("#D6A6F5")}, 
        {38, QColor("#F2994A"), QColor("#FFCC99")}, 
        {48, QColor("#F2C94C"), QColor("#FFF0B3")}, 
        {60, QColor("#EB5757"), QColor("#F5B7B7")}, 
        {72, QColor("#27AE60"), QColor("#A8E6CF")}, 
        {85, QColor("#FF85A2"), QColor("#FFD1DC")}, 
        {100, QColor("#F2994A"), QColor("#FCD19C")}, 
        {120, QColor("#6FCF97"), QColor("#D1F2D1")}, 
        {150, QColor("#219653"), QColor("#A9DFBF")}  
    };
    return props[static_cast<int>(tier)];
}
