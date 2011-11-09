#ifndef APAGE_H
#define APAGE_H

#include <QWidget>

class APage : public QWidget
{
    Q_OBJECT
public:
    explicit APage(QWidget *parent = 0);

    const char * name();
    void setName(const char *);
signals:

public slots:

private:
    char * pageName;
};

#endif // APAGE_H
