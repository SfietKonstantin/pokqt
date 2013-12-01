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

/**
 * @file deck.cpp
 * @short Implementation of Deck
 */

#include "hand.h"
#include <functional>
#include <QtCore/QMultiMap>

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
    if (isEmpty()) {
        return false;
    }

    if (other.isEmpty()) {
        return true;
    }

    QList<Card> myCards = m_cards;
    QList<Card> otherCards = other.cards();

    // We get a sorted list of cards (decending order)
    std::sort(myCards.begin(), myCards.end(), std::greater<Card>());
    std::sort(otherCards.begin(), otherCards.end(), std::greater<Card>());

    bool equal = false;
    QList<Card> myCombo = straightFlush(myCards);
    QList<Card> otherCombo = straightFlush(myCards);
    bool lesser = compareLesser(myCombo, otherCombo, myCards, otherCards, &equal);
    if (!equal) {
        return lesser;
    }

    myCombo = four(myCards);
    otherCombo = four(myCards);
    lesser = compareLesser(myCombo, otherCombo, myCards, otherCards, &equal);
    if (!equal) {
        return lesser;
    }

    myCombo = fullHouse(myCards);
    otherCombo = fullHouse(myCards);
    lesser = compareLesser(myCombo, otherCombo, myCards, otherCards, &equal);
    if (!equal) {
        return lesser;
    }

    myCombo = flush(myCards);
    otherCombo = flush(myCards);
    lesser = compareLesser(myCombo, otherCombo, myCards, otherCards, &equal);
    if (!equal) {
        return lesser;
    }

    myCombo = straight(myCards);
    otherCombo = straight(myCards);
    lesser = compareLesser(myCombo, otherCombo, myCards, otherCards, &equal);
    if (!equal) {
        return lesser;
    }

    myCombo = three(myCards);
    otherCombo = three(myCards);
    lesser = compareLesser(myCombo, otherCombo, myCards, otherCards, &equal);
    if (!equal) {
        return lesser;
    }

    myCombo = twoPairs(myCards);
    otherCombo = twoPairs(myCards);
    lesser = compareLesser(myCombo, otherCombo, myCards, otherCards, &equal);
    if (!equal) {
        return lesser;
    }

    myCombo = pair(myCards);
    otherCombo = pair(myCards);
    lesser = compareLesser(myCombo, otherCombo, myCards, otherCards, &equal);
    if (!equal) {
        return lesser;
    }

    return highCardLesser(myCards, otherCards);
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

QList<Card> Hand::straightFlush(const QList<Card> &cards)
{
    // Straight flush is a straight that is also a flush
    // Basically, we sort cards by suit and try if there is a straight in them
    QMultiMap<Card::Suit, Card> cardsBySuit;

    foreach (const Card &card, cards) {
        cardsBySuit.insert(card.suit(), card);
    }

    QList<Card> straightCards = straight(cardsBySuit.values(Card::Spade));
    if (!straightCards.isEmpty()) {
        return straightCards;
    }
    straightCards = straight(cardsBySuit.values(Card::Heart));
    if (!straightCards.isEmpty()) {
        return straightCards;
    }
    straightCards = straight(cardsBySuit.values(Card::Diamond));
    if (!straightCards.isEmpty()) {
        return straightCards;
    }
    straightCards = straight(cardsBySuit.values(Card::Club));
    if (!straightCards.isEmpty()) {
        return straightCards;
    }

    return QList<Card>();
}

QList<Card> Hand::four(const QList<Card> &cards)
{
    return nCards(cards, 4);
}

QList<Card> Hand::fullHouse(const QList<Card> &cards)
{
    // Full house is basically a 3 followed by a 2
    // Note on comparing full houses: we compare the biggest values in
    // between the 3 before comparing the biggest values between the 2
    QList<Card> updatedCards = cards;
    QList<Card> threeCards = three(updatedCards);

    if (threeCards.isEmpty()) {
        return QList<Card>();
    }

    foreach (const Card &card, threeCards) {
        updatedCards.removeAll(card);
    }

    QList<Card> pairCards = pair(updatedCards);

    if (pairCards.isEmpty()) {
        return QList<Card>();
    }

    QList<Card> returnedCards;
    returnedCards.append(threeCards);
    returnedCards.append(pairCards);

    return returnedCards;
}

QList<Card> Hand::flush(const QList<Card> &cards)
{
    // Idea of the algo is
    // 1. sort the cards by descending order
    // 2. check if there is a group of exaclty 5 cards in the list
    // 3. if yes, return the list, if no return an empty list
    QList<Card> sortedCards = cards;
    std::sort(sortedCards.begin(), sortedCards.end(), std::greater<Card>());

    Card::Suit currentSuit = Card::Invalid;
    QList<Card> returnedCards;

    foreach (const Card &card, sortedCards) {
        if (card.suit() != currentSuit) {
            currentSuit = card.suit();
            returnedCards.clear();
        }
        returnedCards.append(card);

        if (returnedCards.count() == 5) {
            return returnedCards;
        }
    }

    return QList<Card>();
}

