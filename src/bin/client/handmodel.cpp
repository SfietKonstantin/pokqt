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
 * @file handmodel.cpp
 * @short Implementation of Client::HandModel
 */

#include "handmodel.h"

namespace Client
{

/**
 * @internal
 * @brief Get the type of card based on index
 * @param index index of the card.
 * @return type of the card.
 */
inline static QVariant getCardType(int index)
{
    if (index < 0 || index >= 7) {
        return QVariant();
    }

    if (index < 2) {
        return HandModel::Hand;
    }

    if (index < 5) {
        return HandModel::Flop;
    }

    if (index < 6) {
        return HandModel::Turn;
    }

    if (index < 7) {
        return HandModel::River;
    }

    return QVariant();
}

HandModel::HandModel(QObject *parent) :
    QAbstractListModel(parent), m_client(0)
{
}

QHash<int, QByteArray> HandModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(CardRole, "card");
    roles.insert(CardTypeRole, "cardType");
    return roles;
}

int HandModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

int HandModel::count() const
{
    return rowCount();
}

NetworkClient * HandModel::client() const
{
    return m_client;
}

void HandModel::setClient(NetworkClient *client)
{
    if (m_client != client) {
        // Disconnect old client
        if (m_client) {
            disconnect(m_client, &NetworkClient::handChanged, this, &HandModel::slotHandChanged);
        }

        m_client = client;
        connect(m_client, &NetworkClient::handChanged, this, &HandModel::slotHandChanged);
        emit clientChanged();
    }
}

QVariant HandModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    switch (role) {
    case CardRole:
        return QVariant::fromValue(m_data.value(row));
        break;
    case CardTypeRole:
        return getCardType(row);
        break;
    default:
        return QVariant();
        break;
    }
}

void HandModel::slotHandChanged()
{
    if (!m_data.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        while (!m_data.isEmpty()) {
            m_data.takeLast()->deleteLater();
        }
        endRemoveRows();
    }

    const QList<Card> &cards = m_client->hand().cards();

    beginInsertRows(QModelIndex(), 0, cards.count() - 1);

    foreach (const Card &card, cards) {
        m_data.append(CardObject::create(card.suit(), card.rank(), this));
    }

    emit countChanged();
    endInsertRows();
}

}
