#ifndef CARDOBJECT_H
#define CARDOBJECT_H

#include <logic/card.h>
#include <QtCore/QObject>

class CardObject : public QObject
{
    Q_OBJECT
    Q_ENUMS(Suit)
    Q_PROPERTY(Suit suit READ suit CONSTANT)
    Q_PROPERTY(int rank READ rank CONSTANT)
public:
    enum Suit {
        Invalid = Card::Invalid,
        Club = Card::Club,
        Diamond = Card::Diamond,
        Heart = Card::Heart,
        Spade = Card::Spade
    };
    static CardObject * create(Suit suit, int rank, QObject *parent = 0);
    static CardObject * create(Card::Suit suit, int rank, QObject *parent = 0);
    explicit CardObject(QObject *parent = 0);
    Suit suit() const;
    int rank() const;
private:
    Suit m_suit;
    int m_rank;
};

#endif // CARDOBJECT_H
