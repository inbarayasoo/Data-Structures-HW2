#ifndef WET2_UNIONFIND_H
#define WET2_UNIONFIND_H

#include "RecordStock.h"

class RecordsUnionFind {

public:
    RecordsUnionFind();
    ~RecordsUnionFind();
    void initializeRecordsStocks(int *recordsStocks, int numOfRecords);
    RecordsUnionFind& operator=(const RecordsUnionFind& other);
    RecordsUnionFind(const RecordsUnionFind& other);
    int getRecordStockCol(int recordId);
    int getRecordHeight(int recordId);
    int getRecordSales(int recordId);
    void putRecordBOnTopRecordA(int recordA , int recordB);
    void clear();
    void sellRecord(int recordId);
    bool hasSameStock(int recordId1, int recordId2);
//    bool isEmpty();

private:
    RecordStock** m_recordStocks;
    int m_numOfRecords;

    void pathCompression(RecordStock* record, RecordStock* rootRecord);
    int sumExtrasWithoutRoot(RecordStock* record);

    void uniteBtoA(RecordStock* rootA, RecordStock* rootB);
    void uniteAtoB(RecordStock* rootA, RecordStock* rootB);

    RecordStock* getRoot(int recordId);

};

#endif //WET2_UNIONFIND_H
