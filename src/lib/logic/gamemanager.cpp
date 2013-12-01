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
#include "betmanager.h"


static const int INITIAL_TOKEN_COUNT = 1000;
static const int SMALL_BLIND = 10;
static const int BIG_BLIND = 20;
/// @todo TODO: We shouldn't put blinds and token count as constant.

GameManager::GameManager(QObject *parent) :
    QObject(parent), m_status(Invalid), m_middleCardsState(Initial), m_initialPlayer(-1)
    , m_currentPlayer(-1), m_pot(0) , m_betManager(new BetManager(this)), m_maxBetHandle(0)
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

void GameManager::performAction(QObject *handle, int tokenCount)
{
    if (tokenCount == -1) {
        m_playerProperties[handle].setInGame(false);
        m_playerProperties[handle].setBetCount(0);
    } else {
        const PlayerProperties &player = m_playerProperties.value(handle);
        m_playerProperties[handle].setBetCount(player.betCount() + tokenCount);
        m_playerProperties[handle].setTokenCount(player.tokenCount() - tokenCount);
        m_pot += tokenCount;

        // TODO: broadcast some messages, and add some checks here
    }

    performBroadcastPlayers();
    nextTurn();
}

int GameManager::index(int i)
{
    return (i % m_handles.count());
}

QList<PlayerProperties> GameManager::getPlayers() const
{
    QList<PlayerProperties> players;
    foreach (QObject *handle, m_handles) {
        players.append(m_playerProperties.value(handle));
    }

    return players;
}

void GameManager::performBroadcastPlayers()
{
    emit playersBroadcasted(getPlayers(), m_pot);
}

void GameManager::prepareRound()
{
    emit newRound();

    if (2 * m_handles.count() + 5 > m_deck.count()) {
        m_deck.reset();
        m_deck.shuffle();
    }

    m_middleCardsState = Initial;

    // All are in game
    foreach (QObject *handle, m_handles) {
        m_playerProperties[handle].setInGame(true);
    }

    // Distribute 2 cards to everybody
    foreach (QObject *handle, m_handles) {
        QList<Card> cards;
        cards.append(m_deck.draw());
        cards.append(m_deck.draw());
        m_hands[handle].addCards(cards);
        emit cardsDistributed(handle, cards);
    }

    m_initialPlayer = index(m_initialPlayer + 1);
    m_currentPlayer = index(m_initialPlayer + 2);

    m_betManager->setPlayers(getPlayers());

    // Take small and big blinds
    PlayerProperties &firstPlayer = m_playerProperties[m_handles[m_initialPlayer]];
    int smallBlind = qMin(SMALL_BLIND, m_betManager->maxBet());
    firstPlayer.setTokenCount(firstPlayer.tokenCount() - smallBlind);
    firstPlayer.setBetCount(firstPlayer.betCount() + smallBlind);

    PlayerProperties &secondPlayer = m_playerProperties[m_handles[index(m_initialPlayer + 1)]];
    int bigBlind = qMin(BIG_BLIND, m_betManager->maxBet());
    secondPlayer.setTokenCount(secondPlayer.tokenCount() - bigBlind);
    secondPlayer.setBetCount(secondPlayer.betCount() + bigBlind);

    m_pot = smallBlind + bigBlind;
    performBroadcastPlayers();

    m_maxBetHandle = 0;

    if (smallBlind < bigBlind) {
        m_maxBetHandle = m_handles[index(m_initialPlayer + 1)];
    } else {
        m_maxBetHandle = m_handles[m_initialPlayer];
    }

    qDebug() << m_currentPlayer << m_handles;

    emit playerTurnSelected(m_handles[m_currentPlayer]);
}

