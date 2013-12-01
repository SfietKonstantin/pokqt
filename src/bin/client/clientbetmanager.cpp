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

/**
 * @file clientbetmanager.cpp
 * @short Implementation of Client::ClientBetManager
 */

#include "clientbetmanager.h"
#include <QtCore/QDebug>

namespace Client
{

ClientBetManager::ClientBetManager(QObject *parent) :
    BetManager(parent), m_check(false), m_client(0)
{
}

NetworkClient * ClientBetManager::client() const
{
    return m_client;
}

void ClientBetManager::setClient(NetworkClient *client)
{
    if (m_client != client) {

        // Disconnect old client
        if (m_client) {
            disconnect(m_client, &NetworkClient::playersChanged,
                       this, &ClientBetManager::slotPlayersChanged);
            disconnect(this, &ClientBetManager::betSent, m_client, &NetworkClient::sendBet);
            disconnect(this, &ClientBetManager::foldSent, m_client, &NetworkClient::sendFold);
        }

        m_client = client;
        connect(m_client, &NetworkClient::playersChanged,
                this, &ClientBetManager::slotPlayersChanged);
        connect(this, &ClientBetManager::betSent, m_client, &NetworkClient::sendBet);
        connect(this, &ClientBetManager::foldSent, m_client, &NetworkClient::sendFold);
        emit clientChanged();
    }
}

bool ClientBetManager::isCheck() const
{
    return m_check;
}

bool ClientBetManager::raise(int raisedTokenCount)
{
    if (!m_client) {
        return false;
    }

    const PlayerProperties &player = m_client->players().at(m_client->index());
    if (player.betCount() + raisedTokenCount < minBet()
        || player.betCount() + raisedTokenCount > maxBet()
        || player.betCount() + raisedTokenCount > player.tokenCount()) {
        return false;
    }

    emit betSent(raisedTokenCount);
    return true;
}

bool ClientBetManager::call()
{
    if (!m_client) {
        return false;
    }

    const PlayerProperties &player = m_client->players().at(m_client->index());
    if (player.tokenCount() + player.betCount() < minBet()) {
        // Should never happen
        return false;
    }

    emit betSent(minBet() - player.betCount());
    return true;
}

void ClientBetManager::fold()
{
    if (!m_client) {
        return;
    }

    emit foldSent();
}

void ClientBetManager::slotPlayersChanged()
{
    setPlayers(m_client->players());

    const PlayerProperties &player = m_client->players().at(m_client->index());

    bool check = player.betCount() >= minBet();
    if (m_check != check) {
        m_check = check;
        emit checkChanged();
    }
}

}
