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
#include "logic/card.h"

class TstCard: public QObject
{
    Q_OBJECT
private slots:
    void testAssign() {
        // Do some easy assignment tests
        Card card;
        QVERIFY(!card.isValid());
        card.setRank(1);
        QVERIFY(card.rank() == 1);
        QVERIFY(!card.isValid());
        card.setSuit(Card::Club);
        QVERIFY(card.suit() == Card::Club);
        QVERIFY(card.isValid());

        // Do some advanced assignment tests
        card.setRank(1234567);
        QVERIFY(card.rank() == -1);
        QVERIFY(!card.isValid());

        Card card2 (Card::Club, 1);
        QVERIFY(card2.isValid());
        card2.setSuit((Card::Suit) 12323);
        QVERIFY(card2.suit() == Card::Invalid);
        QVERIFY(!card.isValid());
    }
    void testCompare() {
        QList<Card> cards;
        // Create a list of cards that is ordered
        for (int i = 0; i < 13; i++) {
            cards.append(Card(Card::Club, i));
            cards.append(Card(Card::Diamond, i));
            cards.append(Card(Card::Heart, i));
            cards.append(Card(Card::Spade, i));
        }

        // Compare all cards
        for (int i = 0; i < cards.count(); i++) {
            for (int j = i + 1; j < cards.count(); j++) {
                QVERIFY(cards.at(i) < cards.at(j));
            }
            for (int j = 0; j < i; j++) {
                QVERIFY(cards.at(i) > cards.at(j));
            }
            for (int j = 0; j < cards.count(); j++) {
                if (i == j) {
                    QVERIFY(cards.at(i) == cards.at(j));
                } else {
                    QVERIFY(cards.at(i) != cards.at(j));
                }
            }
        }
    }
};

QTEST_MAIN(TstCard)
#include "tst_card.moc"