QList<Card> Hand::straight(const QList<Card> &cards)
{
    // Idea of the algo is
    // 1. sort the cards by descending order
    // 2. check if there is a group of exaclty 5 cards in the list that is decreasing by 1
    //    each time
    // 3. if yes, return the list, if no return an empty list
    // Remark: Aces are both the biggest and smallest card (ie AKQJ10 and 5432A), so
    // we are cheating by adding aces at the end of the sorted list if there are aces.
    // Moreover, we will consider that under rank 0 (that is rank of a 2), we have rank
    // 12 (rank of an ace)
    // Remark 2: we should also manage cases like AKQJJ10, with two or more cards
    // having the same rank.

    QList<Card> sortedCards = cards;
    QList<Card> aces;
    std::sort(sortedCards.begin(), sortedCards.end(), std::greater<Card>());
    foreach (const Card &card, sortedCards) {
        if (card.rank() == 12) { // Ace
            aces.append(card);
        }
    }

    sortedCards.append(aces);

    int currentRank = -1;
    QList<Card> returnedCards;

    foreach (const Card &card, sortedCards) {
        if ((card.rank() == currentRank - 1) || (currentRank == 0 && card.rank() == 12)) {
            // Note that we don't set to the rank of the card
            // otherwise, we might create stuff like 2AKQJ.
            // by -- the rank, after an ace, we get rank -1, that
            // other card can match
            currentRank --;
            returnedCards.append(card);
        } else if (card.rank() != currentRank) {
            currentRank = card.rank();
            returnedCards.clear();
            returnedCards.append(card);
        }
        // Do nothing if we find a card with the same rank

        if (returnedCards.count() == 5) {
            return returnedCards;
        }
    }

    return QList<Card>();
}

QList<Card> Hand::three(const QList<Card> &cards)
{
    return nCards(cards, 3);
}

QList<Card> Hand::twoPairs(const QList<Card> &cards)
{
    // Idea of the algo is
    // 1. search for the first pair (biggest pair)
    // 2. if not found return, else search for a second pair (second biggest pair)
    // 3. if not found return, else construct a list of the two pairs
    QList<Card> updatedCards = cards;
    QList<Card> firstPair = pair(updatedCards);

    if (firstPair.isEmpty()) {
        return QList<Card>();
    }

    foreach (const Card &card, firstPair) {
        updatedCards.removeAll(card);
    }

    QList<Card> secondPair = pair(updatedCards);

    if (secondPair.isEmpty()) {
        return QList<Card>();
    }

    QList<Card> returnedCards;
    returnedCards.append(firstPair);
    returnedCards.append(secondPair);

    return returnedCards;
}

QList<Card> Hand::pair(const QList<Card> &cards)
{
    return nCards(cards, 2);
}

QList<Card> Hand::nCards(const QList<Card> &cards, int n)
{
    // The idea of the algo that extract the pair is
    // 1. sort the cards by descending order
    // 2. check if there is a group of exaclty n cards in the list
    // 3. if yes, return the list, if no return an empty list

    QList<Card> sortedCards = cards;
    std::sort(sortedCards.begin(), sortedCards.end(), std::greater<Card>());

    int currentRank = -1;
    QList<Card> returnedCards;

    foreach (const Card &card, sortedCards) {
        if (card.rank() != currentRank) {
            currentRank = card.rank();
            returnedCards.clear();
        }
        returnedCards.append(card);

        if (returnedCards.count() == n) {
            return returnedCards;
        }
    }

    return QList<Card>();
}

bool Hand::highCardLesser(const QList<Card> &cards1, const QList<Card> cards2)
{
    // Compare the biggest cards
    // We should always compare two lists of different cards
    // otherwise the result is provided by a fallback comparison
    // that is not accurate.
    QList<Card> sortedCards1 = cards1;
    std::sort(sortedCards1.begin(), sortedCards1.end(), std::greater<Card>());
    QList<Card> sortedCards2 = cards2;
    std::sort(sortedCards2.begin(), sortedCards2.end(), std::greater<Card>());

    int n = qMin(sortedCards1.count(), sortedCards2.count());
    for (int i = 0; i < n; i++) {
        const Card &card1 = sortedCards1.at(i);
        const Card &card2 = sortedCards2.at(i);
        if (card1 != card2) {
            return card1 < card2;
        }
    }

    // All the first cards are equal
    // fallback to checking if one have more cards than
    // the other.
    return sortedCards1.count() <= sortedCards2.count();
}

bool Hand::compareLesser(const QList<Card> &combo1, const QList<Card> &combo2,
                         const QList<Card> &allCards1, const QList<Card> &allCards2, bool *equal)
{
    // Note that this function is, at best practical, and could be refactored
    // as a macro, or as a function with a better behaviour.

    // Set equal to false
    if (equal) {
        *equal = false;
    }

    // Noone have this combo, so we are equal
    if (combo1.isEmpty() && combo2.isEmpty()) {
        if (equal) {
            *equal = true;
        }

        // Dummy return
        return true;
    }

    // 1 don't have the combo, but 2 have
    if (combo1.isEmpty()) {
        return true;
    }

    // 2 don't have the combo, but 1 have
    if (combo2.isEmpty()) {
        return false;
    }

    // Both have, we compare the higest card
    if (combo1 != combo2) {
        return highCardLesser(combo1, combo2);
    }

    // Last resort: the combo are the same, compare
    // the highest card is all cards.
    return highCardLesser(allCards1, allCards2);
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
