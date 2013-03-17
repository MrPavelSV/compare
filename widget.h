#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    QPushButton *but1;
    QPushButton *but2;
    QPushButton *but3;
    QPushButton *but4;
    QPushButton *but5;
    QPushButton *but6;
    QLineEdit *lineEdit;
    QTextEdit *TextLog;
    QDir comp1;
    QDir comp2;
    int Sub;
    int norepeat;
    int FolderCompare1;
    int FolderCompare2;
    int CompareFilesOrFoders;
    QDirModel model1;
    QDirModel model2;
    QTreeView tree1;
    QTreeView tree2;
    QStringList listcolumn;
public slots:
    void dirselectet_Widget1();
    void dirselectet1 ( const QModelIndex &);
    void dirselectet_Widget2();
    void dirselectet2 ( const QModelIndex &);
    void compare();
    void sub();
    void My_clear();
    void Print();
    void printPreview(QPrinter *printer);
};

#endif // WIDGET_H
