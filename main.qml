import QtQuick 2.15
import QtQuick.Window 2.15
import FlowGraph 1.0
import "qrc:/"


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

         Loader {
             objectName: "Debug Monitor"
             source: "qrc:/debugconsole/Monitor.qml"
         }

         flow:Flow
         {

         id: root
            SequenceBase
            {
                id:s1
                run: root.run

                onEnter:
                {
                    console.log("s1 started")
                    wait(()=>false, 5000)
                    console.log("s1 finished");
                }

            }

            Sequence
            {
                id:s2
               run: root.run

                onEnter:
                {
                    console.log("s2 started")
                    wait(()=>false, 4000)
                    console.log("s2 finished");
                }

            }
         }


    Rectangle
    {
             color:"red"
    }


    }

}
