package dao;

import application.Customer;

import java.sql.SQLException;

/**
 * @author LICONG
 *
 * 数据访问对象模式
 * customer的Dao接口
 */
public interface CustomerDao {
    /**
     * 查询数据库中用户总数
     * @return
     */
    int totalNum() throws SQLException;

    Customer find(String userId) throws SQLException;

    boolean add(Customer customer) throws SQLException;
}
