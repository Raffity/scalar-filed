#include "sharedmemorychecker.h"

SharedMemoryChecker::SharedMemoryChecker(QObject *parent) : QObject(parent)
{
}

void SharedMemoryChecker::work()
{
    while(true)
    {
        emit send(this->parseString(this->checkSharedMemory()));
        QThread::sleep(5);
    }
}

QString SharedMemoryChecker::checkSharedMemory()
{
    QSharedMemory sharedMemory;
    QString errors = sharedMemory.errorString();
    sharedMemory.setNativeKey("Global\\SharedMemoryExample");
    errors = sharedMemory.errorString();
    QString message;
    if(sharedMemory.attach())
    {
        QBuffer buffer;
        QDataStream in(&buffer);
        QString text;
        QByteArray ar;

        int size = sharedMemory.size();
        sharedMemory.lock();
        QByteArray s = QByteArray((char*) sharedMemory.constData(), sharedMemory.size());
        message = QTextCodec::codecForMib(1015)->toUnicode(s);

        sharedMemory.unlock();
        sharedMemory.detach();
    }
    return message;
}

QList<mp4Vector> SharedMemoryChecker::parseString(QString data)
{
    QList<mp4Vector> listOfPoints;

    if (data.size() < 1)
    {
        return listOfPoints;
    }
    float x = 0;
    float y = 0;
    float z = 0;
    float value = 0;
    QStringList list = data.split(", ");
    QString pointInList;

    foreach(pointInList, list)
    {
        QStringList point = pointInList.split(" ");
        if(point.size() > 3)
        {
            x = point[0].toFloat();
            y = point[1].toFloat();
            z = point[2].toFloat();
            value = point[3].toFloat();
            listOfPoints.push_back(mp4Vector(x, y, z, value));
        }
    }

    return listOfPoints;
}
