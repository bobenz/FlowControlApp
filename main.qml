import QtQuick 2.15
import QtQuick.Window 2.15
import FlowGraph 1.0
import "qrc:/"
import "qrc:/debugconsole"

Window
{
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    Component.onCompleted:
    {
         console.log("WINDOW");
    }

    FlowConsole
    {

         anchors.fill:parent
         openbutton:true
         qrcFlows:["C:\dev\qxfsengine\flows\flows.qrc"]
         // Loader {
         //     objectName: "Debug Monitor"
         //     source: "qrc:/debugconsole/Monitor.qml"
         // }

       Monitor
       {
           property bool cdm_cmd:true
           property bool cdm_result:true
           property bool ptr_cmd:true
           property bool ptr_result:true
           property bool idc_cmd:true
           property bool idc_result:true
           property bool pin_cmd:true
           property bool pin_result:true
       }



    }

}
