#ifndef AIMAGE_H
#define AIMAGE_H

#include <QObject>
#include <QImage>

class AImage : public QObject
{
    Q_OBJECT
public:
    explicit AImage(QObject *parent = 0);
    virtual ~AImage();

    void setName(const char *);
    const char * name();

    bool loadImage(const char *);

    void tint(int, int, int, int);

    AImage * copy() const;
    QImage * qImage() { return this->img; }

    int width() { return this->img->width(); }
    int height() { return this->img->height(); }
signals:

public slots:

private:
    QString imageName;
    QImage * img;
};

#endif // AIMAGE_H
