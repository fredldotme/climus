#ifndef PROGRESSOUTPUT_H
#define PROGRESSOUTPUT_H

#include <QObject>

class ProgressOutput : public QObject
{
    Q_OBJECT
public:
    explicit ProgressOutput(QObject *parent = nullptr);

public slots:
    void handleProgressChange(const float progressPercent);

};

#endif // PROGRESSOUTPUT_H
