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

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

/**
 * @file gamemanager.h
 * @short Definition of GameManager
 */

#include "pokqt_global.h"
#include <QtCore/QObject>
#include <QtCore/QMap>
#include "playerproperties.h"
#include "deck.h"
#include "hand.h"

class BetManager;

/**
 * @brief Game manager
 *
 * The game manager controls the game. It interprets poker rules,
 * distribute cards and token to players, and check winning
 * conditions. It communicates with the NetworkServer via a set
 * of signals and recive orders from slots.
 *
 * Note that the GameManager don't know about the way to communicate
 * with the players. Instead, it uses handles to identify players.
 * These handles are provided as pointers to QObject from the
 * NetworkServer.
 */
class POKQTSHARED_EXPORT GameManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Status of the game
     * @todo TODO: we should use more status to manage the different states of the game
     */
    enum GameStatus {
        /**
         * @short The server is in an invalid state
         */
        Invalid,
        /**
         * @short The server is waiting for players to connect
         */
        WaitingPlayers,
        /**
         * @short The server is managing a game
         */
        Gaming
    };

    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit GameManager(QObject *parent = 0);
public slots:
    /**
     * @brief Starts the server
     */
    void start();
    /**
     * @brief Starts the game
     */
    void startGame();
    /**
     * @brief Stops the server
     */
    void stop();
    /**
     * @brief Add a player
     *
     * Add a player in the game, registering it in the game,
     * and giving him or her an initial amount of tokens.
     *
     * @param handle handle of the player.
     * @param name player's name.
     */
    void addPlayer(QObject *handle, const QString &name);
    /**
     * @brief Remove a player
     * @param handle handle of the player.
     */
    void removePlayer(QObject *handle);
    /**
     * @brief Perform chat operation
     *
     * This slot is used to broadcast a chat message to everybody.
     *
     * @param handle handle of the player.
     * @param message message content.
     */
    void performChat(QObject *handle, const QString &message);
    /**
     * @brief Perform action
     *
     * This slot is used to manage an action performed
     * by a player.
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
     * @param handle handle of the player.
     * @param tokenCount number of token bet.
     */
    void performAction(QObject *handle, int tokenCount);
signals:
    /**
     * @brief Game properties are broadcasted
     *
     * This signals contains information on the game, like
     * the bets of the players and the content of the pot.
     *
     * @param players player properties.
     * @param pot content of the pot.
     */
    void gamePropertiesBroadcasted(const QList<PlayerProperties> &players, int pot);
    /**
     * @brief Player refused
     * @param handle handle to the player.
     */
    void playerRefused(QObject *handle);
    /**
     * @brief A chat message is broadcasted
     * @param name name of the sender.
     * @param message content of the message.
     */
    void chatBroadcasted(const QString &name, const QString &message);
    /**
     * @brief A new round started
     */
    void newRoundBroadcasted();
    /**
     * @brief Cards are distributed to everybody
     * @param cards cards.
     */
    void cardsDistributed(const QList<Card> &cards);
    /**
     * @brief Cards are distributed to a specific player
     * @param handle handle of the player.
     * @param cards cards.
     */
    void cardsDistributed(QObject *handle, const QList<Card> &cards);
    /**
     * @brief A give player is selected to play
     * @param handle handle of the player.
     */
    void playerTurnSelected(QObject *handle);
    /**
     * @brief A round stopped
     */
    void endRoundBroadcasted();
    /**
     * @brief Cards for everybody broadcasted
     *
     * Since clients only have their cards, we should broadcast
     * the cards if people who didn't fold bet the same quantity
     * of tokens.
     *
     * @param hands list of hands for all players.
     */
    void allCardsBroadcasted(const QList<Hand> &hands);
private:
    /**
     * @internal
     * @brief Enumeration representing the state of the cards being distributed
     */
    enum DistributedCardsStatus {
        /**
         * @internal
         * @short Initial pair of cards for all players
         */
        Initial,
        /**
         * @internal
         * @short First three cards in the middle
         */
        Flop,
        /**
         * @internal
         * @short Fourth card in the middle
         */
        Turn,
        /**
         * @internal
         * @short Last card in the middle
         */
        River
    };
    /**
     * @internal
     * @brief Helper method to compute the index of a player with modulo
     * @param i extended index (can be negative or > player count)
     * @return index of a player, after applying modulo.
     */
    int index(int i);
    /**
     * @internal
     * @brief Get the list of player properties
     * @return list of player properties.
     */
    QList<PlayerProperties> getPlayers() const;
    /**
     * @internal
     * @brief Perform the broadcast of game properties
     */
    void performGamePropertiesBroadcast();
    /**
     * @internal
     * @brief Prepare a round
     */
    void prepareRound();
    /**
     * @internal
     * @brief Go to the turn of the next player
     */
    void nextTurn();
    /**
     * @internal
     * @brief Cleanup a round
     */
    void cleanUpRound();
    /**
     * @internal
     * @brief Manage the draw (two people bet the same amount of tokens at the end)
     */
    void manageDraw();
    /**
     * @internal
     * @brief Distribute cards in the middle
     * @param count number of cards to distribute.
     */
    void distributeMiddleCards(int count);
    /**
     * @internal
     * @brief Game status
     */
    GameStatus m_status;
    /**
     * @internal
     * @brief Status of currently distributed cards
     *
     * Used to know what card should be distributed next.
     */
    DistributedCardsStatus m_distributedCardsStatus;
    /**
     * @internal
     * @brief Initial player of current round
     *
     * It is increased at each round.
     */
    int m_initialPlayer;
    /**
     * @internal
     * @brief Current player
     */
    int m_currentPlayer;
    /**
     * @internal
     * @brief Handle to all players in the game
     */
    QList<QObject *> m_handles;
    /**
     * @internal
     * @brief Map that maps a handle to the properties of a player
     */
    QMap<QObject *, PlayerProperties> m_playerProperties;
    /**
     * @internal
     * @brief Map that maps a handle to a player's hand
     */
    QMap<QObject *, Hand> m_hands;
    /**
     * @internal
     * @brief Deck
     */
    Deck m_deck;
    /**
     * @internal
     * @brief Pot
     */
    int m_pot;
    /**
     * @internal
     * @brief Bet manager
     *
     * It is used to compute blinds, and see if a
     * player can afford paying it.
     */
    BetManager *m_betManager;
    /**
     * @internal
     * @brief Handle pointing to the player who has the highest bet currently
     */
    QObject *m_maxBetHandle;
};

#endif // GAMEMANAGER_H