void GameManager::nextTurn()
{
    qDebug() << "Current player (who finished): " << m_currentPlayer;


    // Check if the game is finished
    int inGameCount = 0;
    foreach (QObject *handle, m_handles) {
        const PlayerProperties &player = m_playerProperties.value(handle);
        if (player.isInGame()) {
            inGameCount ++;
        }
    }

    // Only one player left: he / she wins
    if (inGameCount == 1) {
        cleanUpRound();
    }

    // Compute best player
    foreach (QObject *handle, m_handles) {
        const PlayerProperties &best = m_playerProperties.value(m_maxBetHandle);
        const PlayerProperties &player = m_playerProperties.value(handle);
        if (best.betCount() < player.betCount()) {
            m_maxBetHandle = handle;
        }
    }

    // We should reveal newer cards if all players have the same amount bet
    // and if we reached the first player who have done the bet

    bool equalBetReached = true;
    int maxBet = m_playerProperties.value(m_handles[m_currentPlayer]).betCount();
    foreach (const PlayerProperties &player, m_playerProperties) {
        if (player.isInGame() && player.betCount() != maxBet) {
            equalBetReached = false;
        }
    }

    if (m_maxBetHandle == m_handles[m_currentPlayer] && equalBetReached) {
        qDebug() << "Player with max bet reached";
        switch (m_middleCardsState) {
        case Initial:
            distributeMiddleCards(3);
            m_middleCardsState = Flop;
            break;
        case Flop:
            distributeMiddleCards(1);
            m_middleCardsState = Turn;
            break;
        case Turn:
            distributeMiddleCards(1);
            m_middleCardsState = River;
            break;
        case River:
            manageDraw();
            break;
        }
    }

    // Avance to next player
    int indexNext = index(m_currentPlayer + 1);
    bool ok = m_playerProperties.value(m_handles[indexNext]).isInGame();
    while (!ok && indexNext != m_currentPlayer) {
        indexNext = index(indexNext + 1);
        ok = m_playerProperties.value(m_handles[indexNext]).isInGame();
    }

    qDebug() << "Current max" << m_maxBetHandle;
    qDebug() << "Next player selected" << indexNext
             << m_playerProperties.value(m_handles[indexNext]).name() << m_handles[indexNext];
    m_currentPlayer = indexNext;

    emit playerTurnSelected(m_handles[m_currentPlayer]);
}

void GameManager::cleanUpRound()
{
    // Give pot to winner
    PlayerProperties &player = m_playerProperties[m_maxBetHandle];
    player.setTokenCount(player.tokenCount() + m_pot);

    m_pot = 0;

    for (QMap<QObject *, PlayerProperties>::iterator i = m_playerProperties.begin();
         i != m_playerProperties.end(); ++i) {
        i.value().setBetCount(0);
    }

    // Cleanup hands
    m_hands.clear();
    emit endRound();

    performBroadcastPlayers();

    // Restart a new round
    prepareRound();
}

void GameManager::manageDraw()
{
    // First, we should send the cards everybody had
    // (except those who hold) to all the clients
    QList<Hand> hands;
    foreach (QObject *handle, m_handles) {
        const PlayerProperties &player = m_playerProperties.value(handle);
        if (player.isInGame()) {
            hands.append(m_hands.value(handle));
        } else {
            hands.append(Hand());
        }
    }

    emit allCardsSent(hands);

    // Let's compare all the hands
    QMap<Hand, QObject *> handsHandle;

    // We use a QMap that automatically sorts by key,
    // so the last element of handsHandle is the winner
    foreach (QObject *handle, m_handles) {
        const Hand &hand = m_hands.value(handle);
        handsHandle.insert(hand, handle);
    }

    QObject *winner = handsHandle.value(handsHandle.keys().last());

    // We cheat: we pass winner as m_maxBetHandle, so cleanUpRound
    // will give the tokens to the winner, and not the the one who has
    // the highest bet
    m_maxBetHandle = winner;
    cleanUpRound();
}

void GameManager::distributeMiddleCards(int count)
{
    QList<Card> cards;
    for (int i = 0; i < count; i++) {
        cards.append(m_deck.draw());
    }

    foreach (QObject *handle, m_handles) {
        m_hands[handle].addCards(cards);
    }

    emit cardsDistributed(cards);
}
