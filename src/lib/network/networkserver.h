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
#include "logic/hand.h"

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
 *
 * The server uses handles to manage players. Handles
 * are QTcpSocket * but are provided to slots as QObject *,
 * so that the GameManager do not need to care about them.
 * That's why in the slots of this class, there is a
 * systematic cast from QObject to QTcpSocket.
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

    /**
     * @brief A player has been added
     * @param socket handle of the player.
     * @param name name of the player.
     */
    void playerAdded(QTcpSocket *socket, const QString &name);
    /**
     * @brief A player has been removed
     * @param socket handle of the player.
     */
    void playerRemoved(QTcpSocket *socket);
    /**
     * @brief A chat message from a player has been received
     * @param socket handle of the player.
     * @param message content of the message.
     */
    void chatReceived(QTcpSocket *socket, const QString &message);
    /**
     * @brief An action from a player has been received
     *
     * The action is translated by the number of token
     * that is bet:
     * - if it is -1, then the player folded.
     * - otherwise, it is the number of tokens that is bet.
     *
     * If the number of tokens matches the minimum bet, then
     * the player simply checked or called, otherwise it is a
     * raise.
     *
     * @param socket handle of the player.
     * @param tokenCount number of token bet.
     */
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

    /**
     * @brief Send player properties
     *
     * Broadcasting players properties informs other players in the game
     * about the status of the game: who bet, how much, how many
     * tokens do a player have, how much is there in the pot.
     *
     * @param players status of the players to broadcast.
     * @param pot value of the pot.
     */
    void sendPlayerProperties(const QList<PlayerProperties> &players, int pot);
    /**
     * @brief Refuse a player
     * @param handle handle of the player.
     */
    void sendRefusePlayer(QObject *handle);
    /**
     * @brief Send a chat message
     * @param name name of the player who sent the chat message.
     * @param message content of the chat message.
     */
    void sendChat(const QString &name, const QString &message);
    /**
     * @brief Send that a new round started
     */
    void sendNewRound();
    /**
     * @brief Send cards to all players
     *
     * This method is used to distribute cards in the flop,
     * turn or river to all players. You can consider these
     * cards as being in the hand of all players.
     *
     * @param cards cards to be distributed.
     */
    void sendCardsDistribution(const QList<Card> &cards);
    /**
     * @brief Send cards to one player
     *
     * This method is used to distribute cards to one
     * player. It is used to distribute the pair of
     * cards at the beginning of a round.
     *
     * @param handle handle to the player.
     * @param cards cards to be distributed.
     */
    void sendCardsDistribution(QObject *handle, const QList<Card> &cards);
    /**
     * @brief Inform a player that it is his or her turn
     *
     * This slot notifies the client that it is
     * it's turn to bet.
     *
     * @param handle handle to the player.
     */
    void sendPlayerTurn(QObject *handle);
    /**
     * @brief Inform all players that the round is terminated
     *
     * This slot notifies all the clients that the round
     * is terminated. It is used to trigger client side
     * cleanups (like models change). It is usually followed
     * by an update of the player properties.
     */
    void sendEndRound();

    void sendAllCards(const QList<Hand> &hands);
private:
    void reply(QTcpSocket *socket, MessageType type, const QByteArray &data);
    /**
     * @internal
     * @brief Qt TCP server used for this server
     */
    QTcpServer *m_server;
    /**
     * @internal
     * @brief Sockets that are connected.
     *
     * The sockets that are connected are also used
     * as handles for players.
     */
    QList<QTcpSocket *> m_sockets;
    /**
     * @internal
     * @brief Next message size
     *
     * This map is used to store the size of
     * the next message coming from a socket.
     *
     * Message are usually sent as packages over
     * the network. We store the size of a message
     * in a header that is a quint16. So, in order
     * to know if we received enough information, we
     * might have to store the size of the information
     * we except, and it is stored in this attribute.
     */
    QMap<QTcpSocket *, quint16> m_nextMessageSize;
private slots:
    /**
     * @internal
     * @brief Slot used to listen to new connections
     */
    void slotNewConnection();
    /**
     * @internal
     * @brief Slot used to listen to disconnections
     */
    void slotDisconnected();
    void slotReadyRead();
};

#endif // NETWORKSERVER_H
