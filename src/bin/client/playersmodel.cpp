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

#include "playersmodel.h"
#include <QtCore/QDebug>

PlayersModel::PlayersModel(QObject *parent) :
    QAbstractListModel(parent), m_client(0)
{
}

QHash<int, QByteArray> PlayersModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(NameRole, "name");
    roles.insert(TokenCountRole, "tokenCount");
    return roles;
}

int PlayersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

int PlayersModel::count() const
{
    return rowCount();
}

NetworkClient * PlayersModel::client() const
{
    return m_client;
}

void PlayersModel::setClient(NetworkClient *client)
{
    if (m_client != client) {

        // Disconnect old client
        if (m_client) {
            disconnect(m_client, &NetworkClient::playersChanged, this, &PlayersModel::slotPlayersChanged);
        }

        m_client = client;
        connect(m_client, &NetworkClient::playersChanged, this, &PlayersModel::slotPlayersChanged);
        emit clientChanged();
    }
}

QVariant PlayersModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    const PlayerProperties &playerProperties = m_data.at(row);
    switch (role) {
    case NameRole:
        return playerProperties.name();
        break;
    case TokenCountRole:
        return playerProperties.tokenCount();
        break;
    default:
        return QVariant();
        break;
    }
}

void PlayersModel::slotPlayersChanged()
{
    int selfIndex = m_client->index();
    QList<PlayerProperties> players = m_client->players();
    int oldCount = rowCount();
    int newCount = players.count() - 1; // We remove "self" from the list
    if (oldCount > newCount) {
        beginRemoveRows(QModelIndex(), newCount, oldCount - 1);
        while (m_data.count() > newCount) {
            m_data.removeLast();
        }
        emit countChanged();
        endRemoveRows();
    } else if (oldCount < newCount) {
        beginInsertRows(QModelIndex(), oldCount, newCount - 1);
        while (m_data.count() < newCount) {
            m_data.append(PlayerProperties());
        }
        emit countChanged();
        endInsertRows();
    }

    // Populate data
    for (int i = 0; i < newCount; i++) {
        int trueIndex = (selfIndex + i + 1) % (newCount + 1); // i = 0, trueIndex = index + 1 (player after "self")
        m_data[i] = players.at(trueIndex);
    }

    emit dataChanged(index(0), index(newCount - 1));
}
