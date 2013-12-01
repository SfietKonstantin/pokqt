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

#ifndef HANDMODEL_H
#define HANDMODEL_H

/**
 * @file handmodel.h
 * @short Definition of Client::HandModel
 */

#include <QtCore/QAbstractListModel>
#include <network/networkclient.h>
#include "cardobject.h"

namespace Client
{

/**
 * @brief QML model that represent the hand of a player
 *
 * This model is used to represent the hand of a player. It
 * differentiates cards that are in the hand, and cards
 * that are in the flop, turn and river.
 *
 * This model contains all the cards, but sets a different
 * card type for the different kind of cards. You might want
 * to use serveral models to display the different types
 * of cards in the different parts of the gaming table.
 */
class HandModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(CardType)
    /**
     * @short Count
     */
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    /**
     * @short NetworkClient used to get information
     */
    Q_PROPERTY(NetworkClient *client READ client WRITE setClient NOTIFY clientChanged)
public:
    /**
     * @brief Enumeration describing the card type
     */
    enum CardType {
        /**
         * @short The card is in the hand
         */
        Hand,
        /**
         * @short The card is in the flop
         */
        Flop,
        /**
         * @short The card is in the turn
         */
        Turn,
        /**
         * @short The card is in the river
         */
        River
    };
    /**
     * @brief Model role
     */
    enum HandModelRole {
        /**
         * @short %Card
         */
        CardRole,
        /**
         * @short Type of the card
         */
        CardTypeRole
    };
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit HandModel(QObject *parent = 0);
    /**
     * @brief Roles names
     * @return role names.
     */
    QHash<int, QByteArray> roleNames() const;
    /**
     * @brief Row count
     * @param parent parent index.
     * @return number of elements in the model.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    /**
     * @brief Count
     * @return number of elements in the model.
     */
    int count() const;
    /**
     * @brief Get the NetworkClient used to get information
     * @return NetworkClient used to get information.
     */
    NetworkClient * client() const;
    /**
     * @brief Set the NetworkClient used to get information
     * @param client NetworkClient used to get information to set.
     */
    void setClient(NetworkClient *client);
    /**
     * @brief Model data
     * @param index index.
     * @param role role.
     * @return data at the given index with the given role.
     */
    QVariant data(const QModelIndex &index, int role) const;
signals:
    /**
     * @brief Count changed
     */
    void countChanged();
    /**
     * @brief NetworkClient used to get information changed
     */
    void clientChanged();
private:
    /**
     * @internal
     * @brief Model data
     */
    QList<CardObject *> m_data;
    /**
     * @internal
     * @brief NetworkClient used to get information
     */
    NetworkClient *m_client;
private slots:
    /**
     * @internal
     * @brief Slot used to listen to hand changes
     */
    void slotHandChanged();
};

}

#endif // HANDMODEL_H
