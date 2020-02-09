#pragma once
#include <QAbstractItemModel>
#include <QList>

//struct KeyColor {
//    int key;
//    QString color;
//};
struct Inx {
    int y;
    int x;
};

class GameBoardData : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString isMatch READ getIsMatch NOTIFY isMatchChanged)
    Q_PROPERTY(int score READ getScore NOTIFY isScoreChanged)

public:
    Q_INVOKABLE GameBoardData(int sizeX = 5, int sizeY = 8, int colorCount = 4, QObject* parent = nullptr);
    Q_INVOKABLE void swapElements(int indexFirst, int indexSecond);
    Q_INVOKABLE void clearMatchAgain();
    Q_INVOKABLE void shuffle();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE int getScore();
private:
    bool ifNear(int first, int second) const;
    bool checkMatch(QList<int>& data);
    void checkMatchHorisontal();
    void checkMatchVertical();
    void clear();
    void setMatchToNull();


public:
    bool getIsMatch();
    void read(const char* inp = "./input.json");

signals:
    void isMatchChanged();
    void noMatch(int first, int second);
    void isScoreChanged();

private:
    QList<int> m_data;
    QList<QList<int>> matchHorisontal;
    QList<QList<int>> matchVertical;
    QList<QList<int>> match;
    QMap<int, QString> m_colorKey;
    QList<Inx> indexForDelete;
    int m_sizeX;
    int m_sizeY;
    int m_dimension;
    int m_colorCount;
    bool isMatch;
    int tempScore;
public:
    int score;
};
