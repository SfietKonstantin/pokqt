#ifndef BETMANAGER_H
#define BETMANAGER_H

#include <QtCore/QObject>
#include "playerproperties.h"

class BetManager: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int minBet READ minBet NOTIFY minBetChanged)
    Q_PROPERTY(int maxBet READ maxBet NOTIFY maxBetChanged)
public:
    explicit BetManager(QObject *parent = 0);
    void setPlayers(const QList<PlayerProperties> &players);
    int minBet() const;
    int maxBet() const;
signals:
    void minBetChanged();
    void maxBetChanged();
private:
    int m_minBet;
    int m_maxBet;
    QList<PlayerProperties> m_players;
};

#endif // BETMANAGER_H
