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

#ifndef BETMANAGER_H
#define BETMANAGER_H

/**
 * @file betmanager.h
 * @short Definition of BetManager
 */

#include "pokqt_global.h"
#include <QtCore/QObject>
#include "playerproperties.h"

/**
 * @brief Manage the bet range
 *
 * This class computes the range of tokens
 * that have to be bet. minBet() provides the
 * current status of the bet, and the minimal step
 * to bet in order to be able to "call", and maxBet()
 * provides the highest step, that is the total number
 * of tokens of the weakest player.
 */
class POKQTSHARED_EXPORT BetManager: public QObject
{
    Q_OBJECT
    /**
     * @short Minimum bet
     */
    Q_PROPERTY(int minBet READ minBet NOTIFY minBetChanged)
    /**
     * @short Maximum bet
     */
    Q_PROPERTY(int maxBet READ maxBet NOTIFY maxBetChanged)
public:
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit BetManager(QObject *parent = 0);
    /**
     * @brief Set player properties
     *
     * When setting player propertis, we also compute the minimum and maximum
     * bet. So, if the player properties are updated, we should provide
     * the list of players to this class using this method.
     *
     * @param players player properties to set.
     */
    void setPlayers(const QList<PlayerProperties> &players);
    /**
     * @brief Get the minimum bet
     * @return minimum bet.
     */
    int minBet() const;
    /**
     * @brief Get the maximum bet
     * @return maximum bet.
     */
    int maxBet() const;
signals:
    /**
     * @brief Minimum bet changed
     */
    void minBetChanged();
    /**
     * @brief Maximum bet changed
     */
    void maxBetChanged();
private:
    /**
     * @brief Minimum bet
     */
    int m_minBet;
    /**
     * @brief Maximum bet
     */
    int m_maxBet;
};

#endif // BETMANAGER_H
