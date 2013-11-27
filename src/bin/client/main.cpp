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
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <network/networkclient.h>
#include "playersmodel.h"
#include "clientbetmanager.h"
#include "handmodel.h"
#include "cardobject.h"

// "Functor" to be used in Qt compile-time check "connect"
// Connecting to QCoreApplication::quit fails, because it is
// a static slot.
void quit()
{
    QCoreApplication::instance()->quit();
}

int main(int argc, char **argv)
{
    QGuiApplication app (argc, argv);

    QQuickView view;
    QObject::connect(view.engine(), &QQmlEngine::quit, quit);

    qmlRegisterType<NetworkClient>("com.ecp.isia.pokqt", 1, 0, "NetworkClient");
    qmlRegisterType<PlayersModel>("com.ecp.isia.pokqt", 1, 0, "PlayersModel");
    qmlRegisterType<ClientBetManager>("com.ecp.isia.pokqt", 1, 0, "BetManager");
    qmlRegisterType<CardObject>("com.ecp.isia.pokqt", 1, 0, "CardObject");
    qmlRegisterType<HandModel>("com.ecp.isia.pokqt", 1, 0, "HandModel");

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
