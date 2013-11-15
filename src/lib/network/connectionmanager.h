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

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "pokqtnetwork_global.h"
#include <QtCore/QObject>
#include "networkclient.h"

class POKQTNETWORKSHARED_EXPORT ConnectionManager : public QObject
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
    explicit ConnectionManager(QObject *parent = 0);
    Status status() const;
    QString playerName() const;
    void setPlayerName(const QString &playerName);
signals:
    void statusChanged();
    void playerNameChanged();
public slots:
    void connectToHost(const QString &host, int port);
    void disconnectFromHost();
private:
    void setStatus(Status status);
    void reply(NetworkClient::MessageType type, const QByteArray &data);
    Status m_status;
    QString m_playerName;
    NetworkClient *m_networkClient;
    int m_nextMessageSize;
private slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError error);
    void slotReadyRead();
};

#endif // CONNECTIONMANAGER_H
