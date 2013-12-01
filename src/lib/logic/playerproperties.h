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

#ifndef PLAYERPROPERTIES_H
#define PLAYERPROPERTIES_H

/**
 * @file playerproperties.h
 * @short Definition of PlayerProperties
 */
#include <QtCore/QString>

/**
 * @brief Player properties
 *
 * This class stores properties of a give player.
 * For a poker game, you usually needs to store the
 * nickname, and number of tokens you still have and
 * have bet.
 *
 * This class is both used in server side and client
 * side to store the current state of the game.
 *
 * Cards in hand are not stored in this class, since
 * they are stored server-side only (to make sure that
 * there is no way to cheat).
 */
class PlayerProperties
{
public:
    /**
     * @brief Default constructor
     */
    explicit PlayerProperties();
    /**
     * @brief Assignation
     * @param other other PlayerProperties to copy.
     * @return reference to this object.
     */
    PlayerProperties & operator=(const PlayerProperties &other);
    /**
     * @brief Get player's name
     * @return player's name.
     */
    QString name() const;
    /**
     * @brief Set player's name
     * @param name player's name to set.
     */
    void setName(const QString &name);
    /**
     * @brief Get player's token count
     * @return player's token count.
     */
    int tokenCount() const;
    /**
     * @brief Set player's token count
     * @param tokens player's token count to set.
     */
    void setTokenCount(int tokenCount);
    /**
     * @brief Get the number of tokens the player has bet
     * @return the number of tokens the player has bet.
     */
    int betCount() const;
    /**
     * @brief Set the number of tokens the player has bet
     * @param betCount the number of tokens the player has bet to set.
     */
    void setBetCount(int betCount);
    /**
     * @brief Get if the player is still in game
     * @return if the player is still in game.
     */
    bool isInGame() const;
    /**
     * @brief Set if the player is still in game
     * @param inGame if the player is still in game.
     */
    void setInGame(bool inGame);
    /**
     * @brief Equality
     *
     * Implementation of the equality operator. Two player properties
     * are equal if they have the same name and token count.
     *
     * @param other other object.
     * @return if this instance and the other object are equal.
     */
    bool operator==(const PlayerProperties &other);
private:
    /**
     * @internal
     * @brief Player's name
     */
    QString m_name;
    /**
     * @internal
     * @brief Player's token count
     */
    int m_tokenCount;
    /**
     * @internal
     * @brief The number of tokens the player has bet
     */
    int m_betCount;
    /**
     * @internal
     * @brief If the player is still in game
     */
    bool m_inGame;
};

/**
 * @brief Serialize a PlayerProperties in a QDataStream
 * @param stream stream used to serialize.
 * @param playerProperties object to serialize.
 * @return a reference to the stream with the serialized object.
 */
QDataStream &operator <<(QDataStream &stream, const PlayerProperties &playerProperties);
/**
 * @brief Deserialize a PlayerProperties from a QDataStream
 * @param stream stream used to deserialize.
 * @param playerProperties reference to the object that is used to store deserialized data.
 * @return a reference to the stream without the serialized object.
 */
QDataStream &operator >>(QDataStream &stream, PlayerProperties &playerProperties);

#endif // PLAYERPROPERTIES_H
