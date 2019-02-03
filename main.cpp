#include <QCoreApplication>
#include "probe.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Probe *probe = new Probe;
    probe->init();

    return a.exec();
}
