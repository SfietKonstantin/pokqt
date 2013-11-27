#include "cardobject.h"

CardObject::CardObject(QObject *parent) :
    QObject(parent), m_suit(Invalid), m_rank(-1)
{
}

CardObject * CardObject::create(Suit suit, int rank, QObject *parent)
{
    CardObject *card = new CardObject(parent);
    card->m_suit = suit;
    card->m_rank = rank;
    return card;
}

CardObject * CardObject::create(Card::Suit suit, int rank, QObject *parent)
{
    return create((Suit) suit, rank, parent);
}

CardObject::Suit CardObject::suit() const
{
    return m_suit;
}

int CardObject::rank() const
{
    return m_rank;
}

