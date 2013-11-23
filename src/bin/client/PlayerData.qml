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
import "UiConstants.js" as Ui

Rectangle {
    width: Ui.PLAYER_DATA_WIDTH
    height: Ui.PLAYER_DATA_HEIGHT
    property alias name: name.text
    property alias tokenCount: tokenCountText.text
    property alias betCount: betCountText.text

    Text {
        id: name
        anchors.left: parent.left; anchors.leftMargin: Ui.MARGIN_DEFAULT
        anchors.right: parent.right; anchors.rightMargin: Ui.MARGIN_DEFAULT
        anchors.top: parent.top; anchors.topMargin: Ui.MARGIN_DEFAULT
        horizontalAlignment: Text.AlignHCenter
    }

    Item {
        id: tokenCount
        anchors.left: parent.left; anchors.leftMargin: Ui.MARGIN_DEFAULT
        anchors.right: parent.right; anchors.rightMargin: Ui.MARGIN_DEFAULT
        anchors.top: name.bottom; anchors.topMargin: Ui.MARGIN_DEFAULT
        height: Ui.ICON_SIZE

        Image {
            id: tokenCountIcon
            width: Ui.ICON_SIZE
            height: Ui.ICON_SIZE
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: "assets/bank.png"
        }

        Text {
            id: tokenCountText
            anchors.left: tokenCountIcon.right; anchors.leftMargin: Ui.MARGIN_DEFAULT
            anchors.right: parent.right; anchors.rightMargin: Ui.MARGIN_DEFAULT
        }
    }

    Item {
        anchors.left: parent.left; anchors.leftMargin: Ui.MARGIN_DEFAULT
        anchors.right: parent.right; anchors.rightMargin: Ui.MARGIN_DEFAULT
        anchors.top: tokenCount.bottom; anchors.topMargin: Ui.MARGIN_DEFAULT
        height: Ui.ICON_SIZE

        Image {
            id: betCountIcon
            width: Ui.ICON_SIZE
            height: Ui.ICON_SIZE
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: "assets/bet.png"
        }

        Text {
            id: betCountText
            anchors.left: betCountIcon.right; anchors.leftMargin: Ui.MARGIN_DEFAULT
            anchors.right: parent.right; anchors.rightMargin: Ui.MARGIN_DEFAULT
        }
    }
}
