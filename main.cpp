#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "flowconsole.h"
#include "monitoritem.h"
#include "xfsapp.h"
#include "XfsIdcService.h"
#ifdef XFS_CDM_ENABLED
#include "XfsCdmService.h"
#endif
#ifdef XFS_PIN_ENABLED
#include "XfsPinService.h"
#endif
#ifdef XFS_PTR_ENABLED
#include "XfsPtrService.h"
#endif
#ifdef XFS_SIU_ENABLED
#include "XfsSiuService.h"
#endif

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    fc_init();

    /////////////////////////////////
    /// \brief qmlRegisterType
    ///
    ///
    ///
    ///
    ///
    // All XFS service types are C++-owned singletons — never QML-constructible.
    // Register as uncreatable so QML can reference the type name in
    // Connections { target: ... } and property type annotations, but cannot
    // instantiate them. Instances are injected via setContextProperty() below.
    qmlRegisterUncreatableType<XfsIdcService>(
        "XfsEngine", 1, 0,
        "XfsIdcService",
        "XfsIdcService is created by C++");
#ifdef XFS_CDM_ENABLED
    qmlRegisterUncreatableType<XfsCdmService>(
        "XfsEngine", 1, 0,
        "XfsCdmService",
        "XfsCdmService is created by C++");
#endif
#ifdef XFS_PIN_ENABLED
    qmlRegisterUncreatableType<XfsPinService>(
        "XfsEngine", 1, 0,
        "XfsPinService",
        "XfsPinService is created by C++");
#endif
#ifdef XFS_PTR_ENABLED
    qmlRegisterUncreatableType<XfsPtrService>(
        "XfsEngine", 1, 0,
        "XfsPtrService",
        "XfsPtrService is created by C++");
#endif
#ifdef XFS_SIU_ENABLED
    qmlRegisterUncreatableType<XfsSiuService>(
        "XfsEngine", 1, 0,
        "XfsSiuService",
        "XfsSiuService is created by C++");
#endif

    // Initialize XFS subsystem: loads msxfs.dll and calls WFSStartUp.
    // Must happen before QML loads because services call open() from
    // Component.onCompleted.
    if (!XFSApp::instance().initialize()) {
        qCritical() << "XFS subsystem initialization failed.";
        return 1;
    }

    // Register Status enum as a metatype
    qRegisterMetaType<StateBase::Status>("Status");
    qRegisterMetaType<StateBase::Status>("StateBase::Status");
    // Register WaitResult enum as a metatype
    qRegisterMetaType<SequenceBase::WaitResult>("WaitResult");
    qRegisterMetaType<SequenceBase::WaitResult>("SequenceBase::WaitResult");

    // Register C++ types with QML
    qmlRegisterType<StateBase>("FlowGraph", 1, 0, "StateBase");
    qmlRegisterType<DeviceFlow>("FlowGraph", 1, 0, "Flow");
    qmlRegisterType<Trigger>("FlowGraph", 1, 0, "Trigger");

    // Create all service instances here, parented to app for lifetime management.
    // Logical names must match the SP registry entries on the target machine.
    XfsIdcService* idc = new XfsIdcService("IDC30", &app);
#ifdef XFS_CDM_ENABLED
    XfsCdmService* cdm = new XfsCdmService("CDM30", &app);
#endif
#ifdef XFS_PIN_ENABLED
    XfsPinService* pin = new XfsPinService("PIN30", &app);
#endif
#ifdef XFS_PTR_ENABLED
    XfsPtrService* ptr = new XfsPtrService("PRR30", &app);
#endif
#ifdef XFS_SIU_ENABLED
    XfsSiuService* siu = new XfsSiuService("SIU30", &app);
#endif

    QQmlApplicationEngine engine;

    // Expose service instances to QML as context properties.
    // QML accesses them by these names directly — no import needed.
    engine.rootContext()->setContextProperty("idcService", idc);
#ifdef XFS_CDM_ENABLED
    engine.rootContext()->setContextProperty("cdmService", cdm);
#endif
#ifdef XFS_PIN_ENABLED
    engine.rootContext()->setContextProperty("pinService", pin);
#endif
#ifdef XFS_PTR_ENABLED
    engine.rootContext()->setContextProperty("ptrService", ptr);
#endif
#ifdef XFS_SIU_ENABLED
    engine.rootContext()->setContextProperty("siuService", siu);
#endif


    /////////////////////////////////////////
    qmlRegisterType<MonitorItem>("DebugConsole", 1, 0, "MonitorItem");
    //QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
