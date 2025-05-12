#include <cassert>
#include "Customer.h"
Customer::Customer(int clientId, int phone):
        m_customerId(clientId),
        m_phone(phone),
        m_isMember(false),
        m_clientExpenses(0)
        {}

bool Customer::isMember() const {
    return m_isMember;
}

void Customer::makeMember() {
    m_isMember = true;
}

double Customer::getClientExpenses() const {
    return m_clientExpenses;
}

void Customer::increaseMonthlyExpenses(double amount) {
//    assert(amount > 0);
    m_clientExpenses += amount;
}

void Customer::initExpenses() {
//    assert(m_isMember);
    m_clientExpenses = 0;
}

int Customer::getPhone() const {
    return m_phone;
}

int Customer::getCustomerId() const {
    return m_customerId;
}


