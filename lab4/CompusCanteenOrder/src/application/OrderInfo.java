package application;

/**
 * @author LICONG
 *
 * 订单信息视图
 */
public class OrderInfo {
    private int orderId;
    private int merchantId;
    private String orderState, orderTime, shopName, canteenName, foodName;
    private int num, customerId;
    private String customerName, phone, building, room;

    public OrderInfo(int orderId, int merchantId, String orderState, String orderTime, String shopName, String canteenName,
                     String foodName, int num, int customerId, String customerName, String phone, String building, String room){
        this.orderId = orderId;
        this.merchantId = merchantId;
        this.orderState = orderState;
        this.orderTime = orderTime;
        this.shopName = shopName;
        this.canteenName = canteenName;
        this.foodName = foodName;
        this.num = num;
        this.customerId = customerId;
        this.customerName = customerName;
        this.phone = phone;
        this.building = building;
        this.room = room;
    }

    public int getOrderId() {
        return orderId;
    }

    public String getShopName() {
        return shopName;
    }

    public String getOrderState() {
        return orderState;
    }

    public String getFoodName() {
        return foodName;
    }

    public int getNum() {
        return num;
    }

    public String getCustomerName() {
        return customerName;
    }

    public String getPhone() {
        return phone;
    }

    public String getBuilding() {
        return building;
    }

    public String getRoom() {
        return room;
    }

    public String getOrderTime() {
        return orderTime;
    }
}
