/*
 * Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "gamemanager.h"
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

static const int INITIAL_TOKEN_COUNT = 1000;
static const int SMALL_BLEND = 10;
static const int BIG_BLEND = 20;


GameManager::GameManager(QObject *parent) :
    QObject(parent), m_status(Invalid), m_initialPlayer(-1), m_currentPlayer(-1), m_pot(0)
{
}

void GameManager::start()
{
    m_status = WaitingPlayers;
}

void GameManager::startGame()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    m_status = PreparingGame;
    m_initialPlayer = qrand() % m_handles.count();
    prepareRound();
}

void GameManager::stop()
{
    m_status = Invalid;
}

void GameManager::addPlayer(QObject *handle, const QString &name)
{
    if (m_status != WaitingPlayers) {
        emit playerRefused(handle);
    }

    if (m_playerProperties.contains(handle)) {
        qDebug() << Q_FUNC_INFO << "Player already registered for handle"
                 << handle << "and name" << name;
        return;
    }

    m_handles.append(handle);

    PlayerProperties properties;
    properties.setName(name);
    properties.setTokenCount(INITIAL_TOKEN_COUNT);
    m_playerProperties.insert(handle, properties);

    performBroadcastPlayers();
}

void GameManager::removePlayer(QObject *handle)
{
    if (!m_playerProperties.contains(handle)) {
        qDebug() << Q_FUNC_INFO << "Player not registered for handle" << handle;
        return;
    }

    m_handles.removeAll(handle);
    m_playerProperties.remove(handle);

    performBroadcastPlayers();
}

void GameManager::chat(QObject *handle, const QString &message)
{
    if (!m_playerProperties.contains(handle)) {
        return;
    }
    emit chatSent(m_playerProperties.value(handle).name(), message);
}

int GameManager::index(int i)
{
    return (i % m_handles.count());
}

void GameManager::performBroadcastPlayers()
{
    QList<PlayerProperties> players;
    foreach (QObject *handle, m_handles) {
        players.append(m_playerProperties.value(handle));
    }

    emit playersBroadcasted(players, m_pot);
}

void GameManager::prepareRound()
{
    if (2 * m_handles.count() + 5 > m_deck.count()) {
        m_deck.reset();
        m_deck.shuffle();
    }

    // Distribute 2 cards to everybody
    foreach (QObject *handle, m_handles) {
        QList<Card> cards;
        cards.append(m_deck.draw());
        cards.append(m_deck.draw());
        emit cardsDistributed(handle, cards);
    }

    m_initialPlayer = index(m_initialPlayer + 1);
    m_currentPlayer = index(m_initialPlayer + 2);

    // Take small and big blends
    PlayerProperties &firstPlayer = m_playerProperties[m_handles[index(m_initialPlayer)]];
    int smallBlend = qMin(SMALL_BLEND, firstPlayer.tokenCount());
    firstPlayer.setTokenCount(firstPlayer.tokenCount() - smallBlend);

    PlayerProperties &secondPlayer = m_playerProperties[m_handles[index(m_initialPlayer + 1)]];
    int bigBlend = qMin(BIG_BLEND, secondPlayer.tokenCount());
    secondPlayer.setTokenCount(secondPlayer.tokenCount() - bigBlend);


    m_pot = smallBlend + bigBlend;
    performBroadcastPlayers();
}
