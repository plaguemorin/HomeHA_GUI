#ifndef AIMAGE_H
#define AIMAGE_H

#include <QObject>
#include <QImage>

class AImage : public QObject
{
    Q_OBJECT
public:
    explicit AImage(QObject *parent = 0);

    void setName(const char *);
    const char * name();

    bool loadImage(const char *);

    QImage * qImage() { return this->img; }
signals:

public slots:

private:
    QString imageName;
    QImage * img;
};

#endif // AIMAGE_H
