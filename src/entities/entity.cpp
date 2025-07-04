#include "entity.h"

Entity::Entity(const QPixmap &pixmap, QGraphicsItem *parentItem, QObject *parent):
    QObject(parent), QGraphicsPixmapItem(pixmap, parentItem)
{
    setRequireToDestroy(false);
    setBorderPoint(QPointF(0, 0));
    setDestructible(false);
    setBulletCanMoveThroughObject(false);
    setActorCanMoveThroughObject(false);
    setLivesLeft(1);
    setPickable(false);
    setPicked(false);
    setEntityName("Entity");
    setEntityType(EntityType::Unknown); // 设置默认类型
}

void Entity::setProperty(const Entity::Property &key, const QVariant &value)
{
    m_properties.insert(key, value);
}

const QVariant& Entity::getProperty(const Entity::Property &key) const
{
    auto it = m_properties.find(key);
    if (it != m_properties.end()) {
        return it.value();
    }
    // 返回默认值避免崩溃
    static QVariant defaultValue;
    return defaultValue;
}

bool Entity::isDestructible() const
{
    return getProperty(Entity::Property::Destructible).toBool();
}

void Entity::setDestructible(bool state)
{
    setProperty(Entity::Property::Destructible, state);
}

bool Entity::isBulletCanMoveThroughObject() const
{
    return getProperty(Entity::Property::CanBulletMovesThroughObject).toBool();
}

void Entity::setBulletCanMoveThroughObject(bool state)
{
    setProperty(Entity::Property::CanBulletMovesThroughObject, state);
}

bool Entity::isActorCanMoveThroughObject() const
{
    return getProperty(Entity::Property::CanActorMovesThroughObject).toBool();
}

void Entity::setActorCanMoveThroughObject(bool state)
{
    setProperty(Entity::Property::CanActorMovesThroughObject, state);
}

uint Entity::livesLeft() const
{
    return getProperty(Entity::Property::LivesLeft).toUInt();
}

void Entity::setLivesLeft(uint livesLeft)
{
    setProperty(Entity::Property::LivesLeft, livesLeft);
    emit livesLeftChanged(livesLeft);
    if (livesLeft == 0) {
        setRequireToDestroy();
    }
}

bool Entity::isRequireToDestroy() const
{
    return getProperty(Entity::Property::RequireToDestroy).toBool();
}

void Entity::setRequireToDestroy(bool state)
{
    if (getProperty(Entity::Property::RequireToDestroy).toBool() != state) {
        setProperty(Entity::Property::RequireToDestroy, state);
        if (state) {
            emit aboutToBeDestroyed(); // 发出即将被销毁的信号
        }
    }
}

QString Entity::entityName() const
{
    return getProperty(Entity::Property::EntityName).toString();
}

void Entity::setEntityName(const QString &name)
{
    setProperty(Entity::Property::EntityName, name);
}

void Entity::takeDamage()
{
    uint lives = livesLeft();
    if (lives > 0) {
        lives--;
        setLivesLeft(lives);
    }
}

void Entity::setBorderPoint(const QPointF &point)
{
    m_border = point;
}

const QPointF &Entity::borderPoint() const
{
    return m_border;
}

bool Entity::isPicked() const
{
    return getProperty(Entity::Property::Picked).toBool();
}

void Entity::setPicked(bool state)
{
    setProperty(Entity::Property::Picked, state);
}

bool Entity::isPickable() const
{
    return getProperty(Entity::Property::Pickable).toBool();
}

void Entity::setPickable(bool state)
{
    setProperty(Entity::Property::Pickable, state);
}

Entity::EntityType Entity::entityType() const
{
    return static_cast<EntityType>(getProperty(Entity::Property::EntityType).toInt());
}

void Entity::setEntityType(EntityType type)
{
    setProperty(Entity::Property::EntityType, static_cast<int>(type));
}

void Entity::advance(int phase)
{
    if (phase) {
        if (isRequireToDestroy()) {
            // 使用 deleteLater() 避免在迭代过程中删除对象
            deleteLater();
        }
    }
}

