#ifndef WET2_RECORD_H
#define WET2_RECORD_H

class RecordStock {

    //TODO: What about copy constructor and operator=?
public:
    RecordStock(int recordId, int numCopies);
    int getGroupParentId() const;
    int getRecordStockCol() const;
    int getGroupHeight() const;
    int getExtra() const;
    int getGroupSize() const;
    void setExtra(int extra);
    void setGroupParent(int groupParent);
    void increaseGroup(int addSize, int addHeight);
    void setColumn(int newColumn);
    void sellOne();
    int getSales() const;
    bool isParent() const;

    void clearRecordStock();

    static const int DELETED = -2;

private:
    bool m_isParent;
    int m_groupParent;
    int m_groupSize;
    int m_groupHeight;
    int m_column;
    int m_extra;
    int m_soldCount;
};


#endif //WET2_RECORD_H
