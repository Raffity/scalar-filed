#include "sharedmemorychecker.h"

SharedMemoryChecker::SharedMemoryChecker(QObject *parent, QString sharedMemoryName) : QObject(parent)
{
    this->sharedMemoryName = sharedMemoryName.trimmed();
}

void SharedMemoryChecker::work()
{
    while(true)
    {
        QList<mp4Vector> vector = this->parseData(this->checkSharedMemory());
        emit sendSettings(this->nX, this->nY, this->nZ, this->maxX, this->maxY, this->maxZ, this->minX, this->minY, this->minZ);
        emit send(vector);
        QThread::sleep(5);
    }
}

QString SharedMemoryChecker::checkSharedMemory()
{
    QSharedMemory sharedMemory;
    QString errors = sharedMemory.errorString();
    sharedMemory.setNativeKey(this->sharedMemoryName);
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

QList<mp4Vector> SharedMemoryChecker::parseData(QString data)
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
    QList<float> *listX = new QList<float>;
    QList<float> *listY = new QList<float>;
    QList<float> *listZ = new QList<float>;

    foreach(pointInList, list)
    {
        QStringList point = pointInList.split(" ");
        if(point.size() > 3)
        {
            x = point[0].toFloat();
            y = point[1].toFloat();
            z = point[2].toFloat();
            this->maxX = this->maxX < x ? x : this->maxX;
            this->maxY = this->maxY < y ? y : this->maxY;
            this->maxZ = this->maxZ < z ? z : this->maxZ;
            this->minX = this->minX > x ? x : this->minX;
            this->minY = this->minY > y ? y : this->minY;
            this->minZ = this->minZ > z ? z : this->minZ;
            this->checkInList(x, *listX);
            this->checkInList(y, *listY);
            this->checkInList(z, *listZ);
            value = point[3].toFloat();
            listOfPoints.push_back(mp4Vector(x, y, z, value));
        }
    }

    this->nX = listX->count() - 1;
    this->nY = listY->count() - 1;
    this->nZ = listZ->count() - 1;
    return listOfPoints;
}

void SharedMemoryChecker::checkInList(float value, QList<float> &list)
{
    if(list.indexOf(value) == -1)
    {
        list.append(value);
    }
}
