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

#ifndef CARD_H
#define CARD_H

/**
 * @file card.h
 * @short Definition of Card
 */

#include "pokqt_global.h"
#include <QtCore/QDataStream>

/**
 * @brief A card
 *
 * This class represents a class in the classic
 * french card deck. A card have a suit and a rank.
 *
 * Suits are represented by the enumeration Suit.
 * While ranks could have been represented with
 * an enumeration too, they are actually represented
 * with an int. Here is the meaning of the rank integer:
 * - -1: invalid card
 * - 0 to 8: 2 to 10
 * - 9: J
 * - 10: Q
 * - 11: K
 * - 12: A
 *
 * With this matching, we have a matching between the
 * power of the card and the value of the rank integer.
 *
 * Remark: the suit enumeration is also made such that
 * we have an order on suits. For a given rank, a spade
 * is bigger than a heart, that is bigger than a diamond
 * etc.
 */
class POKQTSHARED_EXPORT Card
{
public:
    /**
     * @brief Suit enumeration
     */
    enum Suit {
        /**
         * @brief An invalid card
         */
        Invalid = 0,
        /**
         * @brief The card is a club
         */
        Club = 1,
        /**
         * @brief The card is a diamond
         */
        Diamond = 2,
        /**
         * @brief The card is a heart
         */
        Heart = 3,
        /**
         * @brief The card is a spade
         */
        Spade = 4
    };
    /**
     * @brief Invalid constructor
     *
     * This constructor creates an invalid card.
     */
    explicit Card();
    /**
     * @brief Default constructor
     * @param suit suit to set.
     * @param rank rank to set.
     */
    explicit Card(Suit suit, int rank);
    /**
     * @brief Copy constructor
     * @param other other Card to copy.
     */
    Card(const Card &other);
    /**
     * @brief Assignment
     * @param other other Card to copy.
     * @return reference to this object.
     */
    Card & operator=(const Card &other);
    /**
     * @brief Equality
     * @param other other Card to compare with.
     * @return if the two Card are equal.
     */
    bool operator==(const Card &other) const;
    /**
     * @brief Inequality
     * @param other other Card to compare with.
     * @return if the two Card are not equal.
     */
    bool operator!=(const Card &other) const;
    /**
     * @brief Comparison
     *
     * Provides an order in cards.
     *
     * @param other other Card to compare with.
     * @return if this Card is smaller than the other Card.
     */
    bool operator<(const Card &other) const;
    /**
     * @brief Comparison
     *
     * Provides an order in cards.
     *
     * @param other other Card to compare with.
     * @return if this Card is bigger than the other Card.
     */
    bool operator>(const Card &other) const;
    /**
     * @brief Get if the card is valid
     * @return if the card is valid.
     */
    bool isValid() const;
    /**
     * @brief Get the card's suit
     * @return the card's suit.
     */
    Suit suit() const;
    /**
     * @brief Set the card's suit
     * @param suit suit to set.
     */
    void setSuit(Suit suit);
    /**
     * @brief Get the card's rank
     * @return the card's rank.
     */
    int rank() const;
    /**
     * @brief Set the card's rank
     * @param rank rank to set.
     */
    void setRank(int rank);
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

/**
 * @brief Serialize a Card in a QDataStream
 * @param stream stream used to serialize.
 * @param card object to serialize.
 * @return a reference to the stream with the serialized object.
 */
QDataStream &operator <<(QDataStream &stream, const Card &card);
/**
 * @brief Deserialize a Card from a QDataStream
 * @param stream stream used to deserialize.
 * @param card reference to the object that is used to store deserialized data.
 * @return a reference to the stream without the serialized object.
 */
QDataStream &operator >>(QDataStream &stream, Card &card);

#endif // CARD_H
