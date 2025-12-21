#include "mainwindow.h"

#include <iostream>
#include <string>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mp(nullptr)  // 初始化指针
{
    this->resize(800, 600);
    this->setWindowTitle("Find Path!");

    // 异步加载地图，避免阻塞
    QString filepath = selectMapFile();

    if (!filepath.isEmpty()) {
        mp = new map(filepath);
        //关键：不要在这里调用showResult，等窗口显示后自动绘制
    } else {
        // 用户取消选择，使用默认地图或提示
        qDebug() << "未选择文件，使用默认显示";
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    // @TODO: write your code here
    if (event->key() == Qt::Key_T){
        qDebug()<<"press t";
        transferMatrix();
    }

    if (event->key() == Qt::Key_P){
        qDebug()<<"press p";
        findPath();
        showResult();
    }

}

void MainWindow::showResult(){
    // @TODO: write your code here
    // @TODO: contain two parts : find path and paint
    drawPath = true;
    update();
}

void MainWindow::transferMatrix(){
    // @TODO: write your code here, subproblem one
    // You can add parameters to this method as needed
    drawSafty = true;
    drawProperty = false;
    mp->updateDistance();
    update();
}

void MainWindow::findPath(){
    // @TODO: write your code here, subproblem two
    // You can add parameters to this method as needed
    mp->dfs();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::black);  // 设置边框颜色

    // 定义单元格大小（像素）
    const int cellSize = 40;  //关键：定义合适的大小

    // 如果没有地图，绘制测试网格
    if (!mp) {
        painter.setBrush(Qt::green);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                QRect rect(j * cellSize, i * cellSize, cellSize, cellSize);
                painter.drawRect(rect);
                painter.drawText(rect, Qt::AlignCenter, "0");
            }
        }
        return;
    }

    // 获取地图尺寸
    int h = mp->getHeight();
    int w = mp->getWidth();

    // 居中显示计算
    int startX = (width() - w * cellSize) / 2;
    int startY = (height() - h * cellSize) / 2;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            // 计算单元格位置
            int x = startX + j * cellSize;
            int y = startY + i * cellSize;
            QRect rect(x, y, cellSize, cellSize);

            if (drawProperty)
            {
                int property = mp->getProperty(QPoint(j, i));

                if (property == 0) {  // 空地
                    painter.setBrush(Qt::green);
                    painter.drawRect(rect);
                    painter.drawText(rect, Qt::AlignCenter, "0");
                }
                else if (property == 1) {  // 火堆
                    painter.setBrush(Qt::red);
                    painter.drawRect(rect);
                    painter.drawText(rect, Qt::AlignCenter, "1");
                }
                else {  // 其他值
                    painter.setBrush(Qt::gray);
                    painter.drawRect(rect);
                    painter.drawText(rect, Qt::AlignCenter, QString::number(property));
                }
            }

            else if (drawSafty)
            {
                int safety = mp->getSafety(QPoint(j, i));

                if (safety == 0) {  // fire
                    painter.setBrush(Qt::red);
                    painter.drawRect(rect);
                    painter.drawText(rect, Qt::AlignCenter, "0");
                }
                else if (safety >= 1) {  //
                    painter.setBrush(Qt::green);
                    if (drawPath && mp->isOnPath(QPoint(j,i))) painter.setBrush(Qt::blue);
                    painter.drawRect(rect);
                    painter.drawText(rect, Qt::AlignCenter, QString::number(safety));
                }
                else {  // 其他值
                    painter.setBrush(Qt::gray);
                    painter.drawRect(rect);
                    painter.drawText(rect, Qt::AlignCenter, QString::number(safety));
                }
            }

        }
    }

    // 可选：绘制网格线
    painter.setPen(Qt::black);
    for (int i = 0; i <= h; i++) {
        int y = startY + i * cellSize;
        painter.drawLine(startX, y, startX + w * cellSize, y);
    }
    for (int j = 0; j <= w; j++) {
        int x = startX + j * cellSize;
        painter.drawLine(x, startY, x, startY + h * cellSize);
    }
}

QString MainWindow::selectMapFile() {
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,                    // 父窗口（没有可设为nullptr）
        "选择地图文件",              // 对话框标题
        QDir::currentPath(),        // 初始目录（当前目录）
        "地图文件 (*.txt *.map);;所有文件 (*.*)"  // 文件过滤器
        );

    if (!fileName.isEmpty()) {
        qDebug() << "已选择文件:" << fileName;
    }

    return fileName;  // 如果取消选择，返回空字符串
}
