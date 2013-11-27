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

#ifndef CLIENTBETMANAGER_H
#define CLIENTBETMANAGER_H

#include <QtCore/QObject>
#include <network/networkclient.h>
#include <logic/betmanager.h>

class ClientBetManager : public BetManager
{
    Q_OBJECT
    Q_PROPERTY(NetworkClient *client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(bool check READ isCheck NOTIFY checkChanged)
public:
    explicit ClientBetManager(QObject *parent = 0);
    NetworkClient * client() const;
    void setClient(NetworkClient *client);
    bool isCheck() const;
    Q_INVOKABLE bool raise(int raisedTokenCount);
    Q_INVOKABLE bool call();
signals:
    void clientChanged();
    void checkChanged();
    void betSent(int tokenCount);
    void foldSent();
public slots:
    void fold();
private slots:
    void slotPlayersChanged();
private:
    bool m_check;
    NetworkClient *m_client;
};

#endif // CLIENTBETMANAGER_H
