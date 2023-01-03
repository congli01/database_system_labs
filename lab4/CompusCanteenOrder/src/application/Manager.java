package application;

/**
 * @author LICONG
 *
 * 管理员类
 */
public class Manager {
    private int managerId;
    private String userId;
    private String name;

    public Manager(int managerId, String userId, String name) {
        this.managerId = managerId;
        this.userId = userId;
        this.name = name;
    }

    public int getManagerId() {
        return managerId;
    }

    public String getUserId() {
        return userId;
    }

    public String getName() {
        return name;
    }
}
