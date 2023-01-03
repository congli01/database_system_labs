package application;

/**
 * @author LICONG
 *
 * 订单类
 */
public class Order {
    private int orderId;
    private int addressId;
    private int customerId;
    private int merchantId;
    private float price;
    private String time;
    private String state;
    private String evaluation;

    public Order(int orderId, int addressId, int customerId, int merchantId, float price, String time,
                 String state, String evaluation)
    {
        this.orderId = orderId;
        this.addressId = addressId;
        this.customerId = customerId;
        this.merchantId = merchantId;
        this.price = price;
        this.time = time;
        this.state = state;
        this.evaluation = evaluation;
    }

    public int getOrderId() {
        return orderId;
    }

    public int getAddressId() {
        return addressId;
    }

    public int getCustomerId() {
        return customerId;
    }

    public int getMerchantId() {
        return merchantId;
    }

    public float getPrice() {
        return price;
    }

    public String getTime() {
        return time;
    }

    public String getState() {
        return state;
    }

    public String getEvaluation() {
        return evaluation;
    }
}
