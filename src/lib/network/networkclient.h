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

#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include "pokqt_global.h"
#include "helpers.h"
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtNetwork/QTcpSocket>
#include "logic/playerproperties.h"

class POKQTSHARED_EXPORT NetworkClient : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)
public:
    enum Status {
        NotConnected,
        Connecting,
        Registering,
        Connected
    };
    explicit NetworkClient(QObject *parent = 0);
    Status status() const;
    QString playerName() const;
    void setPlayerName(const QString &playerName);

    // Non-QML API
    QList<PlayerProperties> players() const;
    int index() const;
    int pot() const;
signals:
    void playersChanged();
    void statusChanged();
    void playerNameChanged(); // TODO: change this, use name instead of playerName
    void chatReceived(const QString &playerName, const QString &chat);
public slots:
    void connectToHost(const QString &host, int port);
    void disconnectFromHost();
    void sendChat(const QString &chat);
private:
    void setPlayers(const QList<PlayerProperties> &players, int index, int pot);
    void setStatus(Status status);
    void reply(MessageType type, const QByteArray &data);
    Status m_status;
    int m_index;
    QList<PlayerProperties> m_players;
    int m_pot;
    QString m_playerName;
    QTcpSocket *m_socket;
    int m_nextMessageSize;
private slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError error);
    void slotReadyRead();
};

#endif // NETWORKCLIENT_H
