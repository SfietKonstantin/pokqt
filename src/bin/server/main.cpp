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
 * @file server/main.cpp
 * @short Main server file
 */

#include <QtWidgets/QApplication>
#include <network/networkserver.h>
#include <logic/gamemanager.h>

#include "serverdialog.h"
#include <osignal.h>

/**
 * @brief Namespace for server related classes
 */
namespace Server
{

/**
 * @brief Main object of the server
 */
class ServerObject: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit ServerObject(QObject *parent = 0);
    /**
     * @brief Destructor
     */
    virtual ~ServerObject();
    /**
     * @brief Show the dialog
     */
    void show();
private:
    /**
     * @internal
     * @brief Server dialog
     */
    ServerDialog *m_dialog;
    /**
     * @internal
     * @brief Server (network communications)
     */
    NetworkServer *m_server;
    /**
     * @internal
     * @brief Server (game manager)
     */
    GameManager *m_gameManager;
};

ServerObject::ServerObject(QObject *parent)
    : QObject(parent), m_dialog(new ServerDialog), m_server(new NetworkServer(this))
    , m_gameManager(new GameManager(this))
{
    // Connections between dialog and server
    connect(m_server, &NetworkServer::info, m_dialog, &ServerDialog::displayMessage);
    connect(m_dialog, OSIGNAL1(ServerDialog, started, int), m_server, &NetworkServer::startServer);
    connect(m_dialog, &ServerDialog::stopped, m_server, &NetworkServer::stopServer);

    // Connections between dialog and game manager
    connect(m_dialog, OSIGNAL0(ServerDialog, started), m_gameManager, &GameManager::start);
    connect(m_dialog, &ServerDialog::gameStarted, m_gameManager, &GameManager::startGame);
    connect(m_dialog, &ServerDialog::stopped, m_gameManager, &GameManager::stop);

    // Connections between server and game manager
    connect(m_server, &NetworkServer::playerAdded, m_gameManager, &GameManager::addPlayer);
    connect(m_server, &NetworkServer::playerRemoved, m_gameManager, &GameManager::removePlayer);
    connect(m_gameManager, &GameManager::gamePropertiesBroadcasted,
            m_server, &NetworkServer::sendPlayerProperties);
    connect(m_server, &NetworkServer::chatReceived, m_gameManager, &GameManager::performChat);
    connect(m_gameManager, &GameManager::chatBroadcasted, m_server, &NetworkServer::sendChat);
    connect(m_gameManager, &GameManager::newRoundBroadcasted,
            m_server, &NetworkServer::sendNewRound);
    connect(m_gameManager, OSIGNAL2(GameManager, cardsDistributed,QObject *, const QList<Card> &),
            m_server, OSLOT2(NetworkServer, sendCardsDistribution,QObject *, const QList<Card> &));
    connect(m_gameManager, OSIGNAL1(GameManager, cardsDistributed, const QList<Card> &),
            m_server, OSLOT1(NetworkServer, sendCardsDistribution, const QList<Card> &));
    connect(m_gameManager, &GameManager::playerTurnSelected,
            m_server, &NetworkServer::sendPlayerTurn);
    connect(m_server, &NetworkServer::actionReceived, m_gameManager, &GameManager::performAction);
    connect(m_gameManager, &GameManager::endRoundBroadcasted,
            m_server, &NetworkServer::sendEndRound);
    connect(m_gameManager, &GameManager::allCardsBroadcasted,
            m_server, &NetworkServer::sendAllHands);
}

ServerObject::~ServerObject()
{
    m_dialog->deleteLater();
}

void ServerObject::show()
{
    m_dialog->show();
}

}

/**
 * @brief Server main
 * @param argc argc.
 * @param argv argv.
 * @return exit code.
 */
int main(int argc, char **argv)
{
    QApplication app (argc, argv);

    Server::ServerObject server;
    server.show();

    return app.exec();
}

#include "main.moc"
