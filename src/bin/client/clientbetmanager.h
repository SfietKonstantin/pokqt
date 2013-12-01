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

/**
 * @file clientbetmanager.h
 * @short Definition of Client::ClientBetManager
 */

#include <QtCore/QObject>
#include <network/networkclient.h>
#include <logic/betmanager.h>

namespace Client
{

/**
 * @brief Extrended BetManager for the client
 *
 * This class is an extended BetManager, that also provides
 * methods to be used to perform bets. It uses a
 * NetworkClient to perform those actions.
 */
class ClientBetManager : public BetManager
{
    Q_OBJECT
    /**
     * @short NetworkClient used to perform actions
     */
    Q_PROPERTY(NetworkClient *client READ client WRITE setClient NOTIFY clientChanged)
    /**
     * @short If check is allowed as an action
     */
    Q_PROPERTY(bool check READ isCheck NOTIFY checkChanged)
public:
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit ClientBetManager(QObject *parent = 0);
    /**
     * @brief Get the NetworkClient used to perform actions
     * @return the NetworkClient used to perform actions.
     */
    NetworkClient * client() const;
    /**
     * @brief Set the NetworkClient used to perform actions
     * @param client NetworkClient used to perform actions to set.
     */
    void setClient(NetworkClient *client);
    /**
     * @brief If check is allowed as an action
     * @return if check is allowed as an action.
     */
    bool isCheck() const;
    /**
     * @short Raise
     * @param raisedTokenCount raised token count.
     * @return if the raise is allowed.
     */
    Q_INVOKABLE bool raise(int raisedTokenCount);
    /**
     * @short Call / check
     * @return if the call / check is allowed.
     */
    Q_INVOKABLE bool call();
signals:
    /**
     * @brief NetworkClient used to perform actions changed
     */
    void clientChanged();
    /**
     * @brief If check is allowed as an action changed
     */
    void checkChanged();
    /**
     * @brief Bet sent
     * @param tokenCount number of tokens to bet.
     */
    void betSent(int tokenCount);
    /**
     * @brief Fold sent
     */
    void foldSent();
public slots:
    /**
     * @brief Fold
     */
    void fold();
private slots:
    /**
     * @internal
     * @brief Slot used to manage player properties change
     */
    void slotPlayersChanged();
private:
    /**
     * @internal
     * @brief If check is allowed as an action
     */
    bool m_check;
    /**
     * @internal
     * @brief NetworkClient used to perform actions
     */
    NetworkClient *m_client;
};

}

#endif // CLIENTBETMANAGER_H
