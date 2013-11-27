#include "betmanager.h"

BetManager::BetManager(QObject *parent)
    : QObject(parent)
{
}

int BetManager::minBet() const
{
    return m_minBet;
}

int BetManager::maxBet() const
{
    return m_maxBet;
}

void BetManager::setPlayers(const QList<PlayerProperties> &players)
{
    int minBet = 0;
    int maxBet = INT_MAX;
    foreach (const PlayerProperties &player, players) {
        minBet = qMax(minBet, player.betCount());
        maxBet = qMin(maxBet, player.tokenCount() + player.betCount());
    }

    if (m_minBet != minBet) {
        m_minBet = minBet;
        emit minBetChanged();
    }

    if (m_maxBet != maxBet) {
        m_maxBet = maxBet;
        emit maxBetChanged();
    }
}

