#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QWidget>
#include <QtWidgets>
#include "map.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void transferMatrix();
    void findPath();
    void showResult();

    QString selectMapFile();

    map* mp;
    int limit;

    bool drawProperty = true;
    bool drawSafty = false;
    bool drawPath = false;
};

#endif // MAINWINDOW_H
