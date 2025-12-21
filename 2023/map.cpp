#include "map.h"
#include <QStack>
#include <QDebug>

map::map(const QString& filePath)
{
    QFile file(filePath);

    // open the file and init the map
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << filePath;
        return;
    }

    QTextStream in(&file);
    int currentRow = 0;

    // 第一行：读取尺寸
    if (!in.atEnd()) {
        QString line = in.readLine();
        QStringList size = line.split(' ');
        if (size.size() == 2) {
            width = size[0].toInt();
            height = size[1].toInt();

            // 关键：先检查尺寸是否有效
            if (width <= 0 || height <= 0) {
                qDebug() << "无效的地图尺寸:" << width << "x" << height;
                file.close();
                return;
            }

            // 关键：预分配容器大小
            mapProperty.resize(height);
            safetyGrid.resize(height);
            visited.resize(height);

            for (int i = 0; i < height; i++) {
                mapProperty[i].resize(width);
                safetyGrid[i].resize(width);
                visited[i].resize(width);
            }
        } else {
            qDebug() << "地图尺寸格式错误";
            file.close();
            return;
        }
    } else {
        qDebug() << "地图文件为空";
        file.close();
        return;
    }

    // 后续行：逐行读取地图数据
    while (!in.atEnd() && currentRow < height) {
        QString line = in.readLine();
        QStringList nums = line.split(' ');

        // 检查当前行是否有足够的数据
        if (nums.size() < width) {
            qDebug() << "第" << currentRow << "行数据不足，期望" << width << "列，实际" << nums.size() << "列";
        }

        for (int currentCol = 0; currentCol < qMin(nums.size(), width); currentCol++) {
            int value = nums[currentCol].toInt();
            mapProperty[currentRow][currentCol] = value;

            // 收集火堆位置 - 注意：QPoint(x, y) 其中x是列，y是行
            if (value == 1) {
                fires.append(QPoint(currentCol, currentRow));
                qDebug() << "发现火堆在: (" << currentCol << "," << currentRow << ")";
            }

            visited[currentRow][currentCol] = false;
        }
        currentRow++;
    }

    QString line = in.readLine();
    QStringList nums = line.split(' ');
    limit = nums[0].toInt();

    file.close();

    // 检查是否读取了所有行
    if (currentRow < height) {
        qDebug() << "警告: 只读取了" << currentRow << "行，期望" << height << "行";
    }

    qDebug() << "地图加载完成，尺寸:" << width << "x" << height;
    qDebug() << "发现火堆数量:" << fires.size();
}


map::~map()
{

}

int map::getSafety(QPoint pt){
    int x = pt.x();
    int y = pt.y();

    if (x>width || y > height){
        throw std::runtime_error("访问越界");
        }
    //TODO: check robusty
    return safetyGrid[y][x];
}




bool map::isOnPath(QPoint pt){
    int x = pt.x();
    int y = pt.y();

    if (x>width || y > height){
        throw std::runtime_error("访问越界");
    }
    //TODO: check robusty
    return visited[y][x];
}



void map::updateDistance()
{
    //method 1 : multisource BFS (implement if there is extra time)
    //method 2 : traverse

    //0.corner case
    if (fires.empty()){
        throw std::runtime_error("distance");
    }

    //1.update safe tiles
    // traverse
    for(int i =0 ; i<height ; i++)
    {
        for (int j = 0 ; j<width ; j++)
        {
            if (mapProperty[i][j] == 1) continue;
            else{
                int smallest = 99;

                for (QPoint fire : fires){
                    // update distance
                    int x = fire.x();
                    int y = fire.y();
                    int dis = std::abs(y -i) + std::abs(x-j);

                    smallest = std::min(smallest , dis);
                }
                safetyGrid[i][j] = smallest;
            }
        }
    }

    //2.update dangerous tiles

    for(QPoint fire : fires){
        int x = fire.x();
        int y = fire.y();

        safetyGrid[y][x]= 0;
    }

}

struct PathNode {
    QPoint position;
    QVector<QPoint> path; // 记录到达该点的完整路径
};


void map::dfs() {
    // 清除之前的访问标记
    //int limit = 1;
    for (auto& row : visited) {
        row.fill(false);
    }

    QStack<PathNode> stk;

    // 起点
    PathNode startNode;
    startNode.position = QPoint(0, 0);
    startNode.path.append(QPoint(0, 0));
    stk.push(startNode);

    //markVisited(QPoint(0, 0)); // 标记起点已访问

    bool foundPath = false;
    QVector<QPoint> foundPathPoints; // 存储找到的路径


    //四方向（上下左右）：
    QVector<QPoint> directions = {
        QPoint(1, 0), QPoint(-1, 0),
        QPoint(0, 1), QPoint(0, -1)
    };

    while (!stk.isEmpty() && !foundPath) {
        PathNode currNode = stk.pop();
        QPoint curr = currNode.position;

        // 检查安全距离
        if (getSafety(curr) < limit) {
            continue; // 跳过危险点
        }

        // 到达终点
        if (curr == QPoint(width - 1, height - 1)) {
            foundPath = true;
            foundPathPoints = currNode.path;
            break;
        }

        // 探索邻居
        for (const QPoint& dir : directions) {
            int nx = curr.x() + dir.x();
            int ny = curr.y() + dir.y();

            // 检查边界
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                QPoint neighbor(nx, ny);

                // 关键：如果该点已在当前路径中，跳过
                if (currNode.path.contains(neighbor)) {
                    continue;
                }

                // 检查是否安全
                if (getSafety(neighbor) >= limit) {
                    // 创建新路径节点
                    PathNode nextNode;
                    nextNode.position = neighbor;
                    nextNode.path = currNode.path; // 复制当前路径
                    nextNode.path.append(neighbor); // 添加新点

                    stk.push(nextNode);
                }
            }
        }
    }

    // 输出结果
    if (foundPath) {
        qDebug() << "找到安全路径！长度：" << foundPathPoints.size();
        qDebug() << "路径点：";
        for (const QPoint& p : foundPathPoints) {
            qDebug() << "  (" << p.x() << "," << p.y() << ")";
            markVisited(p);
        }

        // 在这里可以发射信号
        // emit pathFound(foundPathPoints);
    } else {
        qDebug() << "未找到安全路径";
        // 发射无路径信号
        // emit noPathFound();
    }
}







