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

#include "card.h"

// TODO: document ranks
// The number representing the rank goes to -1 to 12
// -1 is an invalid card,
// 0 represents 2
// 8 represents 10
// 9 10 11 12 are respectively J Q K A
Card::Card()
    : m_suit(Invalid), m_rank(-1)
{
}

Card::Card(Suit suit, int rank)
    : m_suit(Invalid), m_rank(-1)
{
    // If the rank is not valid, we stay invalid
    if (rank < 0 || rank > 12) {
        return;
    }

    switch (suit) {
    case Club:
    case Diamond:
    case Heart:
    case Spade:
        m_suit = suit;
        m_rank = rank;
        break;
    default:
        // Invalid card
        break;
    }
}

Card::Card(const Card &other)
    : m_suit(other.suit()), m_rank(other.rank())
{
}

Card & Card::operator=(const Card &other)
{
    m_suit = other.suit();
    m_rank = other.rank();
    return *this;
}

bool Card::operator<(const Card &other)
{
    if (m_rank != other.rank()) {
        return m_rank < other.rank();
    }

    return m_suit < other.suit();
}

bool Card::isValid() const
{
    // The "create" factory method should guarantee that our cards
    // are valid, and invalid cases will only be the cases when
    // rank is -1
    return (m_rank != -1);
}

Card::Suit Card::suit() const
{
    return m_suit;
}

void Card::setSuit(Suit suit)
{
    switch (suit) {
    case Club:
    case Diamond:
    case Heart:
    case Spade:
        m_suit = suit;
        break;
    default:
        break;
    }
}

int Card::rank() const
{
    return m_rank;
}

void Card::setRank(int rank)
{
    if (rank >= 0 && rank <= 12) {
        m_rank = rank;
    }
}

QDataStream &operator <<(QDataStream &stream, const Card &card)
{
    stream << (qint16) card.suit() << (qint16) card.rank();
    return stream;
}

QDataStream &operator >>(QDataStream &stream, Card &card)
{
    qint16 suit;
    qint16 rank;
    stream >> suit >> rank;
    card.setSuit((Card::Suit) suit);
    card.setRank(rank);
    return stream;
}
