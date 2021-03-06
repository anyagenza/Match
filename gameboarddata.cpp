#include <QDebug>
#include <algorithm>
#include <fstream>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

#include "gameboarddata.h"

GameBoardData::GameBoardData(int sizeX, int sizeY, int colorCount, QObject* parent) : QAbstractListModel(parent),
    m_sizeX(sizeX), m_sizeY(sizeY), m_dimension(sizeX * sizeY), m_colorCount(colorCount), m_rd(), m_eng(m_rd())
{
    read(":/input.json");
    if (m_sizeX < 3) {
        m_sizeX = 3;
    }
    if (m_sizeY < 3) {
        m_sizeY = 3;
    }
    m_isMatch = 0;
    m_score = 0;
    std::uniform_int_distribution<> distr(0, colorCount - 1);

    for (int i = 0; i < m_sizeY; i++) {
        matchHorizontal.append(QList<int>());
        matchVertical.append(QList<int>());
        match.append(QList<int>());
        for (int j = 0; j < m_sizeX; j++) {
            matchHorizontal[i].append(int());
            matchVertical[i].append(int());
            match[i].append(int());
        }
    }
    for (int i = 0; i < m_dimension; i++) {
        int randColorKey = distr(m_eng);
        m_data.push_back(randColorKey);
    }
    m_isMatch = checkMatch(m_data);
    shuffle();
}

void GameBoardData::read(QString inp)
{
    QFile file_obj(inp);
    if (!file_obj.exists()) {
        exit(1);

    }
    if (!file_obj.open(QIODevice::ReadOnly|QIODevice::Text)) {

        exit(1);
    }
    QTextStream file_text(&file_obj);
    QString json_string;
    json_string = file_text.readAll();
    file_obj.close();
    QByteArray json_bytes = json_string.toLocal8Bit();
    auto json_doc = QJsonDocument::fromJson(json_bytes);
    QJsonObject json_obj = json_doc.object();
    auto result = json_obj.toVariantMap();
    m_sizeX = result["width"].toInt();
    m_sizeY = result["height"].toInt();
    QVariantList localList = result["colorNames"].toList();
    int count = 0;
    for (auto& el: localList) {
        m_colorKey[count] = el.toString();
        count++;
    }
    m_colorCount = count;
}

void GameBoardData::shuffle()
{
    std::uniform_int_distribution<> distr(0, m_colorCount - 1);
    m_isMatch = true;
    m_score = 0;
    emit isScoreChanged();
    beginResetModel();
    while ((m_isMatch) || (ifGameOver())) {
        for (int i = 0; i < m_dimension; i++) {
            int randColorKey = distr(m_eng);
            m_data[i] = randColorKey;
        }
        m_isMatch = checkMatch(m_data);
    }
    endResetModel();
}

bool GameBoardData::ifNear(int first, int second) const
{
    return ((std::abs(first - second) == 1) || (std::abs(first - second) == m_sizeY));
}


void  GameBoardData::checkMatchHorizontal()
{
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            int currentElement = matchHorizontal[i][j];
            int countMatch = 0;
            int first = j;
            while ((j < m_sizeX - 1) && (currentElement == matchHorizontal[i][j+1])) {
                countMatch++;
                j++;
            }
            for(int k = first; k < first + countMatch + 1; k++) {
                matchHorizontal[i][k] = countMatch + 1;
            }
        }
    }
}

void GameBoardData::checkMatchVertical()
{
    for (int i = 0; i < m_sizeX; i++) {
        for (int j = 0; j < m_sizeY; j++) {
            int currentElement = matchVertical[j][i];
            int countMatch = 0;
            int first = j;
            while ((j < m_sizeY - 1) && (currentElement == matchVertical[j + 1][i])) {
                countMatch++;
                j++;
            }
            for (int k = first; k < first + countMatch + 1; k++) {
                matchVertical[k][i] = countMatch + 1;
            }
        }
    }
}

