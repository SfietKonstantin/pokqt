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

#ifndef HELPERS_H
#define HELPERS_H

/**
 * @file helpers.h
 * @short Helper enumerations and functions
 */

#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include <QtNetwork/QTcpSocket>

/// @todo TODO: rename to globals or something like that

/**
 * @brief Enumeration describing the server-client message types
 */
enum MessageType {
    /**
     * @short Player information
     *
     * - client -> server: client registering name.
     * - server -> client: game properties broadcast.
     */
    PlayerType,
    /**
     * @short Chat
     *
     * - client -> server: requires server to broadcast chat message.
     * - server -> client: server broadcasting chat message.
     */
    ChatType,
    /**
     * @short New round starting
     */
    NewRoundType,
    /**
     * @short Cards being distributed
     */
    CardsType,
    /**
     * @short Changing the player's turn
     */
    TurnType,
    /**
     * @short Client sending an action
     */
    ActionType,
    /**
     * @short At the end of a round all cards being broadcasted
     */
    AllCardsType,
    /**
     * @short A round ends
     */
    EndRoundType
};

/**
 * @brief Send a message through a socket
 *
 * This function sends no additional information.
 *
 * @param socket socket to use.
 * @param messageType message type.
 */
inline static void sendMessage(QTcpSocket *socket, MessageType messageType)
{
    QByteArray data;
    QDataStream stream (&data, QIODevice::WriteOnly);

    stream << (quint16) 0; // Initial size of the packet
    stream << (quint16) messageType;

    // Write size
    stream.device()->seek(0);
    stream << (quint16) (data.size() - sizeof(quint16));

    qDebug() << "Send message of size" << (data.size() - sizeof(quint16));

    socket->write(data);
}

/**
 * @brief Send a message through a socket
 *
 * @param socket socket to use.
 * @param messageType message type.
 * @param message message to send.
 */
inline static void sendMessage(QTcpSocket *socket, MessageType messageType,
                               const QByteArray &message)
{
    QByteArray data;
    QDataStream stream (&data, QIODevice::WriteOnly);

    stream << (quint16) 0; // Initial size of the packet
    stream << (quint16) messageType;
    stream << message;

    // Write size
    stream.device()->seek(0);
    stream << (quint16) (data.size() - sizeof(quint16));

    qDebug() << "Send message of size" << (data.size() - sizeof(quint16));

    socket->write(data);
}

/**
 * @brief Send a string message through a socket
 *
 * @param socket socket to use.
 * @param messageType message type.
 * @param message message to send.
 */
inline static void sendMessageString(QTcpSocket *socket, MessageType messageType,
                                     const QString &message)
{
   sendMessage(socket, messageType, message.toUtf8());
}


#endif // HELPERS_H
