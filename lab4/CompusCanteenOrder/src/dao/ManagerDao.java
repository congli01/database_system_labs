package dao;

import application.Manager;

import java.sql.SQLException;

/**
 * @author LICONG
 *
 * 数据访问对象模式
 * manager的Dao接口
 */
public interface ManagerDao {
    /**
     * 查询数据库中用户总数
     * @return
     */
    int totalNum() throws SQLException;

    Manager find(String userId) throws SQLException;

    boolean add(Manager manager) throws SQLException;
}
