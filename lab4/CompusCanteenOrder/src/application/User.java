package application;

/**
 * @author LICONG
 * 用户类
 */
public class User {
    /**
     * 账号
     */
    private String userId;
    /**
     * 密码
     */
    private String password;
    /**
     * 用户类型
     */
    private UserType type;

    public User(String userId, String password, UserType type) {
        this.userId = userId;
        this.password = password;
        this.type = type;
    }

    public String getUserId() {
        return userId;
    }

    public String getPassword() {
        return password;
    }

    public UserType getType() {
        return type;
    }
}

