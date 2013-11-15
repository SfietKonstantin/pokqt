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
    m_networkClient = new NetworkClient(this);
    connect(m_networkClient, &NetworkClient::connected, this, &ConnectionManager::slotConnected);
    connect(m_networkClient, OSIGNAL(NetworkClient, error, QAbstractSocket::SocketError),
            this, &ConnectionManager::slotError);
    connect(m_networkClient, &NetworkClient::readyRead, this, &ConnectionManager::slotReadyRead);
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
    m_networkClient->connectToHost(QHostAddress(host), port);
}

void ConnectionManager::disconnectFromHost()
{
    setStatus(NotConnected);
    m_networkClient->disconnectFromHost();
}

void ConnectionManager::setStatus(Status status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
    }
}

void ConnectionManager::reply(NetworkClient::MessageType type, const QByteArray &data)
{
    switch (type) {
    case NetworkClient::NameType: {
            QString name = QString::fromUtf8(data);
            if (name != m_playerName) {
                qWarning() << "Nickname receivend from server do not match";
                qWarning() << "You might have changed nickname before being registered";
                setPlayerName(name);
            }
            setStatus(Connected);
        }
        break;
    }
}

void ConnectionManager::slotConnected()
{
    qDebug() << "Connected, sending nickname";
    setStatus(Registering);
    m_networkClient->sendMessageString(NetworkClient::NameType, m_playerName);
}

void ConnectionManager::slotError(QAbstractSocket::SocketError error)
{
    qDebug() << "Connection error" << error << m_networkClient->errorString();
    setStatus(NotConnected);
}

void ConnectionManager::slotReadyRead()
{
    qDebug() << "Received data";

    if (m_nextMessageSize == -1) {
        // We read the size of the message
        if ((uint) m_networkClient->bytesAvailable() < sizeof(quint16)) {
            return;
        }

        quint16 size;
        QDataStream in(m_networkClient);
        in >> size;
        m_nextMessageSize = (int) size;

        qDebug() << "Next message will be size of" << m_nextMessageSize;
    }

    // We read the content of the socket
    if (m_networkClient->bytesAvailable() < m_nextMessageSize) {
        return;
    }

    QDataStream in (m_networkClient);
    quint16 typeInt;
    QByteArray data;
    in >> typeInt;
    in >> data;
    NetworkClient::MessageType type = (NetworkClient::MessageType) typeInt;
    qDebug() << "Received data" << type << data;
    m_nextMessageSize = -1;

    reply(type, data);
}
