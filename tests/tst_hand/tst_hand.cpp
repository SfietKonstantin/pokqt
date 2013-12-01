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

#include <QtCore/QObject>
#include <QtTest/QtTest>
#include "logic/hand.h"

class TstHand: public QObject
{
    Q_OBJECT
private slots:
    void testGeneric() {
        // Some tests to check if the pattern matching algo works
        // We should also write algo that tests if they find wrong cases

        QList<Card> royalFlush;
        royalFlush << Card(Card::Heart, 12) << Card(Card::Heart, 11) << Card(Card::Heart, 10)
                   << Card(Card::Heart, 9) << Card(Card::Heart, 8);
        QVERIFY(!Hand::straightFlush(royalFlush).isEmpty());

        // Tricky one
        royalFlush.clear();
        royalFlush << Card(Card::Heart, 12) << Card(Card::Heart, 11) << Card(Card::Heart, 10)
                   << Card(Card::Heart, 9) << Card(Card::Spade, 8) << Card(Card::Heart, 8);
        QVERIFY(!Hand::straightFlush(royalFlush).isEmpty());

        // 2nd tricky one: we have a flush and a straight that is not a straight flush
        royalFlush.clear();
        royalFlush << Card(Card::Heart, 12) << Card(Card::Heart, 11) << Card(Card::Heart, 10)
                   << Card(Card::Heart, 9) << Card(Card::Heart, 2) << Card(Card::Spade, 8);
        QVERIFY(Hand::straightFlush(royalFlush).isEmpty());

        QList<Card> four;
        four << Card(Card::Heart, 12) << Card(Card::Spade, 12) << Card(Card::Club, 12)
             << Card(Card::Diamond, 12) << Card(Card::Heart, 2);
        QVERIFY(!Hand::four(four).isEmpty());

        QList<Card> fullHouse;
        fullHouse << Card(Card::Heart, 12) << Card(Card::Spade, 12) << Card(Card::Club, 2)
                  << Card(Card::Diamond, 2) << Card(Card::Heart, 2);
        QVERIFY(!Hand::fullHouse(fullHouse).isEmpty());

        QList<Card> flush;
        flush << Card(Card::Heart, 12) << Card(Card::Heart, 7) << Card(Card::Heart, 5)
              << Card(Card::Heart, 11) << Card(Card::Heart, 2);
        QVERIFY(!Hand::flush(flush).isEmpty());

        QList<Card> straight;
        straight << Card(Card::Heart, 7) << Card(Card::Heart, 6) << Card(Card::Club, 5)
                 << Card(Card::Spade, 4) << Card(Card::Diamond, 3);
        QVERIFY(!Hand::straight(straight).isEmpty());

        // Tricky one
        straight.clear();
        straight << Card(Card::Heart, 7) << Card(Card::Heart, 6) << Card(Card::Club, 5)
                 << Card(Card::Spade, 4) << Card(Card::Diamond, 4)<< Card(Card::Diamond, 3);
        QVERIFY(!Hand::straight(straight).isEmpty());

        QList<Card> three;
        three << Card(Card::Heart, 7) << Card(Card::Club, 7) << Card(Card::Spade, 7)
              << Card(Card::Spade, 4) << Card(Card::Diamond, 3);
        QVERIFY(!Hand::three(three).isEmpty());

        QList<Card> twoPairs;
        twoPairs << Card(Card::Heart, 7) << Card(Card::Club, 7) << Card(Card::Spade, 5)
                 << Card(Card::Spade, 4) << Card(Card::Diamond, 4);
        QVERIFY(!Hand::twoPairs(twoPairs).isEmpty());

        QList<Card> pair;
        pair << Card(Card::Heart, 7) << Card(Card::Club, 7) << Card(Card::Spade, 5)
             << Card(Card::Spade, 4) << Card(Card::Diamond, 3);
        QVERIFY(!Hand::pair(pair).isEmpty());
    }

    // We could have generated all possible hands
    // It takes 52 * 51 * 50 * 49 * 48 * 47 * 46 combinaisons ~ 6 10^11 = > 100 gig of RAM
    // I decided to only test simple cases
    void testHandHighestCard() {
        QList<Hand> hands;
        for (int i = 0; i < 13; i++) {
            for (int j = 0; j < 4; j++) {
                Hand hand;
                hand.addCard(Card ((Card::Suit) (j + 1), i));
                hands.append(hand);
            }
        }

        for (int i = 0; i < hands.count(); i++) {
            for (int j = 0; j < i; j++) {
                QVERIFY(hands.at(j) < hands.at(i));
            }
        }
    }
    // TODO: write more unit tests for all combinaisons
};

QTEST_MAIN(TstHand)
#include "tst_hand.moc"

