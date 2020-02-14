#pragma once
#include <QAbstractItemModel>
#include <QList>
#include <regex>
#include <random>

struct Position {
    int y;
    int x;
};

class GameBoardData : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString isMatch READ getIsMatch NOTIFY isMatchChanged)
    Q_PROPERTY(int score READ getScore NOTIFY isScoreChanged)
    Q_PROPERTY(int m_sizeX READ getSizeX CONSTANT)
    Q_PROPERTY(int m_sizeY READ getSizeY CONSTANT)

public:
    Q_INVOKABLE GameBoardData(int sizeX = 5, int sizeY = 8, int colorCount = 4, QObject* parent = nullptr);
    Q_INVOKABLE void swapElements(int indexFirst, int indexSecond);
    Q_INVOKABLE void clearMatchAgain();
    Q_INVOKABLE void shuffle();
    Q_INVOKABLE bool ifGameOver();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool getIsMatch();
    void read(QString inp = ":/input.json");
    int getScore();
    int getSizeX();
    int getSizeY();

private:
    bool ifNear(int first, int second) const;
    bool checkMatch(QList<int>& data);
    void checkMatchHorizontal();
    void checkMatchVertical();
    void clear();
    void setMatchToNull();
    void moveElements(int indexFirst, int indexSecond);
    bool ifGameOverVertical();
    bool ifGameOverHorizontal();

signals:
    void isMatchChanged();
    void noMatch(int first, int second);
    void isScoreChanged();

private:
    QList<int> m_data;
    QList<QList<int>> matchHorizontal;
    QList<QList<int>> matchVertical;
    QList<QList<int>> match;
    QMap<int, QString> m_colorKey;
    QList<Position> indexForDelete;
    int m_sizeX;
    int m_sizeY;
    int m_dimension;
    int m_colorCount;
    bool m_isMatch;
    std::random_device m_rd;
    std::mt19937 m_eng;
    int m_score;
};
