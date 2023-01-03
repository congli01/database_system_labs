package application;

/**
 * @author LICONG
 *
 * 商铺类
 */
public class Shop {
    private int shopId;
    private int canteenId;
    private int merchantId;
    private String name;

    public Shop(int shopId, int canteenId, int merchantId, String name) {
        this.shopId = shopId;
        this.canteenId = canteenId;
        this.merchantId = merchantId;
        this.name = name;
    }

    public int getShopId() {
        return shopId;
    }

    public int getCanteenId() {
        return canteenId;
    }

    public int getMerchantId() {
        return merchantId;
    }

    public String getName() {
        return name;
    }
}
