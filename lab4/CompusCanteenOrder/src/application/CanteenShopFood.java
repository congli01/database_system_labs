package application;

/**
 * @author LICONG
 *
 * 食堂商铺菜品信息
 */
public class CanteenShopFood {
    private String canteenName;
    private String shopName;
    private String foodName;
    private float foodPrice;
    private String foodDescription;

    public CanteenShopFood(String canteenName, String shopName, String foodName, float foodPrice,
                           String foodDescription){
        this.canteenName = canteenName;
        this.shopName = shopName;
        this.foodName = foodName;
        this.foodPrice = foodPrice;
        this.foodDescription = foodDescription;
    }

    public String getCanteenName() {
        return canteenName;
    }

    public String getShopName() {
        return shopName;
    }

    public String getFoodName() {
        return foodName;
    }

    public float getFoodPrice() {
        return foodPrice;
    }

    public String getFoodDescription() {
        return foodDescription;
    }
}
