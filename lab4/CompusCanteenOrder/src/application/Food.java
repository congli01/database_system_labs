package application;

/**
 * @author LICONG
 *
 * 菜品类
 */
public class Food {
    private int foodId;
    private String name;
    private float price;
    private String description;

    public Food(int foodId, String name, float price, String description) {
        this.foodId = foodId;
        this.name = name;
        this.price = price;
        this.description = description;
    }

    public int getFoodId() {
        return foodId;
    }

    public String getName() {
        return name;
    }

    public float getPrice() {
        return price;
    }

    public String getDescription() {
        return description;
    }
}
