#include <cassert>
#include "RecordStock.h"

RecordStock::RecordStock(int recordId, int numCopies) :
    m_isParent(true),
    m_groupParent(recordId),
    m_groupSize(1),
    m_groupHeight(numCopies),
    m_column(recordId),
    m_extra(0),
    m_soldCount(0)
    {}

int RecordStock::getGroupParentId() const {
    return m_groupParent;
}

int RecordStock::getRecordStockCol() const {
//    assert(m_isParent);
    return m_column;
}

int RecordStock::getExtra() const {
    return m_extra;
}

int RecordStock::getGroupHeight() const {
//    assert(m_isParent);
    return m_groupHeight;
}

void RecordStock::setExtra(int extra) {
    m_extra = extra;
}

void RecordStock::setGroupParent(int parent) {
    m_isParent = false;
    m_groupParent = parent;
}

int RecordStock::getGroupSize() const {
//    assert(m_isParent);
    return m_groupSize;
}


void RecordStock::setColumn(int newColumn) {
//    assert(m_isParent);
    m_column = newColumn;
}

void RecordStock::clearRecordStock() {
    m_groupSize = DELETED;
    m_groupHeight = DELETED;
    m_column = DELETED;
}

void RecordStock::increaseGroup(int addSize, int addHeight) {
//    assert(m_isParent);
//    assert(addSize > 0);
//    assert(addHeight > 0);
    m_groupSize += addSize;
    m_groupHeight += addHeight;
}

void RecordStock::sellOne() {
    ++m_soldCount;
}

int RecordStock::getSales() const {
    return m_soldCount;
}

bool RecordStock::isParent() const {
    return m_isParent;
}

