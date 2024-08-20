import QtQuick

Window {
    id: root
    visible: true
    width: 1000
    height: 600
    maximumHeight: height
    minimumHeight: height
    maximumWidth: width
    minimumWidth: width

    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "image://image/yellow"
    }
}
