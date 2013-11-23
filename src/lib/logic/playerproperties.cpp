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
 * @file playerproperties.cpp
 * @short Implementation of PlayerProperties
 */

#include "playerproperties.h"
#include <QtCore/QDataStream>

PlayerProperties::PlayerProperties()
    : m_tokenCount(0), m_betCount(0)
{
}

PlayerProperties & PlayerProperties::operator=(const PlayerProperties &other)
{
    m_name = other.name();
    m_tokenCount = other.tokenCount();
    m_betCount = other.betCount();
    return *this;
}

QString PlayerProperties::name() const
{
    return m_name;
}

void PlayerProperties::setName(const QString &name)
{
    m_name = name;
}

int PlayerProperties::tokenCount() const
{
    return m_tokenCount;
}

void PlayerProperties::setTokenCount(int tokenCount)
{
    m_tokenCount = tokenCount;
}

int PlayerProperties::betCount() const
{
    return m_betCount;
}

void PlayerProperties::setBetCount(int betCount)
{
    m_betCount = betCount;
}

bool PlayerProperties::operator==(const PlayerProperties &other)
{
    return (m_tokenCount == other.tokenCount()
            && m_betCount == other.betCount()
            && m_name == other.name());
}

QDataStream &operator<<(QDataStream &stream, const PlayerProperties &playerProperties)
{
    stream << playerProperties.name() << (qint32) playerProperties.tokenCount()
           << (qint32) playerProperties.betCount();
    return stream;
}

QDataStream &operator >>(QDataStream &stream, PlayerProperties &playerProperties)
{
    QString name;
    qint32 tokenCount;
    qint32 betCount;
    stream >> name >> tokenCount >> betCount;
    playerProperties.setName(name);
    playerProperties.setTokenCount((int) tokenCount);
    playerProperties.setBetCount((int) betCount);
    return stream;
}
