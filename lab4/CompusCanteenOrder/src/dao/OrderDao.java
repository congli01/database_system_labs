package dao;

import application.Order;
import application.OrderInfo;

import java.sql.SQLException;
import java.util.List;

/**
 * @author LICONG
 *
 * order的数据访问对象接口
 */
public interface OrderDao {
    /**
     * 查询数据库中的订单总数
     * @return
     */
    int totalNum() throws SQLException;

    List<OrderInfo> findByCustomer(int customerId) throws SQLException;

    List<OrderInfo> findByMerchant(int merchantId) throws SQLException;

    /**
     * 新建订单
     * @param order
     * @return
     * @throws SQLException
     */
    boolean add(Order order) throws SQLException;

    /**
     * 修改订单状态
     * @param orderId
     * @return
     */
    boolean modifyState(int orderId) throws SQLException;
}
