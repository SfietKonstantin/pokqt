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

#ifndef HAND_H
#define HAND_H

#include "pokqt_global.h"
#include <QtCore/QList>
#include "card.h"

/**
 * @brief A hand
 *
 * This class represents the cards that a player
 * hold in his / her hand.
 *
 * In hold-em poker, players choose 4-5 cards in
 * their hand plus card in the flop / turn / river
 * to create a poker hand. We can then consider that
 * the cards in the flop / turn / river are considered
 * in the hand of a player.
 *
 * This class then groups cards in hand and
 * flop / turn / river together.
 *
 * operator< compares two hands following poker rules. We can
 * determine who is a winner by simply comparing hands with <.
 * Remark that we don't have equality in this version of pokQt:
 * there is always a winner, as we compare the higest card if
 * we don't have a draw.
 */
class POKQTSHARED_EXPORT Hand
{
public:
    /**
     * @brief Default constructor
     *
     * This constructor creates an empty hand.
     */
    explicit Hand();
    /**
     * @brief Copy constructor
     * @param other other hand to copy.
     */
    Hand(const Hand &other);
    /**
     * @brief Assignation
     * @param other other Hand to copy.
     * @return reference to this object.
     */
    Hand & operator=(const Hand &other);
    /**
     * @brief Equality
     * @param other other Hand to compare with.
     * @return if the two Hand are equal.
     */
    bool operator==(const Hand &other) const;
    bool operator<(const Hand &other) const;
    /**
     * @brief Get if the hand is empty
     * @return if the hand is empty.
     */
    bool isEmpty() const;
    /**
     * @brief Get the cards in the hand
     * @return cards in the hand.
     */
    QList<Card> cards() const;
    /**
     * @brief Add a card to the hand
     * @param card card to add.
     */
    void addCard(const Card &card);
    /**
     * @brief Add cards to the hand
     * @param cards cards to add.
     */
    void addCards(const QList<Card> &cards);
    /**
     * @brief Clear the hand
     */
    void clear();
    static QList<Card> straightFlush(const QList<Card> &cards);
    static QList<Card> four(const QList<Card> &cards);
    static QList<Card> fullHouse(const QList<Card> &cards);
    static QList<Card> flush(const QList<Card> &cards);
    static QList<Card> straight(const QList<Card> &cards);
    static QList<Card> three(const QList<Card> &cards);
    static QList<Card> twoPairs(const QList<Card> &cards);
    static QList<Card> pair(const QList<Card> &cards);
private:
    static QList<Card> nCards(const QList<Card> &cards, int n);
    static bool highCardLesser(const QList<Card> &cards1, const QList<Card> cards2);
    static bool compareLesser(const QList<Card> &combo1, const QList<Card> &combo2,
                              const QList<Card> &allCards1, const QList<Card> &allCards2,
                              bool *equal = 0);
    QList<Card> m_cards;
};

QDataStream &operator <<(QDataStream &stream, const Hand &hand);
QDataStream &operator >>(QDataStream &stream, Hand &hand);

#endif // HAND_H
