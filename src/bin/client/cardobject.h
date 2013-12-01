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

#ifndef CARDOBJECT_H
#define CARDOBJECT_H

/**
 * @file cardobject.h
 * @short Definition of Client::CardObject
 */

#include <logic/card.h>
#include <QtCore/QObject>

namespace Client
{

/**
 * @brief QML object to display a card
 *
 * This object translates the Card class as a QObject,
 * with read only properties. It makes the Card class
 * practical to be displayed in QML.
 */
class CardObject : public QObject
{
    Q_OBJECT
    Q_ENUMS(Suit)
    /**
     * @brief Suit
     */
    Q_PROPERTY(Suit suit READ suit CONSTANT)
    /**
     * @brief Rank
     */
    Q_PROPERTY(int rank READ rank CONSTANT)
public:
    /**
     * @brief Suit enumeration
     */
    enum Suit {
        /**
         * @brief An invalid card
         */
        Invalid = Card::Invalid,
        /**
         * @brief The card is a club
         */
        Club = Card::Club,
        /**
         * @brief The card is a diamond
         */
        Diamond = Card::Diamond,
        /**
         * @brief The card is a heart
         */
        Heart = Card::Heart,
        /**
         * @brief The card is a spade
         */
        Spade = Card::Spade
    };
    /**
     * @brief Invalid constructor
     * @param parent parent object.
     */
    explicit CardObject(QObject *parent = 0);
    /**
     * @brief Create a card
     * @param suit suit of the card.
     * @param rank rank of the card.
     * @param parent parent object.
     * @return created CardObject.
     */
    static CardObject * create(Suit suit, int rank, QObject *parent = 0);
    /**
     * @brief Create a card
     * @param suit suit of the card, as a Card::Suit.
     * @param rank rank of the card.
     * @param parent parent object.
     * @return created CardObject.
     */
    static CardObject * create(Card::Suit suit, int rank, QObject *parent = 0);
    /**
     * @brief Get the suit
     * @return suit.
     */
    Suit suit() const;
    /**
     * @brief Get the rank
     * @return rank.
     */
    int rank() const;
private:
    /**
     * @internal
     * @brief Suit
     */
    Suit m_suit;
    /**
     * @internal
     * @brief Rank
     */
    int m_rank;
};

}

#endif // CARDOBJECT_H
