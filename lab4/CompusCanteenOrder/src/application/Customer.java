package application;

/**
 * @author LICONG
 *
 * 顾客类
 */
public class Customer {
    private int customerId;
    private String userId;
    private String phone;
    private String sex;
    private String name;

    public Customer(int customerId, String userId, String phone, String sex, String name) {
        this.customerId = customerId;
        this.userId = userId;
        this.phone = phone;
        this.sex = sex;
        this.name = name;
    }

    public int getCustomerId() {
        return customerId;
    }

    public String getUserId() {
        return userId;
    }

    public String getPhone() {
        return phone;
    }

    public String getSex() {
        return sex;
    }

    public String getName() {
        return name;
    }
}
