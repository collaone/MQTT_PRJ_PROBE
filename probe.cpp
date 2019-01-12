#include "probe.h"
#include <QStorageInfo>
#include <QDebug>
#include <QTimer>

Probe::Probe(QObject *parent)
    : QObject(parent)
{
    m_client = new QMqttClient(this);
    m_client->setClientId("PROBE_CLIENT");
    m_client->setHostname("localhost");
    m_client->setPort(1883);
    sampling = 1000;

    connect(m_client, &QMqttClient::connected, this, &Probe::onConnect);

    connect(m_client, &QMqttClient::messageReceived, this, [](const QByteArray &message) {
        qDebug() << qUtf8Printable(message);
        //sampling = message.toInt();
        //QTimer::singleShot(message.toInt(),this, SLOT (doWork()));
    });

    //QTimer::singleShot(sampling,this, SLOT (doWork()));
    connect(&timer, SIGNAL (timeout()), this, SLOT (doWork()));
    timer.start(sampling);
}
/*
void Probe::setSampling(int sampling)
{
    timer.setInterval(sampling);
}*/

void Probe::init()
{
    qDebug() << "Connecting...";
    m_client->connectToHost();
}

void Probe::doWork()
{
    qDebug() << "Sending data...";

    if (m_client->publish(TOPIC_CPU_TEMP, qUtf8Printable(getCpuTemperature())) == -1)
        qDebug() << "Error while publish";
    if (m_client->publish(TOPIC_CPU_LOAD, qUtf8Printable(GetCPULoad())) == -1)
        qDebug() << "Error while publish";
    if (m_client->publish(TOPIC_DISK_FREE, qUtf8Printable(getDiskFreeSpace())) == -1)
        qDebug() << "Error while publish";
}

QString Probe::getCpuTemperature()
{
    QString fileName = "/sys/class/hwmon/hwmon3/temp1_input";
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }

    QTextStream in(&file);
    QString line = in.readLine();
    double value = line.toDouble();

    return QString::number(value/1000);
}

QString Probe::GetCPULoad()
{
    //grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage "%"}'
    QString fileName = "/proc/stat";
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }

    QTextStream in(&file);
    QString line = in.readLine();
    QStringList tokens = line.split(" ");

    int64_t now_cpu_user = tokens[2].toUInt();
    int64_t now_cpu_nice = tokens[3].toUInt();
    int64_t now_cpu_system = tokens[4].toUInt();
    int64_t now_cpu_idle = tokens[5].toUInt();
    int64_t now_cpu_ioWait = tokens[6].toUInt();

    int64_t diff_cpu_user = now_cpu_user - _cpu_user;
    int64_t diff_cpu_nice = now_cpu_nice - _cpu_nice;
    int64_t diff_cpu_system = now_cpu_system - _cpu_system;
    int64_t diff_cpu_idle = now_cpu_idle - _cpu_idle;
    int64_t diff_cpu_ioWait = now_cpu_ioWait - _cpu_ioWait;

    double total_work = diff_cpu_user + diff_cpu_system;
    double total = diff_cpu_user + diff_cpu_system + diff_cpu_idle + diff_cpu_nice + diff_cpu_ioWait;

    int64_t load = static_cast<int>(total_work/total*100);
    QString cpuLoad = QString::number(load); //.append("%");
    //QString cpuLoad = QString::number(total_work/total*100).append("%");

    // update old values for next reading
    _cpu_user = now_cpu_user;
    _cpu_nice = now_cpu_nice;
    _cpu_system = now_cpu_system;
    _cpu_idle = now_cpu_idle;
    _cpu_ioWait = now_cpu_ioWait;

    file.close();
    return cpuLoad;
}

QString Probe::getDiskFreeSpace()
{
    QStorageInfo storage = QStorageInfo::root();

    return QString::number(storage.bytesAvailable()/1024/1024).append(" MB");
}

void Probe::onConnect() {
    qDebug() << "Connected.";

    if (!m_client->subscribe(TOPIC_COMMAND, 0))
        qDebug() << "Error in subscribe";

    doWork();
}