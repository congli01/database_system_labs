package dao;

import application.User;

import java.sql.SQLException;

/**
 * @author LICONG
 *
 * 数据访问对象模式
 * user的Dao接口
 */
public interface UserDao {
    /**
     * 查找用户
     * @param userId
     * @return userId对于的用户的信息
     */
    User find(String userId) throws SQLException;

    /**
     * 新增用户
     * @param user
     */
    boolean add(User user) throws SQLException;


}
