#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
  Sub=0;
  norepeat=0;
  resize(640,480);

  but1=new QPushButton(tr("Выбираем 1 директорию или файл"),this);
  but2=new QPushButton(tr("Выбираем 2 директорию или файл"),this);
  but3=new QPushButton(tr("Сравнить"),this);
  but4=new QPushButton(tr("Очистить результаты"),this);
  but5=new QPushButton(tr("Вычесть"),this);
  but6=new QPushButton(tr("Печать"),this);

  but2->setDisabled(true);
  but3->setDisabled(true);
  but4->setDisabled(true);
  but5->setDisabled(true);
  but6->setDisabled(true);

  lineEdit=new QLineEdit(this);
  lineEdit->setDisabled(true);
  TextLog=new QTextEdit(this);

  TextLog->setFont(QFont("Lucida Console",8));
  QVBoxLayout *mainLayout = new QVBoxLayout;

  mainLayout->addWidget(but1);
  mainLayout->addWidget(but2);
  mainLayout->addWidget(lineEdit);
  mainLayout->addWidget(but3);
  mainLayout->addWidget(but5);
  mainLayout->addWidget(TextLog);
  mainLayout->addWidget(but4);
  mainLayout->addWidget(but6);
  setLayout(mainLayout);

  connect(but1, SIGNAL(released()),SLOT(dirselectet_Widget1()));
  connect(but2, SIGNAL(released()),SLOT(dirselectet_Widget2()));
  connect(lineEdit,SIGNAL(editingFinished ()), but3, SLOT(click()));
  connect(but3, SIGNAL(released()),SLOT(compare()));
  connect(but5, SIGNAL(released()),SLOT(sub()));
  connect(but4, SIGNAL(released()),SLOT(My_clear()));
  connect(but6, SIGNAL(released()),SLOT(Print()));

  connect(&tree1,SIGNAL(doubleClicked ( const QModelIndex &)),&model1, SLOT(refresh ( const QModelIndex &)));
  connect(&tree2,SIGNAL(doubleClicked ( const QModelIndex &)),&model2, SLOT(refresh ( const QModelIndex &)));
  connect(&tree1,SIGNAL(doubleClicked ( const QModelIndex &)), SLOT(dirselectet1  ( const QModelIndex &)));
  connect(&tree2,SIGNAL(doubleClicked ( const QModelIndex &)), SLOT(dirselectet2  ( const QModelIndex &)));
}

Widget::~Widget()
{
}

void Widget::sub()
{
  Sub=1;
  norepeat=0;
  compare();
}

void Widget::My_clear()
{
  norepeat=0;
  TextLog->clear();
}

void Widget::dirselectet_Widget1()
{
     tree1.setModel(&model1);
     tree1.setAnimated(false);
     tree1.setIndentation(20);
     tree1.setSortingEnabled(true);
     tree1.setWindowTitle(tr("Выбираем 1 директорию или файл"));
     tree1.resize(640, 480);
     tree1.show();
}

void Widget::dirselectet_Widget2()
{
     tree2.setModel(&model2);
     tree2.setAnimated(false);
     tree2.setIndentation(20);
     tree2.setSortingEnabled(true);
     tree2.setWindowTitle(tr("Выбираем 2 директорию или файл"));
     tree2.resize(640, 480);
     tree2.show();
}

void Widget::dirselectet1 ( const QModelIndex & _mi)
{
  QModelIndex tmp_mi;
  QStringList strlisttmp;
  strlisttmp.prepend(_mi.data().toString());
  tmp_mi=_mi.parent();
  while (tmp_mi.data().toString().count()>0) {
    strlisttmp.prepend(tmp_mi.data().toString());
    tmp_mi=tmp_mi.parent();
  }
  QString dirstr;
  for (int i=0; i<strlisttmp.count(); i++) dirstr.append(strlisttmp[i]+"/");
  comp1=QDir(dirstr);
  if (model1.isDir(_mi)) FolderCompare1=1;
  else FolderCompare1=0;
  but1->setText(comp1.absolutePath());
  tree1.close();
  but2->setDisabled(false);
}

