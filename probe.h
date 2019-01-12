#ifndef PROBE_H
#define PROBE_H

#include <QObject>
#include <QMqttClient>
#include <QTimer>

class Probe : public QObject
{
    Q_OBJECT
public:
    explicit Probe(QObject *parent = 0);

    void init();

signals:

public slots:
    void onConnect();
    void doWork();

    QString getCpuTemperature();
    QString GetCPULoad();
    QString getDiskFreeSpace();

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
};

#endif // PROBE_H
