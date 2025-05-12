#include <cassert>
#include "RecordsUnionFind.h"

RecordsUnionFind::RecordsUnionFind() :
        m_recordStocks(nullptr),
        m_numOfRecords(0)
        {}


RecordsUnionFind::~RecordsUnionFind() {
    clear();
}

void RecordsUnionFind::initializeRecordsStocks(int *recordsStocks, int numOfRecords) {
    if (m_recordStocks != nullptr) {
//        assert(0 < m_numOfRecords);
        clear();
    }
    m_recordStocks = new RecordStock*[numOfRecords];
    m_numOfRecords = numOfRecords;
    for(int i = 0 ; i < numOfRecords; ++i) {
        m_recordStocks[i] = new RecordStock(i, recordsStocks[i]);
    }
}

void RecordsUnionFind::clear() {

    for (int i = 0; i < m_numOfRecords; ++i) {
        delete m_recordStocks[i];
    }
    delete[] m_recordStocks;
    m_numOfRecords = 0;
}


RecordStock* RecordsUnionFind::getRoot(int recordId) {
//    assert(recordId < m_numOfRecords);
    RecordStock* currentRecord = m_recordStocks[recordId];
    while (!(currentRecord->isParent())) {
        currentRecord = m_recordStocks[currentRecord->getGroupParentId()];
    }
    pathCompression(m_recordStocks[recordId], currentRecord);
    return currentRecord;
}

void RecordsUnionFind::pathCompression(RecordStock* record, RecordStock* rootRecord) {

    RecordStock* currentRecord = record;
    int toSubtract = 0;
    int heightWithoutRoot = sumExtrasWithoutRoot(record);

    while (currentRecord != rootRecord) {
        int temp = currentRecord->getExtra();
        currentRecord->setExtra(heightWithoutRoot - toSubtract);
        toSubtract += temp;
        currentRecord->setGroupParent(rootRecord->getGroupParentId());
        currentRecord = m_recordStocks[currentRecord->getGroupParentId()];
    }
}

int RecordsUnionFind::getRecordStockCol(int recordId) {
//    assert(recordId < m_numOfRecords);
    return getRoot(recordId)->getRecordStockCol();
}

int RecordsUnionFind::sumExtrasWithoutRoot(RecordStock* record) {

    int extrasWithoutRoot = 0;

    while (!(record->isParent())) {
        extrasWithoutRoot += record->getExtra();
        record = m_recordStocks[record->getGroupParentId()];
    }
    return extrasWithoutRoot;
}

int RecordsUnionFind::getRecordHeight(int recordId) {
//    assert(recordId < m_numOfRecords);
    int extrasWithoutRoot = sumExtrasWithoutRoot(m_recordStocks[recordId]);
    int height = extrasWithoutRoot + getRoot(recordId)->getExtra();
    return height;
}


void RecordsUnionFind::putRecordBOnTopRecordA(int recordA , int recordB) {

    RecordStock* rootA = getRoot(recordA);
    RecordStock* rootB = getRoot(recordB);

    int sizeStockA = rootA->getGroupSize();
    int sizeStockB = rootB->getGroupSize();

    if (sizeStockB <= sizeStockA) {
        uniteBtoA(rootA, rootB);
    }
    else {
        uniteAtoB(rootA, rootB);
    }
}

void RecordsUnionFind::uniteBtoA(RecordStock* rootA, RecordStock* rootB) {

    int heightRecordA = rootA->getGroupHeight();
    int oldExtraStockA = rootA->getExtra();
    int oldExtraStockB = rootB->getExtra();

    rootB->setExtra(oldExtraStockB + heightRecordA - oldExtraStockA);
    rootA->increaseGroup(rootB->getGroupSize(), rootB->getGroupHeight());
    rootB->setGroupParent(rootA->getGroupParentId());

    rootB->clearRecordStock();
}

void RecordsUnionFind::uniteAtoB(RecordStock* rootA, RecordStock* rootB) {
    int heightRecordA = rootA->getGroupHeight();
    int oldExtraStockA = rootA->getExtra();
    int oldExtraStockB = rootB->getExtra();

    rootB->setExtra(oldExtraStockB + heightRecordA);
    int newExtraStockB = rootB->getExtra();
    rootA->setExtra(oldExtraStockA - newExtraStockB);

    rootB->increaseGroup(rootA->getGroupSize(), rootA->getGroupHeight());
    rootB->setColumn(rootA->getRecordStockCol());
    rootA->setGroupParent(rootB->getGroupParentId());
    rootA->clearRecordStock();
}

int RecordsUnionFind::getRecordSales(int recordId) {
//    assert(recordId < m_numOfRecords);
    return m_recordStocks[recordId]->getSales();
}

void RecordsUnionFind::sellRecord(int recordId) {
//    assert(recordId < m_numOfRecords);
    m_recordStocks[recordId]->sellOne();
}

bool RecordsUnionFind::hasSameStock(int recordId1, int recordId2) {
//    assert(0 <= recordId1 && 0 <= recordId2);
//    assert(recordId1 < m_numOfRecords && recordId2 < m_numOfRecords);
    return getRoot(recordId1)->getRecordStockCol() == getRoot(recordId2)->getRecordStockCol();
}

RecordsUnionFind::RecordsUnionFind(const RecordsUnionFind &other) {

    m_numOfRecords = other.m_numOfRecords;

    if (other.m_recordStocks == nullptr) {
        m_recordStocks = nullptr;
        return;
    }

    m_recordStocks = new RecordStock*[m_numOfRecords];

    for (int i = 0; i < m_numOfRecords; ++i) {
        m_recordStocks[i] = new RecordStock(*other.m_recordStocks[i]);
    }

}

RecordsUnionFind &RecordsUnionFind::operator=(const RecordsUnionFind &other) {

        if (this == &other) {
            return *this;
        }

        clear();

        m_numOfRecords = other.m_numOfRecords;

        if (other.m_recordStocks == nullptr) {
            m_recordStocks = nullptr;
            return *this;
        }

        m_recordStocks = new RecordStock*[m_numOfRecords];

        for (int i = 0; i < m_numOfRecords; ++i) {
            m_recordStocks[i] = new RecordStock(*other.m_recordStocks[i]);
        }

        return *this;
}

