package dao;

import application.Customer;
import application.User;

import java.sql.*;

/**
 * @author LICONG
 *
 * customer数据访问对象接口的实现
 */
public class CustomerDaoImpl extends BaseDao implements CustomerDao{
    @Override
    public int totalNum() throws SQLException {
        int num = 0;
        Connection conn = BaseDao.getConnection();
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery("select count(*) as result from canteen.customer");
        while (rs.next()) {
            num = rs.getInt(1);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return num;
    }

    @Override
    public Customer find(String userId) throws SQLException {
        Customer customer = null;
        Connection conn = BaseDao.getConnection();
        String sql = "select * from canteen.customer where user_id = " + userId;
        PreparedStatement stmt = conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        if (rs == null) {
            BaseDao.closeAll(conn, stmt, rs);
            return customer;
        }
        while (rs.next()) {
            customer = new Customer(Integer.parseInt(rs.getString("customer_id")),
                                    rs.getString("user_id"),
                                    rs.getString("phone_number"),
                                    rs.getString("sex"),
                                    rs.getString("customer_name"));
        }
        BaseDao.closeAll(conn, stmt, rs);
        return customer;
    }

    @Override
    public boolean add(Customer customer) throws SQLException {
        Connection conn = BaseDao.getConnection();
        String sql = "insert into canteen.customer(customer_id, user_id, phone_number, sex, customer_name) " +
                     "values(?, ?, ?, ?, ?) ";
        PreparedStatement stmt = conn.prepareStatement(sql);
        stmt.setInt(1, customer.getCustomerId());
        stmt.setString(2, customer.getUserId());
        stmt.setString(3, customer.getPhone());
        stmt.setString(4, customer.getSex());
        stmt.setString(5, customer.getName());
        int result = stmt.executeUpdate();
        BaseDao.closeAll(conn, stmt, null);
        return result >= 1;
    }
}
