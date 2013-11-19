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

#include "networkserver.h"
#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

static const char *NET_TYPE = "net";
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
        socket->close();
        socket->deleteLater();
    }

    m_sockets.clear();
    m_playerProperties.clear();
    m_server->close();
}

void NetworkServer::reply(QTcpSocket *socket, MessageType type, const QByteArray &data)
{
    switch (type) {
    case PlayerType:
        // Set the name
        m_playerProperties[socket].setName(QString::fromUtf8(data));
        m_playerProperties[socket].setTokenCount(1000); // TODO: variable number of tokens
        replyPlayers();
        break;
    case ChatType:
        replyChat(socket, data);
        break;
    }
}

void NetworkServer::replyPlayers()
{
    QList<PlayerProperties> players;

    // Build the list of players
    foreach (QTcpSocket *storedSocket, m_sockets) {
        // If we have a socket that did not registered names yet,
        // we put a placeholder empty string
        players.append(m_playerProperties.value(storedSocket));
    }

    // Send the data to each socket
    for (int i = 0; i< m_sockets.count(); ++i) {
        QByteArray data;
        QDataStream stream (&data, QIODevice::WriteOnly);
        stream << i << players;
        sendMessage(m_sockets[i], PlayerType, data);
    }
}

void NetworkServer::replyChat(QTcpSocket *socket, const QByteArray &data)
{
    // Get name
    QString name = m_playerProperties[socket].name();
    QString chat = QString::fromUtf8(data);

    QByteArray newData;
    QDataStream stream (&newData, QIODevice::WriteOnly);
    stream << name << chat;

    displayMessage(CHAT_TYPE, QString("%1: %2").arg(name, chat));

    foreach (QTcpSocket *playerSocket, m_sockets) {
        sendMessage(playerSocket, ChatType, newData);
    }
}

void NetworkServer::slotNewConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();

    qDebug() << "Received connection" << socket;
    connect(socket, &QTcpSocket::disconnected, this, &NetworkServer::slotDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkServer::slotReadyRead);

    m_sockets.append(socket);
    m_playerProperties.insert(socket, PlayerProperties());

    emit displayMessage(NET_TYPE, "New connection");
}

void NetworkServer::slotDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) {
        return;
    }

    qDebug() << "Received disconnection from" << socket;

    if (m_sockets.contains(socket)) {
        m_playerProperties.remove(socket);
        m_nextMessageSize.remove(socket);
        m_sockets.removeAll(socket);
    }

    socket->deleteLater();

    emit displayMessage(NET_TYPE, "Disconnection");

    // Send the new list of players
    replyPlayers();
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
