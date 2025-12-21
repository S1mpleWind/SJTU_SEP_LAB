#pragma once
#include <QWidget>
#include <QFile>
class map{

public :
    // construct and destruct
    map(const QString& filePath);
    ~map();

    int getSafety(QPoint);
    bool isOnPath(QPoint);

    int getProperty(QPoint pt){
        int x = pt.x();
        int y = pt.y();
        return mapProperty[y][x];
    };

    // update the safety degree
    void updateDistance();

    // dfs searching the map for a path
    void dfs();

    void markVisited(QPoint pt){
        int x = pt.x();
        int y = pt.y();
        visited[y][x] = true;
    }

    int getWidth() {return width;}
    int getHeight() {return height;}

private:
    //property of map
    int width = 0;
    int height = 0;
    int limit = 0;

    //the visited tiles
    QVector<QVector<int>> visited;

    //whether is a flame
    QVector<QVector<int>> mapProperty;

    QVector<QVector<int>> safetyGrid;

    QVector<QPoint> fires;

    //distance
    QVector<QVector<int>> distances;
};


