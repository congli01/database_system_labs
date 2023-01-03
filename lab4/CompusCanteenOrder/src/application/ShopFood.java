package application;

/**
 * @author LICONG
 *
 * 菜品所属商铺
 */
public class ShopFood {
    private int foodId;
    private int shopId;

    public ShopFood(int foodId, int shopId) {
        this.foodId = foodId;
        this.shopId = shopId;
    }

    public int getFoodId() {
        return foodId;
    }

    public int getShopId() {
        return shopId;
    }
}
