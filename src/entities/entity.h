﻿#ifndef ENTITY_H
#define ENTITY_H

#define SWAP_PIXMAPS_DELTA 150
#define BONUS_DURATION 7000
#define ENEMY_SWAP_DIRECTION_DELTA 1000
#define ENEMY_SHOOT_DELTA 1000
#define TANK_SPEED 4
#define BULLET_SPEED 6
#define SCORE_DURATION 2000

#include <QObject>
#include <QGraphicsObject>
#include <QMap>
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>
#include <QSound>

class Entity : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Entity(const QPixmap &pixmap = QPixmap(),
           QGraphicsItem *parentItem = nullptr,
           QObject *parent = nullptr);

    enum class Property : int8_t {
        Destructible = 0,
        CanBulletMovesThroughObject,
        CanActorMovesThroughObject,
        LivesLeft,
        RequireToDestroy,
        EntityName,
        Pickable,
        Picked,
        EntityType  // 新增实体类型属性
    };

    // 添加实体类型枚举
    enum class EntityType : int8_t {
        Unknown = 0,
        PlayerTank,
        EnemyTank,
        Bullet,
        StaticBlock,
        Bonus,
        Base,
        Explosion,
        Shield
    };


    bool isDestructible() const;
    void setDestructible(bool state);

    bool isBulletCanMoveThroughObject() const;
    void setBulletCanMoveThroughObject(bool state);

    bool isActorCanMoveThroughObject() const;
    void setActorCanMoveThroughObject(bool state);

    uint livesLeft() const;
    void setLivesLeft(uint livesLeft);

    bool isRequireToDestroy() const;
    void setRequireToDestroy(bool state = true);

    QString entityName() const;
    void setEntityName(const QString &name);

    virtual void takeDamage();

    void setBorderPoint(const QPointF& point);
    const QPointF& borderPoint() const;

    bool isPicked() const;
    void setPicked(bool state = true);

    bool isPickable() const;
    void setPickable(bool state);

    // 实体类型管理
    EntityType entityType() const;
    void setEntityType(EntityType type);

    // 添加便捷的类型检查方法
    bool isPlayerTank() const { return entityType() == EntityType::PlayerTank; }
    bool isEnemyTank() const { return entityType() == EntityType::EnemyTank; }
    bool isBullet() const { return entityType() == EntityType::Bullet; }
    bool isBonus() const { return entityType() == EntityType::Bonus; }
    bool isStaticBlock() const { return entityType() == EntityType::StaticBlock; }
    bool isBase() const { return entityType() == EntityType::Base; }
    bool isExplosion() const { return entityType() == EntityType::Explosion; }
    bool isShield() const { return entityType() == EntityType::Shield; }

    // 虚析构函数确保正确的多态销毁
    virtual ~Entity() = default;


signals:
    void picked(int type);
    void livesLeftChanged(int health);
    void aboutToBeDestroyed(); // 销毁前发出信号，让其他对象清理引用

protected:
    virtual void advance(int phase) override;

    void setProperty(const Property &key, const QVariant &value);
    const QVariant& getProperty(const Property &key) const;

private:
    QMap<Property, QVariant> m_properties;
    QPointF m_border;
};

inline QDebug operator<< (QDebug dbg, const Entity::Property &property)
{
    QString m_property;
    switch (property) {
        case Entity::Property::CanActorMovesThroughObject:
        {
            m_property = "Entity::Property::CanActorMovesThroughObject ";
            break;
        }
        case Entity::Property::CanBulletMovesThroughObject:
        {
            m_property = "Entity::Property::CanBulletMovesThroughObject ";
            break;
        }
        case Entity::Property::Destructible:
        {
            m_property = "Entity::Property::Destructible ";
            break;
        }
        case Entity::Property::EntityName:
        {
            m_property = "Entity::Property::EntityName ";
            break;
        }
        case Entity::Property::RequireToDestroy:
        {
            m_property = "Entity::Property::RequireToDestroy";
            break;
        }
        case Entity::Property::LivesLeft:
        {
            m_property = "Entity::Property::LivesLeft ";
            break;
        }
        case Entity::Property::Pickable:
        {
            m_property = "Entity::Property::Pickable ";
            break;
        }
        case Entity::Property::Picked:
        {
            m_property = "Entity::Property::Picked ";
            break;
        }
        case Entity::Property::EntityType:
        {
            m_property = "Entity::Property::EntityType ";
            break;
        }
    }
    dbg.nospace() << qUtf8Printable(m_property);

    return dbg.maybeSpace();
}

#endif // ENTITY_H
