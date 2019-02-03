#ifndef PROBE_H
#define PROBE_H

#include <QObject>
#include <QMqttClient>
#include <QTimer>

class Probe : public QObject
{
    Q_OBJECT
public:
    Probe(QObject *parent = nullptr);

    void init();

signals:

public slots:
    void onConnect();
    void onDisconnect();
    void doWork();

private:
    QMqttClient *m_client;
    QTimer timer;
    int sampling;

    int64_t _cpu_user;
    int64_t _cpu_nice;
    int64_t _cpu_system;
    int64_t _cpu_idle;
    int64_t _cpu_ioWait;

    const QString TOPIC_COMMAND = "/probe/command";
    const QString TOPIC_CPU_TEMP = "/probe/cpu/temp";
    const QString TOPIC_CPU_LOAD = "/probe/cpu/load";
    const QString TOPIC_DISK_FREE ="/probe/disk/free";

    QString getCpuTemperature();
    QString GetCPULoad();
    QString getDiskFreeSpace();
};

#endif // PROBE_H
