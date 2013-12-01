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
 * @file networkserver.cpp
 * @short Implementation of NetworkServer
 */

#include "networkserver.h"
#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include "logic/card.h"

/// @todo TODO: don't add too many players. We need that 2 * n_players + 5 <= 52
/// @todo TODO: we shouldn't be able to start a game with zero / one player.
/// @todo TODO: handle player disconnections gracefully during game. Right now, the game is stuck.

/**
 * @brief NET_TYPE
 *
 * Defines a type for NetworkServer::info.
 */
static const char *NET_TYPE = "net";
/**
 * @brief CHAT_TYPE
 *
 * Defines a type for NetworkServer::info.
 */
static const char *CHAT_TYPE = "chat";

NetworkServer::NetworkServer(QObject *parent)
    : QObject(parent)
{
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &NetworkServer::slotNewConnection);
}

void NetworkServer::startServer(int port)
{
    m_server->listen(QHostAddress::Any, port);
}

void NetworkServer::stopServer()
{
    foreach (QTcpSocket *socket, m_sockets) {
        emit playerRemoved(socket);
        socket->close();
        socket->deleteLater();
    }

    m_sockets.clear();
    m_server->close();
}

void NetworkServer::sendPlayerProperties(const QList<PlayerProperties> &players, int pot)
{
    // Send the data to all socket
    for (int i = 0; i < m_sockets.count(); ++i) {
        QByteArray data;
        QDataStream stream (&data, QIODevice::WriteOnly);
        stream << i << players << pot;
        sendMessage(m_sockets[i], PlayerType, data);
    }
}

void NetworkServer::sendRefusePlayer(QObject *handle)
{
    // Handles are sockets
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(handle);
    if (!socket) {
        return;
    }

    socket->disconnectFromHost();
}

void NetworkServer::sendChat(const QString &name, const QString &message)
{
    QByteArray data;
    QDataStream stream (&data, QIODevice::WriteOnly);
    stream << name << message;

    info(CHAT_TYPE, QString("%1: %2").arg(name, message));

    foreach (QTcpSocket *socket, m_sockets) {
        sendMessage(socket, ChatType, data);
    }
}

void NetworkServer::sendNewRound()
{
    foreach (QTcpSocket *socket, m_sockets) {
        sendMessage(socket, NewRoundType);
    }
}

void NetworkServer::sendCardsDistribution(const QList<Card> &cards)
{
    QByteArray data;
    QDataStream stream (&data, QIODevice::WriteOnly);
    stream << cards;

    foreach (QTcpSocket *socket, m_sockets) {
        sendMessage(socket, CardsType, data);
    }
}

void NetworkServer::sendCardsDistribution(QObject *handle, const QList<Card> &cards)
{
    // Handles are sockets
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(handle);
    if (!socket) {
        return;
    }

    QByteArray data;
    QDataStream stream (&data, QIODevice::WriteOnly);
    stream << cards;
    sendMessage(socket, CardsType, data);
}

void NetworkServer::sendPlayerTurn(QObject *handle)
{
    // Handles are sockets
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(handle);
    if (!socket) {
        return;
    }

    sendMessage(socket, TurnType);
}

void NetworkServer::sendEndRound()
{
    foreach (QTcpSocket *socket, m_sockets) {
        sendMessage(socket, EndRoundType);
    }
}

void NetworkServer::sendAllHands(const QList<Hand> &hands)
{
    // Send all hands of people who didn't fold
    QByteArray data;
    QDataStream stream (&data, QIODevice::WriteOnly);
    stream << hands;

    foreach (QTcpSocket *socket, m_sockets) {
        sendMessage(socket, AllCardsType, data);
    }
}

void NetworkServer::reply(QTcpSocket *socket, MessageType type, const QByteArray &data)
{
    switch (type) {
    case PlayerType:
        emit playerAdded(socket, QString::fromUtf8(data));
        break;
    case ChatType:
        emit chatReceived(socket, QString::fromUtf8(data));
        break;
    case NewRoundType: // Do nothing
        break;
    case CardsType: // Do nothing
        break;
    case TurnType: // Do nothing
        break;
    case ActionType: {
            int tokenCount;
            QDataStream stream (data);
            stream >> tokenCount;
            emit actionReceived(socket, tokenCount);
            sendMessage(socket, ActionType);
        }
        break;
    case EndRoundType: // Do nothing
        break;
    case AllCardsType: // Do nothing
        break;
    }
}

void NetworkServer::slotNewConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();

    qDebug() << "Received connection" << socket;
    connect(socket, &QTcpSocket::disconnected, this, &NetworkServer::slotDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkServer::slotReadyRead);

    m_sockets.append(socket);
    emit info(NET_TYPE, "New connection");
}

void NetworkServer::slotDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        return;
    }

    qDebug() << "Received disconnection from" << socket;

    if (m_sockets.contains(socket)) {
        m_nextMessageSize.remove(socket);
        m_sockets.removeAll(socket);
    }

    emit playerRemoved(socket);
    socket->deleteLater();
    emit info(NET_TYPE, "Disconnection");
}

void NetworkServer::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        return;
    }

    qDebug() << "Received data from" << socket;

    if (!m_sockets.contains(socket)) {
        return;
    }

    if (!m_nextMessageSize.contains(socket)) {
        // We read the size of the message
        if ((uint) socket->bytesAvailable() < sizeof(quint16)) {
            return;
        }

        QDataStream in(socket);
        quint16 size;
        in >> size;

        m_nextMessageSize.insert(socket, size);
        qDebug() << "Next message from" << socket << "will be size of" << size;
    }

    // We read the content of the socket
    int size = m_nextMessageSize.value(socket);
    if (socket->bytesAvailable() < size) {
        return;
    }

    QDataStream in (socket);
    quint16 typeInt;
    QByteArray data;
    in >> typeInt;
    in >> data;
    MessageType type = (MessageType) typeInt;
    qDebug() << "Received data from" << socket << type << data;
    m_nextMessageSize.remove(socket);

    reply(socket, type, data);
}