bool GameBoardData::checkMatch(QList<int>& m_data)
{
    for (int x = 0; x < m_sizeY; x++) {
        for (int k = 0; k < m_sizeX; k++) {
            matchHorizontal[x][k] = m_data[m_sizeY*k + x];
            matchVertical[x][k] = matchHorizontal[x][k];
        }
    }
    checkMatchVertical();
    checkMatchHorizontal();
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            match[i][j] = std::max(matchVertical[i][j], matchHorizontal[i][j]);
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
}

void GameBoardData::setMatchToNull()
{
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            matchVertical[i][j] = 0;
            match[i][j] = matchHorizontal[i][j] = 0;
        }
    }
}

bool GameBoardData::getIsMatch()
{
    return m_isMatch;
}

void GameBoardData::clear()
{
    indexForDelete.clear();
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m_sizeX; j++) {
            if (match[i][j] >= 3) {
                indexForDelete.push_back(Position{i ,j});
            }
        }
    }
    QList<int> q;
    for (auto el : indexForDelete) {
        q.push_back(el.x * m_sizeY + el.y);
    }
    std::sort(q.begin(), q.end());
    QMap<int,int> m;
    for (int i = 0; i < m_sizeY; i++) {
        m.insert(i,0);
    }
    m_score += q.size();
    emit isScoreChanged();
    while (!q.isEmpty()) {
        int forRemove = q.back();
        q.pop_back();
        beginRemoveRows(QModelIndex(), forRemove, forRemove);
        m_data.erase(m_data.begin() + forRemove);
        endRemoveRows();
        int temp = m[forRemove / m_sizeY];
        m[forRemove / m_sizeY] = temp + 1;
    }
    std::uniform_int_distribution<> distr(0, m_colorCount - 1);
    for (int i = 0; i < m_sizeY; i++) {
        for (int j = 0; j < m[i]; j++) {
            beginInsertRows(QModelIndex(), i * m_sizeY, i * m_sizeY);
            m_data.insert(i * m_sizeY, distr(m_eng));
            endInsertRows();
        }
    }
    emit isScoreChanged();
    setMatchToNull();
}

void GameBoardData::moveElements(int indexFirst, int indexSecond)
{
    int offsetForHorizontal = indexFirst < indexSecond ? 1 : 0;
    int offsetForVertical = indexFirst < indexSecond ? 0 : 1;
    if (ifNear(indexFirst, indexSecond)) {
        if (std::abs(indexSecond - indexFirst) == 1) {
            beginMoveRows(QModelIndex(),indexFirst,indexFirst,QModelIndex(),indexSecond + offsetForHorizontal);
            endMoveRows();
        } else if (std::abs(indexSecond - indexFirst) == m_sizeY) {
            beginMoveRows(QModelIndex(),indexFirst,indexFirst,QModelIndex(),indexSecond);
            endMoveRows();
            beginMoveRows(QModelIndex(), indexSecond + offsetForVertical,
                          indexSecond + offsetForVertical, QModelIndex(), indexFirst + offsetForVertical);
            endMoveRows();
        }
        int temp = m_data[indexFirst];
        m_data[indexFirst] = m_data[indexSecond];
        m_data[indexSecond] = temp;
        emit isScoreChanged();
    }
    m_isMatch = checkMatch(m_data);
}

void GameBoardData::swapElements(int indexFirst, int indexSecond)
{
    QList<int> copyData  = m_data;
    int temp = copyData[indexFirst];
    copyData[indexFirst] = copyData[indexSecond];
    copyData[indexSecond] = temp;

    if (checkMatch(copyData)) {
        moveElements(indexFirst, indexSecond);
    }
    else {
        emit noMatch(indexFirst, indexSecond);
    }
}

void GameBoardData::clearMatchAgain()
{
    if (m_isMatch) {
        m_isMatch = checkMatch(m_data);
        clear();
    }
}

