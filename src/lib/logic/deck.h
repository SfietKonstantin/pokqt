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

#ifndef DECK_H
#define DECK_H

/**
 * @file deck.h
 * @short Definition of Deck
 */

#include <QtCore/QList>
#include "card.h"

/**
 * @brief A deck
 *
 * This class manages a classic 52-cards. It is able
 * to create a deck, shuffle it, and distribute cards,
 * represented by the Card class.
 */
class Deck
{
public:
    /**
     * @brief Deck
     * @param parent
     */
    explicit Deck();
    /**
     * @brief Get the number of cards in the deck
     * @return number of cards in the deck.
     */
    int count() const;
    /**
     * @brief Get if the deck is empty
     * @return if the deck is empty.
     */
    bool isEmpty() const;
    /**
     * @brief Draw a card from the deck
     * @return card drawn from the deck.
     */
    Card draw();
    /**
     * @brief Clear the deck
     *
     * Clear the deck, removing all cards, and leaving
     * the deck empty.
     */
    void clear();
    /**
     * @brief Reset the deck
     *
     * Reset the deck, creating an ordered deck starting
     * with the 2 of spades, and finishing with the ace of
     * club.
     */
    void reset();
    /**
     * @brief Shuffle the deck
     */
    void shuffle();
private:
    /**
     * @internal
     * @brief Method used to create a deck
     *
     * This method is used to create a deck, by
     * adding all the cards of a given suit to
     * the deck.
     *
     * @param suit suit used.
     */
    void addCards(Card::Suit suit);
    /**
     * @internal
     * @brief Cards in the deck
     */
    QList<Card> m_cards;
};

#endif // DECK_H
