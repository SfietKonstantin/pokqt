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
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

static const char *NET_TYPE = "net";

NetworkServer::NetworkServer(QObject *parent)
    : QTcpServer(parent)
{
    connect(this, &NetworkServer::newConnection, this, &NetworkServer::slotNewConnection);
}

void NetworkServer::startServer(int port)
{
    listen(QHostAddress::Any, port);
}

void NetworkServer::stopServer()
{
    foreach (QTcpSocket *socket, m_sockets) {
        socket->close();
        socket->deleteLater();
    }

    m_sockets.clear();
    m_playerProperties.clear();
    close();
}

void NetworkServer::slotNewConnection()
{
    QTcpSocket *socket = nextPendingConnection();

    qDebug() << "Received connection" << socket;
    connect(socket, &QTcpSocket::disconnected, this, &NetworkServer::slotDisconnected);

    m_sockets.append(socket);
    m_playerProperties.insert(socket, PlayerProperties());

    emit sendMessage(NET_TYPE, "New connection");
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
        m_sockets.removeAll(socket);
    }

    socket->deleteLater();


    emit sendMessage(NET_TYPE, "Disconnection");
}
