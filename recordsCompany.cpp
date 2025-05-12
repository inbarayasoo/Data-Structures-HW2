#include "recordsCompany.h"

RecordsCompany::RecordsCompany() :
        m_customers(HashTable<int, Customer*>()),
        m_recordsUnionFind(RecordsUnionFind()),
        m_memberCustomers(RankTree<int, Customer*>()),
        m_numOfRecords(0)
        {}

StatusType RecordsCompany::addCostumer(int c_id, int phone) {
    if (c_id < 0 || phone < 0) {
        return INVALID_INPUT;
    }

    else if (m_customers.contains(c_id)) {
        return StatusType::ALREADY_EXISTS;
    }

    try {
        auto* customer = new Customer(c_id, phone);
        m_customers.insert(c_id, customer);
    }
    catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

Output_t<int> RecordsCompany::getPhone(int c_id) {
    if (c_id < 0) {
        return {StatusType::INVALID_INPUT};
    }

    if (!m_customers.contains(c_id)) {
        return {StatusType::DOESNT_EXISTS};
    }
    Customer* customer = *m_customers.find(c_id);
    return {customer->getPhone()};
}

StatusType RecordsCompany::makeMember(int c_id) {
    if (c_id < 0) {
        return StatusType::INVALID_INPUT;
    }

    if (!m_customers.contains(c_id)) {
        return StatusType::DOESNT_EXISTS;
    }

    Customer* customer = *m_customers.find(c_id);
    if (customer->isMember()) {
        return StatusType::ALREADY_EXISTS;
    }

    try {
        m_memberCustomers.insert(c_id, customer);
        customer->makeMember();
    }
    catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

Output_t<bool> RecordsCompany::isMember(int c_id) {
    if (c_id < 0) {
        return {StatusType::INVALID_INPUT};
    }

    if (!m_customers.contains(c_id)) {
        return {StatusType::DOESNT_EXISTS};
    }

    Customer* customer = *m_customers.find(c_id);
    return {customer->isMember()};
}

StatusType RecordsCompany::buyRecord(int c_id, int r_id) {
   if (c_id < 0 || r_id < 0) {
        return StatusType::INVALID_INPUT;
    }
    if (!m_customers.contains(c_id) || r_id >= m_numOfRecords) {
        return StatusType::DOESNT_EXISTS;
    }

    Customer* customer = *m_customers.find(c_id);
    if (customer->isMember()) {
        customer->increaseMonthlyExpenses(BASIS_SELLING_PRICE + m_recordsUnionFind.getRecordSales(r_id));
    }

    m_recordsUnionFind.sellRecord(r_id);

    return StatusType::SUCCESS;
}

Output_t<double> RecordsCompany::getExpenses(int c_id) {
    if (c_id < 0) {
        return {StatusType::INVALID_INPUT};
    }

    if (!m_memberCustomers.contains(c_id)) {
        return {StatusType::DOESNT_EXISTS};
    }

    Customer* customer = *m_memberCustomers.find(c_id);
    return {customer->getClientExpenses() - m_memberCustomers.getRank(c_id)};
}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight) {
    if (r_id < 0 || column == nullptr || hight == nullptr) {
        return StatusType::INVALID_INPUT;
    }

    if (r_id >= m_numOfRecords) {
        return StatusType::DOESNT_EXISTS;
    }

    *column = m_recordsUnionFind.getRecordStockCol(r_id);
    *hight = m_recordsUnionFind.getRecordHeight(r_id);

    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double amount) {
    if (c_id1 < 0 || amount <= 0 || c_id2 < c_id1) {
        return StatusType::INVALID_INPUT;
    }
    m_memberCustomers.addExtra(c_id1, c_id2, amount);

    return StatusType::SUCCESS;
}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2) {
    if (r_id1 < 0 || r_id2 < 0) {
        return StatusType::INVALID_INPUT;
    }

    else if (r_id1 >= m_numOfRecords || r_id2 >= m_numOfRecords) {
        return StatusType::DOESNT_EXISTS;
    }

    else if (m_recordsUnionFind.hasSameStock(r_id1, r_id2)) {
        return StatusType::FAILURE;
    }

    m_recordsUnionFind.putRecordBOnTopRecordA(r_id2, r_id1);

    return StatusType::SUCCESS;
}

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records) {
//    assert((number_of_records == 0 && records_stocks == nullptr) || (number_of_records > 0 && records_stocks != nullptr));
    if (number_of_records < 0) {
        return StatusType::INVALID_INPUT;
    }

    m_memberCustomers.initExtras();
    initMemberCustomersExpenses();

    if (number_of_records == 0) {
        return StatusType::SUCCESS;
    }

    try {
        m_recordsUnionFind.initializeRecordsStocks(records_stocks, number_of_records);
        m_numOfRecords = number_of_records;
    }
    catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

void RecordsCompany::initMemberCustomersExpenses() {
    auto** array = new Customer*[m_memberCustomers.getSize()];
    m_memberCustomers.fillElementsInArrayInOrder(array, m_memberCustomers.getSize());
    int size = m_memberCustomers.getSize();
    for (int i = 0; i < size; ++i) {
        array[i]->initExpenses();
    }
    delete[] array;
}

RecordsCompany::~RecordsCompany() {
    int size = m_customers.getSize();
    Customer** array = new Customer*[size];
    m_customers.elementsToArr(array);

    for (int i = 0; i < size; ++i) {
        delete array[i];
    }
    delete[] array;
}
