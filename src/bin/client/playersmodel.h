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

#ifndef PLAYERSMODEL_H
#define PLAYERSMODEL_H

/**
 * @file playersmodel.h
 * @short Definition of Client::HandModel
 */

#include <QtCore/QAbstractListModel>
#include <network/networkclient.h>
#include <logic/playerproperties.h>

namespace Client
{

/**
 * @brief QML model that represent players and the game
 *
 * This model is used to represent players properties, as
 * well as some properties of the game, like the pot.
 *
 * Remark: properties of the player represented by the NetworkClient
 * that is associated to this model is set in the properties of the
 * model. The model itself only contains other players.
 */
class PlayersModel : public QAbstractListModel
{
    Q_OBJECT
    /**
     * @short Count
     */
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    /**
     * @short NetworkClient used to get information
     */
    Q_PROPERTY(NetworkClient *client READ client WRITE setClient NOTIFY clientChanged)
    /**
     * @short Current pot
     */
    Q_PROPERTY(int pot READ pot NOTIFY potChanged)
    /**
     * @short Name of the player
     */
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    /**
     * @short Token count of the player
     */
    Q_PROPERTY(int tokenCount READ tokenCount NOTIFY tokenCountChanged)
    /**
     * @short Token bet by the player
     */
    Q_PROPERTY(int betCount READ betCount NOTIFY betCountChanged)
public:
    /**
     * @brief Model role
     */
    enum PlayersModelRole {
        /**
         * @short Name of a player
         */
        NameRole,
        /**
         * @short Token count of a player
         */
        TokenCountRole,
        /**
         * @short Token bet by a player
         */
        BetCountRole
    };
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit PlayersModel(QObject *parent = 0);
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
     * @brief Get the current pot
     * @return current pot.
     */
    int pot() const;
    /**
     * @brief Get the name of the player
     * @return name of the player.
     */
    QString name() const;
    /**
     * @brief Get the token count of the player
     * @return token count of the player.
     */
    int tokenCount() const;
    /**
     * @brief Get the token bet by the player
     * @return token bet by the player.
     */
    int betCount() const;
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
    /**
     * @brief Current pot changed
     */
    void potChanged();
    /**
     * @brief Name of the player changed
     */
    void nameChanged();
    /**
     * @brief Token count of the player changed
     */
    void tokenCountChanged();
    /**
     * @brief Token bet by the player changed
     */
    void betCountChanged();
private:
    /**
     * @internal
     * @brief Current pot
     */
    int m_pot;
    /**
     * @internal
     * @brief Model data
     */
    QList<PlayerProperties> m_data;
    /**
     * @internal
     * @brief NetworkClient used to get information
     */
    NetworkClient *m_client;
    /**
     * @internal
     * @brief Current player properties
     */
    PlayerProperties m_player;
private slots:
    /**
     * @internal
     * @brief Slot used to listen to player properties changes
     */
    void slotPlayersChanged();
};

}

#endif // PLAYERSMODEL_H
