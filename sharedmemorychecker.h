#ifndef SHAREDMEMORYCHECKER_H
#define SHAREDMEMORYCHECKER_H

#include <QObject>
#include <QSharedMemory>
#include <QBuffer>
#include <QMessageBox>
#include <QTextCodec>
#include <QThread>
#include "mpvector.h"
#include "glwindow.h"

class SharedMemoryChecker : public QObject
{
    Q_OBJECT
public:
    explicit SharedMemoryChecker(QObject *parent = nullptr, QString sharedMemoryName = "");

public slots:
    void work();

signals:
    void send(QList<mp4Vector>);
    void sendSettings(int ,int, int, float, float, float, float, float, float);

protected:
    QString checkSharedMemory();
    QList<mp4Vector> parseData(QString data);
    void checkInList(float value, QList<float> &list);
    QString sharedMemoryName;

    int nX, nY, nZ;
    float maxX, maxY, maxZ, minX, minY, minZ;
};

#endif // SHAREDMEMORYCHECKER_H
