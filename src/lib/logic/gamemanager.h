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

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include "playerproperties.h"
#include "deck.h"

class GameManager : public QObject
{
    Q_OBJECT
public:
    enum GameStatus {
        Invalid,
        WaitingPlayers,
        PreparingGame,

    };

    explicit GameManager(QObject *parent = 0);

public slots:
    void start();
    void startGame();
    void stop();
    void addPlayer(QObject *handle, const QString &name);
    void removePlayer(QObject *handle);
    void chat(QObject *handle, const QString &message);
signals:
    void playersBroadcasted(const QList<PlayerProperties> &players, int pot);
    void playerRefused(QObject *handle);
    void chatSent(const QString &name, const QString &message);
    void cardsDistributed(QObject *handle, const QList<Card> &card);
private:
    int index(int i);
    void performBroadcastPlayers();
    void prepareRound();
    GameStatus m_status;
    int m_initialPlayer;
    int m_currentPlayer;
    QList<QObject *> m_handles;
    QMap<QObject *, PlayerProperties> m_playerProperties;
    QMap<QObject *, bool> m_playerReady;
    Deck m_deck;
    int m_pot;
};

#endif // GAMEMANAGER_H
