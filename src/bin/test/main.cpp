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

#include <QtGui/QGuiApplication>
#include <QtQml/qqml.h>
#include <QtQuick/QQuickView>
#include <QtNetwork/QHostAddress>
#include <connectionmanager.h>

//class Test: public QObject
//{
//    Q_OBJECT
//    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
//public:
//    explicit Test(QObject *parent = 0);
//    QString name() const;
//    void setName(const QString &name);
//public slots:
//    void connectToHost(const QString &ip, int port);
//    void disconnectFromHost();
//    void sendNickname();
//signals:
//    void nameChanged();
//private:
//    QString m_name;
//    NetworkClient *m_client;
//};

//Test::Test(QObject *parent)
//    : QObject(parent)
//{
//    m_client = new NetworkClient(this);
//}

//QString Test::name() const
//{
//    return m_name;
//}

//void Test::setName(const QString &name)
//{
//    if (m_name != name) {
//        m_name = name;
//        emit nameChanged();
//    }
//}

//void Test::connectToHost(const QString &ip, int port)
//{
//    m_client->connectToHost(QHostAddress(ip), port);
//}

//void Test::disconnectFromHost()
//{
//    m_client->disconnectFromHost();
//}

//void Test::sendNickname()
//{
//    m_client->setName(m_name);
//    m_client->sendMessageString(NetworkClient::NicknameType, m_client->name());
//}

int main(int argc, char **argv)
{
    QGuiApplication app (argc, argv);
    QQuickView view;

    qmlRegisterType<ConnectionManager>("com.ecp.isia.pokqt", 1, 0, "ConnectionManager");

    view.setSource(QUrl("qrc:/main.qml"));
    view.show();
    return app.exec();
}

//#include "main.moc"
