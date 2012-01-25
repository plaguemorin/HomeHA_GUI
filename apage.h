#ifndef APAGE_H
#define APAGE_H

#include <QWidget>

class AImage;

class APage : public QWidget
{
    Q_OBJECT
public:
    explicit APage(QWidget *parent = 0);

    const char * name();
    void setName(const char *);

    AImage * background();
    void setBackground(AImage * bgimg);

signals:

public slots:

private:
    char * pageName;
    AImage * backgroundImage;
};

#endif // APAGE_H
