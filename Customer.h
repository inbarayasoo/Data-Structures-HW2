#ifndef WET2_CLIENT_H
#define WET2_CLIENT_H


class Customer {
public:
    Customer(int clientId, int phone);
    Customer(const Customer& client) = default;
    Customer& operator=(const Customer& client) = default;
    ~Customer() = default;

    double getClientExpenses() const;
    void increaseMonthlyExpenses(double amount);
    void initExpenses();
    int getPhone() const;
    int getCustomerId() const;
    bool isMember() const;
    void makeMember();


private:
    int m_customerId;
    int m_phone;
    bool m_isMember;
    double m_clientExpenses;

};

#endif //WET2_CLIENT_H
