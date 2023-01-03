package application;

/**
 * @author LICONG
 *
 * 食堂类
 */
public class Canteen {
    private int canteenId;
    private int managerId;
    private String name;

    public Canteen(int canteenId, int managerId, String name) {
        this.canteenId = canteenId;
        this.managerId = managerId;
        this.name = name;
    }

    public int getCanteenId() {
        return canteenId;
    }

    public int getManagerId() {
        return managerId;
    }

    public String getName() {
        return name;
    }
}
