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
 * @file networkclient.h
 * @short Definition of NetworkClient
 */

#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "pokqt_global.h"
#include "helpers.h"
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtNetwork/QTcpSocket>
#include "logic/playerproperties.h"
#include "logic/hand.h"

/**
 * @brief Network client
 *
 * This class is used to talk to the server, and also manage
 * some of the game logic associated to message that are
 * received from the server.
 *
 * Basically, it provides a Qt / QML interface that can be used
 * by clients to implement the basic features in QML.
 */
class POKQTSHARED_EXPORT NetworkClient : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)
    /**
     * @short Status of the client
     */
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    /**
     * @short Name of the player
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    /**
     * @short If it is the turn of this client to bet
     */
    Q_PROPERTY(bool turn READ turn NOTIFY turnChanged)
public:
    /**
     * @brief An enumeration describing statuses of the client
     */
    enum Status {
        /**
         * @short The client is not connected
         */
        NotConnected,
        /**
         * @short The client connecting
         */
        Connecting,
        /**
         * @short The client trying to get a slot in the server
         */
        Registering,
        /**
         * @short The client is connected and in a game
         */
        Connected
    };
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit NetworkClient(QObject *parent = 0);
    /**
     * @short Get the status of the client
     * @return status of the client.
     */
    Status status() const;
    /**
     * @brief Get the name of the player
     * @return name of the player.
     */
    QString name() const;
    /**
     * @brief Set the name of the player
     * @param name name to set.
     */
    void setName(const QString &name);
    /**
     * @short Get if it is the turn of this client to bet
     * @return if it is the turn of this client to bet.
     */
    bool turn() const;

    // Non-QML API
    /**
     * @brief Get the list of player properties
     *
     * This list is sent by the server.
     *
     * @return list of player properties.
     */
    QList<PlayerProperties> players() const;
    /**
     * @brief Get the index of the player
     *
     * This index is sent by the server.
     *
     * @return index of the player.
     */
    int index() const;
    /**
     * @brief Get the current pot
     *
     * This value is sent by the server.
     *
     * @return current pot.
     */
    int pot() const;
    /**
     * @brief Get the player's hand
     *
     * This value is sent by the server.
     *
     * @return  the player's hand
     */
    Hand hand() const;
signals:
    /**
     * @brief list of player properties changed
     */
    void playersChanged();
    /**
     * @brief Player's hand changed
     */
    void handChanged();
    /**
     * @brief Status of the client changed
     */
    void statusChanged();
    /**
     * @brief Name of the player changed
     */
    void nameChanged();
    /**
     * @brief if it is the turn of this client to bet changed
     */
    void turnChanged();
    /**
     * @brief Chat received
     * @param name name of the poster.
     * @param chat chat message.
     */
    void chatReceived(const QString &name, const QString &chat);
public slots:
    /**
     * @brief Connect to the server
     * @param host ip address of the server.
     * @param port port to be used for the connection.
     */
    void connectToHost(const QString &host, int port);
    /**
     * @brief Disconnect from the server
     */
    void disconnectFromHost();
    /**
     * @brief Send a chat message to the server
     * @param chat chat message.
     */
    void sendChat(const QString &chat);
    /**
     * @brief Send a bet to the server
     * @param tokenCount token count to bet.
     */
    void sendBet(int tokenCount);
    /**
     * @brief Send that the player folded to the server
     */
    void sendFold();
private:
    /**
     * @internal
     * @brief Set game properties
     * @param players player properties.
     * @param index index of the player.
     * @param pot current pot.
     */
    void setGameProperties(const QList<PlayerProperties> &players, int index, int pot);
    /**
     * @internal
     * @brief Set the status of the client
     * @param status status of the client to set.
     */
    void setStatus(Status status);
    /**
     * @internal
     * @brief Reply to a server message
     *
     * Game logic is usually managed here.
     *
     * @param type type of the message.
     * @param data data in the message.
     */
    void reply(MessageType type, const QByteArray &data);
    /**
     * @internal
     * @brief Status of the client
     */
    Status m_status;
    /**
     * @internal
     * @brief Index of the player
     */
    int m_index;
    /**
     * @internal
     * @brief List of player properties
     */
    QList<PlayerProperties> m_players;
    /**
     * @internal
     * @brief Hand of the player
     */
    Hand m_hand;
    /**
     * @internal
     * @brief Current pot
     */
    int m_pot;
    /**
     * @internal
     * @brief Name of the player
     */
    QString m_name;
    /**
     * @internal
     * @brief If it is the turn of this client to bet
     */
    bool m_turn;
    /**
     * @internal
     * @brief Socket used for the connection
     */
    QTcpSocket *m_socket;
    /**
     * @internal
     * @brief Next message size
     *
     * This map is used to store the size of
     * the next message coming from the server.
     *
     * Message are usually sent as packages over
     * the network. We store the size of a message
     * in a header that is a quint16. So, in order
     * to know if we received enough information, we
     * might have to store the size of the information
     * we except, and it is stored in this attribute.
     */
    int m_nextMessageSize;
private slots:
    /**
     * @internal
     * @brief Slot used to react to connection
     */
    void slotConnected();
    /**
     * @internal
     * @brief Slot used to react to errors
     * @param error error type.
     */
    void slotError(QAbstractSocket::SocketError error);
    /**
     * @internal
     * @brief Slot used to read partial information
     *
     * This slot is called when a packet has been received.
     */
    void slotReadyRead();
};

#endif // NETWORKCLIENT_H
