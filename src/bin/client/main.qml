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

import QtQuick 2.0
import com.ecp.isia.pokqt 1.0

Item {
    width: 800
    height: 600

    Image {
        anchors.fill: parent
        source: "assets/background.jpg"
    }

    NetworkClient {
        id: client
        onChatReceived: chatPanel.appendChatMessage(playerName, chat)
    }

    ChatPanel {
        id: chatPanel
        visible: client.status == NetworkClient.Connected
        anchors.top: parent.top; anchors.bottom: parent.bottom
        anchors.right: parent.right
    }

    ConnectDialog {
        visible: client.status == NetworkClient.NotConnected
        anchors.centerIn: parent
        onConnectToHost: {
            client.playerName = nickname
            client.connectToHost(ip, port)
        }
    }

    Item {
        id: gamingBoard
        anchors.left: parent.left; anchors.right: chatPanel.left
        anchors.top: parent.top;anchors.bottom: parent.bottom
        visible: client.status == NetworkClient.Connected

        PathView {
            id: pathView
            interactive: false
            anchors.fill: parent
            preferredHighlightBegin: 1 / (2 * count)
            preferredHighlightEnd: 1 / (2 * count)

            path: Path {
                startX: 0; startY: pathView.height / 2
                PathQuad {
                    x: pathView.width
                    y: pathView.height / 2
                    controlX: pathView.width / 2
                    controlY: pathView.height / 4
                }
            }

            model: PlayersModel {
                client: client
            }

            delegate:Rectangle {
                width: 50; height: 50
                Text {
                    anchors.centerIn: parent
                    text:model.name
                }
            }
        }
    }
}
