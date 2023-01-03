package dao;

import application.Customer;
import application.Merchant;

import java.sql.*;

/**
 * @author LICONG
 *
 * merchant数据访问对象接口的实现
 */
public class MerchantDaoImpl extends BaseDao implements MerchantDao{
    @Override
    public int totalNum() throws SQLException {
        int num = 0;
        Connection conn = BaseDao.getConnection();
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery("select count(*) as result from canteen.merchant");
        while (rs.next()) {
            num = rs.getInt(1);
        }
        BaseDao.closeAll(conn, stmt, rs);
        return num;
    }

    @Override
    public Merchant find(String userId) throws SQLException {
        Merchant merchant = null;
        Connection conn = BaseDao.getConnection();
        String sql = "select * from canteen.merchant where user_id = \"" + userId + "\"";
        PreparedStatement stmt = conn.prepareStatement(sql);
        ResultSet rs = stmt.executeQuery();
        if (rs == null) {
            BaseDao.closeAll(conn, stmt, rs);
            return merchant;
        }
        while (rs.next()) {
            merchant = new Merchant(Integer.parseInt(rs.getString("merchant_id")),
                    rs.getString("user_id"),
                    rs.getString("merchant_name"));
        }
        BaseDao.closeAll(conn, stmt, rs);
        return merchant;
    }

    @Override
    public boolean add(Merchant merchant) throws SQLException {
        Connection conn = BaseDao.getConnection();
        String sql = "insert into canteen.merchant(merchant_id, user_id, merchant_name) " +
                "values(?, ?, ?) ";
        PreparedStatement stmt = conn.prepareStatement(sql);
        stmt.setInt(1, merchant.getMerchantId());
        stmt.setString(2, merchant.getUserId());
        stmt.setString(3, merchant.getName());
        int result = stmt.executeUpdate();
        BaseDao.closeAll(conn, stmt, null);
        return result >= 1;
    }
}
