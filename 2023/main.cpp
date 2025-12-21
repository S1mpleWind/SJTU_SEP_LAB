#include "mainwindow.h"
#include "map.h"
#include <QApplication>

using namespace std;

int main(int argc, char*argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// int main(int argc, char *argv[])
// {
//     QCoreApplication a(argc, argv);

//     // 1. 准备测试文件
//     qDebug() << "1. 当前工作目录:" << QDir::currentPath();
//     qDebug() << "2. 程序所在目录:" << QCoreApplication::applicationDirPath();

//     QString filename = "../../testcase2.txt";
//     // QFile testFile(filename);

//     // if (!testFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
//     //     qDebug() << "创建测试文件失败";
//     //     return -1;
//     // }

//     // QTextStream out(&testFile);
//     // out << "5 5\n";
//     // out << "0 0 0 1 0\n";
//     // out << "0 0 0 0 0\n";
//     // out << "0 1 0 0 0\n";
//     // out << "0 0 0 0 0\n";
//     // out << "0 0 0 0 0\n";
//     // testFile.close();
//     // qDebug() << "测试文件创建完成";

//     // 2. 创建地图对象
//     ::map myMap(filename);
//     qDebug() << "地图对象创建成功，尺寸:"
//              << myMap.getWidth() << "x" << myMap.getHeight();

//     // 3. 计算安全距离
//     try {
//         myMap.updateDistance();
//         qDebug() << "安全距离计算完成";
//     } catch (const std::runtime_error& e) {
//         qDebug() << "安全距离计算失败:" << e.what();
//     }

//     // 4. 测试DFS路径查找
//     qDebug() << "\n开始DFS路径搜索:";
//     int testLimits[] = {2, 3, 5};

//     for (int limit : testLimits) {
//         qDebug() << "\n安全限制条件:" << limit;
//         myMap.dfs(limit);  // 你的dfs方法会输出结果
//     }

//     // 5. 清理
//     QFile::remove(filename);
//     qDebug() << "\n测试完成，文件已清理";

//     return 0;
// }
