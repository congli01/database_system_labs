package application;

/**
 * @author LICONG
 *
 * 菜单类
 */
public class Menu {
    private int foodId;
    private int orderId;
    private int num;

    public Menu(int foodId, int orderId, int num) {
        this.foodId = foodId;
        this.orderId = orderId;
        this.num = num;
    }

    public int getFoodId() {
        return foodId;
    }

    public int getOrderId() {
        return orderId;
    }

    public int getNum() {
        return num;
    }
}
