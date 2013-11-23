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

#include "deck.h"
#include <QtCore/QDateTime>

Deck::Deck(QObject *parent)
    : QObject(parent)
{
}

int Deck::count() const
{
    return m_cards.count();
}

bool Deck::isEmpty() const
{
    return m_cards.isEmpty();
}

Card Deck::draw()
{
    if (isEmpty()) {
        return Card();
    }

    return m_cards.takeFirst();
}

void Deck::clear()
{
    m_cards.clear();
}

void Deck::reset()
{
    m_cards.clear();
    // Create a standard 52-cards deck
    // it is sorted
    addCards(Card::Spade);
    addCards(Card::Heart);
    addCards(Card::Diamond);
    addCards(Card::Club);
}

void Deck::shuffle()
{
#ifdef CPP11
    // Profit from C++11 features when available
    std::shuffle(m_cards.begin(), m_cards.end(), std::default_random_engine(QDateTime::currentMSecsSinceEpoch()));
#else
    // Initialize seed
    qsrand(QDateTime::currentMSecsSinceEpoch());

    // A rather good shuffle algorithm: swap two random entries, n times
    int size = m_cards.count();
    for (int i = 0; i < size; ++i) {
        m_cards.swap(qrand() % size, qrand() % size);
    }
#endif
}

void Deck::addCards(Card::Suit suit)
{
    for (int i = 0; i < 13; i++) {
        m_cards.append(Card(suit, i));
    }
}