QVariant GameBoardData::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_data.count()) {
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

int GameBoardData::getScore()
{
    return m_score;
}

int GameBoardData::getSizeX()
{
    return m_sizeX;
}

int GameBoardData::getSizeY()
{
    return m_sizeY;
}

bool GameBoardData::ifGameOver()
{  
    return (ifGameOverVertical() && ifGameOverHorizontal());
}

bool GameBoardData::ifGameOverHorizontal()
{
    QList<QList<int>> checkList;
    for (int i = 0; i < m_sizeY; i++) {
        checkList.append(QList<int>());
        for (int j = 0; j < m_sizeX; j++) {
            checkList[i].append(int());
        }
    }
    for (int x = 0; x < m_sizeX; x++) {
        for (int k = 0; k < m_sizeY; k++) {
            checkList[k][x] = m_data[m_sizeY * x + k];
        }
    }
    for (int i = 0; i < m_sizeY - 1; i++) {
        for (int j = 0; j < m_sizeX - 2; j++) {
            if ((checkList[i][j] == checkList[i][j+1]) && (checkList[i][j] == checkList[i+1][j+2])) {
                return false;
            }
            if ((checkList[i][j] == checkList[i + 1][j + 1]) && (checkList[i][j] == checkList[i][j + 2])) {
                return false;
            }
            if ((checkList[i + 1][j] == checkList[i][j+1]) && (checkList[i + 1][j] == checkList[i][j+2])) {
                return false;
            }
            if (j + 3 < m_sizeX) {
                if ((checkList[i][j] == checkList[i][j+1]) && (checkList[i][j]== checkList[i][j + 3])) {
                    return false;
                }
                if ((checkList[i][j] == checkList[i][j + 2]) && (checkList[i][j] == checkList[i][j + 3])) {
                    return false;
                }
            }
            if ((checkList[i][j] == checkList[i+1][j+1]) && (checkList[i][j] == checkList[i+1][j+2])) {
                return false;
            }
            if ((checkList[i + 1][j] == checkList[i][j+1]) && (checkList[i + 1][j] == checkList[i+1][j+2])) {
                return false;
            }
            if ((checkList[i + 1][j] == checkList[i+1][j+1]) && (checkList[i + 1][j] == checkList[i][j+2])) {
                return false;
            }
        }
    }

    return true;


}

bool GameBoardData::ifGameOverVertical()
{
    QList<QList<int>> checkList;
    for (int i = 0; i < m_sizeX; i++)
    {
        checkList.append(QList<int>());
        for (int j = 0; j < m_sizeY; j++)
        {
            checkList[i].append(int());
        }
    }
    for (int i = 0; i < m_sizeX; i++) {
        for (int j = 0; j < m_sizeY; j++) {
            checkList[i][j] = m_data[m_sizeY * i + j];
        }
    }
    for (int i = 0; i < m_sizeX ; i++) {
        for (int j = 0; j < m_sizeY - 2; j++) {
            if (i < m_sizeX - 1) {
                if ((checkList[i][j] == checkList[i][j+1]) && (checkList[i][j] == checkList[i+1][j+2])) {
                    return false;
                }
                if ((checkList[i][j] == checkList[i + 1][j + 1]) && (checkList[i][j] == checkList[i][j + 2])) {
                    return false;
                }
                if ((checkList[i + 1][j] == checkList[i][j+1]) && (checkList[i + 1][j] == checkList[i][j+2])) {
                    return false;
                }
                if ((checkList[i][j] == checkList[i+1][j+1]) && (checkList[i][j] == checkList[i+1][j+2])) {
                    return false;
                }
                if ((checkList[i + 1][j] == checkList[i][j+1]) && (checkList[i + 1][j] == checkList[i+1][j+2])) {
                    return false;
                }
                if ((checkList[i + 1][j] == checkList[i+1][j+1]) && (checkList[i + 1][j] == checkList[i][j+2])) {
                    return false;
                }
            }
            if (j + 3 < m_sizeY) {
                if ((checkList[i][j] == checkList[i][j+1]) && (checkList[i][j]== checkList[i][j + 3])) {
                    return false;
                }
                if ((checkList[i][j] == checkList[i][j + 2]) && (checkList[i][j] == checkList[i][j + 3])) {
                    return false;
                }
            }
        }
    }
    return true;
}
