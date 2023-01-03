package application;

/**
 * @author LICONG
 *
 * 商家类
 */
public class Merchant {
    private int merchantId;
    private String userId;
    private String name;

    public Merchant(int merchantId, String userId, String name) {
        this.merchantId = merchantId;
        this.userId = userId;
        this.name = name;
    }

    public int getMerchantId() {
        return merchantId;
    }

    public String getUserId() {
        return userId;
    }

    public String getName() {
        return name;
    }
}
