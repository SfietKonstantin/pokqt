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

#include "networkclient.h"
#include "osignal.h"
#include <QtCore/QDataStream>
#include <QtNetwork/QHostAddress>
#include "logic/card.h"

NetworkClient::NetworkClient(QObject *parent) :
    QObject(parent), m_status(NotConnected), m_index(-1), m_pot(0), m_turn(false)
    , m_nextMessageSize(-1)
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &NetworkClient::slotConnected);
    connect(m_socket, OSIGNAL(QTcpSocket, error, QAbstractSocket::SocketError),
            this, &NetworkClient::slotError);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkClient::slotReadyRead);
}

NetworkClient::Status NetworkClient::status() const
{
    return m_status;
}

QString NetworkClient::name() const
{
    return m_name;
}

void NetworkClient::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();

        if (m_status != NotConnected) {
            qWarning() << "Name will not be changed before next connection";
        }
    }
}

bool NetworkClient::turn() const
{
    return m_turn;
}

QList<PlayerProperties> NetworkClient::players() const
{
    return m_players;
}

int NetworkClient::index() const
{
    return m_index;
}

int NetworkClient::pot() const
{
    return m_pot;
}

Hand NetworkClient::hand() const
{
    return m_hand;
}

void NetworkClient::connectToHost(const QString &host, int port)
{
    setStatus(Connecting);
    m_socket->connectToHost(QHostAddress(host), port);
}

void NetworkClient::disconnectFromHost()
{
    setStatus(NotConnected);
    m_socket->disconnectFromHost();
    setPlayers(QList<PlayerProperties>(), -1, 0);
    m_index = -1;
    m_players.clear();
}

void NetworkClient::sendChat(const QString &chat)
{
    sendMessageString(m_socket, ChatType, chat);
}

void NetworkClient::sendBet(int tokenCount)
{
    QByteArray data;
    QDataStream stream (&data, QIODevice::WriteOnly);
    stream << tokenCount;
    sendMessage(m_socket, ActionType, data);
}

void NetworkClient::sendFold()
{
    sendBet(-1);
}

void NetworkClient::setStatus(Status status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

void NetworkClient::setPlayers(const QList<PlayerProperties> &players, int index, int pot)
{
    if (m_index != index || m_pot != pot || m_players != players) {
        m_players = players;
        m_index = index;
        m_pot = pot;

        emit playersChanged(); // TODO: rename to pot
    }
}

void NetworkClient::reply(MessageType type, const QByteArray &data)
{
    switch (type) {
    case PlayerType: {
            QDataStream stream (data);
            int index;
            QList<PlayerProperties> players;
            int pot;
            stream >> index >> players >> pot;
            if (m_status == Registering) {
                QString playerName = players.value(index).name();
                if (m_name != playerName) {
                    qWarning() << "Nickname receivend from server do not match";
                    qWarning() << "You might have changed nickname before being registered";
                    setName(playerName);
                }

                setStatus(Connected);
            }

            qDebug() << "Player list count" << players.count();
            qDebug() << "Your index" << index;
            qDebug() << "Current pot" << pot;

            setPlayers(players, index, pot);
        }
        break;
    case ChatType: {
            QDataStream stream (data);
            QString name;
            QString chat;
            stream >> name >> chat;
            emit chatReceived(name, chat);
        }
        break;
    case NewRoundType: {
            m_hand.clear();
            emit handChanged();
        }
        break;
    case CardsType: {
            QDataStream stream (data);
            QList<Card> cards;
            stream >> cards;
            foreach (const Card &card, cards) {
                m_hand.addCard(card);
            }
            emit handChanged();
        }
        break;
    case TurnType: {
            if (!m_turn) {
                m_turn = true;
                emit turnChanged();
            }
            qDebug() << "My turn !";
        }
        break;
    case ActionType: {
            if (m_turn) {
                m_turn = false;
                emit turnChanged();
            }
        }
        break;
    case EndRoundType: {
            m_hand.clear();
            emit handChanged();
        }
        break;
    }
}

void NetworkClient::slotConnected()
{
    qDebug() << "Connected, sending nickname";
    setStatus(Registering);
    sendMessageString(m_socket, PlayerType, m_name);
}

void NetworkClient::slotError(QAbstractSocket::SocketError error)
{
    qDebug() << "Connection error" << error << m_socket->errorString();
    setStatus(NotConnected);
}

void NetworkClient::slotReadyRead()
{
    qDebug() << "Received data";

    while (m_socket->bytesAvailable()) {
        if (m_nextMessageSize == -1) {
            // We read the size of the message
            if ((uint) m_socket->bytesAvailable() < sizeof(quint16)) {
                return;
            }

            quint16 size;
            QDataStream in(m_socket);
            in >> size;
            m_nextMessageSize = (int) size;

            qDebug() << "Next message will be size of" << m_nextMessageSize;
        }

        // We read the content of the socket
        if (m_socket->bytesAvailable() < m_nextMessageSize) {
            return;
        }

        QDataStream in (m_socket->read(m_nextMessageSize));
        quint16 typeInt;
        QByteArray data;
        in >> typeInt;
        in >> data;
        MessageType type = (MessageType) typeInt;
        qDebug() << "Received data" << type << "of size" << data.size();
        m_nextMessageSize = -1;

        reply(type, data);
    }
}
