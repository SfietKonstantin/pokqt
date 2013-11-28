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

#include "hand.h"

Hand::Hand()
{
}

Hand::Hand(const Hand &other)
{
    m_cards = other.cards();
}

Hand & Hand::operator=(const Hand &other)
{
    m_cards = other.cards();
    return *this;
}

bool Hand::operator==(const Hand &other) const
{
    return m_cards == other.cards();
}

bool Hand::operator<(const Hand &other) const
{
    // Implement poker rules here

    // Basic rule: we compare the biggest card
    if (isEmpty()) {
        return false;
    }

    if (other.isEmpty()) {
        return true;
    }

    QList<Card> myCards = m_cards;
    QList<Card> otherCards = other.cards();
    std::sort(myCards.begin(), myCards.end());
    std::sort(otherCards.begin(), otherCards.end());

    return myCards.last() < otherCards.last();
}

bool Hand::isEmpty() const
{
    return m_cards.isEmpty();
}

QList<Card> Hand::cards() const
{
    return m_cards;
}

void Hand::addCard(const Card &card)
{
    m_cards.append(card);
}

void Hand::addCards(const QList<Card> &cards)
{
    m_cards.append(cards);
}

void Hand::clear()
{
    m_cards.clear();
}

QDataStream &operator <<(QDataStream &stream, const Hand &hand)
{
    stream << hand.cards();
    return stream;
}

QDataStream &operator >>(QDataStream &stream, Hand &hand)
{
    QList<Card> cards;
    stream >> cards;
    hand.addCards(cards);
    return stream;
}