void Widget::dirselectet2 ( const QModelIndex & _mi)
{
  QModelIndex tmp_mi;
  QStringList strlisttmp;
  strlisttmp.prepend(_mi.data().toString());
  tmp_mi=_mi.parent();
  while (tmp_mi.data().toString().count()>0) {
    strlisttmp.prepend(tmp_mi.data().toString());
    tmp_mi=tmp_mi.parent();
  }
  QString dirstr;
  for (int i=0; i<strlisttmp.count(); i++) dirstr.append(strlisttmp[i]+"/");
  comp2=QDir(dirstr);
  if (model1.isDir(_mi)) FolderCompare2=1;
  else FolderCompare2=0;
  but2->setText(comp2.absolutePath());
  tree2.close();
  but3->setDisabled(false);
  but4->setDisabled(false);
  but5->setDisabled(false);
  but6->setDisabled(false);
  lineEdit->setDisabled(false);
}

void Widget::compare()
{
  if (norepeat==1) return;
  //qDebug()<<"compare()";
  QStringList strlisttmp1;
  QStringList strlisttmp2;
  QStringList strlisttmp3;
  QFile file1;
  QFile file2;

  CompareFilesOrFoders=0;

  int i=0;
  int j=0;
  int k=0;

  strlisttmp1=comp1.entryList();
  strlisttmp2=comp2.entryList();

  listcolumn.clear();

  if (!lineEdit->text().isEmpty()) listcolumn=lineEdit->text().split(" ");

  if (FolderCompare2==FolderCompare1 && FolderCompare1==1 && FolderCompare2==1) {
    for (i=0; i<strlisttmp1.count(); i++) {
      for (j=0; j<strlisttmp2.count(); j++) {
        if (strlisttmp1[i]==strlisttmp2[j]) strlisttmp3.append(strlisttmp1[i]);//qDebug()<<strlisttmp1[i];
      }
    }

    for (i=0; i<strlisttmp3.count(); i++) {
//      QFile file1();
//      QFile file2();
      file1.setFileName(comp1.absolutePath()+"/"+strlisttmp3[i]);
      file2.setFileName(comp2.absolutePath()+"/"+strlisttmp3[i]);
      file1.open(QIODevice::ReadOnly | QIODevice::Text);
      file2.open(QIODevice::ReadOnly | QIODevice::Text);
      k=0;
      while (!file1.atEnd() || !file2.atEnd()) {
        k++;
        QByteArray line1 = file1.readLine();
        QByteArray line2 = file2.readLine();
        if (line1.trimmed()!=line2.trimmed()) {
          QStringList line1List=QString::fromLocal8Bit(line1.trimmed()).split(" ");
          QStringList line2List=QString::fromLocal8Bit(line2.trimmed()).split(" ");
          if (line1List.count()==line2List.count()) {
            for ( j=0; j<line1List.count(); j++) {
              if (listcolumn.count()>0) {
                for (int l=0; l<listcolumn.count(); l++)
                  if (j==(listcolumn[l].toInt())) {
                  if (line1List[j]!=line2List[j]) {
                    if (Sub==0) {
                      CompareFilesOrFoders=1;
                      TextLog->append(comp1.absolutePath()+"/"+strlisttmp3[i]+tr(" строка:")+line1List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line1List[j]);
                      TextLog->append(comp2.absolutePath()+"/"+strlisttmp3[i]+tr(" строка:")+line2List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line2List[j]+"\r\n");
                    }
                    else {
                      CompareFilesOrFoders=1;
                      TextLog->append(comp1.absolutePath()+"/"+strlisttmp3[i]+tr(" строка:")+line1List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line1List[j]);
                      TextLog->append(comp2.absolutePath()+"/"+strlisttmp3[i]+tr(" строка:")+line2List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line2List[j]+tr(" разность:")+QString::number(line1List[j].toInt()-line2List[j].toInt())+"\r\n");
                    }
                  }
                }
              }
              else {
                if (line1List[j]!=line2List[j]) {
                  if (Sub==0) {
                    CompareFilesOrFoders=1;
                    TextLog->append(comp1.absolutePath()+"/"+strlisttmp3[i]+tr(" строка:")+line1List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line1List[j]);
                    TextLog->append(comp2.absolutePath()+"/"+strlisttmp3[i]+tr(" строка:")+line2List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line2List[j]+"\r\n");
                  }
                  else {
                    CompareFilesOrFoders=1;
                    TextLog->append(comp1.absolutePath()+"/"+strlisttmp3[i]+tr(" строка:")+line1List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line1List[j]);
                    TextLog->append(comp2.absolutePath()+"/"+strlisttmp3[i]+tr(" строка:")+line2List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line2List[j]+tr(" разность:")+QString::number(line1List[j].toInt()-line2List[j].toInt())+"\r\n");
                  }
                }
              }
            }
          }
        }
      }
    }
    if (CompareFilesOrFoders==0) {
      TextLog->append(tr("Отличий в файлах или директориях не найдено"));
      Sub=0;
      norepeat=1;
      return;
    }
    Sub=0;
    norepeat=1;
    return;
  }
  else if(FolderCompare2==FolderCompare1 && FolderCompare1==0 && FolderCompare2==0) {
    file1.setFileName(comp1.absolutePath());
    file2.setFileName(comp2.absolutePath());
    file1.open(QIODevice::ReadOnly | QIODevice::Text);
    file2.open(QIODevice::ReadOnly | QIODevice::Text);
    k=0;
    while (!file1.atEnd() || !file2.atEnd()) {
      k++;
      QByteArray line1 = file1.readLine();
      QByteArray line2 = file2.readLine();
      if (line1.trimmed()!=line2.trimmed()) {
        QStringList line1List=QString::fromLocal8Bit(line1.trimmed()).split(" ");
        QStringList line2List=QString::fromLocal8Bit(line2.trimmed()).split(" ");
        if (line1List.count()==line2List.count()) {
          for ( j=0; j<line1List.count(); j++) {
            if (listcolumn.count()>0) {
              for (int l=0; l<listcolumn.count(); l++)
                if (j==(listcolumn[l].toInt())) {
                  if (line1List[j]!=line2List[j]) {
                    if (Sub==0) {
                      CompareFilesOrFoders=1;
                      TextLog->append(comp1.absolutePath()+tr(" строка:")+line1List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line1List[j]);
                      TextLog->append(comp2.absolutePath()+tr(" строка:")+line2List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line2List[j]+"\r\n");
                    }
                    else {
                      CompareFilesOrFoders=1;
                      TextLog->append(comp1.absolutePath()+tr(" строка:")+line1List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line1List[j]);
                      TextLog->append(comp2.absolutePath()+tr(" строка:")+line2List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line2List[j]+tr(" разность:")+QString::number(line1List[j].toInt()-line2List[j].toInt())+"\r\n");
                    }
                  }
                }
             }
            else {
                  if (line1List[j]!=line2List[j]) {
                    if (Sub==0) {
                      CompareFilesOrFoders=1;
                      TextLog->append(comp1.absolutePath()+tr(" строка:")+line1List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line1List[j]);
                      TextLog->append(comp2.absolutePath()+tr(" строка:")+line2List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line2List[j]+"\r\n");
                    }
                    else {
                      CompareFilesOrFoders=1;
                      TextLog->append(comp1.absolutePath()+tr(" строка:")+line1List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line1List[j]);
                      TextLog->append(comp2.absolutePath()+tr(" строка:")+line2List[0]+tr(" столбец:")+QString::number(j)+tr(" значение:")+line2List[j]+tr(" разность:")+QString::number(line1List[j].toInt()-line2List[j].toInt())+"\r\n");
                    }
                  }
             }
          }
        }
       }
     }
     if (CompareFilesOrFoders==0) {
       TextLog->append(tr("Отличий в файлах или директориях не найдено"));
       Sub=0;
       norepeat=1;
       return;
     }
     Sub=0;
     norepeat=1;
     return;
  }
  else if (FolderCompare2!=FolderCompare1 && FolderCompare2==0) {
    TextLog->append(tr("Выбирете директорию, а не файл"));
    Sub=0;
    return;
  }
  else if (FolderCompare2!=FolderCompare1 && FolderCompare2==1) {
    TextLog->append(tr("Выбирете файл, а не директорию"));
    Sub=0;
    return;
  }
}

//void Widget::Print()
//{
//  QPrinter printer;
//  QPrintDialog printDialog(&printer);
//  if (printDialog.exec() == QDialog::Accepted) {
//    QPrintPreviewDialog PrintPreviewDialog(&printer);
//    if (PrintPreviewDialog.exec() == QDialog::Accepted) {
//      TextLog->print(&printer);
//    }
//  return;
//  }
//}

void Widget::Print()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this, Qt::Window);
    connect(&preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printPreview(QPrinter *)));
    preview.exec();
#endif
}

void Widget::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    TextLog->print(printer);
#endif
}
