#include <QDebug>
#include <regex>
#include <random>
#include <algorithm>
#include "gameboarddata.h"

GameBoardData::GameBoardData(int sizeX, int sizeY, int colorCount, QObject* parent) : QAbstractListModel(parent),
    m_sizeX(sizeX), m_sizeY(sizeY), m_dimension(sizeX * sizeY), m_colorCount(colorCount)
{
    m_colorKey[0] = "red";
    m_colorKey[1] = "green";
    m_colorKey[2] = "blue";
    m_colorKey[3] = "yellow";
    isMatch = 0;
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, colorCount - 1);

    for (int i = 0; i < m_sizeY; i++)
    {
        matchHorisontal.append(QList<int>());
        matchVertical.append(QList<int>());
        match.append(QList<int>());
        for (int j = 0; j < m_sizeX; j++)
        {
            matchHorisontal[i].append(int());
            matchVertical[i].append(int());
            match[i].append(int());
        }
    }

    for(int i = 0; i < m_dimension; i++) {
        int randColorKey = distr(eng);
        m_data.push_back(randColorKey);
    }
    isMatch = checkMatch(m_data);
    //auto find = std::find_if(match.begin(), match.end(), [](int n) { return n >= 3; });
    shuffle();
}

void GameBoardData::shuffle()
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, m_colorCount - 1);
    isMatch = true;
    beginResetModel();
    while (isMatch) {
        for(int i = 0; i < m_dimension; i++) {
            int randColorKey = distr(eng);
            m_data[i] = randColorKey;
        }
        isMatch = checkMatch(m_data);
    }
    endResetModel();
}

bool GameBoardData::ifNear(int first, int second) const
{
    return ((std::abs(first - second) == 1) || (std::abs(first - second) == m_sizeY));
}


void  GameBoardData::checkMatchHorisontal()
{
    //qDebug() << m_data;
    //qDebug() << matchHorisontal;
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            int currentElement = matchHorisontal[i][j];
            int countMatch = 0;
            int first = j;
            while ((j < m_sizeX - 1) && (currentElement == matchHorisontal[i][j+1])) {
                countMatch++;
                j++;
            }
            for(int k = first; k < first + countMatch + 1; k++) {
                matchHorisontal[i][k] = countMatch + 1;
            }
        }
    }
    //qDebug() <<"checkHori" << matchHorisontal;
}

void GameBoardData::checkMatchVertical()
{
    //qDebug() << matchVertical;
    for (int i = 0; i < m_sizeX; i++) {
        for (int j = 0; j < m_sizeY; j++) {
            int currentElement = matchVertical[j][i];
            int countMatch = 0;
            int first = j;
            while ((j < m_sizeY - 1) && (currentElement == matchVertical[j + 1][i])) {
                countMatch++;
                j++;
            }
            for(int k = first; k < first + countMatch + 1; k++) {
                matchVertical[k][i] = countMatch + 1;
            }
        }
    }
    //qDebug() << "cheVerti" << matchVertical;
}

bool GameBoardData::checkMatch(QList<int>& m_data)
{

//    for (int x = 0; x < m_sizeX; ++x) {
//        for (int k = 0; k < m_sizeY; ++k) {
//            matchHorisontal[k][x] = m_data[m_sizeY*x + k];
//            matchVertical[k][x] = matchHorisontal[k][x];
//        }
//    }
//    for (int x = 0; x < m_sizeX; ++x) {
//        for (int k = 0; k < m_sizeY; ++k) {
//            matchHorisontal[k][x] = m_data[m_sizeY*x + k];
//            matchVertical[k][x] = matchHorisontal[k][x];
//        }
//    }
    for (int x = 0; x < m_sizeY; x++) {
        for (int k = 0; k < m_sizeX; k++) {
            matchHorisontal[x][k] = m_data[m_sizeY*k + x];
            matchVertical[x][k] = matchHorisontal[x][k];
        }
    }
    //qDebug() << "maatch" << matchHorisontal;
    checkMatchVertical();
    checkMatchHorisontal();
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            match[i][j] = std::max(matchVertical[i][j], matchHorisontal[i][j]);
        }
    }
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            if (match[i][j] >= 3) {
                return true;
            }
        }
    }
    return false;
    //return isMatch;
    //clear(); -------------------
}

void GameBoardData::setMatchToNull()
{
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            matchVertical[i][j] = 0;
            match[i][j] = matchHorisontal[i][j] = 0;
        }
    }
}

bool GameBoardData::getIsMatch()
{
    return isMatch;
}

void GameBoardData::clear()
{
    indexForDelete.clear();
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            if (match[i][j] >= 3) {
                indexForDelete.push_back(Inx{i ,j});
            }
        }
    }
    QList<int> q;
    for (auto el : indexForDelete) {
        q.push_back(el.x * m_sizeY + el.y);
    }
    std::sort(q.begin(), q.end());
    QMap<int,int> m;
    for( int i = 0; i < m_sizeY; i++) {
        m.insert(i,0);
    }
    while (!q.isEmpty()) {
        int forRemove = q.back();
        q.pop_back();
        beginRemoveRows(QModelIndex(), forRemove, forRemove);
        m_data.erase(m_data.begin() + forRemove);
        endRemoveRows();
        int temp = m[forRemove / m_sizeY];
        m[forRemove / m_sizeY] = temp + 1;
    }
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, m_colorCount - 1);
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m[i]; j++) {
            beginInsertRows(QModelIndex(), i * m_sizeY, i * m_sizeY);
            m_data.insert(i * m_sizeY, distr(eng)); //дописати рандом
            endInsertRows();
        }
    }
    setMatchToNull();
}

void GameBoardData::swapElements(int indexFirst, int indexSecond)
{
    int offsetForHorizontal = indexFirst < indexSecond ? 1 : 0;
    int offsetForVertical = indexFirst < indexSecond ? 0 : 1;

    QList<int> copyData {m_data.begin(), m_data.end()};
    int temp = copyData[indexFirst];
    copyData[indexFirst] = copyData[indexSecond];
    copyData[indexSecond] = temp;

    if (checkMatch(copyData)) {
            if (ifNear(indexFirst, indexSecond)) {
                if (std::abs(indexSecond - indexFirst) == 1) {
                    beginMoveRows(QModelIndex(),indexFirst,indexFirst,QModelIndex(),indexSecond + offsetForHorizontal);
                } else if (std::abs(indexSecond - indexFirst) == m_sizeY) {
                    beginMoveRows(QModelIndex(),indexFirst,indexFirst,QModelIndex(),indexSecond);
                    endMoveRows();
                    beginMoveRows(QModelIndex(), indexSecond + offsetForVertical,
                                  indexSecond + offsetForVertical, QModelIndex(), indexFirst + offsetForVertical);
                }
                int temp = m_data[indexFirst];
                m_data[indexFirst] = m_data[indexSecond];
                m_data[indexSecond] = temp;
                endMoveRows();
            }
            endResetModel();
            isMatch = checkMatch(m_data);
            clear();
    }
    else {
        emit noMatch(indexFirst, indexSecond);
    }

}

void GameBoardData::clearMatchAgain()
{
    if (isMatch) {
        isMatch = checkMatch(m_data);
        clear();
        isMatch = checkMatch(m_data);
        qDebug() << "call";

    }
}

QVariant GameBoardData::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_data.count()) {
        return QVariant();
    }
    switch(role) {
    case Qt::DisplayRole:
        return m_colorKey[m_data.value(row)];
    }
    return QVariant();
}

int GameBoardData::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}


