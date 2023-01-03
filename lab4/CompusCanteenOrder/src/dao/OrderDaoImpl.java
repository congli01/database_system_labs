package dao;

import application.Order;
import application.OrderInfo;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class OrderDaoImpl extends BaseDao implements OrderDao{
    @Override
    public int totalNum() throws SQLException {
        int num = 0;
        Connection conn = BaseDao.getConnection();
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery("select count(*) as result from canteen.orderinfo");
        while (rs.next()) {
            num = rs.getInt(1);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return num;
    }

    @Override
    public List<OrderInfo> findByCustomer(int customerId) throws SQLException {
        List<OrderInfo> orderInfoList = new ArrayList<>();
        Connection conn = BaseDao.getConnection();
        String sql = "select * from canteen.orderinfo where customer_id = \"" + customerId + "\"";
        PreparedStatement stmt = conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        if (rs == null) {
            BaseDao.closeAll(conn, stmt, rs);
            return null;
        }
        while (rs.next()) {
            OrderInfo orderInfo = new OrderInfo(Integer.parseInt(rs.getString("order_id")),
                    Integer.parseInt(rs.getString("merchant_id")),
                    rs.getString("order_state"),
                    rs.getString("order_time"),
                    rs.getString("shop_name"),
                    rs.getString("canteen_name"),
                    rs.getString("food_name"),
                    Integer.parseInt(rs.getString("number")),
                    Integer.parseInt(rs.getString("customer_id")),
                    rs.getString("customer_name"),
                    rs.getString("phone_number"),
                    rs.getString("building"),
                    rs.getString("room"));
            orderInfoList.add(orderInfo);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return orderInfoList;
    }

    @Override
    public List<OrderInfo> findByMerchant(int merchantId) throws SQLException {
        List<OrderInfo> orderInfoList = new ArrayList<>();
        Connection conn = BaseDao.getConnection();
        String sql = "select * from canteen.orderinfo where merchant_id = \"" + merchantId + "\"";
        PreparedStatement stmt = conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        if (rs == null) {
            BaseDao.closeAll(conn, stmt, rs);
            return null;
        }
        while (rs.next()) {
            OrderInfo orderInfo = new OrderInfo(Integer.parseInt(rs.getString("order_id")),
                    Integer.parseInt(rs.getString("merchant_id")),
                    rs.getString("order_state"),
                    rs.getString("order_time"),
                    rs.getString("shop_name"),
                    rs.getString("canteen_name"),
                    rs.getString("food_name"),
                    Integer.parseInt(rs.getString("number")),
                    Integer.parseInt(rs.getString("customer_id")),
                    rs.getString("customer_name"),
                    rs.getString("phone_number"),
                    rs.getString("building"),
                    rs.getString("room"));
            orderInfoList.add(orderInfo);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return orderInfoList;
    }

    @Override
    public boolean add(Order order) throws SQLException {
        Connection conn = BaseDao.getConnection();
        String sql = "insert into canteen.od(order_id, address_id, customer_id, merchant_id, order_price, order_state)" +
                     "values(?, ?, ?, ?, ?, ?) ";
        PreparedStatement stmt = conn.prepareStatement(sql);
        stmt.setInt(1, order.getOrderId());
        stmt.setInt(2, order.getAddressId());
        stmt.setInt(3, order.getCustomerId());
        stmt.setInt(4, order.getMerchantId());
        stmt.setFloat(5, order.getPrice());
        stmt.setString(6, order.getState());
        int result = stmt.executeUpdate();
        BaseDao.closeAll(conn, stmt, null);
        return result >= 1;
    }

    @Override
    public boolean modifyState(int orderId) throws SQLException {
        Connection conn = BaseDao.getConnection();
        String sql = "update canteen.od " +
                     "set order_state = \"已接单\" " +
                     "where order_id = \"" + orderId + "\"";
        PreparedStatement stmt = conn.prepareStatement(sql);
        int result = stmt.executeUpdate();
        BaseDao.closeAll(conn, stmt, null);
        return result >= 1;
    }
}
