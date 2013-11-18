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

#include "connectionmanager.h"
#include "osignal.h"
#include <QtCore/QDataStream>
#include <QtNetwork/QHostAddress>

ConnectionManager::ConnectionManager(QObject *parent) :
    QObject(parent), m_status(NotConnected), m_nextMessageSize(-1)
{
    m_index = -1;
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &ConnectionManager::slotConnected);
    connect(m_socket, OSIGNAL(QTcpSocket, error, QAbstractSocket::SocketError),
            this, &ConnectionManager::slotError);
    connect(m_socket, &QTcpSocket::readyRead, this, &ConnectionManager::slotReadyRead);
}

ConnectionManager::Status ConnectionManager::status() const
{
    return m_status;
}

QString ConnectionManager::playerName() const
{
    return m_playerName;
}

void ConnectionManager::setPlayerName(const QString &playerName)
{
    if (m_playerName != playerName) {
        m_playerName = playerName;
        emit playerNameChanged();

        if (m_status != NotConnected) {
            qWarning() << "Nickname will not be changed before next connection";
        }
    }
}

void ConnectionManager::connectToHost(const QString &host, int port)
{
    setStatus(Connecting);
    m_socket->connectToHost(QHostAddress(host), port);
}

void ConnectionManager::disconnectFromHost()
{
    setStatus(NotConnected);
    m_socket->disconnectFromHost();
    m_index = -1;
    m_players.clear();
}

void ConnectionManager::sendChat(const QString &chat)
{
    sendMessageString(m_socket, ChatType, chat);
}

void ConnectionManager::setStatus(Status status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

void ConnectionManager::reply(MessageType type, const QByteArray &data)
{
    switch (type) {
    case PlayerType: {
            QDataStream stream (data);
            int index;
            QList<PlayerProperties> players;
            stream >> index >> players;
            if (m_status == Registering) {
                QString playerName = players.value(index).name();
                if (m_playerName != playerName) {
                    qWarning() << "Nickname receivend from server do not match";
                    qWarning() << "You might have changed nickname before being registered";
                    setPlayerName(playerName);
                }

                setStatus(Connected);
            }

            qDebug() << "Player list count" << players.count();
            qDebug() << "Your index" << index;
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
    }
}

void ConnectionManager::slotConnected()
{
    qDebug() << "Connected, sending nickname";
    setStatus(Registering);
    sendMessageString(m_socket, PlayerType, m_playerName);
}

void ConnectionManager::slotError(QAbstractSocket::SocketError error)
{
    qDebug() << "Connection error" << error << m_socket->errorString();
    setStatus(NotConnected);
}

void ConnectionManager::slotReadyRead()
{
    qDebug() << "Received data";

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

    QDataStream in (m_socket);
    quint16 typeInt;
    QByteArray data;
    in >> typeInt;
    in >> data;
    MessageType type = (MessageType) typeInt;
    qDebug() << "Received data" << type << "of size" << data.size();
    m_nextMessageSize = -1;

    reply(type, data);
}
