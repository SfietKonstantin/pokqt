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

#include <QtWidgets/QApplication>
#include <network/networkserver.h>
#include <logic/gamemanager.h>

#include "serverdialog.h"
#include <osignal.h>

int main(int argc, char **argv)
{
    QApplication app (argc, argv);

    ServerDialog dialog;
    NetworkServer server;
    GameManager gameManager;

    // Connections between dialog and server
    QObject::connect(&server, &NetworkServer::displayMessage,
                     &dialog, &ServerDialog::displayMessage);
    QObject::connect(&dialog, OSIGNAL(ServerDialog, start, int),
                     &server, &NetworkServer::startServer);
    QObject::connect(&dialog, &ServerDialog::stop, &server, &NetworkServer::stopServer);

    // Connections between dialog and game manager
    QObject::connect(&dialog, OSIGNAL0(ServerDialog, start), &gameManager, &GameManager::start);
    QObject::connect(&dialog, &ServerDialog::startGame, &gameManager, &GameManager::startGame);
    QObject::connect(&dialog, &ServerDialog::stop, &gameManager, &GameManager::stop);

    // Connections between server and game manager
    QObject::connect(&server, &NetworkServer::playerAdded, &gameManager, &GameManager::addPlayer);
    QObject::connect(&server, &NetworkServer::playerRemoved,
                     &gameManager, &GameManager::removePlayer);
    QObject::connect(&gameManager, &GameManager::playersBroadcasted,
                     &server, &NetworkServer::sendPlayerProperties);
    QObject::connect(&server, &NetworkServer::chatReceived, &gameManager, &GameManager::chat);
    QObject::connect(&gameManager, &GameManager::chatSent, &server, &NetworkServer::sendChat);
    QObject::connect(&gameManager, &GameManager::newRound, &server, &NetworkServer::sendNewRound);
    QObject::connect(&gameManager, OSIGNAL2(GameManager, cardsDistributed,QObject *, const QList<Card> &),
                     &server, OSLOT2(NetworkServer, sendCardsDistribution,QObject *, const QList<Card> &));
    QObject::connect(&gameManager, OSIGNAL(GameManager, cardsDistributed, const QList<Card> &),
                     &server, OSLOT(NetworkServer, sendCardsDistribution, const QList<Card> &));
    QObject::connect(&gameManager, &GameManager::playerTurnSelected,
                     &server, &NetworkServer::sendPlayerTurn);
    QObject::connect(&server, &NetworkServer::actionReceived,
                     &gameManager, &GameManager::performAction);
    QObject::connect(&gameManager, &GameManager::endRound, &server, &NetworkServer::sendEndRound);
    QObject::connect(&gameManager, &GameManager::allCardsSent,
                     &server, &NetworkServer::sendAllCards);

    dialog.show();

    return app.exec();
}
