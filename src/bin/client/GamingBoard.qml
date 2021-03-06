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
import QtQuick.Controls 1.0
import com.ecp.isia.pokqt 1.0
import "UiConstants.js" as Ui

Item {
    id: gamingBoard

    PathView {
        id: pathView
        anchors.fill: parent
        interactive: false
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
            id: playersModel
            client: networkClient
        }

        delegate: PlayerData {
            name: model.name
            tokenCount: model.tokenCount
            betCount: model.betCount
        }
    }

    PlayerData {
        id: player
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        name: playersModel.name
        tokenCount: playersModel.tokenCount
        betCount: playersModel.betCount
    }

    BetManager {
        id: betManager
        client: networkClient
    }

    Column {
        id: buttons
        visible: networkClient.turn
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        SpinBox {
            id: betSpin
            onMinimumValueChanged: value = minimumValue
            minimumValue: Math.max(0, betManager.minBet - playersModel.betCount)
            maximumValue: Math.min(betManager.maxBet - playersModel.betCount,
                                   playersModel.tokenCount - playersModel.betCount)
        }

        Button {
            text: qsTr("Raise")
            iconSource: "assets/raise.png"
            onClicked: betManager.raise(betSpin.value)
        }
        Button {
            iconSource: betManager.check ? "assets/check.png" : "assets/call.png"
            text: betManager.check ? qsTr("Check") : qsTr("Call")
            onClicked: betManager.call()
        }
        Button {
            iconSource: "assets/fold.png"
            text: qsTr("Fold")
            onClicked: betManager.fold()
        }
    }

    Item {
        anchors.left: buttons.right; anchors.right: player.left
        anchors.top: player.top; anchors.bottom: parent.bottom

        Row {
            anchors.centerIn: parent
            spacing: Ui.MARGIN_DEFAULT
            Repeater {
                model: HandModel {
                    client: networkClient
                }
                delegate: Card {
                    visible: model.cardType == HandModel.Hand
                    width: model.cardType == HandModel.Hand ? Ui.CARD_WIDTH_BIG : 0
                    height: model.cardType == HandModel.Hand ? Ui.CARD_HEIGHT_BIG : 0
                    radius: Ui.CARD_RADIUS_BIG
                    suit: model.card.suit
                    rank: model.card.rank
                }
            }
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: Ui.MARGIN_DEFAULT
        Repeater {
            model: HandModel {
                client: networkClient
            }
            delegate: Card {
                visible: model.cardType != HandModel.Hand
                width: model.cardType != HandModel.Hand ? Ui.CARD_WIDTH_SMALL : 0
                height: model.cardType != HandModel.Hand ? Ui.CARD_HEIGHT_SMALL : 0
                radius: Ui.CARD_RADIUS_BIG
                suit: model.card.suit
                rank: model.card.rank
            }
        }
    }
}
