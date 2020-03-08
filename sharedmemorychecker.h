#ifndef SHAREDMEMORYCHECKER_H
#define SHAREDMEMORYCHECKER_H

#include <QObject>
#include <QSharedMemory>
#include <QBuffer>
#include <QMessageBox>
#include <QTextCodec>
#include <QThread>
#include "mpvector.h"

class SharedMemoryChecker : public QObject
{
    Q_OBJECT
public:
    explicit SharedMemoryChecker(QObject *parent = nullptr);

public slots:
    void work();

signals:
    void send(QList<mp4Vector>);

protected:
    QString checkSharedMemory();
    QList<mp4Vector> parseString(QString data);

};

#endif // SHAREDMEMORYCHECKER_H
