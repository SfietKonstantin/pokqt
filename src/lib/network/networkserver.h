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

#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "pokqt_global.h"
#include "helpers.h"
#include <QtNetwork/QHostAddress>
#include "logic/playerproperties.h"
#include "logic/card.h"

class QTcpSocket;
class QTcpServer;

/**
 * @brief Server class
 *
 * This class is responsible of managing
 * network connections, like client connections
 * and disconnections. It wraps a QTcpServer, that
 * performs all the operations.
 *
 * The NetworkServer class also manages message
 * reception and transmission to clients. However,
 * it do not manage the game logic. The game logic
 * is managed by GameManager. Both classes provides
 * signals and slots that fits together to build a
 * server.
 */
class POKQTSHARED_EXPORT NetworkServer: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit NetworkServer(QObject *parent = 0);
signals:
    void displayMessage(const QString &type, const QString &message);
    void playerAdded(QTcpSocket *socket, const QString &name);
    void playerRemoved(QTcpSocket *socket);
    void chatReceived(QTcpSocket *socket, const QString &message);
    void actionReceived(QTcpSocket *socket, int tokenCount);
public slots:
    /**
     * @brief Start the server
     * @param port port to use.
     */
    void startServer(int port);
    /**
     * @brief Stops the server
     */
    void stopServer();

    void broadCastPlayers(const QList<PlayerProperties> &players, int pot);
    void refusePlayer(QObject *handle);
    void chat(const QString &name, const QString &message);
    void newRound();
    void distributeCards(QObject *handle, const QList<Card> &cards);
    void distributeCards(const QList<Card> &cards);
    void sendPlayerTurn(QObject *handle);
private:
    void reply(QTcpSocket *socket, MessageType type, const QByteArray &data);
    QHostAddress m_address;
    int m_port;
    QTcpServer *m_server;
    QList<QTcpSocket *> m_sockets;
    QMap<QTcpSocket *, quint16> m_nextMessageSize;
private slots:
    void slotNewConnection();
    void slotDisconnected();
    void slotReadyRead();
};

#endif // NETWORKSERVER_H
